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

  std::cout << simulator.registers[2] << std::endl;

  return 0;
}