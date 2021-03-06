#pragma once
#include <boost/serialization/access.hpp>

#include "Node.hxx"
#include "Tensor.hxx"

class Eta : public Tensor {
 private:
  char i1, i2;

  friend class boost::serialization::access;

 public:
  Eta() = default;
  Eta(char const i1, char const i2);

  char order () const override;

  void exchangeTensorIndices (std::map<char, char> const & exchange_map) override;
  int sortIndices () override;

  int evaluate(std::map <char, char> const & eval_map) const override;
  mpq_class symmetrize() override;
  bool containsIndex (char i) const override;
  std::string print () const override;
  std::string printMaple () const override;
  int applyTensorSymmetries (int parity) override;

  bool lessThan(Node const * other) const override;
  bool equals(Node const * other) const override;

  std::unique_ptr<Node> clone () const override;

  char getOther (char i) const;

  template<class Archive>
  void serialize (Archive & ar, unsigned int const version);

  ~Eta() = default;
};
