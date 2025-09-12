#include <iostream>
#include <vector>

/*
Regresa el k-esimo elemento en orden de magnitud de un arreglo desordenado.
Separa según un pivote e itera dentro de la ventana que genera el pivote.
Complejidad: O(n^2)
 */
int quickSelect(std::vector<int> vec, int k) {
  int n = vec.size();
  if (k < 0 || k >= n)
    throw std::logic_error("Quick Select Index out of range.");
  int l = 0;
  int r = n - 1;

  int s = -1;

  while (s != k) {
    s = l;
    int pivot = vec[l];

    for (int i = l + 1; i < r; i++) {
      if (vec[i] < pivot) {
        s++;
        // Intercambio
        int temp = vec[s];
        vec[s] = vec[i];
        vec[i] = temp;
      }
    }
    // Intercambio pivote a s
    int temp = vec[s];
    vec[s] = vec[l];
    vec[l] = temp;

    if (s > k)
      r = s;
    if (s < k)
      l = s + 1;

    // Debug
    /*
    for (int i = 0; i < vec.size(); i++) {
      std::cout << vec[i] << " ";
    }
    std::cout << "\n";
    */
  }

  return vec[s];
}

int main(int argc, char *argv[]) {
  std::cout << "Quick-select\n";

  std::vector<int> vector = {6, 1, 24, 4, 5, 7, 2, 8, 9};
  int k = 4;

  std::cout << "Searching the " << k << "-th ordered number in:\n";
  for (int i = 0; i < vector.size(); i++) {
    std::cout << vector[i] << " ";
  }
  std::cout << "\n";

  int result = quickSelect(vector, k);

  std::cout << "The number in the " << k
            << "-th ordered position is: " << result << "\n";

  return 0;
}