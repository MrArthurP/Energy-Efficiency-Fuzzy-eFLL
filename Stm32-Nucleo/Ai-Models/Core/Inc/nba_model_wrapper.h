/**
  ******************************************************************************
  * @file           : nba_model_wrapper.h
  * @brief          : Wrapper de conveniencia para o modelo "NbaModel", gerado
  *                    pelo X-CUBE-AI a partir de nba_model.h. Esconde a
  *                    API nativa (ai_handle, ai_buffer, ai_error) atras da
  *                    mesma interface usada pelos demais modelos do projeto.
  *
  * ATENCAO: este arquivo NAO e gerado pelo X-CUBE-AI (ao contrario de
  * nba_model.h) - pode ser editado manualmente sem risco de ser
  * sobrescrito numa nova geracao do modelo.
  ******************************************************************************
  */
#ifndef NBA_MODEL_WRAPPER_H
#define NBA_MODEL_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Aloca os buffers de ativacao e inicializa a rede "NbaModel".
 *         Deve ser chamada uma unica vez antes do primeiro uso de
 *         NbaModel_DecideLigarGPRS. Diferente dos modelos do m2cgen
 *         (stateless), este modelo e stateful: mantem um handle de rede
 *         e buffers estaticos entre chamadas.
 * @retval 0 em sucesso, -1 em falha
 */
int NbaModel_Init(void);

/**
 * @brief  Executa a rede "NbaModel" sobre (bateria_pct, taxa_normalizada)
 *         e retorna a decisao binaria ja calculada pelo X-CUBE-AI
 *         (a rede devolve a classe prevista diretamente).
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario, -1 em erro de
 *         execucao (rede nao inicializada ou falha no ai_run)
 */
int NbaModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* NBA_MODEL_WRAPPER_H */
