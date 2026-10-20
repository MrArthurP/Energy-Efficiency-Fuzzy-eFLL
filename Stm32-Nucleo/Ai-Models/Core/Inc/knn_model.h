/**
  ******************************************************************************
  * @file           : knn_model.h
  * @brief          : Interface do modelo de Machine Learning - KNN
  *                    (K-Nearest Neighbors) - ajustado via Controlador Fuzzy
  *                    para decidir ligar ou nao o GPRS da coleira de
  *                    monitoramento de animais silvestres, com base na carga
  *                    da bateria e na taxa de acerto no envio de dados via
  *                    GPRS/satelite.
  *
  * Portado via knn_to_c.py do modelo treinado em Python (sklearn) para C puro.
  * Os dados de treino ficam em memoria somente leitura (flash); nao ha
  * alocacao dinamica.
  *
  * Parametros do modelo: 296 amostras, K = 4, metrica euclidiana,
  * pesos uniformes, pre-processamento embutido: nao.
  ******************************************************************************
  */
#ifndef KNN_MODEL_H
#define KNN_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Numero de features de entrada do modelo */
#define KNN_MODEL_NUM_INPUTS  2
/* Numero de amostras de treino armazenadas no modelo */
#define KNN_MODEL_NUM_SAMPLES 296
/* Numero de vizinhos considerados na votacao */
#define KNN_MODEL_K           4
/* Numero de classes de saida */
#define KNN_MODEL_NUM_CLASSES 2

/* Configuracao do modelo (informativa) */
#define KNN_MODEL_WEIGHTS_DISTANCE 0
#define KNN_MODEL_P_NORM           2   /* 2 = euclidiana, 1 = manhattan */
#define KNN_MODEL_HAS_PREPROCESS   0

/* Indices das classes devolvidos por KnnModel_Score() */
#define KNN_MODEL_CLASSE_NAO_LIGA 0
#define KNN_MODEL_CLASSE_LIGA     1

/* Rotulos originais das classes, indexados pelo valor devolvido por Score */
extern const char *const KnnModel_ClassNames[KNN_MODEL_NUM_CLASSES];

/**
 * @brief  Executa o modelo (portado via knn_to_c.py do modelo treinado em
 *         Python/sklearn) e retorna o indice da classe prevista.
 * @param  input: vetor com KNN_MODEL_NUM_INPUTS valores de entrada BRUTOS
 *                (mesma ordem e unidade do treinamento); a normalizacao,
 *                quando existe, e feita dentro da funcao.
 * @retval indice da classe prevista (0 .. KNN_MODEL_NUM_CLASSES-1), igual a
 *         posicao em model.classes_ do sklearn
 */
int KnnModel_Score(const float *input);

/**
 * @brief  Wrapper de conveniencia: executa o modelo e retorna diretamente
 *         a decisao binaria.
 * @param  bateria_pct: percentual de carga da bateria (0-100)
 * @param  taxa_normalizada: taxa de acerto normalizada (0-100)
 * @retval 1 se o GPRS deve ser ligado, 0 caso contrario
 */
int KnnModel_DecideLigarGPRS(float bateria_pct, float taxa_normalizada);

#ifdef __cplusplus
}
#endif

#endif /* KNN_MODEL_H */
