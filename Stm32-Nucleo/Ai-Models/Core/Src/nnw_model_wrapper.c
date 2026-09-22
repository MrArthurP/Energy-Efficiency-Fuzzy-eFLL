/**
  ******************************************************************************
  * @file           : nnw_model_wrapper.c
  * @brief          : Implementacao do wrapper de "NnwModel" (ver
  *                    nnw_model_wrapper.h). Usa a API publica gerada pelo
  *                    X-CUBE-AI para nnw_model.h.
  *
  * ATENCAO: este arquivo NAO e gerado pelo X-CUBE-AI - pode ser editado
  * manualmente sem risco de ser sobrescrito numa nova geracao do modelo.
  ******************************************************************************
  */
#include "nnw_model_wrapper.h"
#include "nnw_model.h"
#include "nnw_model_data.h"

#include <stddef.h>
#include <stdint.h>

static ai_handle s_nnw_model_network = AI_HANDLE_NULL;
static ai_error  s_nnw_model_ultimo_erro;

/* Buffer de ativacao da rede - tamanho definido em nnw_model_data.h.
 * Os pesos NAO precisam de buffer proprio aqui: o endereco deles ja vem
 * embutido no descritor gerado em nnw_model_data.c. */
AI_ALIGNED(AI_NNW_MODEL_ACTIVATIONS_ALIGNMENT)
static ai_u8 s_nnw_model_activations[AI_NNW_MODEL_DATA_ACTIVATIONS_SIZE];

/* Buffers de entrada/saida reutilizados a cada chamada */
static ai_float s_nnw_model_in[2];
static int32_t s_nnw_model_out1[1];
static float s_nnw_model_out2[4];


int NnwModel_Init(void)
{
    const ai_handle ativacoes[] = { s_nnw_model_activations };

    s_nnw_model_ultimo_erro = ai_nnw_model_create_and_init(
        &s_nnw_model_network, ativacoes, NULL);

    return (s_nnw_model_ultimo_erro.type == AI_ERROR_NONE) ? 0 : -1;
}


int NnwModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{
    ai_buffer *ai_input;
    ai_buffer *ai_output;
    ai_i32 lotes_processados;

    if (s_nnw_model_network == AI_HANDLE_NULL)
    {
        return -1; /* NnwModel_Init nao foi chamado (ou falhou) */
    }

    s_nnw_model_in[0] = (ai_float)bateria_pct;
    s_nnw_model_in[1] = (ai_float)taxa_normalizada;

    ai_input  = ai_nnw_model_inputs_get(s_nnw_model_network, NULL);
    ai_output = ai_nnw_model_outputs_get(s_nnw_model_network, NULL);

    ai_input[0].data = AI_HANDLE_PTR(s_nnw_model_in);
    ai_output[0].data = AI_HANDLE_PTR(s_nnw_model_out1);
    ai_output[1].data = AI_HANDLE_PTR(s_nnw_model_out2);

    lotes_processados = ai_nnw_model_run(s_nnw_model_network, ai_input, ai_output);
    if (lotes_processados != 1)
    {
        s_nnw_model_ultimo_erro = ai_nnw_model_get_error(s_nnw_model_network);
        return -1;
    }

    /* A rede ja calcula a classe prevista (saida 1, formato S32);
     * assume-se a mesma convencao 0 = nao liga / 1 = liga usada no
     * treinamento do modelo - confira se bate com o seu dataset. */
    return (int)s_nnw_model_out1[0];
}
