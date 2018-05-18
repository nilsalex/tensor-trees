#pragma once

#include <map>
#include <set>
#include <utility>
#include <vector>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/set.hpp>

#include <gmpxx.h>

struct eval_mat {
  friend class boost::serialization::access;

  std::set<std::pair<std::pair<size_t, size_t>, int>> values;

  template<class Archive>
  void serialize (Archive & ar, unsigned int const);

  void save (std::string const & filename) const;

  void load (std::string const & filename);

  std::set<size_t> row_set () const;

};

std::set<size_t> findDependentVariables (std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> const & matrix, size_t rows, size_t cols);
