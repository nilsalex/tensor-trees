#pragma once
#include <gmpxx.h>
#include <string>

#include "Tree.hxx"
#include "Node.hxx"

class Tensor : public Node {
 private:
 
 public:
  virtual void exchangeTensorIndices (std::map<char, char> const & exchange_map) = 0;
  virtual int sortIndices () = 0;

  std::set<size_t> getVariableSet () const override;
  std::map<size_t, mpq_class> const * getCoefficientMap () const override;

  virtual int evaluate(std::map <char, char> const & eval_map) const = 0;
  virtual mpq_class symmetrize() = 0;
  void multiply (mpq_class const & factor) override;
  void substituteVariables (std::map<size_t, size_t> const & subs_map) override;

  ~Tensor() = default;
};
