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
  std::string indices {"ab"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, 1}
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
    std::pair<char, char> metric_indices {0, 0};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 10 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

    do {
            std::map<char, char> eval_map {
              {'a', metric_indices.first},
              {'b', metric_indices.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / (10);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
    } while (nextIndexPairSymmetric (metric_indices));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  reduceNumerical (tree, function);

  saveTree (tree, "ansatz_2.prs");

  if (checkSaveAndLoad(tree)) {
    std::cout << "Tree and saved tree coincide." << std::endl;
  } else {
    std::cout << "ERROR! TREE DOES NOT COINCIDE WITH SAVED TREE!" << std::endl;
  }

  std::cout << printTree (tree) << std::endl;

  return 0;
}
