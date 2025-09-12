#include <fstream>
#include <iostream>
#include <vector>

struct StringMatch {
  bool hasMatch;
  int begin;
  int end;
  std::string stringMatch;

  StringMatch() {
    hasMatch = false;
    begin = -1;
    end = -1;
    stringMatch = "";
  }

  StringMatch(bool hasMatch, int begin, int end, std::string stringMatch) {
    this->hasMatch = hasMatch;
    this->begin = begin;
    this->end = end;
    this->stringMatch = stringMatch;
  }

  operator bool() const { return hasMatch; }
};

StringMatch stringMatcher(std::string text, std::string pattern) {
  int n = text.length();
  int m = pattern.length();
  int search_window = n - m;

  bool found = false;

  for (int i = 0; i <= search_window && !found; i++) {
    int j = 0;
    while (j < m && text[i + j] == pattern[j]) {
      // Debug ---
      std::cout << text[i + j] << pattern[j] << "\n";
      std::cout << j << " " << m-1 << "\n";
      // ---------
      j++;
    }
    if (j == m) {
      return StringMatch(true, i, i + m - 1, text.substr(i, m));
    }
  }

  return StringMatch();
}

std::string getTextInFile(std::string fileDir) {
  std::ifstream file(fileDir);
  if (!file.is_open())
    throw std::runtime_error("Text file '" + fileDir + "' not found");

  std::string text((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  return text;
}

std::vector<std::string> getPatternsInFile(std::string fileDir) {
  std::ifstream file(fileDir);
  if (!file.is_open())
    throw std::runtime_error("Text file '" + fileDir + "' not found");

  std::vector<std::string> patterns;
  std::string pattern;
  while (std::getline(file, pattern)) {
    pattern.erase(pattern.find_last_not_of(" \t\r\n") +
                  1);     // remove trailing spaces
    if (!pattern.empty()) // skip empty
      patterns.push_back(pattern);
  }
  return patterns;
}

int main(int argc, char *argv[]) {
  std::cout << "String-matching\n\n";

  std::string text =
      getTextInFile("./act1.1/stringMatching/string-matching-Texto.txt");
  std::vector<std::string> patterns =
      getPatternsInFile("./act1.1/stringMatching/string-matching-Patrones.txt");

  std::cout << "Searching in:\n";
  std::cout << text << "\n\n";
  std::cout << "Patterns:\n";
  for (int i = 0; i < patterns.size(); i++) {
    std::cout << patterns[i] << "\n";
  }
  std::cout << "\n";

  for (int i = 0; i < patterns.size(); i++) {
    std::cout << "Searching: " << patterns[i] << "\n";
    StringMatch stringMatch = stringMatcher(text, patterns[i]);
    if (stringMatch) {
      std::cout << "String match found at indexes[" << stringMatch.begin << ", "
                << stringMatch.end
                << "\nString Found: " << stringMatch.stringMatch << "\n";
    } else {
      std::cout << "String match not found\n";
    }
    std::cout << "\n";
  }

  return 0;
}