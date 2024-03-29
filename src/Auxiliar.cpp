#include <fstream>
#include <iostream>
#include "Auxiliar.hpp"
#include "Instruction.hpp"
#include "Section.hpp"
#include "unistd.h"

void Auxiliar::readFile(const char* filename, std::vector<std::string>& instructions) {
  std::ifstream file;
  file.open(filename);

  char charInstruction[50];
  std::string strInstruction;

  while (file.getline(charInstruction, 50)) {
    strInstruction = charInstruction;
    instructions.push_back(strInstruction);
  }

  /*for (size_t i = 0; i < instructions.size(); ++i) {
    std::cout << instructions[i] << std::endl;
  }*/
  file.close();
}

/**
 * @brief Method to know if a line read is an instruction or section
 * 
 * @param instruction 
 * @return int, -1 means not an instruction
 */

int Auxiliar::isInstruction(std::string instruction) {
  int pos = instruction.find(":");
  if (pos == -1) {
    pos = instruction.find("end");
  }
  return pos;
}

void Auxiliar::saveInstructions(
  std::vector<Instruction>& instructions, std::string instruction) {
  std::string currentText;
  std::vector<std::string> separated;
  Instruction newInstruction;

  separate(instruction, separated);

  currentText = separated[0];
  newInstruction.setOpcode(currentText);
  // add load to mov between registers
  if (currentText == "ld") {
    if (separated[1][0] == 'R') {
      newInstruction.setMemIndex(stoll(separated[1].substr(
        1,separated[1].size())));
      newInstruction.setReg1(separated[2]);
    } else {
      newInstruction.setMemIndex(stoll(separated[1]));
      newInstruction.setReg1(separated[2]);
    }
    instructions.push_back(newInstruction);
  } else if (currentText == "str") {
    if (separated[1][0] == 'R') {
      newInstruction.setMemIndex(stoll(separated[1].substr(
        1,separated[1].size())));
      newInstruction.setReg1(separated[2]);
    } else {
      newInstruction.setMemIndex(stoll(separated[1]));
      newInstruction.setReg1(separated[2]);
    }
    instructions.push_back(newInstruction);
  } else if (currentText == "mov") {
    if (separated.size() > 3) {
      if (stoi(separated[4]) < 5) {
        newInstruction.setOffsetLocation(1);
        if (separated[2][0] == 'R') {
          newInstruction.setOffsetReg(separated[2]);
        } else {
          newInstruction.setOffset(stoi(separated[2]));
        }
        newInstruction.setReg1(separated[1]);
        newInstruction.setReg2(separated[3]);
      } else {
        if (separated[3][0] == 'R') {
          newInstruction.setOffsetReg(separated[3]);
        } else {
          newInstruction.setOffset(stoi(separated[3]));
        }
        if (separated[1][0] == 'R') {
          newInstruction.setReg1(separated[1]);
          newInstruction.setReg2(separated[2]);
        } else {
          newInstruction.setValue(stoi(separated[1]));
          newInstruction.setReg2(separated[2]);
        }
      }
    } else if (separated[1][0] == 'R') {
      newInstruction.setReg1(separated[1]);
      newInstruction.setReg2(separated[2]);
    } else {
      newInstruction.setValue(stoi(separated[1]));
      newInstruction.setReg2(separated[2]);
    }
    instructions.push_back(newInstruction);
  } else if (currentText == "add" || currentText == "sub" 
    || currentText == "mult") {
    newInstruction.setReg1(separated[1]);
    newInstruction.setReg2(separated[2]);
    newInstruction.setReg3(separated[3]);
    instructions.push_back(newInstruction);
  } else if (currentText == "cmp") {
    newInstruction.setReg1(separated[1]);
    newInstruction.setReg2(separated[2]);
    instructions.push_back(newInstruction);
  } else if (currentText == "jmp" || currentText == "je" 
      || currentText == "ja" || currentText == "jb" ||
      currentText == "call") {
    newInstruction.setSection(separated[1]);
    instructions.push_back(newInstruction);
  } else {
    std::cout << currentText << "is not a valid instruction" << std::endl;
  }
}

int Auxiliar::saveSection(std::vector<Section_t>& sections, std::string instruction,
  int index) {
  if (instruction != "\n" && instruction != "\t" &&
    instruction != "" && instruction != " ") {
    Section_t newSection;

    newSection.index = index;
    if (instruction != "end") {
      instruction.pop_back();
    }
    newSection.name = instruction;
    sections.push_back(newSection);
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

void Auxiliar::getInstructions(std::vector<Instruction>& instructions,
    std::vector<Section_t>& sections, const char* filename) {
  std::vector<std::string> readInstructions;
  std::string file = "./tests/";
  file += filename;
  file += ".txt";
  Auxiliar::readFile(file.c_str(),
    readInstructions);
  size_t currentInstructionIndex = 0;
  for (size_t i = 0; i < readInstructions.size(); ++i) {
    if ((readInstructions[i] != "\n") && (readInstructions[i] != "\t") &&
    (readInstructions[i] != "") && (readInstructions[i] != " ")) {
      if (Auxiliar::isInstruction(readInstructions[i]) == -1) {
        Auxiliar::saveInstructions(instructions, readInstructions[i]);
        ++currentInstructionIndex;
      } else {
        if (Auxiliar::saveSection(sections, readInstructions[i], currentInstructionIndex)
        == EXIT_SUCCESS) {
          if (readInstructions[i] == "end") {
            Instruction end;
            end.setOpcode("end");
            instructions.push_back(end);
          }
        }
      }
    }
  }
}

// Adapted from: 
// https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
void Auxiliar::separate(std::string line, std::vector<std::string>& separated) {
  std::string word = "";
  int index = 0; // index where [ was found
  int notFound = 1;

  std::string newLine;
  int firstChar = 0;
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] != 32) {
      firstChar = 1;
    }
    if (firstChar) {
      newLine+=line[i];
    }
  }
  line = newLine;

  for (auto x : line) {
    if (x != '[' && notFound) {
      ++index;
    } else {
      notFound = 0;
    }
    if ((x == ' ' || x == ',' || x == ']' || x == '\n') && word != "") {
      separated.push_back(word);
      word = "";
    } else {
      if (x != ' ' && x != ',' && x != ']' && x != '[' &&
        x != '\n' && x != '\0') {
        word = word + x;
      }
    }
  }
  if (word != "") {
    separated.push_back(word);
  }
  if (!notFound) {
    separated.push_back(to_string(index));
  }
}
