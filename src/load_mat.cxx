#include <iostream>

#include "LinearAlgebra.hxx"

int main () {
  eval_mat mat;
  mat.load("eval_mat.prs");
/*
  auto & v = mat.values;
  std::for_each(v.cbegin(), v.cend(),
    [] (auto const & a) {
      std::cout << a.first.first << "\t" << a.first.second << "\t" << a.second << std::endl;
    });
*/
  auto rows = mat.row_set();

  std::for_each (rows.cbegin(), rows.cend(),
    [n=0] (auto const & r) mutable {
      if (r - n > 1) {
        std::cout << n + 1 << " -- " << r - 1 << std::endl;
      }
      n = r;
    });

  return 0;
}
