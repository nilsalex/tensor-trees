#include <iostream>
#include <algorithm>
#include <utility>
#include <map>
#include <set>
#include <vector>

#include "Indices.hxx"

typedef std::vector<int> T;

std::vector<T> permutations_A {
  {0, 1, 2, 3},
  {1, 0, 2, 3},
  {0, 1, 3, 2},
  {1, 0, 3, 2},
  {2, 3, 0, 1},
  {3, 2, 0, 1},
  {2, 3, 1, 0},
  {3, 2, 1, 0}
};

std::vector<T> permutations_B {
  {0, 3, 1, 2},
  {1, 3, 0, 2},
  {0, 2, 1, 3},
  {1, 2, 0, 3},
  {2, 1, 3, 0},
  {3, 1, 2, 0},
  {2, 0, 3, 1},
  {3, 0, 2, 1},
  {0, 3, 2, 1},
  {1, 3, 2, 0},
  {0, 2, 3, 1},
  {1, 2, 3, 0},
  {2, 1, 0, 3},
  {3, 1, 0, 2},
  {2, 0, 1, 3},
  {3, 0, 1, 2}
};

std::pair<int, int> evaluate (T const & t1, T const & t2) {
  std::pair<int, int> ret {0, 0};

  std::for_each (permutations_A.cbegin(), permutations_A.cend(),
    [&ret,&t1,&t2] (auto const & p) {
      if (t1 == T {t2[p[0]], t2[p[1]], t2[p[2]], t2[p[3]]}) {
        ret.first += 1;
      }
    });

  std::for_each (permutations_B.cbegin(), permutations_B.cend(),
    [&ret,&t1,&t2] (auto const & p) {
      if (t1 == T {t2[p[0]], t2[p[1]], t2[p[2]], t2[p[3]]}) {
        ret.second += 1;
      }
    });

  return ret;
}

int getVariableNumber (int a, int b, int p, int q, int e, int f, int g, int h) {
  if (a > b) {
    std::swap (a, b);
  }

  if (p > q) {
    std::swap (p, q);
  }

  if (e > f) {
    std::swap (e, f);
  }

  if (g > h) {
    std::swap (g, h);
  }

  if (e > g || (e == g && f > h)) {
    std::swap (e, g);
    std::swap (f, h);
  }

  return (h + 4 * g + 16 * f + 64 * e + 256 * q + 1024 * p + 4096 * b + 16384 * a);
}

int main () {

  std::map<int, std::pair<int, int>> components;
  std::set<int> zero_components;

  std::pair<char, char> pair_sym_ab { 0, 0 };
  std::pair<char, char> pair_sym_pq { 0, 0 };
  std::pair<char, char> pair_sym_ef { 0, 0 };
  std::pair<char, char> pair_sym_gh { 0, 0 };

  do {
    do {
      do {
        do {
          char const & a = pair_sym_ab.first;
          char const & b = pair_sym_ab.second;
          char const & p = pair_sym_pq.first;
          char const & q = pair_sym_pq.second;
          char const & e = pair_sym_ef.first;
          char const & f = pair_sym_ef.second;
          char const & g = pair_sym_gh.first;
          char const & h = pair_sym_gh.second;

          auto pair = evaluate ( {a, b, p, q}, {e, f, g, h} );
          int var = getVariableNumber (a, b, p, q, e, f, g, h);

          if (pair == std::make_pair(0, 0)) {
            zero_components.insert(var);
          } else {
            components.insert(std::make_pair(var, pair));
          }

        } while (nextIndexPairSymmetric(pair_sym_ab));
      } while (nextIndexPairSymmetric(pair_sym_pq));
    } while (nextIndexPairSymmetric(pair_sym_ef));
  } while (nextIndexPairSymmetric(pair_sym_gh));

  std::for_each (components.cbegin(), components.cend(),
    [] (auto const & c) {
      if (c.second.first == 0) {
        std::cout << "M_" << c.first << " = " << c.second.second << " B" << std::endl;
      } else if (c.second.second == 0) {
        std::cout << "M_" << c.first << " = " << c.second.first << " A" << std::endl;
      } else {
        std::cout << "M_" << c.first << " = " << c.second.first << " A + " << c.second.second << " B" << std::endl;
      }
    });

  std::cout << zero_components.size() << std::endl;
  std::cout << components.size() << std::endl;

  return 0;
}
