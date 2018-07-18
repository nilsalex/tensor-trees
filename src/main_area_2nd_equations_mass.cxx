#include <iostream>

#include "Algorithms.hxx"
#include "Indices.hxx"

int main () {
  
  auto tree_eq_abcdmn     = std::make_unique<Tree<Node>>();
  auto tree_eq_abcdijklmn = std::make_unique<Tree<Node>>();

  {
    auto tree_M_abcdmjkl   = loadTree ("area_1st_mass.prs");
    auto tree_Q_abcdmpnqijkl = loadTree ("area_2nd_mass.prs");

    shiftVariables (tree_Q_abcdmpnqijkl, getVariableSet(tree_M_abcdmjkl).size());

    {
      auto var_set_1 = getVariableSet (tree_M_abcdmjkl);
      size_t size_1 = var_set_1.size();
      auto var_set_2 = getVariableSet (tree_Q_abcdmpnqijkl);
      size_t size_2 = var_set_2.size();

      var_set_1.merge (var_set_2);
      assert (size_1 + size_2 == var_set_1.size());
    }

    auto tree_Q_abcdmpqrijkl = copyTree (tree_Q_abcdmpnqijkl);
    
    auto tree_M_abcdmjnk = copyTree (tree_M_abcdmjkl);

    exchangeTensorIndices (tree_M_abcdmjkl,     {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
    exchangeTensorIndices (tree_Q_abcdmpnqijkl, {{'e', 'm'}, {'f', 'p'}, {'g', 'n'}, {'h', 'q'}});
    exchangeTensorIndices (tree_Q_abcdmpqrijkl, {{'e', 'm'}, {'f', 'p'}, {'g', 'q'}, {'h', 'r'}});

    exchangeTensorIndices (tree_M_abcdmjnk,     {{'e', 'm'}, {'f', 'j'}, {'g', 'n'}, {'h', 'k'}});
    auto tree_M_abcdmjkl_2 = copyTree (tree_M_abcdmjkl);

    multiplyTree (tree_M_abcdmjnk,   2);
    multiplyTree (tree_M_abcdmjkl_2,   -1);

    contractTreeWithEta (tree_M_abcdmjnk,   'j', 'k');
    contractTreeWithEpsilon3 (tree_M_abcdmjkl_2,   'n', 'j', 'k', 'l');

    std::swap (tree_eq_abcdmn, tree_M_abcdmjnk);
    mergeTrees (tree_eq_abcdmn, tree_M_abcdmjkl_2);

    canonicalizeTree (tree_eq_abcdmn);
  
    multiplyTree (tree_Q_abcdmpnqijkl, 6);
    multiplyTree (tree_Q_abcdmpqrijkl, -3);
  
    contractTreeWithEta      (tree_Q_abcdmpnqijkl, 'p', 'q');
    contractTreeWithEpsilon3 (tree_Q_abcdmpqrijkl, 'n', 'p', 'q', 'r');

    multiplyTreeWithEta      (tree_M_abcdmjkl, 'i', 'n');

    exchangeSymmetrizeTree (tree_M_abcdmjkl, {{'i', 'j'}, {'j', 'i'}}, -1);
    exchangeSymmetrizeTree (tree_M_abcdmjkl, {{'k', 'l'}, {'l', 'k'}}, -1);
    exchangeSymmetrizeTree (tree_M_abcdmjkl, {{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1);
  
    std::swap (tree_eq_abcdijklmn, tree_M_abcdmjkl);
    mergeTrees (tree_eq_abcdijklmn, tree_Q_abcdmpnqijkl);
    mergeTrees (tree_eq_abcdijklmn, tree_Q_abcdmpqrijkl);

    canonicalizeTree (tree_eq_abcdijklmn);
  }

  auto eval_fun_1st = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};

      do {
        for (char i_m = 0; i_m < 4; ++i_m) {
          for (char i_n = 0; i_n < 4; ++i_n) {
            std::map<char, char> eval_map {
              {'a', area_indices.first.first},
              {'b', area_indices.first.second},
              {'c', area_indices.second.first},
              {'d', area_indices.second.second},
              {'m', i_m},
              {'n', i_n},
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
          }
        }
      } while (nextAreaIndices(area_indices));

  };

  auto eval_fun = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};

    do {
      do {
        for (char i_m = 0; i_m < 4; ++i_m) {
          for (char i_n = 0; i_n < 4; ++i_n) {
            std::map<char, char> eval_map {
              {'a', area_indices_1.first.first},
              {'b', area_indices_1.first.second},
              {'c', area_indices_1.second.first},
              {'d', area_indices_1.second.second},
              {'i', area_indices_2.first.first},
              {'j', area_indices_2.first.second},
              {'k', area_indices_2.second.first},
              {'l', area_indices_2.second.second},
              {'m', i_m},
              {'n', i_n},
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
          }
        }
      } while (nextAreaIndices(area_indices_1));
    } while (nextAreaIndices(area_indices_2));

  };

  std::cout << "**************************************" << std::endl;
  std::cout << "** gauge invariance equation solver **" << std::endl;
  std::cout << "**     first order area metric      **" << std::endl;
  std::cout << "**************************************" << std::endl;

  std::cout << "### second order equations ###" << std::endl;
  solveNumerical ({
//      {tree_eq_abcdmn, eval_fun_1st},
      {tree_eq_abcdijklmn, eval_fun}
  });

}
