#include "kmp.hpp"

#include <iostream>

namespace kmp {

  /*
  Crea un LPS (Longest Prefix Sufix) y cada vez que falla en
  una comparación del KMP, se regresa al indice LPS[j-1] del
  patrón para verificar evitar iterar el patrón desde el inicio
  Complejidad: O(n)
  */
  std::vector<std::pair<int, int>> stringMatch(const std::string& text, const std::string& pattern){
    int n = text.size();
    int m = pattern.size();

    if(n < m || n <= 0 || m <= 0) return {}; // No empty strings or longer pattern lenght

    std::vector<std::pair<int, int>> result{};

    // Generate LPS
    int* LPS = new int[m]();
    LPS[0] = 0;
    
    for(int i = 1; i < m; i++){
      int len = LPS[i-1]; // up to match lenght
      while (len > 0 && pattern[i] != pattern[len]) {
          len = LPS[len - 1]; // fall back
      }
      if(pattern[i] == pattern[len]) LPS[i] = len+1;
    }

    // Bebug
    
    std::cout << "Debug LPS:\n";
    for(int i = 0; i < m; i++){
      std::cout << pattern[i] << " ";
    }
    std::cout << "\n";
    for(int i = 0; i < m; i++){
      std::cout << LPS[i] << " ";
    }
    std::cout << "\n";
    
    // ENDDEBUG

    // KMP

    int i = 0;
    int j = 0;
    while(i < n){
      if(text[i] == pattern[j]){
        i++; j++;
      } else {
        if(j > 0){
          j = LPS[j-1];
        } else {
          i++;
        }
      }

      if(j == m){
        result.emplace_back(i-m, i);
        j = LPS[j-1];
      }
    }

    delete[] LPS;
    
    return result;
  }
  
  std::pair<int, int> stringMatchFirst(const std::string& text, const std::string& pattern) {
      auto all = stringMatch(text, pattern);
      if (!all.empty()) return all.front();
      return {-1, -1};
  }

}