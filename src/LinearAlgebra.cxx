#include <set>
#include <vector>

#include <Eigen/Dense>
#include <gmpxx.h>

#include "LinearAlgebra.hxx"

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

std::set<size_t> findDependentVariables (std::set<std::vector<mpq_class>> const & matrix) {

  MatrixXq mq(matrix.size(), matrix.begin()->size());

  std::for_each(matrix.cbegin(), matrix.cend(),
    [row_counter=0,&mq] (auto const & v) mutable {
      std::for_each(v.cbegin(), v.cend(),
        [column_counter=0,&row_counter,&mq] (auto const & q) mutable {
          mq(row_counter, column_counter) = q;
          ++column_counter;
        });
      ++row_counter;
    });

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);
  
  MatrixXq kq = lu_decompq.kernel();

  std::set<size_t> ret; 

  for (int column_counter = 0; column_counter < kq.cols(); ++column_counter) {
    for (int row_counter = kq.rows() - 1; row_counter >= 0; --row_counter) {
      if (kq(row_counter, column_counter) == 0 ) {
        continue;
      } else if (std::find(ret.begin(), ret.end(), row_counter) != ret.end()) {
        continue;
      } else {
        ret.insert(row_counter);
        break;
      }
    }
  }

  return ret;
}
