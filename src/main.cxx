#include "Epsilon.hxx"
#include "Eta.hxx"
#include "Builder.hxx"

int main () {
  std::string indices {"abcdefghijklpqrstu"};

  auto tree = buildEpsilonEtaTree (indices);

  return 0;
}
