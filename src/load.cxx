#include <iostream>
#include <limits>

#include "Algorithms.hxx"

int main (int argc, char * argv []) {
  if (argc != 2) {
    std::cout << "Usage:" << std::endl;
    std::cout << "  load <filename>" << std::endl;
    return 0;
  }

  auto tree = loadTree (argv[1]);

  std::cout << "Tree loaded. Hit return to exit.";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  return 0;
}
