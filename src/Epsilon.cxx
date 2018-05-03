#include <sstream>
#include "Algorithms.hxx"

#include "Epsilon.hxx"
#include "eval.hxx"

Epsilon::Epsilon(char const i1, char const i2, char const i3, char const i4) : Tensor(4, std::string({i1, i2, i3, i4})) {}

int Epsilon::order () const { return 0; }

std::string Epsilon::print () const {
  std::stringstream ss;
  ss << "Epsilon {" << get_indices() << "}";

  return ss.str();
}

mpq_class Epsilon::evaluate(std::map <char, size_t> const & eval_map) const {
  return epsilon_eval.at(64 * eval_map.at(get_indices()[0])
                       + 16 * eval_map.at(get_indices()[1])
                       +  4 * eval_map.at(get_indices()[2])
                       +      eval_map.at(get_indices()[3]));
}

std::map<size_t, mpq_class> Epsilon::evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const {
  prefactor *= this->evaluate(eval_map);
  if (prefactor == 0) {
    return std::map<size_t, mpq_class> {};
  } else {
    return evaluateForest (tree.forest, eval_map, prefactor);
  }
}

mpq_class Epsilon::symmetrize() {
  return sortIndices();
}

int Epsilon::applyTensorSymmetries (Forest<Node> & forest, int parity) {
  parity *= sortIndices();
  if (parity == 0) {
    return 0;
  } else {
    ::applyTensorSymmetries(forest, parity);
    return parity;
  }
}

bool Epsilon::lessThan(Node const * other) const {
  return (get_indices() < static_cast<Epsilon const *>(other)->get_indices());
}

bool Epsilon::equals(Node const * other) const {
  return (get_indices() == static_cast<Epsilon const *>(other)->get_indices());
}

std::unique_ptr<Node> Epsilon::clone () const {
  return std::unique_ptr<Node>(new Epsilon(*this));
}

