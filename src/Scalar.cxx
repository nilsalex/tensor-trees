#include <sstream>
#include "Algorithms.hxx"

#include "Scalar.hxx"

Scalar::Scalar (size_t variable, mpq_class fraction) : variable(variable), fraction(fraction) { }

int Scalar::order () const { return 100; }

bool Scalar::isZero () const { return (fraction == 0); }

int Scalar::get_variable() const { return variable; }

std::string Scalar::print () const {
  std::stringstream ss;
  ss << "Fraction " << fraction.get_num() << " " << fraction.get_den() << " Scalar " << variable;

  return ss.str();
}

void Scalar::multiplyForest (Forest<Node> &, mpq_class factor) {
  fraction *= factor;
}

std::unique_ptr<Node> Scalar::clone () const {
  return std::unique_ptr<Node>(new Scalar(*this));
}

std::map<size_t, mpq_class> Scalar::evaluateTree (Tree<Node> const &, std::map<char, size_t> const &, mpq_class prefactor) const {
  return std::map<size_t, mpq_class> {{variable, fraction * prefactor}};
}

int Scalar::applyTensorSymmetries (Forest<Node> &, int parity) {
  fraction *= parity;
  return parity;
}

void Scalar::exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map) {
  ::exchangeTensorIndices (forest, exchange_map);
}

Forest<Node> Scalar::splitUnsortedTrees (std::unique_ptr<Tree<Node>> const & tree) const {
  Forest<Node> ret (1);
  ret[0] = std::make_unique<Tree<Node>>();
  ret[0]->node = tree->node->clone();
  ret[0]->forest = copyForest(tree->forest);
  return ret;
}

bool Scalar::lessThan (Node const * other) const {
  if (variable < static_cast<Scalar const *>(other)->variable) {
    return true;
  } else if (variable > static_cast<Scalar const *>(other)->variable) {
    return false;
  } else {
    return (fraction < static_cast<Scalar const *>(other)->fraction);
  }
}

bool Scalar::equals (Node const * other) const {
  return (variable == static_cast<Scalar const *>(other)->variable && fraction == static_cast<Scalar const *>(other)->fraction);
}

std::unique_ptr<Node> Scalar::addOther(Scalar const * other) {
  return std::make_unique<Scalar> (variable, fraction + other->fraction);
}

