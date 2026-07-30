#include "eFLL_wrapper.h"
#include "Fuzzy.h"
#include "FuzzyInput.h"
#include "FuzzyOutput.h"
#include "FuzzyRule.h"
#include "FuzzySet.h"
#include "FuzzyRuleAntecedent.h"
#include "FuzzyRuleConsequent.h"

extern "C" {

Fuzzy *Fuzzy_create(void) {
    return new Fuzzy();
}

void Fuzzy_destroy(Fuzzy *instance) {
    delete instance;
}

FuzzyInput *FuzzyInput_create(int index) {
    return new FuzzyInput(index);
}

void FuzzyInput_destroy(FuzzyInput *instance) {
    delete instance;
}

bool FuzzyInput_addFuzzySet(FuzzyInput *instance, FuzzySet *set) {
    return instance->addFuzzySet(set);
}

FuzzyOutput *FuzzyOutput_create(int index) {
    return new FuzzyOutput(index);
}

void FuzzyOutput_destroy(FuzzyOutput *instance) {
    delete instance;
}

bool FuzzyOutput_addFuzzySet(FuzzyOutput *instance, FuzzySet *set) {
    return instance->addFuzzySet(set);
}

FuzzySet *FuzzySet_create(float a, float b, float c, float d) {
    return new FuzzySet(a, b, c, d);
}

void FuzzySet_destroy(FuzzySet *instance) {
    delete instance;
}

FuzzyRuleAntecedent *FuzzyRuleAntecedent_create(void) {
    return new FuzzyRuleAntecedent();
}

void FuzzyRuleAntecedent_destroy(FuzzyRuleAntecedent *instance) {
    delete instance;
}

bool FuzzyRuleAntecedent_joinSingle(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet) {
    return instance->joinSingle(fuzzySet);
}

bool FuzzyRuleAntecedent_joinWithAND_FuzzySet(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet1, FuzzySet *fuzzySet2) {
    return instance->joinWithAND(fuzzySet1, fuzzySet2);
}

bool FuzzyRuleAntecedent_joinWithOR_FuzzySet(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet1, FuzzySet *fuzzySet2) {
    return instance->joinWithOR(fuzzySet1, fuzzySet2);
}

bool FuzzyRuleAntecedent_joinWithAND_FuzzySet_Antecedent(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet, FuzzyRuleAntecedent *antecedent) {
    return instance->joinWithAND(fuzzySet, antecedent);
}

bool FuzzyRuleAntecedent_joinWithOR_FuzzySet_Antecedent(FuzzyRuleAntecedent *instance, FuzzySet *fuzzySet, FuzzyRuleAntecedent *antecedent) {
    return instance->joinWithOR(fuzzySet, antecedent);
}

bool FuzzyRuleAntecedent_joinWithAND_Antecedent_FuzzySet(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent, FuzzySet *fuzzySet) {
    return instance->joinWithAND(antecedent, fuzzySet);
}

bool FuzzyRuleAntecedent_joinWithOR_Antecedent_FuzzySet(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent, FuzzySet *fuzzySet) {
    return instance->joinWithOR(antecedent, fuzzySet);
}

bool FuzzyRuleAntecedent_joinWithAND_Antecedent(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent1, FuzzyRuleAntecedent *antecedent2) {
    return instance->joinWithAND(antecedent1, antecedent2);
}

bool FuzzyRuleAntecedent_joinWithOR_Antecedent(FuzzyRuleAntecedent *instance, FuzzyRuleAntecedent *antecedent1, FuzzyRuleAntecedent *antecedent2) {
    return instance->joinWithOR(antecedent1, antecedent2);
}

FuzzyRuleConsequent *FuzzyRuleConsequent_create(void) {
    return new FuzzyRuleConsequent();
}

void FuzzyRuleConsequent_destroy(FuzzyRuleConsequent *instance) {
    delete instance;
}

bool FuzzyRuleConsequent_addOutput(FuzzyRuleConsequent *instance, FuzzySet *set) {
    return instance->addOutput(set);
}

FuzzyRule *FuzzyRule_create(int index, FuzzyRuleAntecedent *antecedent, FuzzyRuleConsequent *consequent) {
    return new FuzzyRule(index, antecedent, consequent);
}

void FuzzyRule_destroy(FuzzyRule *instance) {
    delete instance;
}

bool Fuzzy_addFuzzyInput(Fuzzy *instance, FuzzyInput *input) {
    return instance->addFuzzyInput(input);
}

bool Fuzzy_addFuzzyOutput(Fuzzy *instance, FuzzyOutput *output) {
    return instance->addFuzzyOutput(output);
}

bool Fuzzy_addFuzzyRule(Fuzzy *instance, FuzzyRule *rule) {
    return instance->addFuzzyRule(rule);
}

bool Fuzzy_setInput(Fuzzy *instance, int fuzzyInputIndex, float crispValue) {
    return instance->setInput(fuzzyInputIndex, crispValue);
}

bool Fuzzy_fuzzify(Fuzzy *instance) {
    return instance->fuzzify();
}

float Fuzzy_defuzzify(Fuzzy *instance, int fuzzyOutputIndex) {
    return instance->defuzzify(fuzzyOutputIndex);
}

}
