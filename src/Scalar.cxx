#include <iostream>
#include <sstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/export.hpp>

#include "Scalar.hxx"
BOOST_CLASS_EXPORT(Scalar)

Scalar::Scalar (size_t variable, mpq_class const & fraction) : map(std::map<size_t, mpq_class>({{variable, fraction}})) { }

Scalar::Scalar (Scalar const & other) : map(other.map) { }

namespace boost {
namespace serialization {

template<class Archive>
void save (Archive & ar, const mpq_class & q, unsigned int const) {
  ar << q.get_num().get_si();
  ar << q.get_den().get_si();
}

template<class Archive>
void load (Archive & ar, mpq_class & q, unsigned int const) {
  long num, den;
  ar >> num;
  ar >> den;
  q = mpq_class (num, den);
}

}
}

BOOST_SERIALIZATION_SPLIT_FREE(mpq_class)

template<class Archive>
void Scalar::serialize (Archive & ar, unsigned int const) {
  ar & boost::serialization::base_object<Node>(*this);
  ar & map;
}

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

std::string Scalar::printMaple () const {
  std::stringstream ss;
  if (map.size() == 0) {
    assert(false);
  } else if (map.size() == 1) {
    auto const & p = *(map.begin());
    if (p.second == 0) {
      assert(false);
    } else if (p.second < 0) {
      ss << "(" << p.second << ")";
    } else {
      ss << p.second;
    }
    ss << " * " << "mu__" << p.first;
  } else {
    assert(false);
  }

  return ss.str();
}

void Scalar::multiply (mpq_class const & factor) {
  std::for_each(map.begin(), map.end(), [&factor] (auto & p) { p.second *= factor; });
}

std::unique_ptr<Node> Scalar::clone () const {
  return std::make_unique<Scalar>(*this);
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

void Scalar::removeVariables (std::set<size_t> const & variables) {
  std::for_each (variables.cbegin(), variables.cend(),
    [this] (auto const & v) {
      this->map.erase(v);
    });
}
