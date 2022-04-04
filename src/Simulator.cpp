#include "Simulator.hpp"
#include <iostream>
#include <unistd.h>
// benchmarks, recursive, factorial, sorting, 4 more or less
// stage 4 how many, as many as you want to portray in the slides
Simulator::Simulator() {
  this->finished = 0;
  this->cycles = 0;
  this->numberInstructions = 0;
  this->PC = -1;
  this->branch = 0;
  Register first(0,0);
  registers.push_back(first);

  for (int i = 1; i < 32; ++i) {
    Register reg((i*32)-32, (i*32)-1);
    registers.push_back(reg);
  }
  Instruction temp;
  temp.setOpcode("");
  for (int i = 0; i < 3; ++i) {
    currentInstructions.push_back(temp);
  }
}

void Simulator::fetch() {
  ++PC;
  if (branch) {
    currentInstructions[0].setOpcode("wait"); // this is how it waits
    branch = 0;
    PC -= 2;
    ++cycles;
  } else if (PC < (int)instructions.size()) {
    currentInstructions[0] = instructions[PC];
    ++cycles;
  } else {
    currentInstructions[0].setOpcode("done");
  }
  cout << currentInstructions[0].getOpcode() << "\t";
}
void Simulator::decode() {
  Instruction* currentInstruction = &currentInstructions[1];
  /* std::cout << "\nFLAG: " << flag << std::endl;
  sleep(1); */
  if (currentInstruction->getOpcode() == "jmp" ||
    currentInstruction->getOpcode() == "ja" ||
    currentInstruction->getOpcode() == "je" ||
    currentInstruction->getOpcode() == "jb" ||
    currentInstruction->getOpcode() == "call") {
    currentInstructions[0].setOpcode("wait"); // this is how it waits
    this->branch = 1;
  }
  if (currentInstruction->getOpcode() == "end") {
    ++cycles;
  }
  // std::cout << currentInstruction->getOpcode() << "\t";
}

void Simulator::execute() {
  this->currentInstruction = currentInstructions[2];
  std::string opcode = this->currentInstruction.getOpcode();
  if (opcode == "ld") {
    LSUnit::load(this->registers, currentInstruction, this->memory);
  } else if (opcode == "str") {
    LSUnit::store(this->registers, currentInstruction, this->memory);
  } else if (opcode == "mov") {
    LSUnit::move(this->registers, currentInstruction, this->memory);
  } else if (opcode == "add") {
    ALU::add(this->registers, this->currentInstruction);
  } else if (opcode == "sub") {
    ALU::substract(this->registers, this->currentInstruction);
  } else if (opcode == "mult") {
    ALU::multiply(this->registers, this->currentInstruction);
  } else if (opcode == "cmp") {
    ALU::compare(this->registers, this->currentInstruction, this->flag);
  } else if (opcode == "jmp") {
    BranchUnit::jump(this->PC, currentInstruction, this->sections);
  } else if (opcode == "je") {
    BranchUnit::jumpEqual(this->PC, currentInstruction, this->flag, this->sections);
  } else if (opcode == "ja") {
    BranchUnit::jumpAbove(this->PC, currentInstruction, this->flag, this->sections);
  } else if (opcode == "jb") {
    BranchUnit::jumpBelow(this->PC, currentInstruction, this->flag, this->sections);
  } else if (opcode == "call") {
    BranchUnit::call(this->PC, currentInstruction, this->calls, this->callRegister,
      this->registers, this->sections);
  } else if (opcode == "end") {
    end();
    ++cycles;
  }
  // std::cout << this->currentInstruction.getOpcode() << std::endl;
}

void Simulator::end() {
  /* printRegisters();
  sleep(10); */
  if (!this->calls.empty()) {
    checkRegisters();
    PC = this->calls.back();
    this->registers = this->callRegister.back();
    this->calls.pop_back();
    this->callRegister.pop_back();
  } else {
    finished = 1;
  }
}

void Simulator::checkRegisters() {
  (this->callRegister[this->callRegister.size()-1])[0].value = this->registers[0].value;
}

void Simulator::printRegisters() {
  for(int i = 0; i < 12; ++i) {
    cout << "R" << i << "\t";
  }
  cout << endl;
  for(int i = 0; i < 12; ++i) {
    cout << this->registers[i].value << "\t";
  }
  cout << endl;
}

void Simulator::simulate() {
  while(!finished) {
    fetch();
    decode();
    execute();
    currentInstructions[2] = currentInstructions[1];
    currentInstructions[1] = currentInstructions[0];
  }
}
