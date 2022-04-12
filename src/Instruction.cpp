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
  this->offset = 0;
  this->offsetReg = 0;
  this->hasOffsetInFront = 0;
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

void Instruction::setOffsetLocation(int location) {
  this->hasOffsetInFront = location;
}

std::string Instruction::getOpcode() {return this->opcode;}

std::string Instruction::getSection() {return this->section;}

int Instruction::getValue() {return this->value;}

size_t Instruction::getMemIndex() {return this->memIndex;}

size_t Instruction::getR1() {return this->register1;}

size_t Instruction::getR2() {return this->register2;}

size_t Instruction::getR3() {return this->register3;}

int Instruction::getOffset() {return this->offset;}

int Instruction::getOffsetReg() {return this->offsetReg;}

int Instruction::offsetInFront() {return this->hasOffsetInFront;}

ostream& Instruction::print(ostream& output) {
  output << "OP code: " << this->opcode << endl;
  output << "Section: " << this->section << endl;
  output << "Value: " << this->value << endl;
  output << "Mem index: " << this->memIndex <<endl;
  output << "R1: " << this->register1 << endl;
  output << "R2: " << this->register2 << endl;
  output << "R3: " << this->register3 << endl;
  output << "Offset: " << this->offset << endl;
  output << "Offset Index: " << this->offsetReg << endl;
  output << "Offset in front: " << this->hasOffsetInFront << endl;
  output << endl;
  return output;
}

int Instruction::operator!=(Instruction& other) {
  if (this->opcode != other.getOpcode() ||
    this->register1 != other.getR1() ||
    this->register2 != other.getR2() ||
    this->register3 != other.getR3()) {
    return 1;
  }
  return 0;
}
