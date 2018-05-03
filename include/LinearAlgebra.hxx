#pragma once

#include <set>
#include <vector>

#include <gmpxx.h>

std::set<size_t> findDependentVariables (std::set<std::vector<mpq_class>> const & matrix);
