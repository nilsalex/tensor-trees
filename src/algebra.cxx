#include <iostream>

#include "Algorithms.hxx"
#include "Builder.hxx"

int main () {
  auto tree = loadTree ("area_2nd_kinetic.prs");
//  auto tree = buildEpsilonEtaTree ("abcdef");

  std::cout << printTree (tree) << std::endl;

  contractTreeWithEpsilon (tree, 'm', 'b', 'c', 'd');
//  contractTreeWithEta (tree, 'j', 'l');
  canonicalizeTree (tree);

  std::cout << "contracted tree" << std::endl;

  std::cout << printTree (tree) << std::endl;
}
