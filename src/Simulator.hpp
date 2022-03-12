#ifndef SIMULATOR_
#define SIMULATOR_

#include <vector>
#include <string>
#include "Instruction.hpp"
#include "Register.hpp"
#include "Section.hpp"

class Simulator {

 private:
  void load();
  void store();
  void move();
  void add();
  void substract();
  void multiply();
  void compare();
  void jump();
  void jumpEqual();
  void jumpAbove();
  void jumpBelow();
  void call();
  void end();
  int find(const std::string&);
  void checkRegisters();

 public:
  std::vector<Register_t> registers;
  std::vector<int> memory = std::vector<int>(1024);
  std::vector<int> calls;
  std::vector<std::vector<Register_t>> callRegister;
  std::vector<Instruction> instructions;
  std::vector<Section_t> sections;
  Instruction currentInstruction;

  int finished;
  int cycles;
  int numberInstructions;
  int PC;

  int flag; // for comparisons

  Simulator();

  void fetch();
  void decode();
  void execute();
  void simulate();

};

#endif  // SIMULATOR_