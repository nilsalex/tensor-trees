#include <iostream>

#include "Algorithms.hxx"

#include "Builder.hxx"

#include "Node.hxx"

#include "Tensor.hxx"
#include "Epsilon.hxx"
#include "Eta.hxx"

#include "Scalar.hxx"

#include "Indices.hxx"

int main () {
//  auto tree = buildEpsilonEtaTree ("abcdefghijklpq");
  auto tree = loadTree ("area_2nd_kinetic.prs");

  contractTreeWithEpsilon3 (tree, 'n', 'j', 'k', 'l');

  canonicalizeTree (tree);

  std::cout << printTree (tree);
/*
  auto tree = std::make_unique<Tree<Node>>();

  {
    auto eta_ac = std::make_unique<Eta>('a', 'c');
    auto eta_bd = std::make_unique<Eta>('b', 'd');
    auto eta_mn = std::make_unique<Eta>('m', 'n');
    auto eta_bm = std::make_unique<Eta>('b', 'm');
    auto eta_dm = std::make_unique<Eta>('d', 'm');
    auto eta_dn = std::make_unique<Eta>('d', 'n');

    auto epsilon_abcn = std::make_unique<Epsilon>('a', 'b', 'c', 'n');
    auto epsilon_abcm = std::make_unique<Epsilon>('a', 'b', 'c', 'm');
    auto epsilon_abcd = std::make_unique<Epsilon>('a', 'b', 'c', 'd');

    auto scalar_a = std::make_unique<Scalar>(1, 1);
    auto scalar_b = std::make_unique<Scalar>(2, 1);
    auto scalar_c = std::make_unique<Scalar>(3, 1);
    auto scalar_d = std::make_unique<Scalar>(4, 1);
    auto scalar_e = std::make_unique<Scalar>(5, 1);

    std::vector<Node *> branch = {eta_ac.get(), eta_bd.get(), eta_mn.get(), scalar_a.get()};
    insertBranch (tree, branch);

    branch = {eta_ac.get(), eta_bm.get(), eta_dn.get(), scalar_b.get()};
    insertBranch (tree, branch);

    branch = {epsilon_abcn.get(), eta_dm.get(), scalar_c.get()};
    insertBranch (tree, branch);

    branch = {epsilon_abcm.get(), eta_dn.get(), scalar_d.get()};
    insertBranch (tree, branch);

    branch = {epsilon_abcd.get(), eta_mn.get(), scalar_e.get()};
    insertBranch (tree, branch);

    std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
      {{{'a', 'b'}, {'b', 'a'}}, -1},
      {{{'c', 'd'}, {'d', 'c'}}, -1},
      {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1}
    };

    std::for_each(exchange_symmetries.cbegin(), exchange_symmetries.cend(),
      [&tree,n=0,&exchange_symmetries] (auto const & p) mutable {
        exchangeSymmetrizeTree (tree, p.first, p.second);
      });

    sortTree (tree);
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

  solveNumerical (tree, eval_fun_mass);
*/

  return 0;
}
