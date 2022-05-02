#include "BranchPredictor.hpp"

BranchPredictor::BranchPredictor() {
  this->PC = nullptr;
  this->currentInstructions = nullptr;
  this->executing = nullptr;
  this->currentInstruction = nullptr;
}
int BranchPredictor::checkPrediction(int i, int branchExecuted) {
  if (branchExecuted) {
    if (correctPrediction()) {
      (*this->PC) = currentInstruction->currentPC;
      return 1;
    } else {
      (*this->PC) = currentInstruction->previousPC;
      for (int k = 0; k < 4; ++k) {
        for (int j = 0; j < 2; ++j) {
          (*this->currentInstructions)[j][k].setOpcode("wait");
        }
      }
      for (int j = i; j < (int)this->executing->size(); ++j) {
        (*this->executing)[j].setOpcode("wait");
      }
      if ((*this->OoO)) {
        (*this->OoO) = 0;
      }
    }
  } else {
    (*this->PC) = currentInstruction->previousPC;
    for (int k = 0; k < 4; ++k) {
      for (int j = 0; j < 2; ++j) {
        (*this->currentInstructions)[j][k].setOpcode("wait");
      }
    }
    for (int j = i; j < (int)this->executing->size(); ++j) {
      (*this->executing)[j].setOpcode("wait");
    }
    if ((*this->OoO)) {
      (*this->OoO) = 0;
    }
  }
  return 0;
}
int BranchPredictor::correctPrediction() {
  return *(this->PC) == currentInstruction->predPC;
}

void BranchPredictor::setAtributes(int& PC, std::vector<Instruction>& executing,
                                   std::vector<std::vector<Instruction>>& currentInstructions,
                                   Instruction& currentInstruction,
                                   int& OoO) {
  this->PC = &PC;
  this->executing = &executing;
  this->currentInstructions = &currentInstructions;
  this->currentInstruction = &currentInstruction;
  this->OoO = &OoO;
}
