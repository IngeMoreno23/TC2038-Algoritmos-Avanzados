#pragma once

#include <string>
#include <vector>
#include <utility>

namespace z_array {

  std::vector<std::pair<int, int>> stringMatch(const std::string& text, const std::string& pattern);

  std::pair<int, int> stringMatchFirst(const std::string& text, const std::string& pattern);

}