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

  return 0;
}