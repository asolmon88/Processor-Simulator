#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Instruction.hpp"

Instruction::Instruction(std::string& instruction) {
  int initialPos = instruction.find(" ");
  this->opcode = instruction.substr(0, initialPos);
  cout << "OPcode: " << this->opcode << "espacio" << endl;
  int endPos = instruction.find(",", initialPos);
  this->register1 = stoll(instruction.substr(initialPos, endPos));
  cout << "REG1: " << this->register1 << "espacio" << endl;
}

void Instruction::setReg1(size_t reg1) {

}

void Instruction::setReg2(size_t reg1) {

}

void Instruction::setReg3(size_t reg1) {

}

int Instruction::changeToMem() {

}
