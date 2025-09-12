#include <iostream>
#include <vector>

/*
Busca de manera iterativa que combinaciones de monedas logran sumar k.
Devuelve el total de posibilidades de formar k.
Complejidad O(n*k). N = coins, k = suma total.
*/
int coinChangeTabulation(const std::vector<int> &coins, int k) {
  int n = coins.size();

  std::vector<std::vector<int>> tab(n + 1, std::vector<int>(k + 1, 0));
  tab[0][0] = 1;

  for (int i = 1; i < n + 1; i++) {
    for (int j = 0; j < k + 1; j++) {
      tab[i][j] += tab[i - 1][j];

      int x = j - coins[i - 1];
      if (x >= 0) {
        tab[i][j] += tab[i][x];
      }
    }
  }

  // Debug
  for (int i = 0; i < n + 1; i++) {
    for (int j = 0; j < k + 1; j++) {
      std::cout << tab[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  return tab[n][k];
}

int main() {
  std::cout << "Coin Change:\n";

  std::vector<int> coins = {1, 2, 5, 10};
  int k = 7;

  int numCombinations = coinChangeTabulation(coins, k);

  std::cout << "Formas de sumar '" << k << "' con {";

  for (int i = 0; i < coins.size() - 1; i++) {
    std::cout << coins[i] << ", ";
  }
  std::cout << coins[coins.size() - 1] << "} = " << numCombinations;

  return 0;
}