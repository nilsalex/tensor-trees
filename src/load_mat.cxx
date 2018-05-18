#include <iostream>

#include <Eigen/Dense>
#include <gmpxx.h>

#include "LinearAlgebra.hxx"

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

int main () {
  eval_mat mat;
  mat.load("eval_mat.prs");

  std::map<size_t, size_t> var_map;
  std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> mpq_mat;

  std::for_each (mat.values.begin(), mat.values.end(),
    [n=0,&var_map,&mpq_mat] (auto & v) mutable {
      size_t var = v.first.second;
      auto it = var_map.find(var);
      if (it == var_map.end()) {
        var_map[var] = n;
        ++n;
      }
      mpq_class frac (v.second, 256);
      mpq_mat.insert(std::make_pair(std::make_pair(v.first.first, var_map.at(var)), frac));
    });

  auto dependent = findDependentVariables (mpq_mat, mpq_mat.rbegin()->first.first, var_map.size());

  std::cout << dependent.size() << " dependent variables" << std::endl;
  std::for_each(dependent.cbegin(), dependent.cend(), [] (auto const & v) { std::cout << v << std::endl; });

  return 0;
}
