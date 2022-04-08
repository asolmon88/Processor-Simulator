#include "ALU.hpp"

ALU::ALU() {
  this->multiplyCycles = 0;
  this->compareCycles = 0;
}

void ALU::add(std::vector<Register_t>& registers,
    Instruction& currentInstruction) {
  registers[currentInstruction.getR3()].value =
   registers[currentInstruction.getR2()] + registers[currentInstruction.getR1()];
  /* std::cout << "R" << currentInstruction.getR3() << ": " << registers[currentInstruction.getR3()] << std::endl;
  std::cout << "R" << currentInstruction.getR2() << ": " << registers[currentInstruction.getR2()] << std::endl;
  std::cout << "R" << currentInstruction.getR1() << ": " << registers[currentInstruction.getR1()] << std::endl; */
}
void ALU::substract(std::vector<Register_t>& registers,
    Instruction& currentInstruction) {
  registers[currentInstruction.getR3()].value =
   registers[currentInstruction.getR2()] - registers[currentInstruction.getR1()];
}
void ALU::multiply(std::vector<Register_t>& registers,
    Instruction& currentInstruction) {
  registers[currentInstruction.getR3()].value =
   registers[currentInstruction.getR2()] * registers[currentInstruction.getR1()];
}
void ALU::compare(std::vector<Register_t>& registers,
    Instruction& currentInstruction, int& flag) {
  flag = registers[currentInstruction.getR2()] -
    registers[currentInstruction.getR1()];
}