// Controlador Fuzzy para ligar ou não o GPRS de um dispositivo baseado na carga da bateria e na taxa de consumo de energia

// importando a biblioteca Fuzzy
#include<Fuzzy.h>
// definindo o pino de reset do Arduino para reiniciar o programa
#define RESET asm ("jmp (0x0000)")

// contador para imprimir o número de linhas de simulação no console
int contador = 0;

// Criando o objeto Fuzzy

Fuzzy *fuzzy = new Fuzzy();

// Criando os conjuntos fuzzy para as entradas e saída

// os parâmetros  da função 'Fuzzyset' são: (início do conjunto, início do pico, fim do pico, fim do conjunto). O pico é a região onde a pertinência é máxima (1), e o início e fim do conjunto são as regiões onde a pertinência começa a aumentar ou diminuir.

// Uma das especificações ao construir a lógica fuzzy é garantir que a transição entre os conjuntos seja suave, ou seja, que haja uma sobreposição entre eles. Isso é importante para evitar decisões abruptas e garantir que o sistema possa lidar com incertezas de forma eficaz.

// Conjuntos fuzzy relacionados à bateria
// Bateria reserva: 0% a 5% 
FuzzySet *Breserva = new FuzzySet(0, 0, 0, 5);
// Bateria extremamente_baixa: 5% a 20%
FuzzySet *Bextremamente_baixa = new FuzzySet(5, 10, 10, 20);
// Bateria baixa: 15% a 45%
FuzzySet *Bbaixa = new FuzzySet(15, 30, 40, 45);
// Bateria media: 35% a 70%
FuzzySet *Bmedia = new FuzzySet(35, 50, 50, 70);
// Bateria alta: 60% a 100%
FuzzySet *Balta = new FuzzySet(60, 75, 75, 101);

// Conjuntos fuzzy relacionados à taxa de acerto no envio de dados do GPRS

// O GPRS tem uma taxa de acerto que pode variar de 0 a 1 (ou 0% a 100%), e isso influencia na decisão de ligar ou não o GPRS para economizar energia. A taxa é enviada pelo satélite, identificada por nome específico dos dados enviados, além de longitude e latitude.

// Taxa baixíssima: 0% a 20%
FuzzySet *Tbaixissima = new FuzzySet(0, 10, 10, 20);
// Taxa baixa: 10% a 50%
FuzzySet *Tbaixa = new FuzzySet(10, 30, 30, 50);
// Taxa aceitável: 40% a 80%
FuzzySet *Taceitavel = new FuzzySet(40, 55, 55, 80);
// Taxa alta: 70% a 100%
FuzzySet *Talta = new FuzzySet(70, 90, 90, 100);

// Conjuntos fuzzy relacionados à decisão de ligar ou não o GPRS

// Esse conjunto representa a função fuzzy de saída, que será usado para a classificação final da decisão de ligar ou não o GPRS. O valor de saída é uma porcentagem que indica a pertinência da decisão de ligar o GPRS, onde 0% significa "não ligar" e 100% significa "ligar".

// Liga: 0 a 100, com pico em 33,3 a 66,6.
FuzzySet *Liga = new FuzzySet(0, (1/3)*100, (1/3)*100, (2/3)*100);
// Não liga: 33,3 a 100, com pico em 66,6 a 100.
FuzzySet *NaoLiga = new FuzzySet((1/3)*100, (2/3)*100, (2/3)*100, (3/3)*100+1);

// o valor de saída será um valor entre 0 e 100, que posteriormente poderá ser normalizado para uma classificação binária (0 ou 1). O valor ideal (treashold) é calculado usando o método de defuzzificação, que leva em consideração as pertinências dos conjuntos fuzzy de saída para determinar um valor final.

// função para imprimir as colunas do console, para facilitar a visualização dos resultados da simulação

// Nesse caso temos um total de 20 regras fuzzy: 4 regras para cada um dos 5 conjuntos fuzzy de entrada (Breserva, Bextremamente_baixa, Bbaixa, Bmedia, Balta) combinados com os 4 conjuntos fuzzy de taxa (Tbaixissima, Tbaixa, Taceitavel, Talta). Cada regra é representada por uma combinação específica dos conjuntos fuzzy de entrada e uma decisão de saída (Liga ou NaoLiga).

void imprime_colunas() {
  Serial.begin(9600);
  Serial.println("");
  Serial.print("index");
  Serial.print(",");
  Serial.print("Regra1_1");
  Serial.print(",");
  Serial.print("Regra1_2");
  Serial.print(",");
  Serial.print("Regra1_3");
  Serial.print(",");
  Serial.print("Regra1_4");
  Serial.print(",");
  Serial.print("Regra2_1");
  Serial.print(",");
  Serial.print("Regra2_2");
  Serial.print(",");
  Serial.print("Regra2_3");
  Serial.print(",");
  Serial.print("Regra2_4");
  Serial.print(",");
  Serial.print("Regra3_1");
  Serial.print(",");
  Serial.print("Regra3_2");
  Serial.print(",");
  Serial.print("Regra3_3");
  Serial.print(",");
  Serial.print("Regra3_4");
  Serial.print(",");
  Serial.print("Regra4_1");
  Serial.print(",");
  Serial.print("Regra4_2");
  Serial.print(",");
  Serial.print("Regra4_3");
  Serial.print(",");
  Serial.print("Regra4_4");
  Serial.print(",");
  Serial.print("Regra5_1");
  Serial.print(",");
  Serial.print("Regra5_2");
  Serial.print(",");
  Serial.print("Regra5_3");
  Serial.print(",");
  Serial.print("Regra5_4");
  Serial.print(",");
  Serial.print("Entrada1");
  Serial.print(",");
  Serial.print("Entrada2");
  Serial.print(",");
  Serial.print("PertinenciaLiga");
  Serial.print(",");
  Serial.println("PertinenciaNãoLiga");
  Serial.print(",");
  Serial.println("Saída");
  Serial.print(",");
  Serial.println("Estado de Saída");
}

// A função setup é responsável por configurar o ambiente de simulação, definindo os conjuntos fuzzy para as entradas e saída, e criando as regras fuzzy que governam a lógica de decisão do sistema. 
void setup() { 

  // Imprime as colunas do console para facilitar a visualização dos resultados da simulação
  imprime_colunas();

  // Inicializa a semente do gerador de números aleatórios usando um valor lido de um pino analógico, para garantir que as simulações sejam diferentes a cada execução.
  randomSeed(analogRead(0));

  // Criando os conjuntos fuzzy para as entradas e saída

  // criação de um ponteiro para um objeto FuzzyInput, que representa a entrada de carga da bateria. O número 1 passado como argumento é o identificador dessa entrada, que será usado posteriormente para definir as regras fuzzy.
  FuzzyInput *Bateria = new FuzzyInput(1);
  Bateria->addFuzzySet(Breserva);
  Bateria->addFuzzySet(Bextremamente_baixa);
  Bateria->addFuzzySet(Bbaixa);
  Bateria->addFuzzySet(Bmedia);
  Bateria->addFuzzySet(Balta);
  
  // Adiciona a entrada de carga da bateria ao sistema fuzzy, permitindo que as regras fuzzy possam usar essa entrada para tomar decisões.
  fuzzy->addFuzzyInput(Bateria);

  // repete-se o mesmo processo para a segunda entrada, que representa a taxa de acerto no envio de dados do GPRS. O número 2 é o identificador dessa entrada. O mesmo método é aplicado para o conjunto de saída.
  FuzzyInput *Taxa = new FuzzyInput(2);
  Taxa->addFuzzySet(Tbaixissima);
  Taxa->addFuzzySet(Tbaixa);
  Taxa->addFuzzySet(Taceitavel);
  Taxa->addFuzzySet(Talta);

  fuzzy->addFuzzyInput(Taxa);

  FuzzyOutput *GPRS = new FuzzyOutput(1);
  GPRS->addFuzzySet(Liga);
  GPRS->addFuzzySet(NaoLiga);
  fuzzy->addFuzzyOutput(GPRS); 

// Definição das regras fuzzy

// As regras fuzzy são criadas usando objetos FuzzyRuleAntecedent para definir as condições de entrada, e objetos FuzzyRuleConsequent para definir as ações de saída. Cada regra é então adicionada ao sistema fuzzy usando o método addFuzzyRule.

// O objetivo é fazer que, quanto menos bateria houver, menor será o consumo pelo sistema, fazendo com que, ao invés da bateria ser consumida de forma linear, o consumo adquire uma característica exponencial.

// Regras 1

// Para a construção das regras Fuzzy, optou-se por priorizar a garantia do funcionamento da coleira quando a bateria estiver no nivel crítico. Dessa forma, os pesquisadores poderão ir até o local aproximado do animal para recuperar a coleira e os dados coletados usando **Triangulação??**, mesmo que a taxa de acerto no envio de dados do GPRS seja alta. Por isso, as regras 1_1, 1_2, 1_3 e 1_4 priorizam a decisão de não ligar o GPRS quando a bateria estiver em nível de reserva, independentemente da taxa de acerto no envio de dados do GPRS, pois isso pode ajudar a economizar energia e prolongar a vida útil da bateria, aumentando as chances de recuperar a coleira e os dados coletados.

FuzzyRuleAntecedent *regras1_1 = new FuzzyRuleAntecedent();
regras1_1->joinWithAND(Breserva, Talta);
FuzzyRuleConsequent *saida_regra1_1 = new FuzzyRuleConsequent();
saida_regra1_1->addOutput(NaoLiga);
FuzzyRule *regra1_1 = new FuzzyRule(1, regras1_1, saida_regra1_1);
fuzzy->addFuzzyRule(regra1_1);

FuzzyRuleAntecedent *regras1_2 = new FuzzyRuleAntecedent();
regras1_2->joinWithAND(Breserva, Taceitavel);
FuzzyRuleConsequent *saida_regra1_2 = new FuzzyRuleConsequent();
saida_regra1_2->addOutput(NaoLiga);
FuzzyRule *regra1_2 = new FuzzyRule(2, regras1_2, saida_regra1_2);
fuzzy->addFuzzyRule(regra1_2);

FuzzyRuleAntecedent *regras1_3 = new FuzzyRuleAntecedent();
regras1_3->joinWithAND(Breserva, Tbaixa);
FuzzyRuleConsequent *saida_regra1_3 = new FuzzyRuleConsequent();
saida_regra1_3->addOutput(NaoLiga);
FuzzyRule *regra1_3 = new FuzzyRule(3, regras1_3, saida_regra1_3);
fuzzy->addFuzzyRule(regra1_3);

FuzzyRuleAntecedent *regras1_4 = new FuzzyRuleAntecedent();
regras1_4->joinWithAND(Breserva, Tbaixissima);
FuzzyRuleConsequent *saida_regra1_4 = new FuzzyRuleConsequent();
saida_regra1_4->addOutput(NaoLiga);
FuzzyRule *regra1_4 = new FuzzyRule(4, regras1_4, saida_regra1_4);
fuzzy->addFuzzyRule(regra1_4);

// Regras 2

// O conjunto de regras 2 prioriza a decisão de ligar o GPRS quando a bateria estiver em nível extremamente baixo, independentemente da taxa de acerto no envio de dados do GPRS, pois isso pode ajudar a garantir que os dados sejam enviados antes que a bateria se esgote completamente, aumentando as chances de recuperar a coleira e os dados coletados.
FuzzyRuleAntecedent *regras2_1 = new FuzzyRuleAntecedent();
regras2_1->joinWithAND(Bextremamente_baixa, Talta);
FuzzyRuleConsequent *saida_regra2_1 = new FuzzyRuleConsequent();
saida_regra2_1->addOutput(Liga);
FuzzyRule *regra2_1 = new FuzzyRule(5, regras2_1, saida_regra2_1);
fuzzy->addFuzzyRule(regra2_1);

FuzzyRuleAntecedent *regras2_2 = new FuzzyRuleAntecedent();
regras2_2->joinWithAND(Bextremamente_baixa, Taceitavel);
FuzzyRuleConsequent *saida_regra2_2 = new FuzzyRuleConsequent();
saida_regra2_2->addOutput(Liga);
FuzzyRule *regra2_2 = new FuzzyRule(6, regras2_2, saida_regra2_2);
fuzzy->addFuzzyRule(regra2_2);

FuzzyRuleAntecedent *regras2_3 = new FuzzyRuleAntecedent();
regras2_3->joinWithAND(Bextremamente_baixa, Tbaixa);
FuzzyRuleConsequent *saida_regra2_3 = new FuzzyRuleConsequent();
saida_regra2_3->addOutput(NaoLiga);
FuzzyRule *regra2_3 = new FuzzyRule(7, regras2_3, saida_regra2_3);
fuzzy->addFuzzyRule(regra2_3);

FuzzyRuleAntecedent *regras2_4 = new FuzzyRuleAntecedent();
regras2_4->joinWithAND(Bextremamente_baixa, Tbaixissima);
FuzzyRuleConsequent *saida_regra2_4 = new FuzzyRuleConsequent();
saida_regra2_4->addOutput(NaoLiga);
FuzzyRule *regra2_4 = new FuzzyRule(8, regras2_4, saida_regra2_4);
fuzzy->addFuzzyRule(regra2_4);

// Regras 3
// O conjunto de regras 3 realiza a decisão de ligar ou não o GPRS com base na combinação da bateria em nível baixo e a taxa de acerto no envio de dados do GPRS, priorizando a decisão de ligar o GPRS quando a taxa de acerto for alta ou aceitável, e a decisão de não ligar o GPRS quando a taxa de acerto for baixa ou baixíssima.
FuzzyRuleAntecedent *regras3_1 = new FuzzyRuleAntecedent();
regras3_1->joinWithAND(Bbaixa, Talta);
FuzzyRuleConsequent *saida_regra3_1 = new FuzzyRuleConsequent();
saida_regra3_1->addOutput(Liga);
FuzzyRule *fuzzyRule3_1 = new FuzzyRule(9, regras3_1, saida_regra3_1);
fuzzy->addFuzzyRule(fuzzyRule3_1);

FuzzyRuleAntecedent *regras3_2 = new FuzzyRuleAntecedent();
regras3_2->joinWithAND(Bbaixa, Taceitavel);
FuzzyRuleConsequent *saida_regra3_2 = new FuzzyRuleConsequent();
saida_regra3_2->addOutput(Liga);
FuzzyRule *fuzzyRule3_2 = new FuzzyRule(10, regras3_2, saida_regra3_2);
fuzzy->addFuzzyRule(fuzzyRule3_2);

FuzzyRuleAntecedent *regras3_3 = new FuzzyRuleAntecedent();
regras3_3->joinWithAND(Bbaixa, Tbaixa);
FuzzyRuleConsequent *saida_regra3_3 = new FuzzyRuleConsequent();
saida_regra3_3->addOutput(NaoLiga);
FuzzyRule *fuzzyRule3_3 = new FuzzyRule(11, regras3_3, saida_regra3_3);
fuzzy->addFuzzyRule(fuzzyRule3_3);

FuzzyRuleAntecedent *regras3_4 = new FuzzyRuleAntecedent();
regras3_4->joinWithAND(Bbaixa, Tbaixissima);
FuzzyRuleConsequent *saida_regra3_4 = new FuzzyRuleConsequent();
saida_regra3_4->addOutput(NaoLiga);
FuzzyRule *fuzzyRule3_4 = new FuzzyRule(12, regras3_4, saida_regra3_4);
fuzzy->addFuzzyRule(fuzzyRule3_4);

// Regras 4
// O conjunto de regras 4 realiza a decisão de ligar ou não o GPRS com base na combinação da bateria em nível médio e a taxa de acerto no envio de dados do GPRS, priorizando a decisão de ligar o GPRS quando a taxa de acerto for alta, aceitável ou baixa, e a decisão de não ligar o GPRS quando a taxa de acerto for baixíssima.
FuzzyRuleAntecedent *regras4_1 = new FuzzyRuleAntecedent();
regras4_1->joinWithAND(Bmedia, Talta);
FuzzyRuleConsequent *saida_regra4_1 = new FuzzyRuleConsequent();
saida_regra4_1->addOutput(Liga);
FuzzyRule *fuzzyRule4_1 = new FuzzyRule(13, regras4_1, saida_regra4_1);
fuzzy->addFuzzyRule(fuzzyRule4_1);

FuzzyRuleAntecedent *regras4_2 = new FuzzyRuleAntecedent();
regras4_2->joinWithAND(Bmedia, Taceitavel);
FuzzyRuleConsequent *saida_regra4_2 = new FuzzyRuleConsequent();
saida_regra4_2->addOutput(Liga);
FuzzyRule *fuzzyRule4_2 = new FuzzyRule(14, regras4_2, saida_regra4_2);
fuzzy->addFuzzyRule(fuzzyRule4_2);

FuzzyRuleAntecedent *regras4_3 = new FuzzyRuleAntecedent();
regras4_3->joinWithAND(Bmedia, Tbaixa);
FuzzyRuleConsequent *saida_regra4_3 = new FuzzyRuleConsequent();
saida_regra4_3->addOutput(Liga);
FuzzyRule *fuzzyRule4_3 = new FuzzyRule(15, regras4_3, saida_regra4_3);
fuzzy->addFuzzyRule(fuzzyRule4_3);

FuzzyRuleAntecedent *regras4_4 = new FuzzyRuleAntecedent();
regras4_4->joinWithAND(Bmedia, Tbaixissima);
FuzzyRuleConsequent *saida_regra4_4 = new FuzzyRuleConsequent();
saida_regra4_4->addOutput(NaoLiga);
FuzzyRule *fuzzyRule4_4 = new FuzzyRule(16, regras4_4, saida_regra4_4);
fuzzy->addFuzzyRule(fuzzyRule4_4);

// Regras 5
// O conjunto de regras 5 realiza a decisão de ligar ou não o GPRS com base na combinação da bateria em nível alto e a taxa de acerto no envio de dados do GPRS, priorizando a decisão de ligar o GPRS quando a taxa de acerto for alta, aceitável, baixa ou baixíssima, pois quando a bateria está em nível alto, o consumo de energia do GPRS pode ser menos preocupante, e garantir que os dados sejam enviados pode ser mais importante para recuperar a coleira e os dados coletados.
FuzzyRuleAntecedent *regras5_1_antecedente = new FuzzyRuleAntecedent();
regras5_1_antecedente->joinWithAND(Balta, Talta);
FuzzyRuleConsequent *regras5_1_consequente = new FuzzyRuleConsequent();
regras5_1_consequente->addOutput(Liga);
FuzzyRule *regra5_1 = new FuzzyRule(17, regras5_1_antecedente, regras5_1_consequente);
fuzzy->addFuzzyRule(regra5_1);

FuzzyRuleAntecedent *regras5_2_antecedente = new FuzzyRuleAntecedent();
regras5_2_antecedente->joinWithAND(Balta, Taceitavel);
FuzzyRuleConsequent *regras5_2_consequente = new FuzzyRuleConsequent();
regras5_2_consequente->addOutput(Liga);
FuzzyRule *regra5_2 = new FuzzyRule(18, regras5_2_antecedente, regras5_2_consequente);
fuzzy->addFuzzyRule(regra5_2);

FuzzyRuleAntecedent *regras5_3_antecedente = new FuzzyRuleAntecedent();
regras5_3_antecedente->joinWithAND(Balta, Tbaixa);
FuzzyRuleConsequent *regras5_3_consequente = new FuzzyRuleConsequent();
regras5_3_consequente->addOutput(Liga);
FuzzyRule *regra5_3 = new FuzzyRule(19, regras5_3_antecedente, regras5_3_consequente);
fuzzy->addFuzzyRule(regra5_3);

FuzzyRuleAntecedent *regras5_4_antecedente = new FuzzyRuleAntecedent();
regras5_4_antecedente->joinWithAND(Balta, Tbaixissima);
FuzzyRuleConsequent *regras5_4_consequente = new FuzzyRuleConsequent();
regras5_4_consequente->addOutput(NaoLiga);
FuzzyRule *regra5_4 = new FuzzyRule(20, regras5_4_antecedente, regras5_4_consequente);
fuzzy->addFuzzyRule(regra5_4);

}

// A função loop é responsável por executar a simulação, gerando valores aleatórios para as entradas (carga da bateria e taxa de acerto no envio de dados do GPRS), aplicando as regras fuzzy para determinar a decisão de ligar ou não o GPRS, e imprimindo os resultados no console. A simulação é executada repetidamente, com um atraso de 200 milissegundos entre cada iteração, para permitir a visualização dos resultados.

void loop() {
  
  // Gera 300 valores decrescentes entre 100 e 0
  if (contador < 300) {
    float passo = 100.0 / 299.0;
    float input1 = 100.0 - (contador * passo);
    int input2 = random(0, 100);  // mantém aleatório

    int out_class = 0;

    fuzzy->setInput(1, input1); // Bateria
    fuzzy->setInput(2, input2); // Taxa de acerto

    fuzzy->fuzzify();

    float output1 = fuzzy->defuzzify(1);

    if (Liga->getPertinence() > NaoLiga->getPertinence()){
      out_class = 1;
    }
    else{
      out_class = 0;
    }

  Serial.print(contador); //1_1
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(1)); //1_1
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(2)); //1_2
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(3)); //1_3
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(4)); //1_4
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(5)); //2_1
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(6)); //2_2
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(7)); //2_3
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(8)); //2_4
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(9)); //3_1
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(10)); //3_2
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(11)); //3_3
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(12)); //3_4
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(13)); //4_1
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(14)); //4_2
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(15)); //4_3
  Serial.print(","); 
  Serial.print(fuzzy->isFiredRule(16)); //4_4
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(17)); //5_1
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(18)); //5_2
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(19)); //5_3
  Serial.print(",");
  Serial.print(fuzzy->isFiredRule(20)); //5_4
  Serial.print(",");
  Serial.print(input1, 2);
  Serial.print(",");
  Serial.print(input2);
  Serial.print(",");
  Serial.print(Liga->getPertinence());
  Serial.print(",");
  Serial.print(NaoLiga->getPertinence());
  Serial.print(",");
  Serial.print(output1);
  Serial.print(",");
  Serial.println(out_class);
  contador++;
  delay(200);
  }
}