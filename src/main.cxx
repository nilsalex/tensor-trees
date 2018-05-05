#include <algorithm>
#include <chrono>
#include <iostream>

#include "Algorithms.hxx"
#include "LinearAlgebra.hxx"
#include "Tree.hxx"
#include "Tensor.hxx"
#include "Builder.hxx"

int main () {
  std::string indices {"abcdefghpq"};
//  std::string indices {"abcd"};
//  std::string indices {"abcdefgh"};

  auto forest = buildEpsilonEtaForest (indices);

  symmetrizeForest(*forest, {{'a', 'b'}, {'b', 'a'}}, -1);
  symmetrizeForest(*forest, {{'c', 'd'}, {'d', 'c'}}, -1);
  symmetrizeForest(*forest, {{'e', 'f'}, {'f', 'e'}}, -1);
  symmetrizeForest(*forest, {{'g', 'h'}, {'h', 'g'}}, -1);
  symmetrizeForest(*forest, {{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1);
  symmetrizeForest(*forest, {{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'h', 'f'}}, 1);
  symmetrizeForest(*forest, {{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}}, 1);
  symmetrizeForest(*forest, {{'p', 'q'}, {'q', 'p'}}, 1);

  applyTensorSymmetries(*forest);

  splitUnsortedTreesRepeat(forest);

  mergeForest(*forest);
  removeEmptyBranches(*forest);
  shrinkForest(*forest);

  redefineVariables(*forest);

  std::cout << printForest(*forest);

  return 0;
}
