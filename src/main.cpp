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

  std::cout << simulator.registers[0] << std::endl;

 /*  for (auto x : simulator.instructions) {
    cout << x << endl;
  } */
  return 0;
}