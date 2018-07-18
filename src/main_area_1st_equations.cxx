#include <iostream>

#include "Algorithms.hxx"
#include "Indices.hxx"

std::unique_ptr<Tree<Node>> firstOrderEquation () {
  auto tree_eq_abcdmn = std::make_unique<Tree<Node>>();

  auto tree_M_abcdmjnk   = loadTree ("area_1st_mass.prs");
  auto tree_M_abcdmjkl     = copyTree (tree_M_abcdmjnk);

  exchangeTensorIndices (tree_M_abcdmjnk,   {{'e', 'm'}, {'f', 'j'}, {'g', 'n'}, {'h', 'k'}});
  exchangeTensorIndices (tree_M_abcdmjkl,   {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});

  multiplyTree (tree_M_abcdmjnk,   2);
  multiplyTree (tree_M_abcdmjkl,   -1);

  contractTreeWithEta (tree_M_abcdmjnk,   'j', 'k');
  contractTreeWithEpsilon3 (tree_M_abcdmjkl,   'n', 'j', 'k', 'l');

  std::swap (tree_eq_abcdmn, tree_M_abcdmjnk);
  mergeTrees (tree_eq_abcdmn, tree_M_abcdmjkl);
  
  return std::move(tree_eq_abcdmn);
}

std::unique_ptr<Tree<Node>> secondOrderEquation () {
  auto tree_eq_abcdijklmn = std::make_unique<Tree<Node>>();

  auto tree_M_abcdijkl = loadTree ("area_1st_mass.prs");
  size_t first_order_var_num = getVariableSet (tree_M_abcdijkl).size();

  auto tree_M_abcdmjkl = copyTree(tree_M_abcdijkl);
  auto tree_M_mbcdijkl = copyTree(tree_M_abcdijkl);

  exchangeTensorIndices (tree_M_abcdijkl, {{'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
  exchangeTensorIndices (tree_M_abcdmjkl, {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
  exchangeTensorIndices (tree_M_mbcdijkl, {{'a', 'm'}, {'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});

  multiplyTree (tree_M_abcdijkl, -1);
  multiplyTree (tree_M_abcdmjkl, 4);
  multiplyTree (tree_M_mbcdijkl, 4);

  multiplyTreeWithEta (tree_M_abcdijkl, 'm', 'n');
  multiplyTreeWithEta (tree_M_abcdmjkl, 'i', 'n');
  multiplyTreeWithEta (tree_M_mbcdijkl, 'a', 'n');

  exchangeSymmetrizeTree (tree_M_abcdmjkl, {{'i', 'j'}, {'j', 'i'}}, -1);
  exchangeSymmetrizeTree (tree_M_abcdmjkl, {{'k', 'l'}, {'l', 'k'}}, -1);
  exchangeSymmetrizeTree (tree_M_abcdmjkl, {{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1);

  exchangeSymmetrizeTree (tree_M_mbcdijkl, {{'a', 'b'}, {'b', 'a'}}, -1);
  exchangeSymmetrizeTree (tree_M_mbcdijkl, {{'c', 'd'}, {'d', 'c'}}, -1);
  exchangeSymmetrizeTree (tree_M_mbcdijkl, {{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1);

  auto tree_Q_abcdmfnhijkl = loadTree ("area_2nd_mass.prs");
  shiftVariables (tree_Q_abcdmfnhijkl, first_order_var_num);

  auto tree_Q_abcdmfghijkl = copyTree (tree_Q_abcdmfnhijkl);

  exchangeTensorIndices (tree_Q_abcdmfnhijkl, {{'e', 'm'}, {'g', 'n'}});
  exchangeTensorIndices (tree_Q_abcdmfghijkl, {{'e', 'm'}});

  multiplyTree (tree_Q_abcdmfnhijkl, 24);
  multiplyTree (tree_Q_abcdmfghijkl, -12);

  contractTreeWithEta (tree_Q_abcdmfnhijkl, 'f', 'h');
  contractTreeWithEpsilon3 (tree_Q_abcdmfghijkl, 'n', 'f', 'g', 'h');

  mergeTrees (tree_eq_abcdijklmn, tree_M_abcdijkl);
  mergeTrees (tree_eq_abcdijklmn, tree_M_abcdmjkl);
  mergeTrees (tree_eq_abcdijklmn, tree_M_mbcdijkl);

  mergeTrees (tree_eq_abcdijklmn, tree_Q_abcdmfnhijkl);
  mergeTrees (tree_eq_abcdijklmn, tree_Q_abcdmfghijkl);

  return std::move(tree_eq_abcdijklmn);
}

int main () {
  
  auto tree_eq_1st = firstOrderEquation();
  auto tree_eq_2nd = secondOrderEquation();

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

  auto eval_fun_2nd = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
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
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));

  };

  solveNumerical ({
//      {tree_eq_1st, eval_fun_1st},
      {tree_eq_2nd, eval_fun_2nd},
      });

}
