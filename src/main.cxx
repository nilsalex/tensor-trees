#include <algorithm>
#include <chrono>
#include <iostream>

#include "Algorithms.hxx"
#include "Tree.hxx"
#include "Tensor.hxx"
#include "Builder.hxx"

int main () {
//  std::string indices {"abcdefghpq"};
  std::string indices {"abcd"};

  auto forest = std::make_unique<Forest<Node>>();
  {
    EtaBuilder etaBuilder;
    auto etaForest = etaBuilder.buildForest(indices);
  
    EpsilonBuilder epsilonBuilder(etaBuilder.get_leaf_counter());
    auto epsilonForest = epsilonBuilder.buildForest(indices);

    forest->reserve(etaForest.size() + epsilonForest.size());
    std::for_each(etaForest.begin(), etaForest.end(),
      [&forest] (auto & t) {
        forest->push_back(std::make_unique<Tree<Node>>());
        forest->back().swap(t);
      });
    std::for_each(epsilonForest.begin(), epsilonForest.end(),
      [&forest] (auto & t) {
        forest->push_back(std::make_unique<Tree<Node>>());
        forest->back().swap(t);
      });
  }

  symmetrizeForest(*forest, {{'a', 'b'}, {'b', 'a'}}, -1);
  symmetrizeForest(*forest, {{'c', 'd'}, {'d', 'c'}}, -1);
/*
  symmetrizeForest(*forest, {{'e', 'f'}, {'f', 'e'}}, -1);
  symmetrizeForest(*forest, {{'g', 'h'}, {'h', 'g'}}, -1);
*/
  symmetrizeForest(*forest, {{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1);
/*
  symmetrizeForest(*forest, {{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'h', 'f'}}, 1);
  symmetrizeForest(*forest, {{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}}, 1);
  symmetrizeForest(*forest, {{'p', 'q'}, {'q', 'p'}}, 1);
*/
  applyTensorSymmetries(*forest);

  int iterations = 0;
  while (!isForestSorted(*forest)) {
    auto new_forest = std::make_unique<Forest<Node>>(splitUnsortedTrees(*forest));
    std::swap(forest, new_forest);
    forest->shrink_to_fit();
    ++iterations;
  }

  mergeForest(*forest);
  shrinkForest(*forest);
  removeEmptyBranches(*forest);
  std::cout << printForest(*forest);

  auto map = getVariableMap(*forest);
  auto mat = getCoefficientMatrix(*forest, map);

  std::for_each (mat.cbegin(), mat.cend(),
    [] (auto const & v) {
      std::for_each(v.cbegin(), v.cend(),
        [] (auto const & q) {
          std::cout << q << "\t";
        });
      std::cout << std::endl;
    });

  return 0;
}
