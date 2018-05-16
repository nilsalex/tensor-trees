#include <algorithm>
#include "Algorithms.hxx"

#include "Tensor.hxx"

void Tensor::multiply (mpq_class const &) { }

std::set<size_t> Tensor::getVariableSet () const { return std::set<size_t> {}; }

std::map<size_t, mpq_class> const * Tensor::getCoefficientMap () const { return nullptr; }

void Tensor::substituteVariables (std::map<size_t, size_t> const &) { }
