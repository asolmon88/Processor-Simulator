#ifndef SIMULATOR_
#define SIMULATOR_

#include <vector>
#include "Instruction.hpp"
#include "Section.hpp"

class Simulator {

 public:
  std::vector<int> registers;
  std::vector<int> memory;
  std::vector<Instruction> instructions;
  std::vector<Section_t> sections;

  int finished;
  int cycles;
  int numberInstructions;
  int PC;

  Simulator();

  void fetch();
  void decode();
  void execute();
  void simulate();

};

#endif  // SIMULATOR_