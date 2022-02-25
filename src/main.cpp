#include <iostream>
#include <vector>
#include <string>

#include "Auxiliar.hpp"
#include "Instruction.hpp"
#include "Section.hpp"
#include "Simulator.hpp"

int main() {

  Simulator simulator;

  Auxiliar::getInstructions(simulator.instructions, simulator.sections);

  simulator.simulate();

  std::cout << simulator.registers[5] << std::endl;
  std::cout << simulator.registers[6] << std::endl;
  std::cout << simulator.memory[3] << std::endl;
  std::cout << simulator.registers[2] << std::endl;
  /* for (auto x : simulator.instructions) {
    cout << x << endl;
  } */

  return 0;
}