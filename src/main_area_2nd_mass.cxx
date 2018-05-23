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
  std::string indices {"abcdefghijkl"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, -1},
    {{{'k', 'l'}, {'l', 'k'}}, -1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'h', 'f'}}, 1},
    {{{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1}
  };

  std::vector<std::pair<std::map<char, char>, int>> multi_exchange_symmetry = {
     {{{'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}, {'i', 'e'}, {'j', 'f'}, {'k', 'g'}, {'l', 'h'}}, 1},
     {{{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}}, 1},
     {{{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}, {'i', 'a'}, {'j', 'b'}, {'k', 'c'}, {'l', 'd'}}, 1},
     {{{'a', 'i'}, {'b', 'j'}, {'c', 'k'}, {'d', 'l'}, {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}, {'i', 'e'}, {'j', 'f'}, {'k', 'g'}, {'l', 'h'}}, 1},
     {{{'a', 'i'}, {'b', 'j'}, {'c', 'k'}, {'d', 'l'}, {'i', 'a'}, {'j', 'b'}, {'k', 'c'}, {'l', 'd'}}, 1}
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

  std::cout << "Applying multi exchange symmetry ..." << std::endl;
  multiExchangeSymmetrizeTree (tree, multi_exchange_symmetry);
  std::cout << "... applied." << std::endl;

  std::cout << std::endl;
  std::cout << "Sorting ansatz tree ..." << std::endl;
  sortTree (tree);
  redefineScalarsSym (tree);
  std::cout << "... sorted." << std::endl;
  std::cout << std::endl;

  auto variable_set = getVariableSet (tree);
  std::cout << "There are " << variable_set.size() << " variables." << std::endl;

  auto function = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::tuple<std::pair<std::pair<char, char>, std::pair<char, char>>, std::pair<std::pair<char, char>, std::pair<char, char>>, std::pair<std::pair<char, char>, std::pair<char, char>>> area_indices {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 23 * 22 * 21 / 6 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

          do {
            std::map<char, char> eval_map {
              {'a', std::get<0>(area_indices).first.first},
              {'b', std::get<0>(area_indices).first.second},
              {'c', std::get<0>(area_indices).second.first},
              {'d', std::get<0>(area_indices).second.second},
              {'e', std::get<1>(area_indices).first.first},
              {'f', std::get<1>(area_indices).first.second},
              {'g', std::get<1>(area_indices).second.first},
              {'h', std::get<1>(area_indices).second.second},
              {'i', std::get<2>(area_indices).first.first},
              {'j', std::get<2>(area_indices).first.second},
              {'k', std::get<2>(area_indices).second.first},
              {'l', std::get<2>(area_indices).second.second},
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / ((23 * 22 * 21) / 6);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
          } while (nextAreaIndicesTripleSymmetric(area_indices));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  evaluateNumerical (tree, function);

/*
  std::cout << "print tree? [y/n] ";

  char c;
  std::cin >> c;

  if (c == 'y') {
    std::cout << std::endl;
    std::cout << printTree (tree);
  }
*/

  std::ofstream of ("area_2nd_mass.txt");

  of << printTreeMaple (tree);

  return 0;
}
