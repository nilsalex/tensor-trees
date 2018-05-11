#include <algorithm>
#include "Algorithms.hxx"

#include "Tensor.hxx"

Tensor::Tensor(size_t const rank, std::string const & indices) : rank(rank), indices(indices) { }

size_t Tensor::get_rank() const { return rank; }
std::string const & Tensor::get_indices() const { return indices; }

void Tensor::multiply (mpq_class const &) { }

int Tensor::sortIndices() {
  int ret = 1;
  bool swapped = true;
  do {
    swapped = false;
    for (size_t counter = 1; counter < indices.size(); ++counter) {
      if (indices[counter - 1] == indices[counter]) {
        ret = 0;
      } else if (indices[counter - 1] > indices[counter]) {
        std::swap(indices[counter - 1], indices[counter]);
        swapped = true;
        ret *= -1;
      }
    }
  } while (swapped);

  return ret;
}

void Tensor::exchangeTensorIndices (std::map<char, char> const & exchange_map) {
  std::for_each(indices.begin(), indices.end(),
    [&exchange_map] (auto & i) {
      if (exchange_map.count(i) > 0) {
        i = exchange_map.at(i);
      }
    });
}

std::set<size_t> Tensor::getVariableSet () const { return std::set<size_t> {}; }

std::map<size_t, mpq_class> const & Tensor::getCoefficientMap () const { return std::map<size_t, mpq_class> {}; }

void Tensor::setVariablesToZero (Forest<Node> & forest, std::set<size_t> const & variables) {
  ::setVariablesToZero (forest, variables);
}

void Tensor::substituteVariables (std::map<size_t, size_t> const &) { }
