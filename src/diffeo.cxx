#include "Indices.hxx"

#include <gmpxx.h>

#include <algorithm>
#include <map>
#include <utility>
#include <set>
#include <iostream>

typedef int Variable;

typedef std::map<Variable, mpq_class> Equation;

std::set<int> getZeroVars (std::set<Equation> const & equations) {
  std::set<int> ret;

  std::for_each (equations.cbegin(), equations.cend(),
    [&ret] (auto const & equation) {
      if (equation.size() == 1) {
        ret.insert(equation.begin()->first);
      }
    });

  return ret;
}

std::set<Equation> getReducedEquations (std::set<Equation> const & equations, std::set<int> const & zero_vars) {
  std::set<Equation> ret;

  std::for_each (equations.cbegin(), equations.cend(),
    [&ret,&zero_vars] (auto const & equation) {
      Equation new_eq;

      std::for_each (equation.cbegin(), equation.cend(),
        [&new_eq, &zero_vars] (auto const & term) {
          if (zero_vars.find(term.first) == zero_vars.cend()) {
            new_eq.insert(term);
          }
        });
      if (new_eq.size() > 0) {
        ret.insert(new_eq);
      }
    });

  return ret;
}

void printEquations (std::set<Equation> const & equations) {
  std::for_each (equations.cbegin(), equations.cend(),
    [] (auto const & equation) {
      printEquation(equation);
      std::cout << std::endl;
    });
}

void printEquation (Equation const & equation) {
  std::for_each (equation.cbegin(), equation.cend(),
    [] (auto const & term) {
      if (term.second >= 0 ) {
        std::cout << " + ";
      } else {
        std::cout << " ";
      }
      std::cout << term.second << " * M_" << term.first;
    });
}

void normalize (Equation & equation) {
  std::for_each (equation.begin(), equation.end(),
    [c=equation.begin()->second] (auto & term) {
      if (term.second > 8) {
        throw(0);
      }
      term.second = term.second / c;
    });
}

void removeZeros (Equation & equation) {
  for (auto it = equation.begin(); it != equation.end(); ) {
    if (it->second == 0) {
      it = equation.erase(it);
    } else {
      ++it;
    }
  }
}

void updateOrInsert (std::pair<Variable, mpq_class> const & term, Equation & equation) {
                auto it = std::find_if (equation.begin(), equation.end(),
                  [&term] (auto & t) {
                    return (t.first == term.first);
                  });

                if (it == equation.end()) {
                  equation.insert(term);
                } else {
                  it->second += term.second;
                }
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

std::set<int> getVariablesNotContained(std::set<Equation> const & equations) {
  std::set<int> ret;

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

          int var = getVariableNumber(a, b, p , q, e, f, g, h);
          bool var_found = false;

          for (auto it_eq = equations.cbegin(); it_eq != equations.cend(); ++it_eq) {
            auto it_term = std::find_if (it_eq->cbegin(), it_eq->cend(),
              [var] (auto const & term) {
                return (term.first == var);
              });
            
            if (it_term != it_eq->cend()) {
              var_found = true;
              break;
            }
          }

          if (!var_found) {
            ret.insert(var);
          }
        } while (nextIndexPairSymmetric(pair_sym_ab));
      } while (nextIndexPairSymmetric(pair_sym_pq));
    } while (nextIndexPairSymmetric(pair_sym_ef));
  } while (nextIndexPairSymmetric(pair_sym_gh));

  return ret;
}

int main () {

  std::set<Equation> equations;

  std::pair<char, char> pair_sym_ab { 0, 0 };
  std::pair<char, char> pair_sym_pq { 0, 0 };
  std::pair<char, char> pair_sym_ef { 0, 0 };
  std::pair<char, char> pair_sym_gh { 0, 0 };

  do {
    do {
      do {
        do {
          for (char m = 0; m < 4; ++m) {
            for (char n = 0; n < 4; ++n) {

              char const & a = pair_sym_ab.first;
              char const & b = pair_sym_ab.second;
              char const & p = pair_sym_pq.first;
              char const & q = pair_sym_pq.second;
              char const & e = pair_sym_ef.first;
              char const & f = pair_sym_ef.second;
              char const & g = pair_sym_gh.first;
              char const & h = pair_sym_gh.second;

              Equation equation;
              
              std::pair<Variable, mpq_class> term;
              
              // delta m e
              if (m == e) {
                term = {getVariableNumber (a, b,
                                           p, q,
                                           n, f,
                                           g, h),
                        1};
                equation.insert(term);
              }

              // delta m g
              if (m == g) {
                term = {getVariableNumber (a, b,
                                           p, q,
                                           n, h,
                                           e, f),
                        1};
                updateOrInsert (term, equation);
              }

              // delta m f 
              if (m == f) {
                term = {getVariableNumber (a, b,
                                           p, q,
                                           n, e,
                                           g, h),
                        1};
                updateOrInsert (term, equation);
              }

              // delta m h
              if (m == h) {
                term = {getVariableNumber (a, b,
                                           p, q,
                                           n, g,
                                           e, f),
                        1};
                updateOrInsert (term, equation);
              }

              // delta b n 
              if (n == b) {
                term = { getVariableNumber (a, m,
                                            p, q,
                                            e, f,
                                            g, h),
                        -1};
                updateOrInsert (term, equation);
              }

              // delta a n
              if (n == a) {
                term = { getVariableNumber (b, m,
                                            p, q,
                                            e, f,
                                            g, h),
                        -1};
                updateOrInsert (term, equation);
              }

              // delta q n
              if (n == q) {
                term = {getVariableNumber (a, b,
                                           p, m,
                                           e, f,
                                           g, h),
                        -1};
                updateOrInsert (term, equation);
              }

              // delta p n
              if (n == p) {
                term = {getVariableNumber (a, b,
                                           q, m,
                                           e, f,
                                           g, h),
                        -1};
                updateOrInsert (term, equation);
              }

              removeZeros (equation);

              if (equation.size() > 0) {
                normalize (equation);
                equations.insert(equation);
              }

            }
          }
        } while (nextIndexPairSymmetric(pair_sym_ab));
      } while (nextIndexPairSymmetric(pair_sym_pq));
    } while (nextIndexPairSymmetric(pair_sym_ef));
  } while (nextIndexPairSymmetric(pair_sym_gh));

  printEquations (equations);

/*
  auto not_contained = getVariablesNotContained(equations);

  std::for_each (not_contained.cbegin(), not_contained.cend(),
    [] (auto const & v) {
      std::cout << v << std::endl;
    });
*/
  auto zero_vars = getZeroVars (equations);

  auto equations_reduced_1 = getReducedEquations (equations, zero_vars);

  std::cout << "#########################################################" << std::endl;
  std::cout << "##########   reduced equations  ########################" << std::endl;
  std::cout << "#########################################################" << std::endl;

  printEquations (equations_reduced_1);

  std::cout << zero_vars.size() << std::endl;
  std::cout << equations.size() << std::endl;
  std::cout << equations_reduced_1.size() << std::endl;
}
