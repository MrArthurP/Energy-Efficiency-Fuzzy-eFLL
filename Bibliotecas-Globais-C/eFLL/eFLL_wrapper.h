#ifndef EFLL_WRAPPER_H
#define EFLL_WRAPPER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Fuzzy Fuzzy;
typedef struct FuzzyInput FuzzyInput;
typedef struct FuzzyOutput FuzzyOutput;
typedef struct FuzzyRule FuzzyRule;
typedef struct FuzzySet FuzzySet;
typedef struct FuzzyRuleAntecedent FuzzyRuleAntecedent;
typedef struct FuzzyRuleConsequent FuzzyRuleConsequent;

Fuzzy *Fuzzy_create(void);
void Fuzzy_destroy(Fuzzy *instance);

FuzzyInput *FuzzyInput_create(int index);
void FuzzyInput_destroy(FuzzyInput *instance);
bool FuzzyInput_addFuzzySet(FuzzyInput *instance, FuzzySet *set);

FuzzyOutput *FuzzyOutput_create(int index);
void FuzzyOutput_destroy(FuzzyOutput *instance);
bool FuzzyOutput_addFuzzySet(FuzzyOutput *instance, FuzzySet *set);

FuzzySet *FuzzySet_create(float a, float b, float c, float d);
void FuzzySet_destroy(FuzzySet *instance);

FuzzyRuleAntecedent *FuzzyRuleAntecedent_create(void);
void FuzzyRuleAntecedent_destroy(FuzzyRuleAntecedent *instance);
bool FuzzyRuleAntecedent_joinSingle(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet);
bool FuzzyRuleAntecedent_joinWithAND_FuzzySet(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet1, FuzzySet *fuzzySet2);
bool FuzzyRuleAntecedent_joinWithOR_FuzzySet(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet1, FuzzySet *fuzzySet2);
bool FuzzyRuleAntecedent_joinWithAND_FuzzySet_Antecedent(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet, FuzzyRuleAntecedent *antecedent);
bool FuzzyRuleAntecedent_joinWithOR_FuzzySet_Antecedent(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet, FuzzyRuleAntecedent *antecedent);
bool FuzzyRuleAntecedent_joinWithAND_Antecedent_FuzzySet(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent, FuzzySet *fuzzySet);
bool FuzzyRuleAntecedent_joinWithOR_Antecedent_FuzzySet(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent, FuzzySet *fuzzySet);
bool FuzzyRuleAntecedent_joinWithAND_Antecedent(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent1, FuzzyRuleAntecedent *antecedent2);
bool FuzzyRuleAntecedent_joinWithOR_Antecedent(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent1, FuzzyRuleAntecedent *antecedent2);

FuzzyRuleConsequent *FuzzyRuleConsequent_create(void);
void FuzzyRuleConsequent_destroy(FuzzyRuleConsequent *instance);
bool FuzzyRuleConsequent_addOutput(FuzzyRuleConsequent *instance, FuzzySet *set);

FuzzyRule *FuzzyRule_create(int index, FuzzyRuleAntecedent *antecedent, FuzzyRuleConsequent *consequent);
void FuzzyRule_destroy(FuzzyRule *instance);

bool Fuzzy_addFuzzyInput(Fuzzy *instance, FuzzyInput *input);
bool Fuzzy_addFuzzyOutput(Fuzzy *instance, FuzzyOutput *output);
bool Fuzzy_addFuzzyRule(Fuzzy *instance, FuzzyRule *rule);
bool Fuzzy_setInput(Fuzzy *instance, int fuzzyInputIndex, float crispValue);
bool Fuzzy_fuzzify(Fuzzy *instance);
float Fuzzy_defuzzify(Fuzzy *instance, int fuzzyOutputIndex);

#ifdef __cplusplus
}
#endif

#endif // EFLL_WRAPPER_H
