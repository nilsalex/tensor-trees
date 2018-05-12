#include <iostream>
#include <sstream>

#include "Algorithms.hxx"

#include "Scalar.hxx"

Scalar::Scalar (size_t variable, mpq_class const & fraction) : map(std::map<size_t, mpq_class>({{variable, fraction}})) { }

Scalar::Scalar (Scalar const & other) : map(other.map) { }

char Scalar::order () const { return 100; }

bool Scalar::isZero () const {
  return std::all_of(map.cbegin(), map.cend(),
    [] (auto const & p) {
      return (p.second == 0);
    });
}

std::map<size_t, mpq_class> const & Scalar::get() const { return map; }

std::string Scalar::print () const {
  std::stringstream ss;
  
  std::for_each(map.cbegin(), map.cend(),
    [&ss] (auto const & p) {
      ss << "Fraction " << p.second.get_num() << " " << p.second.get_den() << " Variable " << p.first << " ";
    });

  return ss.str();
}

void Scalar::multiply (mpq_class const & factor) {
  std::for_each(map.begin(), map.end(), [&factor] (auto & p) { p.second *= factor; });
}

std::unique_ptr<Node> Scalar::clone () const {
  return std::make_unique<Scalar>(*this);
}

std::map<size_t, mpq_class> Scalar::evaluateTree (Tree<Node> const &, std::map<char, size_t> const &, mpq_class prefactor) const {
  std::map<size_t, mpq_class> ret;
  std::for_each(map.cbegin(), map.cend(),
    [&ret,&prefactor] (auto const & p) {
      ret.insert({p.first, p.second * prefactor});
    });
  return ret;
}

int Scalar::applyTensorSymmetries (int parity) {
  std::for_each(map.begin(), map.end(),
    [&parity] (auto & p) {
      p.second *= parity;
    });
  return parity;
}

void Scalar::exchangeTensorIndices (std::map<char, char> const &) {
}

bool Scalar::lessThan (Node const * other) const {
  Scalar const & otherScalar = *(static_cast<Scalar const *>(other));

  return std::lexicographical_compare (map.cbegin(), map.cend(), otherScalar.map.cbegin(), otherScalar.map.cend());
}

bool Scalar::equals (Node const * other) const {
  Scalar const & otherScalar = *(static_cast<Scalar const *>(other));

  return std::equal(map.cbegin(), map.cend(), otherScalar.map.cbegin(), otherScalar.map.cend());
}

void Scalar::addOther(Scalar const * other) {
  std::for_each(other->map.cbegin(), other->map.cend(),
    [this] (auto const & p) {
      auto it = this->map.find(p.first);
      if (it == map.end()) {
        this->map.insert(p);
      } else {
        it->second += p.second;
      }
    });
}

std::set<size_t> Scalar::getVariableSet () const {
  std::set<size_t> ret;
  std::for_each(map.cbegin(), map.cend(),
    [&ret] (auto const & p) {
      ret.insert(p.first);
    });
  return ret;
}

std::map<size_t, mpq_class> const * Scalar::getCoefficientMap () const {
  return &map;
}

void Scalar::setVariablesToZero (Forest<Node> &, std::set<size_t> const & variables) {
  std::for_each(variables.cbegin(), variables.cend(),
    [this] (auto const & v) {
      this->map.erase(v);
    });
}

void Scalar::substituteVariables (std::map<size_t, size_t> const & subs_map) {
  Scalar new_scalar;

  std::for_each(map.begin(), map.end(),
    [&subs_map,&new_scalar] (auto & p) {
      if (subs_map.count(p.first) == 1) {
        auto const new_var = subs_map.at(p.first);
        new_scalar.map[new_var] = std::move(p.second);
      } else {
        new_scalar.map.insert(std::move(p));
      }
    });

  std::swap(this->map, new_scalar.map);
}
