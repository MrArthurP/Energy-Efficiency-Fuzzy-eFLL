/**
  ******************************************************************************
  * @file           : qda_model_wrapper.c
  * @brief          : Implementacao do wrapper de "QdaModel" (ver
  *                    qda_model_wrapper.h). Usa a API publica gerada pelo
  *                    X-CUBE-AI para qda_model.h.
  *
  * ATENCAO: este arquivo NAO e gerado pelo X-CUBE-AI - pode ser editado
  * manualmente sem risco de ser sobrescrito numa nova geracao do modelo.
  ******************************************************************************
  */
#include "qda_model_wrapper.h"
#include "qda_model.h"
#include "qda_model_data.h"

#include <stddef.h>
#include <stdint.h>

static ai_handle s_qda_model_network = AI_HANDLE_NULL;
static ai_error  s_qda_model_ultimo_erro;

/* Buffer de ativacao da rede - tamanho definido em qda_model_data.h.
 * Os pesos NAO precisam de buffer proprio aqui: o endereco deles ja vem
 * embutido no descritor gerado em qda_model_data.c. */
AI_ALIGNED(AI_QDA_MODEL_ACTIVATIONS_ALIGNMENT)
static ai_u8 s_qda_model_activations[AI_QDA_MODEL_DATA_ACTIVATIONS_SIZE];

/* Buffers de entrada/saida reutilizados a cada chamada */
static ai_float s_qda_model_in[2];
static int32_t s_qda_model_out1[1];
static float s_qda_model_out2[4];


int QdaModel_Init(void)
{
    const ai_handle ativacoes[] = { s_qda_model_activations };

    s_qda_model_ultimo_erro = ai_qda_model_create_and_init(
        &s_qda_model_network, ativacoes, NULL);

    return (s_qda_model_ultimo_erro.type == AI_ERROR_NONE) ? 0 : -1;
}


int QdaModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada)
{
    ai_buffer *ai_input;
    ai_buffer *ai_output;
    ai_i32 lotes_processados;

    if (s_qda_model_network == AI_HANDLE_NULL)
    {
        return -1; /* QdaModel_Init nao foi chamado (ou falhou) */
    }

    s_qda_model_in[0] = (ai_float)bateria_pct;
    s_qda_model_in[1] = (ai_float)taxa_normalizada;

    ai_input  = ai_qda_model_inputs_get(s_qda_model_network, NULL);
    ai_output = ai_qda_model_outputs_get(s_qda_model_network, NULL);

    ai_input[0].data = AI_HANDLE_PTR(s_qda_model_in);
    ai_output[0].data = AI_HANDLE_PTR(s_qda_model_out1);
    ai_output[1].data = AI_HANDLE_PTR(s_qda_model_out2);

    lotes_processados = ai_qda_model_run(s_qda_model_network, ai_input, ai_output);
    if (lotes_processados != 1)
    {
        s_qda_model_ultimo_erro = ai_qda_model_get_error(s_qda_model_network);
        return -1;
    }

    /* A rede ja calcula a classe prevista (saida 1, formato S32);
     * assume-se a mesma convencao 0 = nao liga / 1 = liga usada no
     * treinamento do modelo - confira se bate com o seu dataset. */
    return (int)s_qda_model_out1[0];
}
