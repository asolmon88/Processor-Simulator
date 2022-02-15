#include "Simulator.hpp"
#include <iostream>
#include <unistd.h>

Simulator::Simulator() {
  this->finished = 0;
  this->cycles = 0;
  this->numberInstructions = 0;
  this->PC = -1;
}

void Simulator::fetch() {
  ++PC;
  currentInstruction = instructions[PC];
  ++cycles;
}
void Simulator::decode() {
  ++cycles;
}

void Simulator::execute() {
  std::string opcode = currentInstruction.getOpcode();
  /* std::cout << "CURRENT: " << opcode << std::endl;
  std::cout << "\nFLAG: " << flag << std::endl; */
  if (opcode == "ld") {
    load();
  } else if (opcode == "str") {
    store();
  } else if (opcode == "mov") {
    move();
  } else if (opcode == "add") {
    add();
  } else if (opcode == "sub") {
    substract();
  } else if (opcode == "mult") {
    multiply();
  } else if (opcode == "cmp") {
    compare();
  } else if (opcode == "jmp") {
    jump();
  } else if (opcode == "je") {
    jumpEqual();
  } else if (opcode == "ja") {
    jumpAbove();
  } else if (opcode == "jb") {
    jumpBelow();
  } else if (opcode == "end") {
    end();
  }
}

void Simulator::load() {
  registers[currentInstruction.getR1()] = 
    this->memory[currentInstruction.getMemIndex()];
}
void Simulator::store() {
  memory[currentInstruction.getMemIndex()] =
    registers[currentInstruction.getR1()];
}
void Simulator::move() {
  if (currentInstruction.getR1() > 0 && 
    currentInstruction.getR2() > 0) {
    registers[currentInstruction.getR2()] =
      registers[currentInstruction.getR1()]; 
  } else {
    registers[currentInstruction.getR2()] = currentInstruction.getValue();
  }
}
void Simulator::add() {
  registers[currentInstruction.getR3()] =
   registers[currentInstruction.getR2()] + registers[currentInstruction.getR1()];
}
void Simulator::substract() {
  registers[currentInstruction.getR3()] =
   registers[currentInstruction.getR2()] - registers[currentInstruction.getR1()];
}
void Simulator::multiply() {
  registers[currentInstruction.getR3()] =
   registers[currentInstruction.getR2()] * registers[currentInstruction.getR1()];
}
void Simulator::compare() {
  flag = registers[currentInstruction.getR2()] -
    registers[currentInstruction.getR1()];
}
void Simulator::jump() {
  PC = find(currentInstruction.getSection())-1;
}
void Simulator::jumpEqual() {
  if (!flag) {
    jump();
  }
}
void Simulator::jumpAbove() {
  if (flag > 0) {
    jump();
  }
}
void Simulator::jumpBelow() {
  if (flag < 0) {
    jump();
  }
}
void Simulator::end() {
  finished = 1;
}

int Simulator::find(const std::string& passed) {
  int startingIndex = 0;
  for (int i = 0; i < sections.size(); ++i) {
    if (sections[i].name == passed) {
      startingIndex = sections[i].index;
    }
  }
  return startingIndex;
}

void Simulator::simulate() {
  while(!finished) {
    fetch();
    decode();
    execute();
  }
}
