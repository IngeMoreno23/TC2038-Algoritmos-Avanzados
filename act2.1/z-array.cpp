#include "kmp.hpp"

#include <iostream>

namespace z_array {

  /*
  Espera strings que no contengan '$' dentro de ellos.
  Se construye una ventana y calcula para cada z_array[i]
  el tamaño del prefijo que coincide con el patron.
  Cada z_array[i] == pattern.size() es una coincidencia
  Complejidad: O(n)
  */
  std::vector<std::pair<int, int>> stringMatch(const std::string& text, const std::string& pattern){
    int n = text.size();
    int m = pattern.size();

    if(n < m || n <= 0 || m <= 0) return {}; // No empty strings or longer pattern lenght

    std::vector<std::pair<int, int>> result{};

    // Generate Z-Array
    std::string z_string = pattern + "$" + text;
    int x = z_string.size();
    int* z_array = new int[x]();
    z_array[0] = -1;

    // Z-algorithm

    int L = 0;
    int R = 0;
    for(int i = 0; i < x; i++){
        if(i <= R){
            int k = i - L;
            if(z_array[k] < R - i +1){
                z_array[i] = z_array[k];
            } else {
                L = i;
                while(R < x && z_string[R-L] == z_string[R]){
                    R++;
                }
                z_array[i] = R - L;
                R--;
            }
        } else {
            L = i;
            R = i;
            while(R < x && z_string[R-L] == z_string[R]){
                    R++;
                }
                z_array[i] = R - L;
                R--;
        }
        if(z_array[i] == m){
            result.emplace_back(i - m - 1, i - 1);
        }
    }

    // Bebug
    
    std::cout << "Debug Z-Array:\n";
    for(int i = 0; i < x; i++){
      std::cout << z_string[i] << " ";
    }
    std::cout << "\n";
    for(int i = 0; i < x; i++){
      std::cout << z_array[i] << " ";
    }
    std::cout << "\n";
    
    // ENDDEBUG

    delete[] z_array;
    
    return result;
  }
  
  std::pair<int, int> stringMatchFirst(const std::string& text, const std::string& pattern) {
      auto all = stringMatch(text, pattern);
      if (!all.empty()) return all.front();
      return {-1, -1};
  }

}