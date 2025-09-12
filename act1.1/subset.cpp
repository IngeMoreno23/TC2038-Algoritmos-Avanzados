#include <iostream>
#include <vector>

/*
Genera un contador binario del tamaño length.
Reutiliza el mismo vector para llenar primero ceros y luego unos.
Complejidad: O(2^n).
*/
void generateBinary(int length, std::vector<int> &current) {
  if (current.size() == length) {
    for (int bit : current) {
      std::cout << bit;
    }
    std::cout << "\n";
    return;
  }

  current.push_back(0);
  generateBinary(length, current);
  current.pop_back();

  current.push_back(1);
  generateBinary(length, current);
  current.pop_back();
}

int main(int argc, char *argv[]) {
  std::cout << "Subsets\n";
  std::cout << "Write Max Number of Bits: ";
    
  int n;
  std::cin >> n;

  for (int i = 1; i <= n; ++i) {
    std::vector<int> current;
    generateBinary(i, current);
  }

  return 0;
}