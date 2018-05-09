#include <sstream>
#include "Algorithms.hxx"

#include "Eta.hxx"
#include "eval.hxx"

Eta::Eta(char const i1, char const i2) : Tensor(2, std::string({i1, i2})) {}

int Eta::order () const { return 10; }

std::string Eta::print () const {
  std::stringstream ss;
  ss << "Eta {" << get_indices() << "}";

  return ss.str();
}

mpq_class Eta::evaluate(std::map <char, size_t> const & eval_map) const {
  return eta_eval.at(4 * eval_map.at(get_indices()[0]) + eval_map.at(get_indices()[1]));
}

std::map<size_t, mpq_class> Eta::evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const {
  prefactor *= this->evaluate(eval_map);
  if (prefactor == 0) {
    return std::map<size_t, mpq_class> {};
  } else {
    return evaluateForest (tree.forest, eval_map, prefactor);
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
  return (get_indices() < static_cast<Eta const *>(other)->get_indices());
}

bool Eta::equals(Node const * other) const {
  return (get_indices() == static_cast<Eta const *>(other)->get_indices());
}

std::unique_ptr<Node> Eta::clone () const {
  return std::unique_ptr<Node>(new Eta(*this));
}
