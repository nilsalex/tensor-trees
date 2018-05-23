#include <cassert>
#include "Indices.hxx"

bool nextIndexPair (std::pair<char, char> & indices) {
  if (indices.second < 3) {
    indices.second++;
    return true;
  } else if (indices.first < 3) {
    indices.first++;
    indices.second = 0;
    return true;
  } else {
    indices = {0, 0};
    return false;
  }
}

bool nextIndexPairSymmetric (std::pair<char, char> & indices) {
  assert (indices.first <= indices.second);
  if (indices.second < 3) {
    indices.second++;
    assert (indices.first <= indices.second);
    return true;
  } else if (indices.first != indices.second) {
    indices.first++;
    indices.second = indices.first;
    assert (indices.first <= indices.second);
    return true;
  } else {
    indices = {0, 0};
    assert (indices.first <= indices.second);
    return false;
  }
}

bool nextIndexPairAntiSymmetric (std::pair<char, char> & indices) {
  assert (indices.first < indices.second);
  if (indices.second < 3) {
    ++indices.second;
    assert (indices.first < indices.second);
    return true;
  } else if (indices.second > indices.first + 1) {
    ++indices.first;
    indices.second = indices.first + 1;
    assert (indices.first < indices.second);
    return true;
  } else {
    indices = {0, 1};
    return false;
  }
}

bool nextAreaIndices (std::pair<std::pair<char, char>, std::pair<char, char>> & indices) {
  assert (indices.first <= indices.second);
  assert (indices.first.first < indices.first.second);
  assert (indices.second.first < indices.second.second);
  if (indices.second < std::pair<char, char> {2, 3}) {
    nextIndexPairAntiSymmetric (indices.second);
    assert (indices.first <= indices.second);
    assert (indices.first.first < indices.first.second);
    assert (indices.second.first < indices.second.second);
    return true;
  } else if (indices.first != indices.second) {
    nextIndexPairAntiSymmetric (indices.first);
    indices.second = indices.first;
    assert (indices.first <= indices.second);
    assert (indices.first.first < indices.first.second);
    assert (indices.second.first < indices.second.second);
    return true;
  } else {
    indices = {{0, 1}, {0, 1}};
    assert (indices.first <= indices.second);
    assert (indices.first.first < indices.first.second);
    assert (indices.second.first < indices.second.second);
    return false;
  }
}

bool nextAreaIndicesPairSymmetric (std::pair<std::pair<std::pair<char, char>, std::pair<char, char>>, std::pair<std::pair<char, char>, std::pair<char, char>>> & indices) {
  assert (indices.first <= indices.second);
  if (indices.second < std::pair<std::pair<char, char>, std::pair<char, char>> {{2, 3}, {2, 3}}) {
    nextAreaIndices (indices.second);
    assert (indices.first <= indices.second);
    return true;
  } else if (indices.first != indices.second) {
    nextAreaIndices (indices.first);
    indices.second = indices.first;
    assert (indices.first <= indices.second);
    return true;
  } else {
    indices = {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}};
    assert (indices.first <= indices.second);
    return false;
  }
}

bool nextAreaIndicesTripleSymmetric (std::tuple<std::pair<std::pair<char, char>, std::pair<char, char>>, std::pair<std::pair<char, char>, std::pair<char, char>>, std::pair<std::pair<char, char>, std::pair<char, char>>> & indices) {
  assert (std::get<0>(indices) <= std::get<1>(indices));
  assert (std::get<1>(indices) <= std::get<2>(indices));
  assert (std::get<0>(indices) <= std::get<2>(indices));
  if (std::get<2>(indices) < std::pair<std::pair<char, char>, std::pair<char, char>> {{2, 3}, {2, 3}}) {
    nextAreaIndices (std::get<2>(indices));
  assert (std::get<0>(indices) <= std::get<1>(indices));
  assert (std::get<1>(indices) <= std::get<2>(indices));
  assert (std::get<0>(indices) <= std::get<2>(indices));
    return true;
  } else if (std::get<1>(indices) != std::get<2>(indices)) {
    nextAreaIndices (std::get<1>(indices));
    std::get<2>(indices) = std::get<1>(indices);
    assert (std::get<0>(indices) <= std::get<1>(indices));
    assert (std::get<1>(indices) <= std::get<2>(indices));
    assert (std::get<0>(indices) <= std::get<2>(indices));
    return true;
  } else if (std::get<0> (indices) != std::get<1> (indices)) {
    nextAreaIndices (std::get<0>(indices));
    std::get<1>(indices) = std::get<0>(indices);
    std::get<2>(indices) = std::get<0>(indices);
    assert (std::get<0>(indices) <= std::get<1>(indices));
    assert (std::get<1>(indices) <= std::get<2>(indices));
    assert (std::get<0>(indices) <= std::get<2>(indices));
    return true;
  } else {
    indices = {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}};
    assert (std::get<0>(indices) <= std::get<1>(indices));
    assert (std::get<1>(indices) <= std::get<2>(indices));
    assert (std::get<0>(indices) <= std::get<2>(indices));
    return false;
  }
}
