#include <algorithm>
#include <chrono>
#include <iostream>

#include "Algorithms.hxx"
#include "Tree.hxx"
#include "Tensor.hxx"
#include "Builder.hxx"

int main () {
#if 0
  std::string indices {"abcdefghijklpq"};

  std::map <char, size_t> eval_map {
    {'a', 1},
    {'b', 2},
    {'c', 3},
    {'d', 0},
    {'e', 3},
    {'f', 0},
    {'g', 0},
    {'h', 3},
    {'i', 2},
    {'j', 1},
    {'k', 1},
    {'l', 2},
    {'p', 1},
    {'q', 1}
  };
/*
  std::string indices {"abcd"};

  std::map <char, size_t> eval_map {
    {'a', 1},
    {'b', 0},
    {'c', 0},
    {'d', 1},
  };
*/

  std::cout << "Constructing ansatz tree for" << indices.size() << " indices {";
  for (size_t counter = 0; counter < indices.size() - 1; ++counter) {
    std::cout << indices[counter] << " ";
  }
  std::cout << indices[indices.size() - 1] << "}" << std::endl;

  auto start = std::chrono::high_resolution_clock::now();
  Forest<Node> forest;
  size_t size = 0;
  {
    EtaBuilder etaBuilder;
    auto etaForest = etaBuilder.buildForest(indices);
  
    EpsilonBuilder epsilonBuilder(etaBuilder.get_leaf_counter());
    auto epsilonForest = epsilonBuilder.buildForest(indices);

    size = epsilonBuilder.get_leaf_counter();

    forest.reserve(etaForest.size() + epsilonForest.size());
    std::for_each(etaForest.begin(), etaForest.end(),
      [&forest] (auto & t) {
        forest.push_back(std::make_unique<Tree<Node>>());
        forest.back().swap(t);
      });
    std::for_each(epsilonForest.begin(), epsilonForest.end(),
      [&forest] (auto & t) {
        forest.push_back(std::make_unique<Tree<Node>>());
        forest.back().swap(t);
      });
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;

  std::cout << " ... done!" << std::endl;
  std::cout << "Tree containing " << size << " ansätze constructed in" << std::endl;
  std::cout << "  " << diff.count() << " s" << std::endl;
//  std::cout << printForest(forest, 0);

  std::cout << std::endl;
  std::cout << "Evaluating indices {";
  std::for_each(eval_map.cbegin(), eval_map.cend(), [] (auto const & a) { std::cout << a.second << " "; });
  std::cout << "} .. " << std::endl;

  start = std::chrono::high_resolution_clock::now();
  auto _map = evaluateForest (forest, eval_map, 1);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;

  std::cout << "... done!" << std::endl;
  std::cout << "Ansatz evaluated in" << std::endl;
  std::cout << "  " << diff.count() << " s" << std::endl;

  std::cout << std::endl;
  std::cout << "Result of evaluation:" << std::endl;

  std::for_each(_map.cbegin(), _map.cend(),
    [](auto const & p) {
      std::cout << "Variable " << p.first << ":\t" << p.second << std::endl;
    });
#endif
  return 0;
}
