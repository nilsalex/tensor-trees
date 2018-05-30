#include <iostream>

#include "Algorithms.hxx"
#include "Indices.hxx"

int main () {
  
  auto tree_eq_abcdmn = std::make_unique<Tree<Node>>();
  auto tree_eq_abcdmnpq = std::make_unique<Tree<Node>>();

  {
    auto tree_M_abcdmjnk   = loadTree ("area_1st_mass.prs");
    auto tree_M_abcdmjnkpq = loadTree ("area_1st_kinetic.prs");
  
    auto tree_M_abcdmjkl     = copyTree (tree_M_abcdmjnk);
    auto tree_M_abcdmjklpq   = copyTree (tree_M_abcdmjnkpq);

    exchangeTensorIndices (tree_M_abcdmjnk,   {{'e', 'm'}, {'f', 'j'}, {'g', 'n'}, {'h', 'k'}});
    exchangeTensorIndices (tree_M_abcdmjnkpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'n'}, {'h', 'k'}});
  
    exchangeTensorIndices (tree_M_abcdmjkl,   {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
    exchangeTensorIndices (tree_M_abcdmjklpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
  
    multiplyTree (tree_M_abcdmjnk,   2);
    multiplyTree (tree_M_abcdmjnkpq, 2);
  
    multiplyTree (tree_M_abcdmjkl,   -1);
    multiplyTree (tree_M_abcdmjklpq, -1);
  
    contractTreeWithEta (tree_M_abcdmjnk,   'j', 'k');
    contractTreeWithEta (tree_M_abcdmjnkpq, 'j', 'k');
  
    contractTreeWithEpsilon3 (tree_M_abcdmjkl,   'n', 'j', 'k', 'l');
    contractTreeWithEpsilon3 (tree_M_abcdmjklpq, 'n', 'j', 'k', 'l');
  
    std::swap (tree_eq_abcdmn, tree_M_abcdmjnk);
    mergeTrees (tree_eq_abcdmn, tree_M_abcdmjkl);
  
    std::swap (tree_eq_abcdmnpq, tree_M_abcdmjnkpq);
    mergeTrees (tree_eq_abcdmnpq, tree_M_abcdmjklpq);

    multiExchangeSymmetrizeTree (tree_eq_abcdmnpq, 
        {{{{'p', 'q'}, {'q', 'p'}}, 1},
         {{{'m', 'p'}, {'p', 'm'}}, 1},
         {{{'m', 'p'}, {'p', 'q'}, {'q', 'm'}}, 1},
         {{{'m', 'q'}, {'p', 'm'}, {'q', 'p'}}, 1},
         {{{'m', 'q'}, {'q', 'm'}}, 1}} );
  }

  auto eval_fun_mass = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
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

  auto eval_fun_kinetic = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};

    do {
      for (char i_m = 0; i_m < 4; ++i_m) {
        for (char i_n = 0; i_n < 4; ++i_n) {
          for (char i_p = 0; i_p < 4; ++i_p) {
            for (char i_q = 0; i_q < 4; ++i_q) {
              std::map<char, char> eval_map {
                {'a', area_indices.first.first},
                {'b', area_indices.first.second},
                {'c', area_indices.second.first},
                {'d', area_indices.second.second},
                {'m', i_m},
                {'n', i_n},
                {'p', i_p},
                {'q', i_q}
              };
              auto eval_res_tmp = evaluateTree (t, eval_map);
              if (!eval_res_tmp.empty()) {
                eval_res_set.insert (std::move(eval_res_tmp));
              }
            }
          }
        }
      }
    } while (nextAreaIndices(area_indices));

  };
  std::cout << "**************************************" << std::endl;
  std::cout << "** gauge invariance equation solver **" << std::endl;
  std::cout << "**     first order area metric      **" << std::endl;
  std::cout << "**************************************" << std::endl;
  
  std::cout << "### mass term equations ###" << std::endl;
  solveNumerical ({{tree_eq_abcdmn,   eval_fun_mass}});
 
  std::cout << std::endl;
  std::cout << "### kinetic term equations ###" << std::endl;
  solveNumerical ({{tree_eq_abcdmnpq, eval_fun_kinetic}});

}
