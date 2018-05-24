#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "Epsilon.hxx"
BOOST_CLASS_EXPORT(Epsilon)
#include "eval.hxx"

Epsilon::Epsilon(char const i1, char const i2, char const i3, char const i4) : i1(i1), i2(i2), i3(i3), i4(i4) {}

template<class Archive>
void Epsilon::serialize (Archive & ar, unsigned int const) {
  ar & boost::serialization::base_object<Node>(*this);
  ar & i1;
  ar & i2;
  ar & i3;
  ar & i4;
}

void Epsilon::exchangeTensorIndices (std::map<char, char> const & exchange_map) {
  auto it = exchange_map.find(i1);
  if (it != exchange_map.end()) {
    i1 = it->second;
  }
  it = exchange_map.find(i2);
  if (it != exchange_map.end()) {
    i2 = it->second;
  }
  it = exchange_map.find(i3);
  if (it != exchange_map.end()) {
    i3 = it->second;
  }
  it = exchange_map.find(i4);
  if (it != exchange_map.end()) {
    i4 = it->second;
  }
}

int Epsilon::sortIndices() {
  int ret = 1;
  bool swapped = true;
  do {
    swapped = false;
    if (i1 == i2) {
      ret = 0;
    } else if (i1 > i2) {
      std::swap (i1, i2);
      swapped = true;
      ret *= -1;
    }
    if (i2 == i3) {
      ret = 0;
    } else if (i2 > i3) {
      std::swap (i2, i3);
      swapped = true;
      ret *= -1;
    }
    if (i3 == i4) {
      ret = 0;
    } else if (i3 > i4) {
      std::swap (i3, i4);
      swapped = true;
      ret *= -1;
    }
  } while (swapped);

  return ret;
}

char Epsilon::order () const { return 0; }

std::string Epsilon::print () const {
  std::stringstream ss;
  ss << "Epsilon {" << i1 << i2 << i3 << i4 << "}";

  return ss.str();
}

std::string Epsilon::printMaple () const {
  std::stringstream ss;
  ss << "LeviCivita[~" << i1 << ", ~" << i2 << ", ~" << i3 << ", ~" << i4 << "]";

  return ss.str();
}

int Epsilon::evaluate(std::map <char, char> const & eval_map) const {
  return (- 1 * epsilon_eval.at(64 * eval_map.at(i1)
                       + 16 * eval_map.at(i2)
                       +  4 * eval_map.at(i3)
                       +      eval_map.at(i4)));
}

mpq_class Epsilon::symmetrize() {
  return sortIndices();
}

bool Epsilon::containsIndex (char i) const {
  return (i == i1 || i == i2 || i == i3 || i == i4);
}

int Epsilon::applyTensorSymmetries (int parity) {
  int epsilon_parity = sortIndices();
  return (parity * epsilon_parity);
}

bool Epsilon::lessThan(Node const * other) const {
  auto other_eps = static_cast<Epsilon const *>(other);
  if (i1 < other_eps->i1) {
    return true;
  } else if (i1 > other_eps->i1) {
    return false;
  } else if (i2 < other_eps->i2) {
    return true;
  } else if (i2 > other_eps->i2) {
    return false;
  } else if (i3 < other_eps->i3) {
    return true;
  } else if (i3 > other_eps->i3) {
    return false;
  } else if (i4 < other_eps->i4) {
    return true;
  } else {
    return false;
  }
}

bool Epsilon::equals(Node const * other) const {
  if (other == nullptr) {
    return false;
  } else if (typeid(*this) != typeid(*other)) {
    return false;
  } else {
    auto other_eps = static_cast<Epsilon const *> (other);
    return (i1 == other_eps->i1 && i2 == other_eps->i2 && i3 == other_eps->i3 && i4 == other_eps->i4);
  }
}

std::unique_ptr<Node> Epsilon::clone () const {
  return std::unique_ptr<Node>(new Epsilon(*this));
}

