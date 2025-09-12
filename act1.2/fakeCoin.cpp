#include <iostream>
#include <vector>

enum Weight { unknown = 0, light = 1, heavy = 2 };

struct FakeCoinSpecs {
  int index;
  Weight type;

  FakeCoinSpecs(int index, Weight type) : index(index), type(type) {}
};

FakeCoinSpecs getFakeCoinRecursive(std::vector<int> &reals,
                                   std::vector<int> &unsafeCoins,
                                   std::vector<int> &safeCoins) {
  int n = unsafeCoins.size();

  // Base cases
  // n <= 0 xd
  if (n <= 0)
    throw std::logic_error("Imposible pero por si acaso.");

  // n = 1
  if (n == 1) {
    // Sanity check
    if (reals[unsafeCoins[0]] == reals[safeCoins[0]]) {
      std::cerr << "No Fake-coin, all the same!";
      return FakeCoinSpecs(unsafeCoins[0], Weight::unknown);
    }
    if (reals[unsafeCoins[0]] > reals[safeCoins[0]])
      return FakeCoinSpecs(unsafeCoins[0], Weight::heavy);
    return FakeCoinSpecs(unsafeCoins[0], Weight::light);
  }

  // n = 2
  if (n == 2) {
    // Sanity check
    if (reals[unsafeCoins[0]] == reals[unsafeCoins[1]]) {
      std::cerr << "No Fake-coin, all the same!";
      return FakeCoinSpecs(unsafeCoins[0], Weight::unknown);
    }
    if (reals[unsafeCoins[0]] > reals[safeCoins[0]])
      return FakeCoinSpecs(unsafeCoins[0], Weight::heavy);
    if (reals[unsafeCoins[0]] < reals[safeCoins[0]])
      return FakeCoinSpecs(unsafeCoins[0], Weight::light);
    if (reals[unsafeCoins[1]] > reals[safeCoins[0]])
      return FakeCoinSpecs(unsafeCoins[1], Weight::heavy);
    return FakeCoinSpecs(unsafeCoins[1], Weight::light);
  }

  int a = n / 3;
  int b = a;
  int c = a;

  int r = n % 3;

  // a and b needs to be equal in size.
  if (r == 1)
    c += 1;
  else if (r == 2) {
    a += 1;
    b += 1;
  } // else no change

  // std::cout << "abc: " << a << " " << b << " " << c << " " << r << "\n";

  // Pesa
  int weightA = 0;
  int weightB = 0;
  for (int i = 0; i < a; i++) {
    weightA += reals[unsafeCoins[i]];
    weightB += reals[unsafeCoins[a + i]];
  }

  // std::cout << "Ws: " << weightA << " " << weightB << "\n";

  if (weightA == weightB) { // C is unsafe
    for (int i = 0; i < a + b; i++) {
      int temp = unsafeCoins[i];
      unsafeCoins.erase(unsafeCoins.begin());
      safeCoins.push_back(temp);
    }
  } else {
    for (int i = a + b; i < a + b + c; i++) {
      int temp = unsafeCoins[i];
      unsafeCoins.erase(unsafeCoins.begin() + a + b);
      safeCoins.push_back(temp);
    }
  }

  // Bebug
  /*
  std::cout << "unsafe: " << unsafeCoins.size() << "\n";
  for (int i = 0; i < unsafeCoins.size(); i++) {
    std::cout << reals[unsafeCoins[i]] << " ";
  }
  std::cout << "\n";
  std::cout << "safe:\n";
  for (int i = 0; i < safeCoins.size(); i++) {
    std::cout << reals[safeCoins[i]] << " ";
  }
  std::cout << "\n";
  */

  return getFakeCoinRecursive(reals, unsafeCoins, safeCoins);
}

/*
Divide las monedas en 3 sets
Compara los primeros 2 y decide cual de los 3 descartar como monedas buenas.
Compara hasta tener 3 o 2 monedas finales.
Complejidad: O(log(n))
*/
FakeCoinSpecs getFakeCoin(std::vector<int> coins) {
  if (coins.size() < 3) {
    std::cerr << "Unable to compare with less than 3 coins";
    return FakeCoinSpecs(-1, Weight::unknown);
  }
  std::vector<int> unsafeCoins(coins.size(), 0);
  std::vector<int> safeCoins;

  for (int i = 0; i < coins.size(); i++) {
    unsafeCoins[i] = i;
  }

  // Bebug
  /*
  std::cout << "unsafe: " << unsafeCoins.size() << "\n";
  for (int i = 0; i < unsafeCoins.size(); i++) {
    std::cout << coins[unsafeCoins[i]] << " ";
  }
  std::cout << "\n";
  std::cout << "safe:\n";
  for (int i = 0; i < safeCoins.size(); i++) {
    std::cout << coins[safeCoins[i]] << " ";
  }
  std::cout << "\n";
  */

  FakeCoinSpecs specs = getFakeCoinRecursive(coins, unsafeCoins, safeCoins);

  // Bebug
  /*
  std::cout << "unsafe: " << unsafeCoins.size() << "\n";
  for (int i = 0; i < unsafeCoins.size(); i++) {
    std::cout << coins[unsafeCoins[i]] << " ";
  }
  std::cout << "\n";
  std::cout << "safe:\n";
  for (int i = 0; i < safeCoins.size(); i++) {
    std::cout << coins[safeCoins[i]] << " ";
  }
  std::cout << "\n";
  */

  return specs;
}

int main(int argc, char *argv[]) {
  std::cout << "Fake-coin\n";

  std::vector<int> coins = {
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

  for (int i = 0; i < coins.size(); i++) {
    std::cout << coins[i] << " ";
  }
  std::cout << "\n\n";

  FakeCoinSpecs fakeCoinInfo = getFakeCoin(coins);

  switch (fakeCoinInfo.type) {
  case Weight::light:
    std::cout << "Found at index (" << fakeCoinInfo.index << "):\n";
    std::cout << "With value of: " << coins[fakeCoinInfo.index] << "\n";
    std::cout << "Its weight is: lighter"
              << "\n";
    break;
  case Weight::heavy:
    std::cout << "Found at index (" << fakeCoinInfo.index << "):\n";
    std::cout << "With value of: " << coins[fakeCoinInfo.index] << "\n";
    std::cout << "Its weight is: heavier"
              << "\n";
    break;
  default:
    std::cout << "Error finding the fake coin\n";
    break;
  }

  return 0;
}