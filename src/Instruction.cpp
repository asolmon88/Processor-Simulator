#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Instruction.hpp"

Instruction::Instruction() {
  this->memIndex = -1;
}

void Instruction::setOpcode(std::string opcode) {
  this->opcode = opcode;
}

void Instruction::setReg1(std::string reg1) {
  this->register1 = stoll(reg1.substr(1,reg1.size()));
}

void Instruction::setReg2(std::string reg2) {
  this->register2 = stoll(reg2.substr(1,reg2.size()));
}

void Instruction::setReg3(std::string reg3) {
  this->register1 = stoll(reg3.substr(1,reg3.size()));
}

void Instruction::setValue(size_t value) {
  this->value = value;
}

void Instruction::setMemIndex(size_t index) {
  this->memIndex = index;
}

void Instruction::setSection(std::string section) {
  this->section = section;
}

int Instruction::changeToMem() {

}
