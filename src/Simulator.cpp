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
  // cout << currentInstructions[0].getOpcode() << "\t";
}
void Simulator::decode() {
  Instruction* currentInstruction = &currentInstructions[1];
  /* std::cout << "\nFLAG: " << flag << std::endl;
  sleep(1); */
  if (currentInstruction->getOpcode() == "jmp" ||
    currentInstruction->getOpcode() == "ja" ||
    currentInstruction->getOpcode() == "je" ||
    currentInstruction->getOpcode() == "jb") {
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
    load();
  } else if (opcode == "str") {
    store();
  } else if (opcode == "mov") {
    move();
  } else if (opcode == "add") {
    ALU::add(this->registers, this->currentInstruction);
  } else if (opcode == "sub") {
    ALU::substract(this->registers, this->currentInstruction);
  } else if (opcode == "mult") {
    ALU::multiply(this->registers, this->currentInstruction);
  } else if (opcode == "cmp") {
    ALU::compare(this->registers, this->currentInstruction, this->flag);
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
    ++cycles;
  }
  // std::cout << this->currentInstruction.getOpcode() << std::endl;
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
  /* printRegisters();
  sleep(10); */
  jump();
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
  /* cout << "Passing from" << endl;
  cout << this->callRegister[
    this->callRegister.size()-1][0] << endl;
  cout << "To" << endl;
  cout << this->callRegister[
    this->callRegister.size()-2][0] << endl;
  cout << "---------------------------" << endl << endl; */
  //sleep(3);
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
