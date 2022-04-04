#include "BranchUnit.hpp"

void BranchUnit::jump(int& PC, Instruction& currentInstruction,
  std::vector<Section_t>& sections) {
  PC = find(currentInstruction.getSection(), sections)-1;
}
void BranchUnit::jumpEqual(int& PC, Instruction& currentInstruction,
  int& flag, std::vector<Section_t>& sections) {
  if (!flag) {
    jump(PC, currentInstruction, sections);
  }
}
void BranchUnit::jumpAbove(int& PC, Instruction& currentInstruction,
  int& flag, std::vector<Section_t>& sections) {
  if (flag > 0) {
    jump(PC, currentInstruction, sections);
  }
}
void BranchUnit::jumpBelow(int& PC, Instruction& currentInstruction,
  int& flag, std::vector<Section_t>& sections) {
  if (flag < 0) {
    jump(PC, currentInstruction, sections);
  }
}
void BranchUnit::call(int& PC, Instruction& currentInstruction,
  std::vector<int>& calls,
  std::vector<std::vector<Register_t>>& callRegister,
  std::vector<Register_t> registers,
  std::vector<Section_t>& sections) {
  calls.push_back(PC);
  callRegister.push_back(registers);
  /* printRegisters();
  sleep(10); */
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