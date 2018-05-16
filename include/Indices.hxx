#pragma once

#include <utility>

bool nextIndexPair (std::pair<char, char> & indices);

bool nextIndexPairSymmetric (std::pair<char, char> & indices);

bool nextIndexPairAntiSymmetric (std::pair<char, char> & indices);

bool nextAreaIndices (std::pair<std::pair<char, char>, std::pair<char, char>> & indices);

bool nextAreaIndicesPairSymmetric (std::pair<std::pair<std::pair<char, char>, std::pair<char, char>>, std::pair<std::pair<char, char>, std::pair<char, char>>> & indices);
