#include <fstream>
#include <map>
#include <set>
#include <vector>

#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <Eigen/Dense>
#include <gmpxx.h>

#include "LinearAlgebra.hxx"

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

template<class Archive>
void eval_mat::serialize (Archive & ar, unsigned int const) {
  ar & values;
}

void eval_mat::save (std::string const & filename) const {
  std::ofstream file;
  file.open(filename);
  boost::archive::text_oarchive oa {file};
  oa << *this;
}

void eval_mat::load (std::string const & filename) {
  std::ifstream file;
  file.open(filename);
  boost::archive::text_iarchive ia {file};
  ia >> *this;
}

std::set<size_t> eval_mat::row_set () const {
  std::set<size_t> ret;
  std::transform (values.cbegin(), values.cend(), std::inserter (ret, ret.begin()),
    [] (auto const & v) {
      return v.first.second;
    });
  return ret;
}

std::set<size_t> findDependentVariables (std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> const & matrix, size_t rows, size_t cols) {
  MatrixXq mq(rows, cols);

  std::for_each(matrix.cbegin(), matrix.cend(),
    [&mq] (auto const & v) mutable {
      mq (v.first.first, v.first.second) = v.second;
    });

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "Rank of the system : " << lu_decompq.rank() << std::endl;
  
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
