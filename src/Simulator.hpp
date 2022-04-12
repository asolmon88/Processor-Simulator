#ifndef SIMULATOR_
#define SIMULATOR_

#include <queue>
#include <string>
#include <vector>
#include "ALU.hpp"
#include "BranchUnit.hpp"
#include "LSUnit.hpp"
#include "Instruction.hpp"
#include "Register.hpp"
#include "Section.hpp"

class Simulator {

 private:
  void jump();
  void jumpEqual();
  void jumpAbove();
  void jumpBelow();
  void call();
  void end();
  int find(const std::string&);
  void checkRegisters();
  void setBusy();
  int checkScoreboard();
  void deleteFromExecute(int index);
  void unableRegisters(Instruction&);
  void enableRegisters(Instruction&);
  void printRegisters();

 public:
  // Registers and memory
  std::vector<int> scoreboard = std::vector<int>(33);
  std::vector<Register_t> registers;
  std::vector<int> memory = std::vector<int>(1024);

  // To handle recursion
  std::vector<int> calls;
  std::vector<std::vector<Register_t>> callRegister;

  // To handle instructions
  std::vector<Instruction> instructions;
  std::vector<Section_t> sections;
  std::vector<Instruction> currentInstructions;
  Instruction currentInstruction;

  // execution Units
  ALU alu;
  BranchUnit branchUnit;
  LSUnit lsUnit;

  // execution
  std::queue<Instruction> readyExecute;
  std::vector<Instruction> executing;

  int busy;

  int finished;
  int cycles;
  int numberInstructions;
  int PC;

  int flag; // for comparisons

  int branch;

  Simulator();

  void fetch();
  void decode();
  void execute();
  void simulate();

};

#endif  // SIMULATOR_