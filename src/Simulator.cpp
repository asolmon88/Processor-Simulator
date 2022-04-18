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
  this->IPC = 0;

  Register first(0,0);
  registers.push_back(first);

  this->scoreboard[0] = 1;

  // fills registers and scoreboard
  for (int i = 1; i < 32; ++i) {
    Register reg((i*32)-32, (i*32)-1);
    registers.push_back(reg);
    this->scoreboard[i] = 1;
  }

  this->scoreboard[32] = 1;

  // fills the instructions for F,D,E
  Instruction temp;
  temp.setOpcode("");
  for (int i = 0; i < 2; ++i) {
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
  if (!readyExecute.empty()) {
    unableRegisters(readyExecute.front());
    this->executing.push_back(readyExecute.front());
    readyExecute.pop();
  }
  int executed = 0;

  IPC = (IPC + this->executing.size())/2;

  for (int i = 0; i < (int)this->executing.size() ; ++i) {
    currentInstruction = this->executing[i];
    std::string opcode = currentInstruction.getOpcode();
    cout << opcode << ",";
    if (opcode == "ld") {
      if (lsUnit.loadCycles == 0) {
        lsUnit.loadCycles = cycles+4;
      } else if (cycles < lsUnit.loadCycles) {
        ++cycles;
      } else {
        lsUnit.load(this->registers, currentInstruction, this->memory);
        lsUnit.loadCycles = 0;
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "str") {
      lsUnit.store(this->registers, currentInstruction, this->memory);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed = 1;
    } else if (opcode == "mov") {
      lsUnit.move(this->registers, currentInstruction, this->memory);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed = 1;
    } else if (opcode == "add") {
      alu.add(this->registers, currentInstruction);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed = 1;
    } else if (opcode == "sub") {
      alu.substract(this->registers, currentInstruction);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed = 1;
    } else if (opcode == "mult") {
      if (!alu.multiplyCycles) {
        alu.multiplyCycles = 1;
        ++cycles;
      } else {
        alu.multiply(this->registers, currentInstruction);
        alu.multiplyCycles = 0;
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "cmp") {
      if (!alu.compareCycles) {
        alu.compareCycles = 1;
        ++cycles;
      } else {
        alu.compare(this->registers, currentInstruction, this->flag);
        alu.compareCycles = 0;
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "jmp") {
      branchUnit.jump(this->PC, currentInstruction, this->sections);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed = 1;
    } else if (opcode == "je") {
      branchUnit.jumpEqual(this->PC, currentInstruction, this->flag, this->sections,
        this->cycles);
      if (!branchUnit.jeCycles) {
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "ja") {
      branchUnit.jumpAbove(this->PC, currentInstruction, this->flag, this->sections,
        this->cycles);
      if (!branchUnit.jaCycles) {
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "jb") {
      branchUnit.jumpBelow(this->PC, currentInstruction, this->flag, this->sections,
        this->cycles);
      if (!branchUnit.jbCycles) {
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "call") {
      if (!branchUnit.callCycles) {
        branchUnit.callCycles = 1;
        ++cycles;
      } else {
        branchUnit.call(this->PC, currentInstruction, this->calls, this->callRegister,
          this->registers, this->sections);
        branchUnit.callCycles = 0;
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed = 1;
      }
    } else if (opcode == "end") {
      end();
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      ++cycles;
    } else {
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed = 1;
    }
    if (executed) {
      executed = 0;
      i = -1;
    }
    /* if (opcode != "") {std::cout << opcode << ",";} */
  }
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
  if (branchUnit.callCycles || branchUnit.jaCycles ||
    branchUnit.jbCycles || branchUnit.jeCycles ||
    branchUnit.jmpCycles) {
    this->busy = 1;
  } else if (lsUnit.loadCycles || alu.compareCycles || alu.multiplyCycles) {
    this->busy = 2;
  } else {
    this->busy = 0;
  }
}

int Simulator::checkScoreboard() {
  if (currentInstructions[1].getOpcode()[0] != 'j' &&
    currentInstructions[1].getOpcode() != "call") {
    if (currentInstructions[1].getR1() != 34) {
      if (currentInstructions[1].getR2() != 34) {
        if (currentInstructions[1].getR3() != 34) {
          if (scoreboard[currentInstructions[1].getR3()] == 1 &&
            scoreboard[currentInstructions[1].getR1()] == 1 &&
            scoreboard[currentInstructions[1].getR2()] == 1) {
            return 1;
          }
        } else {
          if (scoreboard[currentInstructions[1].getR1()] == 1 &&
            scoreboard[currentInstructions[1].getR2()] == 1) {
            return 1;
          }
        }
      }
    } else if (currentInstructions[1].getR2() != 34) {
      if (scoreboard[currentInstructions[1].getR2()] == 1) {
        return 1;
      }
    }
  }
  return 0;
}

void Simulator::deleteFromExecute(int index) {
  std::vector<Instruction>::iterator j = this->executing.begin();

  while (j != this->executing.end() && *j != this->executing[index]) {
    ++j;
  }

  this->executing.erase(j);
}

void Simulator::unableRegisters(Instruction& current) {
  if (current.getR1() != 34) {
    scoreboard[current.getR1()] = 0;
  }
  if (current.getR2() != 34) {
    scoreboard[current.getR2()] = 0;
  }
  if (current.getR3() != 34) {
    scoreboard[current.getR3()] = 0;
  }
}

void Simulator::enableRegisters(Instruction& current) {
  if (current.getR1() != 34) {
    scoreboard[current.getR1()] = 1;
  }
  if (current.getR2() != 34) {
    scoreboard[current.getR2()] = 1;
  }
  if (current.getR3() != 34) {
    scoreboard[current.getR3()] = 1;
  }
}

void Simulator::simulate() {
  while(!finished) {
    if (!busy) {
      fetch();
      decode();
    } else if (busy == 2) {
      if (checkScoreboard()) {
        std::cout << std::endl;
        readyExecute.push(currentInstructions[1]);
        currentInstructions[1] = currentInstructions[0];
        --cycles;
        fetch();
        decode();
      }
    } else {
      cout << "busy" << "\t" << "busy" << "\t";
    }
    execute();
    setBusy();
    if (!busy) {
      std::cout << std::endl;
      readyExecute.push(currentInstructions[1]);
      currentInstructions[1] = currentInstructions[0];
    }
  }
}
