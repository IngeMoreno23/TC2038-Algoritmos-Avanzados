#include <iostream>
#include "kmp.hpp"

std::ostream& operator<<(std::ostream& os, const std::pair<int,int>& p) {
    return os << "{" << p.first << ", " << p.second << "}";
}

// Print match and underline matches
void printFormatedStringMatch(const std::string& text, const std::string& pattern, const std::vector<std::pair<int, int>>& matches){
  std::cout << "Finding Pattern: '";
  for(char c : pattern){
      std::cout << "\033[4m" << c << "\033[0m";
  }
  std::cout << "'\n";
  
  // print pairs
  std::cout << "Matches: ";
  if(matches.size() > 0){
    for(auto pair : matches){
      std::cout << "\n" << pair;
    }
  } else {
    std::cout << "No match found :(";
  }
  std::cout << "\n";

  // print underlined
  std::cout << "Text: '";
  for(int i = 0; i < (int)text.size(); i++){
    bool underlined = false;
    for(auto p : matches){
        if(i >= p.first && i < p.second){
            underlined = true;
            break;
        }
    }
    if(underlined)
        std::cout << "\033[4m" << text[i] << "\033[0m";
    else
        std::cout << text[i]; 
  }
  std::cout << "'\n";
}

int main(){
  std::cout << "String Match BenchMark\n";

  std::string text = "abcabcabcdabcdabcabcdabc";
  std::string pattern = "abcabcd";
  std::vector<std::pair<int, int>> kmpResults = kmp::stringMatch(text, pattern);

  printFormatedStringMatch(text, pattern, kmpResults);

  return 0;
}