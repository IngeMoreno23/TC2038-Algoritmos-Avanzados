#include <iostream>
#include <vector>

int powHelper(int base, int exponent, std::vector<int> &memo) {
  if (exponent < 0)
    throw std::logic_error("Negative exponent not implemented");
  if (exponent == 0)
    return 1;
  if (exponent == 1)
    return base;

  int half = (int)exponent / 2;
  int residue = exponent % 2;

  int left = memo[half] != 0 ? memo[half] : powHelper(base, half, memo);
  if (memo[half] == 0)
    memo[half] = left;
  int right = memo[half + residue] != 0 ? memo[half + residue]
                                        : powHelper(base, half + residue, memo);
  if (memo[half + residue] == 0)
    memo[half + residue] = right;

  return left * right;
}

/*
Utiliza memoization en un vector para evitar calcular exponentes precalculados.
Divide el exponente a la mitad y multiplica 2 bases con ese nuevo exponente
reduciendo las iteraciones a la mitad.
Complejidad: O(log(n))
*/
int pow(int base, int exponent) {
  if (exponent < 0)
    throw std::logic_error("Negative exponent not implemented");

  std::vector<int> memo = std::vector<int>(exponent, 0); // Stores index as a^i
  int result = powHelper(base, exponent, memo);

  std::cout << "Memoization vector: (index = exponent)\n";
  for (int i = 0; i < memo.size(); i++) {
    std::cout << memo[i] << " ";
  }
  std::cout << "\n\n";

  return result;
}

int main(int argc, char *argv[]) {
  std::cout << "Decrease And Conquer\n\n";

  int base = 2;
  int exponent = 29;

  int result = pow(base, exponent);

  std::cout << base << "^" << exponent << " = " << result << "\n";

  return 0;
}