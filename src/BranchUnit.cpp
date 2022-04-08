#include "BranchUnit.hpp"

BranchUnit::BranchUnit() {
  this->callCycles = 0;
  this->jmpCycles = 0;
  this->jaCycles = 0;
  this->jbCycles = 0;
  this->jeCycles = 0;
}

void BranchUnit::jump(int& PC, Instruction& currentInstruction,
  std::vector<Section_t>& sections) {
  PC = find(currentInstruction.getSection(), sections)-1;
}
void BranchUnit::jumpEqual(int& PC, Instruction& currentInstruction,
  int& flag, std::vector<Section_t>& sections, int& cycles) {
  if (!flag) {
    if (!this->jeCycles) {
      this->jeCycles = 1;
      ++cycles;
    } else {
      jump(PC, currentInstruction, sections);
      this->jeCycles = 0;
    }
  }
}
void BranchUnit::jumpAbove(int& PC, Instruction& currentInstruction,
  int& flag, std::vector<Section_t>& sections, int& cycles) {
  if (flag > 0) {
    if (!this->jaCycles) {
      this->jaCycles = 1;
      ++cycles;
    } else {
      jump(PC, currentInstruction, sections);
      this->jaCycles = 0;
    }
  }
}
void BranchUnit::jumpBelow(int& PC, Instruction& currentInstruction,
  int& flag, std::vector<Section_t>& sections, int& cycles) {
  if (flag < 0) {
    if (!this->jbCycles) {
      this->jbCycles = 1;
      ++cycles;
    } else {
      jump(PC, currentInstruction, sections);
      this->jbCycles = 0;
    }
  }
}
void BranchUnit::call(int& PC, Instruction& currentInstruction,
  std::vector<int>& calls,
  std::vector<std::vector<Register_t>>& callRegister,
  std::vector<Register_t> registers,
  std::vector<Section_t>& sections) {
  calls.push_back(PC);
  callRegister.push_back(registers);
  jump(PC, currentInstruction, sections);
}

int BranchUnit::find(const std::string& passed, std::vector<Section_t>& sections) {
  int startingIndex = 0;
  for (size_t i = 0; i < sections.size(); ++i) {
    if (sections[i].name == passed) {
      startingIndex = sections[i].index;
    }
  }
  return startingIndex;
}