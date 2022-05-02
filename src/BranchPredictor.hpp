#ifndef PROCESSOR_BRANCHPREDICTOR_
#define PROCESSOR_BRANCHPREDICTOR_

#include "Instruction.hpp"

#include <vector>

class BranchPredictor {
 public:
  int* PC;
  std::vector<Instruction>* executing;
  std::vector<std::vector<Instruction>>* currentInstructions;
  Instruction* currentInstruction;
  int* OoO;

  BranchPredictor();
  int checkPrediction(int i, int branchExecuted);
  int correctPrediction();

  void setAtributes(int& PC, std::vector<Instruction>& executing,
    std::vector<std::vector<Instruction>>& currentInstructions,
    Instruction& currentInstruction, int& OoO);

};

#endif //PROCESSOR_BRANCHPREDICTOR_
