#include <fstream>
#include <map>
#include <set>
#include <vector>

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

std::set<size_t> findDependentVariables (std::set<std::map<size_t, mpq_class>> const & matrix) {
  size_t min_var = std::numeric_limits<size_t>::max();
  size_t max_var = 0;

  std::for_each (matrix.cbegin(), matrix.cend(),
    [&min_var,&max_var] (auto const & m) {
      auto it = m.begin();
      if (min_var > it->first) {
        min_var = it->first;
      }
      size_t _max_var = it->first;
      while (++it != m.end()) {
        _max_var = it->first;
      }
      if (max_var < _max_var) {
        max_var = _max_var;
      }
    });

  MatrixXq mq(matrix.size(), max_var - min_var + 1);

  std::for_each(matrix.cbegin(), matrix.cend(),
    [row_counter=0,&mq,&min_var,&max_var] (auto const & v) mutable {
      for (size_t col_counter = 0; col_counter == max_var - min_var + 1; ++col_counter) {
        if (v.count(min_var + col_counter) == 1) {
          mq(row_counter, col_counter) = v.at(min_var + col_counter);
        } else {
          mq(row_counter, col_counter) = 0;
        }
      }
      ++row_counter;
    });

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);
  
  MatrixXq kq = lu_decompq.kernel();
/*
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
*/
  return {};
}
