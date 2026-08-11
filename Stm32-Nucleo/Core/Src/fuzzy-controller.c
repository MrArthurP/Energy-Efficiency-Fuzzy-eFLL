/**
  ******************************************************************************
  * @file           : fuzzy-controller.c
  * @brief          : Controlador fuzzy para decidir ligar (ou não) o GPRS de
  *                    um dispositivo, com base na carga da bateria e na taxa
  *                    de acerto no envio de dados via GPRS/satélite.
  *
  * Portado do sketch original em Arduino (que usava a lib eFLL diretamente
  * em C++) para C puro, através do wrapper "extern C" em eFLL_wrapper.cpp.
  * Nenhuma dependência do Arduino (Serial, random, analogRead, delay, map,
  * RESET) permanece aqui - tudo isso era específico da simulação no Arduino
  * IDE e não faz sentido dentro do firmware STM32.
  ******************************************************************************
  */

#include "fuzzy-controller.h"
#include "eFLL_wrapper.h"
#include <stddef.h>

/* Número total de regras do sistema (5 conjuntos de bateria x 4 conjuntos
 * de taxa) */
#define NUM_REGRAS 20

/* --------------------------------------------------------------------- */
/* Estado interno do módulo (encapsulado - não exposto no .h)            */
/* --------------------------------------------------------------------- */

static Fuzzy *fuzzy = NULL;

/* Conjuntos fuzzy de entrada: carga da bateria */
static FuzzySet *Breserva            = NULL; /* 0%  a 5%   */
static FuzzySet *Bextremamente_baixa = NULL; /* 5%  a 20%  */
static FuzzySet *Bbaixa              = NULL; /* 15% a 45%  */
static FuzzySet *Bmedia              = NULL; /* 35% a 70%  */
static FuzzySet *Balta               = NULL; /* 60% a 100% */

/* Conjuntos fuzzy de entrada: taxa de acerto do GPRS (já normalizada 0-100) */
static FuzzySet *Qmuitobaixa = NULL;
static FuzzySet *Qbaixa      = NULL;
static FuzzySet *Qalta       = NULL;
static FuzzySet *Qmuitoalta  = NULL;

/* Conjuntos fuzzy de saída: decisão de ligar o GPRS */
static FuzzySet *NaoLiga = NULL;
static FuzzySet *Liga    = NULL;

/* Entradas/saída do sistema fuzzy */
static FuzzyInput  *Bateria = NULL;
static FuzzyInput  *Taxa    = NULL;
static FuzzyOutput *GPRS    = NULL;

/* Guardamos os ponteiros de cada regra criada para podermos liberar a
 * memória corretamente em FuzzyController_Deinit() */
static FuzzyRuleAntecedent *g_antecedentes[NUM_REGRAS];
static FuzzyRuleConsequent *g_consequentes[NUM_REGRAS];
static FuzzyRule           *g_regras[NUM_REGRAS];
static int g_num_regras_criadas = 0;

static int g_inicializado = 0;

/* --------------------------------------------------------------------- */
/* Funções auxiliares privadas                                           */
/* --------------------------------------------------------------------- */

/**
 * @brief Cria e adiciona ao sistema fuzzy uma regra do tipo:
 *        SE (entradaA E entradaB) ENTÃO saida
 * @retval 0 em caso de sucesso, -1 se alguma alocação falhar
 */
static int adicionar_regra(int indice, FuzzySet *entradaA, FuzzySet *entradaB, FuzzySet *saida)
{
    if (g_num_regras_criadas >= NUM_REGRAS)
    {
        return -1;
    }

    FuzzyRuleAntecedent *antecedente = FuzzyRuleAntecedent_create();
    if (antecedente == NULL)
    {
        return -1;
    }
    FuzzyRuleAntecedent_joinWithAND_FuzzySet(antecedente, entradaA, entradaB);

    FuzzyRuleConsequent *consequente = FuzzyRuleConsequent_create();
    if (consequente == NULL)
    {
        FuzzyRuleAntecedent_destroy(antecedente);
        return -1;
    }
    FuzzyRuleConsequent_addOutput(consequente, saida);

    FuzzyRule *regra = FuzzyRule_create(indice, antecedente, consequente);
    if (regra == NULL)
    {
        FuzzyRuleConsequent_destroy(consequente);
        FuzzyRuleAntecedent_destroy(antecedente);
        return -1;
    }

    if (!Fuzzy_addFuzzyRule(fuzzy, regra))
    {
        FuzzyRule_destroy(regra);
        FuzzyRuleConsequent_destroy(consequente);
        FuzzyRuleAntecedent_destroy(antecedente);
        return -1;
    }

    g_antecedentes[g_num_regras_criadas] = antecedente;
    g_consequentes[g_num_regras_criadas] = consequente;
    g_regras[g_num_regras_criadas]       = regra;
    g_num_regras_criadas++;

    return 0;
}

/* --------------------------------------------------------------------- */
/* API pública                                                           */
/* --------------------------------------------------------------------- */

float FuzzyController_NormalizaTaxa(float rssi)
{
    /* Equivalente ao map(valor, 0, 31, 0, 100) do Arduino */
    return (rssi / 31.0f) * 100.0f;
}

int FuzzyController_Init(void)
{
    if (g_inicializado)
    {
        return 0;
    }

    fuzzy = Fuzzy_create();
    if (fuzzy == NULL)
    {
        return -1;
    }

    /* ---- Conjuntos fuzzy: bateria ------------------------------------ */
    Breserva            = FuzzySet_create(0.0f, 0.0f, 15.0f, 25.0f);
    Bextremamente_baixa  = FuzzySet_create(15.0f, 25.0f, 25.0f, 40.0f);
    Bbaixa               = FuzzySet_create(25.0f, 40.0f, 40.0f, 57.5f);
    Bmedia               = FuzzySet_create(40.0f, 57.5f, 57.5f, 75.0f);
    Balta                = FuzzySet_create(57.5f, 75.0f, 100.0f, 100.0f);

    /* ---- Conjuntos fuzzy: taxa de acerto (já em escala 0-100) -------- */
    Qmuitobaixa = FuzzySet_create(0.0f, 0.0f,
                                   FuzzyController_NormalizaTaxa(9.0f),
                                   FuzzyController_NormalizaTaxa(12.0f));
    Qbaixa      = FuzzySet_create(FuzzyController_NormalizaTaxa(9.0f),
                                   FuzzyController_NormalizaTaxa(12.0f),
                                   FuzzyController_NormalizaTaxa(12.0f),
                                   FuzzyController_NormalizaTaxa(17.0f));
    Qalta       = FuzzySet_create(FuzzyController_NormalizaTaxa(12.0f),
                                   FuzzyController_NormalizaTaxa(17.0f),
                                   FuzzyController_NormalizaTaxa(17.0f),
                                   FuzzyController_NormalizaTaxa(25.5f));
    Qmuitoalta  = FuzzySet_create(FuzzyController_NormalizaTaxa(17.0f),
                                   FuzzyController_NormalizaTaxa(25.5f),
                                   100.0f, 100.0f);

    /* ---- Conjuntos fuzzy: saída (decisão de ligar o GPRS) ------------ */
    NaoLiga = FuzzySet_create(0.0f, 0.0f, 20.0f, 80.0f);
    Liga    = FuzzySet_create(20.0f, 80.0f, 100.0f, 100.0f);

    if (Breserva == NULL || Bextremamente_baixa == NULL || Bbaixa == NULL ||
        Bmedia == NULL || Balta == NULL || Qmuitobaixa == NULL ||
        Qbaixa == NULL || Qalta == NULL || Qmuitoalta == NULL ||
        NaoLiga == NULL || Liga == NULL)
    {
        return -1;
    }

    /* ---- Entrada 1: Bateria ------------------------------------------ */
    Bateria = FuzzyInput_create(FUZZY_INPUT_BATERIA);
    if (Bateria == NULL)
    {
        return -1;
    }
    FuzzyInput_addFuzzySet(Bateria, Breserva);
    FuzzyInput_addFuzzySet(Bateria, Bextremamente_baixa);
    FuzzyInput_addFuzzySet(Bateria, Bbaixa);
    FuzzyInput_addFuzzySet(Bateria, Bmedia);
    FuzzyInput_addFuzzySet(Bateria, Balta);
    Fuzzy_addFuzzyInput(fuzzy, Bateria);

    /* ---- Entrada 2: Taxa de acerto do GPRS --------------------------- */
    Taxa = FuzzyInput_create(FUZZY_INPUT_TAXA);
    if (Taxa == NULL)
    {
        return -1;
    }
    FuzzyInput_addFuzzySet(Taxa, Qmuitobaixa);
    FuzzyInput_addFuzzySet(Taxa, Qbaixa);
    FuzzyInput_addFuzzySet(Taxa, Qalta);
    FuzzyInput_addFuzzySet(Taxa, Qmuitoalta);
    Fuzzy_addFuzzyInput(fuzzy, Taxa);

    /* ---- Saída: decisão de ligar o GPRS ------------------------------- */
    GPRS = FuzzyOutput_create(FUZZY_OUTPUT_GPRS);
    if (GPRS == NULL)
    {
        return -1;
    }
    FuzzyOutput_addFuzzySet(GPRS, Liga);
    FuzzyOutput_addFuzzySet(GPRS, NaoLiga);
    Fuzzy_addFuzzyOutput(fuzzy, GPRS);

    /* ---- Regras -------------------------------------------------------
     * Regras 1 (Breserva): prioriza NÃO ligar o GPRS, independentemente
     * da taxa de acerto, para preservar a bateria em nível de reserva.  */
    if (adicionar_regra(1, Breserva, Qmuitoalta, Liga)    != 0) return -1;
    if (adicionar_regra(2, Breserva, Qalta,      NaoLiga) != 0) return -1;
    if (adicionar_regra(3, Breserva, Qbaixa,     NaoLiga) != 0) return -1;
    if (adicionar_regra(4, Breserva, Qmuitobaixa,NaoLiga) != 0) return -1;

    /* Regras 2 (Bextremamente_baixa) */
    if (adicionar_regra(5, Bextremamente_baixa, Qmuitoalta, Liga)    != 0) return -1;
    if (adicionar_regra(6, Bextremamente_baixa, Qalta,      Liga)    != 0) return -1;
    if (adicionar_regra(7, Bextremamente_baixa, Qbaixa,     NaoLiga) != 0) return -1;
    if (adicionar_regra(8, Bextremamente_baixa, Qmuitobaixa,NaoLiga) != 0) return -1;

    /* Regras 3 (Bbaixa) */
    if (adicionar_regra(9,  Bbaixa, Qmuitoalta, Liga)    != 0) return -1;
    if (adicionar_regra(10, Bbaixa, Qalta,      Liga)    != 0) return -1;
    if (adicionar_regra(11, Bbaixa, Qbaixa,     NaoLiga) != 0) return -1;
    if (adicionar_regra(12, Bbaixa, Qmuitobaixa,NaoLiga) != 0) return -1;

    /* Regras 4 (Bmedia) */
    if (adicionar_regra(13, Bmedia, Qmuitoalta, Liga)    != 0) return -1;
    if (adicionar_regra(14, Bmedia, Qalta,      Liga)    != 0) return -1;
    if (adicionar_regra(15, Bmedia, Qbaixa,     Liga)    != 0) return -1;
    if (adicionar_regra(16, Bmedia, Qmuitobaixa,NaoLiga) != 0) return -1;

    /* Regras 5 (Balta) */
    if (adicionar_regra(17, Balta, Qmuitoalta, Liga)    != 0) return -1;
    if (adicionar_regra(18, Balta, Qalta,      Liga)    != 0) return -1;
    if (adicionar_regra(19, Balta, Qbaixa,     Liga)    != 0) return -1;
    if (adicionar_regra(20, Balta, Qmuitobaixa,NaoLiga) != 0) return -1;

    g_inicializado = 1;
    return 0;
}

float FuzzyController_Run(float bateria_pct, float taxa_normalizada)
{
    if (!g_inicializado)
    {
        return -1.0f;
    }

    Fuzzy_setInput(fuzzy, FUZZY_INPUT_BATERIA, bateria_pct);
    Fuzzy_setInput(fuzzy, FUZZY_INPUT_TAXA, taxa_normalizada);

    Fuzzy_fuzzify(fuzzy);

    return Fuzzy_defuzzify(fuzzy, FUZZY_OUTPUT_GPRS);
}

int FuzzyController_DecideLigarGPRS(float saida_defuzzificada)
{
    return (saida_defuzzificada >= FUZZY_GPRS_THRESHOLD) ? 1 : 0;
}

int FuzModelo_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{
    float saida;

    if (!g_inicializado)
    {
        return 0;
    }

    saida = FuzzyController_Run(bateria_pct, taxa_normalizada);

    return FuzzyController_DecideLigarGPRS(saida);
}

void FuzzyController_Deinit(void)
{
    int i;

    if (!g_inicializado)
    {
        return;
    }

    for (i = 0; i < g_num_regras_criadas; i++)
    {
        FuzzyRule_destroy(g_regras[i]);
        FuzzyRuleConsequent_destroy(g_consequentes[i]);
        FuzzyRuleAntecedent_destroy(g_antecedentes[i]);
    }
    g_num_regras_criadas = 0;

    if (GPRS != NULL)    { FuzzyOutput_destroy(GPRS);    GPRS = NULL; }
    if (Taxa != NULL)    { FuzzyInput_destroy(Taxa);     Taxa = NULL; }
    if (Bateria != NULL) { FuzzyInput_destroy(Bateria);  Bateria = NULL; }

    if (Liga != NULL)            { FuzzySet_destroy(Liga);            Liga = NULL; }
    if (NaoLiga != NULL)         { FuzzySet_destroy(NaoLiga);         NaoLiga = NULL; }
    if (Qmuitoalta != NULL)      { FuzzySet_destroy(Qmuitoalta);      Qmuitoalta = NULL; }
    if (Qalta != NULL)           { FuzzySet_destroy(Qalta);           Qalta = NULL; }
    if (Qbaixa != NULL)          { FuzzySet_destroy(Qbaixa);          Qbaixa = NULL; }
    if (Qmuitobaixa != NULL)     { FuzzySet_destroy(Qmuitobaixa);     Qmuitobaixa = NULL; }
    if (Balta != NULL)           { FuzzySet_destroy(Balta);           Balta = NULL; }
    if (Bmedia != NULL)          { FuzzySet_destroy(Bmedia);          Bmedia = NULL; }
    if (Bbaixa != NULL)          { FuzzySet_destroy(Bbaixa);          Bbaixa = NULL; }
    if (Bextremamente_baixa != NULL) { FuzzySet_destroy(Bextremamente_baixa); Bextremamente_baixa = NULL; }
    if (Breserva != NULL)        { FuzzySet_destroy(Breserva);        Breserva = NULL; }

    if (fuzzy != NULL) { Fuzzy_destroy(fuzzy); fuzzy = NULL; }

    g_inicializado = 0;
}