#include <vector>
#include <string>
#include <iostream>
using namespace std;

#include "Instruction.hpp"

Instruction::Instruction() {
  this->section = "main";
  this->value = 0;
  this->memIndex = 2000;
  this->register1 = 34;
  this->register2 = 34;
  this->register3 = 34;
  this->offset = -1;
  this->offsetReg = -1;
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

void Instruction::setValue(int value) {
  this->value = value;
}

void Instruction::setOffset(int value) {
  this->offset = value;
}

void Instruction::setOffsetReg(std::string reg) {
  this->offsetReg = stoi(reg.substr(1,reg.size()));;
}

void Instruction::setMemIndex(size_t index) {
  this->memIndex = index;
}

void Instruction::setSection(std::string section) {
  this->section = section;
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

Instruction& Instruction::operator=(Instruction& other) {
  this->opcode = other.getOpcode();
  this->section = other.getSection();
  this->value = other.getValue();
  this->memIndex = other.getMemIndex();
  this->register1 = other.getR1();
  this->register2 = other.getR2();
  this->register3 = other.getR3();
  return *this;
}
