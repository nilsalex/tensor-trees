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
typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, 1> VectorXq;

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
  MatrixXq mq(cols, rows);

  std::for_each(matrix.cbegin(), matrix.cend(),
    [&mq] (auto const & v) {
      mq (v.first.second, v.first.first) = v.second;
    });

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "rank of the system : " << lu_decompq.rank() << std::endl;

  std::set<size_t> ret;
  VectorXq vector = VectorXq::Zero (cols);
  for (size_t counter = 0; counter < cols; ++counter) {
    if (counter != 0) {
      vector (cols - counter) = 0;
    }
    vector (cols - counter - 1) = 1;   
    if (lu_decompq.solve(vector).isZero()) {
      ret.insert (cols - counter - 1);
    }
  }

  return ret;
}

std::map<size_t, std::map<size_t, mpq_class>> solveLinearSystem (std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> const & matrix, size_t rows, size_t cols) {
  MatrixXq mq (rows, cols);

  std::for_each (matrix.cbegin(), matrix.cend(),
      [&mq] (auto const & v) {
        mq (v.first.first, v.first.second) = v.second;
      });

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "rank of the system : " << lu_decompq.rank() << std::endl;

  std::cout << lu_decompq.kernel() << std::endl;

  return {};

}
