#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "Eta.hxx"
BOOST_CLASS_EXPORT(Eta)

Eta::Eta(char const i1, char const i2) : i1(i1), i2(i2) {}

template<class Archive>
void Eta::serialize (Archive & ar, unsigned int const) {
  ar & boost::serialization::base_object<Node>(*this);
  ar & i1;
  ar & i2;
}

void Eta::exchangeTensorIndices (std::map<char, char> const & exchange_map) {
  auto it = exchange_map.find(i1);
  if (it != exchange_map.end()) {
    i1 = it->second;
  }
  it = exchange_map.find(i2);
  if (it != exchange_map.end()) {
    i2 = it->second;
  }
}

int Eta::sortIndices() {
  if (i1 > i2) {
    std::swap (i1, i2);
  }
  return 1;
}

char Eta::order () const { return 10; }

std::string Eta::print () const {
  std::stringstream ss;
  ss << "Eta {" << i1 << i2 << "}";

  return ss.str();
}

std::string Eta::printMaple () const {
  std::stringstream ss;
  ss << "g_[~" << i1 << ", ~" << i2 << "]";

  return ss.str();
}

int Eta::evaluate(std::map <char, char> const & eval_map) const {
  char _i1 = eval_map.at(i1);
  char _i2 = eval_map.at(i2);
  if (_i1 != _i2) {
    return 0;
  } else if (_i1 > 0) {
    return -1;
  } else {
    return 1;
  }
}

mpq_class Eta::symmetrize() {
  sortIndices();
  return 1;
}

bool Eta::containsIndex (char i) const {
  return (i == i1 || i == i2);
}

int Eta::applyTensorSymmetries (int parity) {
  sortIndices();
  return parity;
}

bool Eta::lessThan(Node const * other) const {
  auto other_eta = static_cast<Eta const *> (other);
  if (i1 < other_eta->i1) {
    return true;
  } else if (i1 > other_eta->i1) {
    return false;
  } else if (i2 < other_eta->i2) {
    return true;
  } else {
    return false;
  }
}

bool Eta::equals(Node const * other) const {
  if (other == nullptr) {
    return false;
  } else if (typeid(*this) != typeid (*other)) {
    return false;
  } else {
    auto other_eta = static_cast<Eta const *> (other);
    return (i1 == other_eta->i1 && i2 == other_eta->i2);
  }
}

std::unique_ptr<Node> Eta::clone () const {
  return std::unique_ptr<Node>(new Eta(*this));
}

char Eta::getOther (char i) const {
  assert (i1 != i2);
  assert ((i == i1) != (i == i2));
  if (i == i1) {
    return i2;
  } else {
    return i1;
  }
}
