#include <iostream>
#include <vector>

/*
Encuentra 2 numeros *distintos* que sumados sean igual que target.
Complejidad: O(n^2).
*/
std::pair<int, int> twoSum(std::vector<int> vec, int target) {
  int n = vec.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (vec[i] + vec[j] == target) {
        return {i, j};
      }
    }
  }
  return {-1, -1};
}

int main(int argc, char *argv[]) {
  std::cout << "Brute Force Two-Sum\n";

  std::vector<int> vec = {1, 2, 3, 4, 5, 6};
  int target = 9;

  std::cout << "Set of Numbers: ";
  for (int i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";
  std::cout << "Target: ";
  std::cout << target << "\n\n";

  std::pair<int, int> result = twoSum(vec, target);

  if (result.first != -1) {
    std::cout << "Par encontrado en los índices: " << result.first << " y "
              << result.second << "\nLos números son: " << vec[result.first]
              << " y " << vec[result.second] << " = " << target << "\n";
  } else {
    std::cout << "No se encontró ningún par que sume "
              << "\n";
  }

  return 0;
}