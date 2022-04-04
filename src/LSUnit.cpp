#include "LSUnit.hpp"

void LSUnit::load(std::vector<Register_t>& registers,
  Instruction& currentInstruction, std::vector<int>& memory) {
  registers[currentInstruction.getR1()].value = 
    memory[currentInstruction.getMemIndex()];
}
void LSUnit::store(std::vector<Register_t>& registers,
  Instruction& currentInstruction, std::vector<int>& memory) {
  memory[currentInstruction.getMemIndex()] =
    registers[currentInstruction.getR1()].value;
}
void LSUnit::move(std::vector<Register_t>& registers,
  Instruction& currentInstruction, std::vector<int>& memory) {
  //cout << currentInstruction << endl;
  if (currentInstruction.getOffset()) {
    int index = currentInstruction.getOffset()/4;
    Register_t* currentRegister = &registers[currentInstruction.getR2()];
    if (currentInstruction.offsetInFront()) {
      currentRegister = &registers[currentInstruction.getR1()];
      if (index <= currentRegister->endIndex) {
        registers[currentInstruction.getR2()].value =
          memory[currentRegister->startIndex+index];
      }
    } else {
      if (index <= currentRegister->endIndex) {
        if (currentInstruction.getR1() < 34 &&
          currentInstruction.getR2() < 34) {
          memory[currentRegister->startIndex+index] =
            registers[currentInstruction.getR1()].value;
        } else {
          //cout << *currentRegister;
          //cout << "valor: " << currentRegister->startIndex << endl;
          memory[currentRegister->startIndex+index] =
            currentInstruction.getValue();
        }
      }
    }
  } else if (currentInstruction.getOffsetReg()) {
    int index = registers[currentInstruction.getOffsetReg()].value/4;
    Register_t* currentRegister = &registers[currentInstruction.getR2()];
    if (currentInstruction.offsetInFront()) {
      currentRegister = &registers[currentInstruction.getR1()];
      if (index <= currentRegister->endIndex) {
        registers[currentInstruction.getR2()].value =
          memory[currentRegister->startIndex+index];
      }
    } else {
      if (index <= currentRegister->endIndex) {
        if (currentInstruction.getR1() < 34 &&
          currentInstruction.getR2() < 34) {
          memory[currentRegister->startIndex+index] =
            registers[currentInstruction.getR1()].value;
        } else {
          memory[currentRegister->startIndex+index] =
            currentInstruction.getValue();
        }
      }
    }
  } else {
    if (currentInstruction.getR1() < 34 &&
      currentInstruction.getR2() < 34) {
      registers[currentInstruction.getR2()].value =
        registers[currentInstruction.getR1()].value; 
    } else {
      registers[currentInstruction.getR2()].value = currentInstruction.getValue();
    }
  }
}