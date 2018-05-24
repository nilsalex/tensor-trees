#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "Tensor.hxx"
BOOST_CLASS_EXPORT(Tensor)

void Tensor::multiply (mpq_class const &) { }

template<class Archive>
void Tensor::serialize (Archive & ar, unsigned int const version) {
  ar & boost::serialization::base_object<Node>(*this);
}

std::set<size_t> Tensor::getVariableSet () const { return std::set<size_t> {}; }

std::map<size_t, mpq_class> const * Tensor::getCoefficientMap () const { return nullptr; }

void Tensor::substituteVariables (std::map<size_t, size_t> const &) { }

void Tensor::removeVariables (std::set<size_t> const &) { }
