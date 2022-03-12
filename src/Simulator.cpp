#include "Simulator.hpp"
#include <iostream>
#include <unistd.h>

Simulator::Simulator() {
  this->finished = 0;
  this->cycles = 0;
  this->numberInstructions = 0;
  this->PC = -1;
  Register first(0,0);
  registers.push_back(first);

  for (int i = 1; i < 32; ++i) {
    Register reg((i*32)-32, (i*32)-1);
    registers.push_back(reg);
  }
}

void Simulator::fetch() {
  ++PC;
  currentInstruction = instructions[PC];
  ++cycles;
}
void Simulator::decode() {
  std::string opcode = currentInstruction.getOpcode();
  /* std::cout << "CURRENT: " << opcode << std::endl;
  std::cout << "\nFLAG: " << flag << std::endl;
  sleep(1); */
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
  } else if (opcode == "call") {
    call();
  } else if (opcode == "end") {
    end();
  }
  ++cycles;
}

void Simulator::execute() {
  ++cycles;
}

void Simulator::load() {
  registers[currentInstruction.getR1()].value = 
    this->memory[currentInstruction.getMemIndex()];
}
void Simulator::store() {
  memory[currentInstruction.getMemIndex()] =
    registers[currentInstruction.getR1()].value;
}
void Simulator::move() {
  //cout << currentInstruction << endl;
  if (currentInstruction.getOffset()) {
    int index = currentInstruction.getOffset()/4;
    Register_t* currentRegister = &registers[currentInstruction.getR2()];
    if (currentInstruction.offsetInFront()) {
      currentRegister = &registers[currentInstruction.getR1()];
      if (index <= currentRegister->endIndex) {
        this->registers[currentInstruction.getR2()].value =
          this->memory[currentRegister->startIndex+index];
      }
    } else {
      if (index <= currentRegister->endIndex) {
        if (currentInstruction.getR1() < 34 &&
          currentInstruction.getR2() < 34) {
          this->memory[currentRegister->startIndex+index] =
            registers[currentInstruction.getR1()].value;
        } else {
          //cout << *currentRegister;
          //cout << "valor: " << currentRegister->startIndex << endl;
          this->memory[currentRegister->startIndex+index] =
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
        this->registers[currentInstruction.getR2()].value =
          this->memory[currentRegister->startIndex+index];
      }
    } else {
      if (index <= currentRegister->endIndex) {
        if (currentInstruction.getR1() < 34 &&
          currentInstruction.getR2() < 34) {
          this->memory[currentRegister->startIndex+index] =
            registers[currentInstruction.getR1()].value;
        } else {
          this->memory[currentRegister->startIndex+index] =
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
void Simulator::add() {
  registers[currentInstruction.getR3()].value =
   registers[currentInstruction.getR2()] + registers[currentInstruction.getR1()];
  /* std::cout << "R" << currentInstruction.getR3() << ": " << registers[currentInstruction.getR3()] << std::endl;
  std::cout << "R" << currentInstruction.getR2() << ": " << registers[currentInstruction.getR2()] << std::endl;
  std::cout << "R" << currentInstruction.getR1() << ": " << registers[currentInstruction.getR1()] << std::endl; */
}
void Simulator::substract() {
  registers[currentInstruction.getR3()].value =
   registers[currentInstruction.getR2()] - registers[currentInstruction.getR1()];
}
void Simulator::multiply() {
  registers[currentInstruction.getR3()].value =
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
void Simulator::call() {
  this->calls.push_back(this->PC);
  this->callRegister.push_back(this->registers);
  jump();
}
void Simulator::end() {
  if (!this->calls.empty()) {
    if (this->calls.size() >= 2) {
      checkRegisters();
    }
    PC = this->calls.back();
    this->registers = this->callRegister.back();
    this->calls.pop_back();
  } else {
    finished = 1;
  }
}

int Simulator::find(const std::string& passed) {
  int startingIndex = 0;
  for (size_t i = 0; i < sections.size(); ++i) {
    if (sections[i].name == passed) {
      startingIndex = sections[i].index;
    }
  }
  return startingIndex;
}

void Simulator::checkRegisters() {
  std::vector<Register_t>* register1 = &(this->callRegister[
    this->callRegister.size()-1]);
  std::vector<Register_t>* register2 = &(this->callRegister[
    this->callRegister.size()-2]);
  (*register2)[0].value = (*register1)[0].value;
  cout << "Valor que se pasa " << (*register1)[0] << endl;
}

void Simulator::simulate() {
  while(!finished) {
    fetch();
    decode();
    execute();
  }
}
