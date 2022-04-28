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
  this->OoO = 0;
  this->TI = 0;
  this->prediction = 0;
  this->correctPredictions = 0;
  this->changePC = 0;

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
  std::vector<Instruction> tempVect;
  for (int i = 0; i < 4; ++i) {
    tempVect.push_back(temp);
  }
  tempVect.shrink_to_fit();
  for (int j = 0; j < 2; ++j) {
    this->currentInstructions.push_back(tempVect);
  }
  this->currentInstructions.shrink_to_fit();
}

void Simulator::fetch() {
  ++PC;
  if (!this->OoO) {
    if (branch) {
      for (int i = 0; i < 4; ++i) {
        currentInstructions[0][i].setOpcode("wait"); // this is how it waits
      }
      branch = 0;
      PC -= 5;
      ++cycles;
    } else if (PC < (int)instructions.size()) {
      for (int i = 0; i < 4; ++i) {
        if (PC < (int)instructions.size()) {
          currentInstructions[0][i] = instructions[PC];
        } else {
          currentInstructions[0][i].setOpcode("done");
        }
        preDecode(currentInstructions[0][i]);
        if (this->changePC) {
          this->previousPC.push(this->PC);
          this->PC = this->predPC.back();
          this->changePC = 0;
        }
        ++PC;
      }
      --PC;
      ++cycles;
    } else {
      for (int i = 0; i < 4; ++i) {
        currentInstructions[0][i].setOpcode("done");
      }
    }
  } else {
    if (PC < (int)instructions.size()) {
      currentInstructions[0][3] = instructions[PC];
    } else {
      currentInstructions[0][3].setOpcode("done");
    }
    ++cycles;
  }
  // cout << currentInstructions[0].getOpcode() << "\t";
}
void Simulator::decode() {
  if (!this->OoO) {
    for (int i = 0; i < 4; ++i) {
      Instruction* currentInstruction = &currentInstructions[1][i];
      if (branch) {
        currentInstruction->setOpcode("wait");
      } else if (currentInstruction->getOpcode() == "call") {
        currentInstructions[0][0].setOpcode("wait");
        currentInstructions[0][1].setOpcode("wait");
        currentInstructions[0][2].setOpcode("wait");
        currentInstructions[0][3].setOpcode("wait"); // this is how it waits
        this->PC = this->callPC;
        this->branch = 1;
        this->predPC = std::queue<int>();
        this->previousPC = std::queue<int>();
      }
      if (currentInstruction->getOpcode() == "end") {
        ++cycles;
      }
      // std::cout << currentInstruction->getOpcode() << "\t";
    }
  } else {
    Instruction* currentInstruction = &currentInstructions[1][3];
    /* std::cout << "\nFLAG: " << flag << std::endl;
    sleep(1); */
    if (branch) {
      currentInstruction->setOpcode("wait");
    } else if (currentInstruction->getOpcode() == "call") {
      currentInstructions[0][0].setOpcode("wait");
      currentInstructions[0][1].setOpcode("wait");
      currentInstructions[0][2].setOpcode("wait");
      currentInstructions[0][3].setOpcode("wait"); // this is how it waits
      this->branch = 1;
      this->predPC = std::queue<int>();
      this->previousPC = std::queue<int>();
    }
    if (currentInstruction->getOpcode() == "end") {
      ++cycles;
    }
  }
  // std::cout << currentInstruction->getOpcode() << "\t";
}

void Simulator::execute() {
  std::vector<int> temp = std::vector<int>(3);
  temp[0] = 0;  // flag to know if it executed
  temp[1] = 0;  // flag to know if it's OoO (1) or normal (0)
  temp[2] = this->TI;  // just to differentiate from others
  while (!readyExecute.empty()) {
    if (this->OoO) {
      temp[1] = 1;
    } else {
      temp[1] = 0;
      this->OoO = 0;
    }
    this->executed.push_back(temp);
    this->executing.push_back(readyExecute.front());
    readyExecute.pop();
  }
  // to print
  std::cout << "Fetch\tDecode\tExecute" << std::endl;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 2; ++j) {
      std::cout << this->currentInstructions[j][i].getOpcode() << "\t";
    }
    if (i < (int)this->executing.size()) {
      std::cout << this->executing[i].getOpcode();
    }
    std::cout << std::endl;
  }
  if (this->executing.size() > 4) {
    std::cout << "EXECUTING: ";
    for (int k = 4; k < (int)this->executing.size(); ++k) {
      std::cout << this->executing[k].getOpcode() << ",";
    }
    std::cout << std::endl;
  }
  std::cout << "--------------------------------" << std::endl;
  //sleep(3);

  IPC = (IPC + (float)this->executing.size())/2;
  int branchExecuted = -1;
  for (int i = 0; i < (int)this->executing.size() ; ++i) {
    currentInstruction = this->executing[i];
    unableRegisters(currentInstruction);
    std::string opcode = currentInstruction.getOpcode();
    // cout << opcode << ",";
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
        executed[i][0] = 1;
      }
    } else if (opcode == "str") {
      lsUnit.store(this->registers, currentInstruction, this->memory);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed[i][0] = 1;
    } else if (opcode == "mov") {
      lsUnit.move(this->registers, currentInstruction, this->memory);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed[i][0] = 1;
    } else if (opcode == "add") {
      alu.add(this->registers, currentInstruction);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed[i][0] = 1;
    } else if (opcode == "sub") {
      alu.substract(this->registers, currentInstruction);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed[i][0] = 1;
    } else if (opcode == "mult") {
      if (!alu.multiplyCycles) {
        alu.multiplyCycles = 1;
        ++cycles;
      } else {
        alu.multiply(this->registers, currentInstruction);
        alu.multiplyCycles = 0;
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed[i][0] = 1;
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
        executed[i][0] = 1;
      }
    } else if (opcode == "jmp") {
      this->currentPC = this->PC;
      branchUnit.jump(this->PC, currentInstruction, this->sections);
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed[i][0] = 1;
      this->PC = this->currentPC;
      this->predPC.pop();
      this->previousPC.pop();
    } else if (opcode == "je") {
      this->currentPC = this->PC;
      branchExecuted = branchUnit.jumpEqual(this->PC, currentInstruction,
        this->flag, this->sections, this->cycles);
      if (!branchUnit.jeCycles) {
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed[i][0] = 1;
        checkPrediction(i, branchExecuted);
      }
    } else if (opcode == "ja") {
      this->currentPC = this->PC;
      branchExecuted = branchUnit.jumpAbove(this->PC, currentInstruction, this->flag, this->sections,
        this->cycles);
      if (!branchUnit.jaCycles) {
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed[i][0] = 1;
        checkPrediction(i, branchExecuted);
      }
    } else if (opcode == "jb") {
      this->currentPC = this->PC;
      branchExecuted = branchUnit.jumpBelow(this->PC, currentInstruction, this->flag, this->sections,
        this->cycles);
      if (!branchUnit.jbCycles) {
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed[i][0] = 1;
        checkPrediction(i, branchExecuted);
      }
    } else if (opcode == "call") {
      if (!branchUnit.callCycles) {
        branchUnit.callCycles = 1;
        ++cycles;
      } else {
        this->PC = this->callPC;
        this->callPC = 0;
        branchUnit.call(this->PC, currentInstruction, this->calls, this->callRegister,
          this->registers, this->sections);
        branchUnit.callCycles = 0;
        this->previousPCRec.push_back(this->previousPC);
        this->predPCRec.push_back(this->predPC);
        enableRegisters(currentInstruction);
        deleteFromExecute(i);
        executed[i][0] = 1;
      }
    } else if (opcode == "end") {
      end();
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      ++cycles;
    } else {
      enableRegisters(currentInstruction);
      deleteFromExecute(i);
      executed[i][0] = 1;
    }
    if (!branchExecuted) {
      break;
    } else if (executed[i][0]) {
      deleteFromExecute(i,1);
      ++this->TI;
      i = -1;
    } else {
      int k = i;
      int found = 0;
      ++k;
      while (k < (int)this->executing.size() && !found) {
        if (this->executed[k][1] == 1) {
          i = k-1;
          found = 1;
        }
        ++k;
      }
      if (!found) {
        break;
      }
    }
    /* if (opcode != "") {std::cout << opcode << ",";} */
  }
}

void Simulator::preDecode(Instruction& current) {
  if (current.getOpcode() == "jmp") {
    this->changePC = 2;
    this->predPC.push(branchUnit.find(current.getSection(),
      sections)-1);
  } else if (current.getOpcode() == "ja" ||
    current.getOpcode() == "je" ||
    current.getOpcode() == "jb") {
    this->prediction = 1;
    this->changePC = 1;
    this->predPC.push(branchUnit.find(current.getSection(),
      sections)-1);
  } else if (current.getOpcode() == "call") {
    if (!this->callPC) {
      this->callPC = this->PC;
    }
  }
}

void Simulator::end() {
  /* printRegisters();
  sleep(10); */
  if (!this->calls.empty()) {
    checkRegisters();
    this->PC = this->calls.back();
    this->registers = this->callRegister.back();
    this->previousPC = this->previousPCRec.back();
    this->predPC = this->predPCRec.back();
    this->calls.pop_back();
    this->callRegister.pop_back();
    this->previousPCRec.pop_back();
    this->predPCRec.pop_back();
  } else {
    this->finished = 1;
  }
}

int Simulator::correctPrediction() {
  return this->PC == this->predPC.front();
}

void Simulator::checkPrediction(int i, int branchExecuted) {
  if (branchExecuted) {
    if (correctPrediction()) {
      this->PC = this->currentPC;
      ++correctPredictions;
    } else {
      this->PC = this->previousPC.front();
      for (int k = 0; k < 4; ++k) {
        for (int j = 0; j < 2; ++j) {
          this->currentInstructions[j][k].setOpcode("wait");
        }
      }
      for (int j = i; j < (int)this->executing.size(); ++j) {
        this->executing[j].setOpcode("wait");
      }
      this->previousPC = std::queue<int>();
      this->predPC = std::queue<int>();
    }
  } else {
    this->PC = this->previousPC.front();
    for (int k = 0; k < 4; ++k) {
      for (int j = 0; j < 2; ++j) {
        this->currentInstructions[j][k].setOpcode("wait");
      }
    }
    for (int j = i; j < (int)this->executing.size(); ++j) {
      this->executing[j].setOpcode("wait");
    }
    this->previousPC = std::queue<int>();
    this->predPC = std::queue<int>();
  }
  this->prediction = 0;
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
  if (currentInstructions[1][0].getOpcode()[0] != 'j' &&
    currentInstructions[1][0].getOpcode() != "call") {
    if (currentInstructions[1][0].getR1() != 34) {
      if (currentInstructions[1][0].getR2() != 34) {
        if (currentInstructions[1][0].getR3() != 34) {
          if (scoreboard[currentInstructions[1][0].getR3()] == 1 &&
            scoreboard[currentInstructions[1][0].getR1()] == 1 &&
            scoreboard[currentInstructions[1][0].getR2()] == 1) {
            return 1;
          }
        } else {
          if (scoreboard[currentInstructions[1][0].getR1()] == 1 &&
            scoreboard[currentInstructions[1][0].getR2()] == 1) {
            return 1;
          }
        }
      }
    } else if (currentInstructions[1][0].getR2() != 34) {
      if (scoreboard[currentInstructions[1][0].getR2()] == 1) {
        return 1;
      }
    }
  }
  return 0;
}

void Simulator::deleteFromExecute(int index, int array) {
  if (!array) {
    std::vector<Instruction>::iterator j = this->executing.begin();

    while (j != this->executing.end() && *j != this->executing[index]) {
      ++j;
    }

    this->executing.erase(j);
  } else {
    std::vector<std::vector<int>>::iterator j = this->executed.begin();

    while (j != this->executed.end() &&
      ((*j)[0] != this->executed[index][0] ||
      (*j)[1] != this->executed[index][1] ||
      (*j)[2] != this->executed[index][2])) {
      ++j;
    }

    this->executed.erase(j);
  }
}

void Simulator::orderInstructions(int stage) {
  if (stage) {
    // decode
    for (int i = 0; i < 3; ++i) {
      currentInstructions[1][i] = currentInstructions[1][i+1];
    }
  } else {
    // fetch
    for (int i = 0; i < 3; ++i) {
      currentInstructions[0][i] = currentInstructions[0][i+1];
    }
  }
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
    std::cout << "CYCLE: " << this->cycles << std::endl;
    if (!busy || prediction) {
      fetch();
      decode();
    } else if (busy == 2) {
      if (checkScoreboard()) {
        readyExecute.push(currentInstructions[1][0]);
        --cycles;
        this->OoO = 1;
        orderInstructions(1);  // decode
        currentInstructions[1][3] = currentInstructions[0][0];
        orderInstructions(0);  // fetch
        fetch();
        decode();
      }
    }
    execute();
    setBusy();
    if (!busy) {
      for (int i = 0; i < 4; ++i) {
        readyExecute.push(currentInstructions[1][i]);
      }
      currentInstructions[1] = currentInstructions[0];
    }
  }
  printRegisters();
}
