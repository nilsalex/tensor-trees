#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "Epsilon.hxx"
BOOST_CLASS_EXPORT(Epsilon)
#include "eval.hxx"

Epsilon::Epsilon(char const i1, char const i2, char const i3, char const i4) : i1(i1), i2(i2), i3(i3), i4(i4) {}

template<class Archive>
void Epsilon::serialize (Archive & ar, unsigned int const) {
  ar & boost::serialization::base_object<Node>(*this);
  ar & i1;
  ar & i2;
  ar & i3;
  ar & i4;
}

void Epsilon::exchangeTensorIndices (std::map<char, char> const & exchange_map) {
  auto it = exchange_map.find(i1);
  if (it != exchange_map.end()) {
    i1 = it->second;
  }
  it = exchange_map.find(i2);
  if (it != exchange_map.end()) {
    i2 = it->second;
  }
  it = exchange_map.find(i3);
  if (it != exchange_map.end()) {
    i3 = it->second;
  }
  it = exchange_map.find(i4);
  if (it != exchange_map.end()) {
    i4 = it->second;
  }
}

int Epsilon::sortIndices() {
  int ret = 1;
  bool swapped = true;
  do {
    swapped = false;
    if (i1 == i2) {
      ret = 0;
    } else if (i1 > i2) {
      std::swap (i1, i2);
      swapped = true;
      ret *= -1;
    }
    if (i2 == i3) {
      ret = 0;
    } else if (i2 > i3) {
      std::swap (i2, i3);
      swapped = true;
      ret *= -1;
    }
    if (i3 == i4) {
      ret = 0;
    } else if (i3 > i4) {
      std::swap (i3, i4);
      swapped = true;
      ret *= -1;
    }
  } while (swapped);

  return ret;
}

char Epsilon::order () const { return 0; }

std::string Epsilon::print () const {
  std::stringstream ss;
  ss << "Epsilon {" << i1 << i2 << i3 << i4 << "}";

  return ss.str();
}

std::string Epsilon::printMaple () const {
  std::stringstream ss;
  ss << "LeviCivita[~" << i1 << ", ~" << i2 << ", ~" << i3 << ", ~" << i4 << "]";

  return ss.str();
}

int Epsilon::evaluate(std::map <char, char> const & eval_map) const {
  return (- 1 * epsilon_eval.at(64 * eval_map.at(i1)
                       + 16 * eval_map.at(i2)
                       +  4 * eval_map.at(i3)
                       +      eval_map.at(i4)));
}

mpq_class Epsilon::symmetrize() {
  return sortIndices();
}

bool Epsilon::containsIndex (char i) const {
  return (i == i1 || i == i2 || i == i3 || i == i4);
}

int Epsilon::applyTensorSymmetries (int parity) {
  int epsilon_parity = sortIndices();
  return (parity * epsilon_parity);
}

bool Epsilon::lessThan(Node const * other) const {
  auto other_eps = static_cast<Epsilon const *>(other);
  if (i1 < other_eps->i1) {
    return true;
  } else if (i1 > other_eps->i1) {
    return false;
  } else if (i2 < other_eps->i2) {
    return true;
  } else if (i2 > other_eps->i2) {
    return false;
  } else if (i3 < other_eps->i3) {
    return true;
  } else if (i3 > other_eps->i3) {
    return false;
  } else if (i4 < other_eps->i4) {
    return true;
  } else {
    return false;
  }
}

bool Epsilon::equals(Node const * other) const {
  if (other == nullptr) {
    return false;
  } else if (typeid(*this) != typeid(*other)) {
    return false;
  } else {
    auto other_eps = static_cast<Epsilon const *> (other);
    return (i1 == other_eps->i1 && i2 == other_eps->i2 && i3 == other_eps->i3 && i4 == other_eps->i4);
  }
}

std::unique_ptr<Node> Epsilon::clone () const {
  return std::unique_ptr<Node>(new Epsilon(*this));
}

std::tuple<int, char, std::map<char, char>> Epsilon::multiplyWithOther3 (char j1, char j2, char j3) const {
  assert (j1 != j2 && j1 != j3 && j2 != j3);
  bool const is_contained_j1 = containsIndex (j1);
  bool const is_contained_j2 = containsIndex (j2);
  bool const is_contained_j3 = containsIndex (j3);
  std::vector<char> contained;
  if (is_contained_j1) { contained.push_back (j1); }
  if (is_contained_j2) { contained.push_back (j2); }
  if (is_contained_j3) { contained.push_back (j3); }

  if (contained.size() == 0) {
    return {-24, i1, {{j1, i2}, {j2, i3}, {j3, i4}}};
  } else if (contained.size() == 3) {
    std::vector<char> i_vec {i1, i2, i3, i4};
    int parity = 1;
    if (std::find (contained.cbegin(), contained.cend(), i_vec[0]) == contained.cend()) {
      // do nothing
    } else if (std::find (contained.cbegin(), contained.cend(), i_vec[1]) == contained.cend()) {
      std::iter_swap (i_vec.begin(), i_vec.begin() + 1);
      parity *= -1;
    } else if (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend()) {
      std::iter_swap (i_vec.begin(), i_vec.begin() + 2);
      parity *= -1;
    } else {
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
      std::iter_swap (i_vec.begin(), i_vec.begin() + 3);
      parity *= -1;
    }
    if ( (i_vec[1] == j1 && i_vec[2] == j2 && i_vec[3] == j3) ||
         (i_vec[2] == j1 && i_vec[3] == j2 && i_vec[1] == j3) ||
         (i_vec[3] == j1 && i_vec[1] == j2 && i_vec[2] == j3) ) {
      // sorted
    } else {
      // unsorted
      assert ( (i_vec[1] == j1 && i_vec[3] == j2 && i_vec[2] == j3) ||
               (i_vec[3] == j1 && i_vec[2] == j2 && i_vec[1] == j3) ||
               (i_vec[2] == j1 && i_vec[1] == j2 && i_vec[3] == j3));
      parity *= -1;
    }
    return {-6 * parity, i_vec[0], { }};
  } else if (contained.size() == 2) {
    std::vector<char> i_vec {i1, i2, i3, i4};
    std::vector<char> j_vec {j1, j2, j3};
    int parity = 1;
    if (!is_contained_j1) {
      // nothing
    } else if (!is_contained_j2) {
      j_vec = {j2, j1, j3};
      parity *= -1;
    } else {
      assert (!is_contained_j3);
      j_vec = {j3, j1, j2};
    }
    if (std::find (contained.cbegin(), contained.cend(), i_vec[0]) == contained.cend()) {
      if (std::find (contained.cbegin(), contained.cend(), i_vec[1]) == contained.cend()) {
        // do nothing
      } else if (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend()) {
        std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 2);
        parity *= -1;
      } else {
        assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
        std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 3);
        parity *= -1;
      }
    } else if (std::find (contained.cbegin(), contained.cend(), i_vec[1]) == contained.cend()) {
      std::iter_swap (i_vec.begin(), i_vec.begin() + 1);
      parity *= -1;
      if (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend()) {
        std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 2);
        parity *= -1;
      } else {
        assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
        std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 3);
        parity *= -1;
      }
    } else {
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
      std::iter_swap (i_vec.begin(), i_vec.begin() + 2);
      std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 3);
    }
    if (i_vec[2] == contained[0]) {
      // sorted
    } else {
      assert (i_vec[2] == contained[1]);
      parity *= -1;
    }
    return { -4 * parity, i_vec[0], {{j_vec[0], i_vec[1]}} };
  } else if (contained.size() == 1) {
    std::vector<char> i_vec {i1, i2, i3, i4};
    std::vector<char> j_vec {j1, j2, j3};
    int parity = 1;
    if (!is_contained_j1) {
      if (!is_contained_j2) {
        // nothing
      } else {
        assert (!is_contained_j3);
        j_vec = {j1, j3, j2};
        parity *= -1;
      }
    } else {
      assert (!is_contained_j2);
      assert (!is_contained_j3);
      j_vec = {j2, j3, j1};
    }
    if (std::find (contained.cbegin(), contained.cend(), i_vec[0]) != contained.cend()) {
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[1]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
      std::iter_swap (i_vec.begin(), i_vec.begin() + 1);
      std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 2);
      std::iter_swap (i_vec.begin() + 2, i_vec.begin() + 3);
      parity *= -1;
    } else if (std::find (contained.cbegin(), contained.cend(), i_vec[1]) != contained.cend()) {
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[0]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
      std::iter_swap (i_vec.begin() + 1, i_vec.begin() + 2);
      std::iter_swap (i_vec.begin() + 2, i_vec.begin() + 3);
    } else if (std::find (contained.cbegin(), contained.cend(), i_vec[2]) != contained.cend()) {
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[0]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[1]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[3]) == contained.cend());
      std::iter_swap (i_vec.begin() + 2, i_vec.begin() + 3);
      parity *= -1;
    } else if (std::find (contained.cbegin(), contained.cend(), i_vec[3]) != contained.cend()) {
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[0]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[1]) == contained.cend());
      assert (std::find (contained.cbegin(), contained.cend(), i_vec[2]) == contained.cend());
      // nothing
    }
    return { -6 * parity, i_vec[0], {{j_vec[0], i_vec[1]}, {j_vec[1], i_vec[2]}} };
  } else {
    assert (false);
    return {0, 0, {}};
  }
}
