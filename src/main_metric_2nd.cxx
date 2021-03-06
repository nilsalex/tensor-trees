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
  std::string indices {"abcdefpq"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, 1},
    {{{'c', 'd'}, {'d', 'c'}}, 1},
    {{{'e', 'f'}, {'f', 'e'}}, 1},
    {{{'a', 'c'}, {'b', 'd'}, {'p', 'q'}, {'c', 'a'}, {'d', 'b'}, {'q', 'p'}}, 1}
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

  std::for_each(exchange_symmetries.cbegin(), exchange_symmetries.cend(),
    [&tree,n=0,&exchange_symmetries] (auto const & p) mutable {
      std::cout << "Applying exchange symmetry " << ++n << " out of " << exchange_symmetries.size() << " ..." << std::endl;
      exchangeSymmetrizeTree (tree, p.first, p.second);
      redefineScalarsSym (tree);
      std::cout << "... applied." << std::endl;
    });

  std::cout << std::endl;
  std::cout << "Sorting ansatz tree ..." << std::endl;
  sortTree (tree);
  redefineScalarsSym (tree);
  std::cout << "... sorted." << std::endl;
  std::cout << std::endl;

  auto variable_set = getVariableSet (tree);
  std::cout << "There are " << variable_set.size() << " variables." << std::endl;

  auto function = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<std::pair<char, char>, std::pair<char, char>>,std::pair<std::pair<char, char>, std::pair<char, char>>> area_indices {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}};
    std::pair<char, char> metric_indices_1 {0, 0};
    std::pair<char, char> metric_indices_2 {0, 0};
    std::pair<char, char> metric_indices_3 {0, 0};
    std::pair<char, char> derivative_indices {0, 0};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 10 * 10 * 10 * 10 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

    do {
      do {
        do {
          do {
            std::map<char, char> eval_map {
              {'a', metric_indices_1.first},
              {'b', metric_indices_1.second},
              {'c', metric_indices_2.first},
              {'d', metric_indices_2.second},
              {'e', metric_indices_3.first},
              {'f', metric_indices_3.second},
              {'p', derivative_indices.first},
              {'q', derivative_indices.second},
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / (10 * 10 * 10 * 10);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
          } while (nextIndexPairSymmetric (metric_indices_1));
        } while (nextIndexPairSymmetric (metric_indices_2));
      } while (nextIndexPairSymmetric (metric_indices_3));
    } while (nextIndexPairSymmetric (derivative_indices));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  reduceNumerical (tree, function);

/*
  std::cout << "print tree? [y/n] ";

  char c;
  std::cin >> c;

  if (c == 'y') {
    std::cout << std::endl;
    std::cout << printTree (tree);
  }
*/
  saveTree (tree, "tree.prs");

  auto tree_2 = loadTree ("tree.prs");

  if (compareTrees (tree, tree_2)) {
    std::cout << "trees coincide!" << std::endl;
  } else {
    std::cout << "ERROR: TREES DO NOT COINCIDE!" << std::endl;
  }

  if (tree->isValid() && tree_2->isValid()) {
    std::cout << "both trees are valid." << std::endl;
  } else {
    std::cout << "ERROR: AT LEAST ONE TREE IS NOT VALID!" << std::endl;
  }

  return 0;
}
