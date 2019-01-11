#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Algorithms.hxx"
#include "LinearAlgebra.hxx"
#include "Tree.hxx"
#include "Tensor.hxx"
#include "Eta.hxx"
#include "Scalar.hxx"
#include "Builder.hxx"

#include "Indices.hxx"

int main () {
  std::string indices {"abcdefghijklmnpqrs"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, -1},
    {{{'k', 'l'}, {'l', 'k'}}, -1},
    {{{'m', 'n'}, {'n', 'm'}}, 1},
    {{{'p', 'q'}, {'q', 'p'}}, 1},
    {{{'r', 's'}, {'s', 'r'}}, 1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'l', 'f'}}, 1},
    {{{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1}
  };

  std::cout << "################################" << std::endl;
  std::cout << "Spacetime ansatz generator v0.1\u03b1" << std::endl;
  std::cout << "################################" << std::endl;
  std::cout << std::endl;
  std::cout << "     indices : " << indices << std::endl;
  std::cout << "# of indices : " << indices.size() << std::endl;
  std::cout << std::endl;
  std::cout << "Constructing ansatz tree ..." << std::endl;

  auto tree = buildEpsilonEtaTree (indices);

  std::cout << "... ansatz tree constructed." << std::endl;
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "Sorting ansatz tree ..." << std::endl;
  sortTree (tree);
  redefineScalarsSym (tree);
  std::cout << "... sorted." << std::endl;
  std::cout << std::endl;

  auto variable_set = getVariableSet (tree);
  std::cout << "There are " << variable_set.size() << " variables." << std::endl;

  std::cout << printTree (tree) << std::endl;

  return 0;
}
