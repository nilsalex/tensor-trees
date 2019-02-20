#pragma once
#include <memory>
#include <vector>

#include "Tree.hxx"

class EtaBuilder {
 private:
  size_t leaf_counter;

 public:
  EtaBuilder() : leaf_counter(0) {};
  EtaBuilder(size_t counter) : leaf_counter(counter) {};

  Tree * buildTree (std::string const & indices, Tree * parent = nullptr);

  size_t get_leaf_counter() const { return leaf_counter; };

  static std::vector<std::string> pickEta (std::string const & indices);

  ~EtaBuilder() = default;
};

class EpsilonBuilder {
 private:
  size_t leaf_counter;

 public:
  EpsilonBuilder() : leaf_counter(0) {};
  EpsilonBuilder(size_t counter) : leaf_counter(counter) {};

  Tree * buildTree (std::string const & indices, Tree * parent = nullptr);

  size_t get_leaf_counter() const { return leaf_counter; };

  static std::vector<std::string> pickEpsilon (std::string const & indices);

  ~EpsilonBuilder() = default;
};

Tree * buildEpsilonEtaTree (std::string const & indices);
