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
  std::vector<std::vector<char>> islist {{'a', 'b', 'c', 'd'}, {'a', 'c', 'b', 'd'}};

  auto tree = treeFromIndicesEta (islist);

  std::cout << printTree (tree);

  return 0;
}
