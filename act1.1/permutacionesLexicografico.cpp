#include <bits/stdc++.h>
#include <iostream>
#include <vector>

bool comp(int a, int b) { return a < b; }

void printVector(const std::vector<int> &vec) {
  for (int i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";
}

int findMaxIIndex(std::vector<int> &vec) {
  int maxIndex = -1;
  for (int i = 0; i < vec.size() - 1; i++) {
    if (vec[i] < vec[i + 1])
      maxIndex = i;
  }
  return maxIndex;
}

int findMaxJIndex(std::vector<int> &vec, int i) {
  int maxIndex = 0;
  for (int j = 0; j < vec.size(); j++) {
    if (vec[i] < vec[j])
      maxIndex = j;
  }
  return maxIndex;
}

void exchangeValues(std::vector<int> &vec, int i, int j) {
  int temp = vec[i];
  vec[i] = vec[j];
  vec[j] = temp;
}

void invertValues(std::vector<int> &vec, int i, int j) {
  if (i < 0)
    i = 0;
  if (j >= vec.size())
    j = vec.size() - 1;

  while (i < j) {
    std::swap(vec[i], vec[j]);
    i++;
    j--;
  }
}

/*
Se itera desde ordenado hasta invertirse completamente.
Por las permutaciones se obtiene cada combinación posible, volviendolo
factorial.
Complejidad: O(n!)
*/
bool nextPermutation(std::vector<int> &vec) {
  int i = findMaxIIndex(vec);
  if (i == -1)
    return false;

  int j = findMaxJIndex(vec, i);

  exchangeValues(vec, i, j);
  invertValues(vec, i + 1, vec.size() - 1);

  return true;
}

int main(int argc, char *argv[]) {
  std::cout << "Permutaciones en orden lexicográfico\n";

  std::vector<int> vec = {1, 2, 3, 6, 5, 4}; // Unordered
  printVector(vec);
  std::cout << "\n";

  // Needs to be ordered first
  std::cout << "Sorted Set\n";
  sort(vec.begin(), vec.end(), comp);

  do {
    printVector(vec);
  } while (nextPermutation(vec));

  return 0;
}