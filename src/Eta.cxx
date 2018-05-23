#include <sstream>
#include "Algorithms.hxx"

#include "Eta.hxx"
#include "eval.hxx"

Eta::Eta(char const i1, char const i2) : i1(i1), i2(i2) {}

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
  auto other_eta = static_cast<Eta const *> (other);
  return (i1 == other_eta->i1 && i2 == other_eta->i2);
}

std::unique_ptr<Node> Eta::clone () const {
  return std::unique_ptr<Node>(new Eta(*this));
}
