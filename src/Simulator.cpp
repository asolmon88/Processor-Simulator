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
  this->busy = 0;
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
  std::cout << currentInstruction->getOpcode() << "\t";
}

void Simulator::execute() {
  this->currentInstruction = currentInstructions[2];
  std::string opcode = this->currentInstruction.getOpcode();
  if (opcode == "ld") {
    if (lsUnit.loadCycles == 0) {
      lsUnit.loadCycles = cycles+4;
    } else if (cycles < lsUnit.loadCycles) {
      ++cycles;
    } else {
      lsUnit.load(this->registers, currentInstruction, this->memory);
      lsUnit.loadCycles = 0;
    }
  } else if (opcode == "str") {
    lsUnit.store(this->registers, currentInstruction, this->memory);
  } else if (opcode == "mov") {
    lsUnit.move(this->registers, currentInstruction, this->memory);
  } else if (opcode == "add") {
    alu.add(this->registers, this->currentInstruction);
  } else if (opcode == "sub") {
    alu.substract(this->registers, this->currentInstruction);
  } else if (opcode == "mult") {
    if (!alu.multiplyCycles) {
      alu.multiplyCycles = 1;
      ++cycles;
    } else {
      alu.multiply(this->registers, this->currentInstruction);
      alu.multiplyCycles = 0;
    }
  } else if (opcode == "cmp") {
    if (!alu.compareCycles) {
      alu.compareCycles = 1;
      ++cycles;
    } else {
      alu.compare(this->registers, this->currentInstruction, this->flag);
      alu.compareCycles = 0;
    }
  } else if (opcode == "jmp") {
    branchUnit.jump(this->PC, currentInstruction, this->sections);
  } else if (opcode == "je") {
    branchUnit.jumpEqual(this->PC, currentInstruction, this->flag, this->sections,
      this->cycles);
  } else if (opcode == "ja") {
    branchUnit.jumpAbove(this->PC, currentInstruction, this->flag, this->sections,
      this->cycles);
  } else if (opcode == "jb") {
    branchUnit.jumpBelow(this->PC, currentInstruction, this->flag, this->sections,
      this->cycles);
  } else if (opcode == "call") {
    if (!branchUnit.callCycles) {
      branchUnit.callCycles = 1;
      ++cycles;
    } else {
      branchUnit.call(this->PC, currentInstruction, this->calls, this->callRegister,
        this->registers, this->sections);
      branchUnit.callCycles = 0;
    }
  } else if (opcode == "end") {
    end();
    ++cycles;
  }
  std::cout << this->currentInstruction.getOpcode() << std::endl;
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

void Simulator::setBusy() {
  if (lsUnit.loadCycles || alu.compareCycles || alu.multiplyCycles ||
    branchUnit.callCycles || branchUnit.jaCycles || branchUnit.jbCycles ||
    branchUnit.jeCycles || branchUnit.jmpCycles) {
    this->busy = 1;
  } else {
    this->busy = 0;
  }
}

void Simulator::simulate() {
  while(!finished) {
    if (!busy) {
      fetch();
      decode();
    } else {
      cout << "busy" << "\t" << "busy" << "\t";
    }
    execute();
    setBusy();
    if (!busy) {
      currentInstructions[2] = currentInstructions[1];
      currentInstructions[1] = currentInstructions[0];
    }
  }
}
