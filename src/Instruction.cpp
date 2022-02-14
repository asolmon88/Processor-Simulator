#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Instruction.hpp"

Instruction::Instruction() {
  this->section = "main";
  this->value = 0;
  this->memIndex = 6000;
  this->register1 = 0;
  this->register2 = 0;
  this->register3 = 0;
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
  this->register3 = stoll(reg3.substr(1,reg3.size()));
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

std::string Instruction::getOpcode() {return this->opcode;}

std::string Instruction::getSection() {return this->section;}

int Instruction::getValue() {return this->value;}

size_t Instruction::getMemIndex() {return this->memIndex;}

size_t Instruction::getR1() {return this->register1;}

size_t Instruction::getR2() {return this->register2;}

size_t Instruction::getR3() {return this->register3;}

ostream& Instruction::print(ostream& output) {
  output << "OP code: " << this->opcode << endl;
  output << "Section: " << this->section << endl;
  output << "Value: " << this->value << endl;
  output << "Mem index: " << this->memIndex <<endl;
  output << "R1: " << this->register1 << endl;
  output << "R2: " << this->register2 << endl;
  output << "R3: " << this->register3 << endl;
  output << endl;
  return output;
}
