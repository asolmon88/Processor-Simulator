#ifndef SIMULATOR_
#define SIMULATOR_

#include <queue>
#include <string>
#include <vector>
#include "ALU.hpp"
#include "BranchPredictor.hpp"
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
  void deleteFromExecute(int index, int array = 0);
  void unableRegisters(Instruction&);
  void enableRegisters(Instruction&);
  void orderInstructions(int stage);  // stage 0 fetch, stage 1 decode
  void printRegisters();
  void preDecode(Instruction& current);  // used for branch prediction

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
  std::vector<std::vector<Instruction>> currentInstructions;
  Instruction currentInstruction;

  // execution Units
  ALU alu;
  BranchUnit branchUnit;
  LSUnit lsUnit;
  BranchPredictor predictor;

  // execution
  std::queue<Instruction> readyExecute;
  std::vector<Instruction> executing;
  int OoO;
  std::vector<std::vector<int>> executed;

  int busy;

  // instructions per cycle
  float IPC;
  // total instructions
  int TI;

  int finished;
  int cycles;
  int numberInstructions;
  int PC;

  int flag; // for comparisons

  int branch;

  // for branch prediction
  int changePC;
  int correctPredictions;

  Simulator();

  void fetch();
  void decode();
  void execute();
  void simulate();

};

#endif  // SIMULATOR_