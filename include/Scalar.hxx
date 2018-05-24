#pragma once
#include <boost/serialization/access.hpp>

#include "Node.hxx"

class Scalar : public Node {
 private:
  std::map<size_t, mpq_class> map;

  friend class boost::serialization::access;

 public:
  Scalar () = default;

  Scalar (size_t variable, mpq_class const & fraction);

  Scalar (Scalar const & other);

  char order () const override;
  std::map<size_t, mpq_class> const & get() const;
  std::string print () const override;
  std::string printMaple () const override;

  int applyTensorSymmetries (int parity) override;
  void exchangeTensorIndices (std::map<char, char> const & exchange_map);

  void multiply (mpq_class const & factor) override;

  std::unique_ptr<Node> clone () const override;

  std::set<size_t> getVariableSet () const override;
  std::map<size_t, mpq_class> const * getCoefficientMap () const override;

  void addOther(Scalar const * other);

  bool lessThan (Node const * other) const override;
  bool equals (Node const * other) const override;
  bool isZero () const;

  void substituteVariables (std::map<size_t, size_t> const & subs_map) override;
  void removeVariables (std::set<size_t> const & variables) override;

  template<class Archive>
  void serialize (Archive & ar, unsigned int const version);

  ~Scalar() = default;
};
