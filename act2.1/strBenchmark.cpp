#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include "kmp.hpp"
#include "z-array.hpp"

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

std::vector<std::pair<std::string, std::string>> generateRandomTestCases(int count) {
    std::vector<std::pair<std::string, std::string>> testCases;
    testCases.reserve(count);

    int textLenght = 1000;
    int patternLenght = 5;
    unsigned seed = 123;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<char> dist('a', 'b');

    for (int i = 0; i < count; i++) {
        std::string text;
        text.reserve(textLenght);
        for (int j = 0; j < textLenght; j++) {
            text.push_back(dist(gen));
        }

        std::string pattern;
        pattern.reserve(patternLenght);
        for (int j = 0; j < patternLenght; j++) {
            pattern.push_back(dist(gen));
        }

        testCases.emplace_back(text, pattern);
    }

    return testCases;
}

bool resultsEqual(const std::vector<std::pair<int, int>>& a, const std::vector<std::pair<int, int>>& b) {
    if(a.size() != b.size()) return false;
    for(int i = 0; i < (int)a.size(); i++) {
        if(a[i].first != b[i].first || a[i].second != b[i].second) {
            return false;
        }
    }
    return true;
}

int main(){
  std::cout << "String Match BenchMark\n";

  std::string text = "abcabcabcdabcdabcabcdabc";
  std::string pattern = "abcabcd";

  std::vector<std::pair<int, int>> kmpResults = kmp::stringMatch(text, pattern);
  std::vector<std::pair<int, int>> z_arrayResults = z_array::stringMatch(text, pattern);

  std::cout << "================ Main Test ================\n";
  std::cout << "--------------- KMP Results ---------------\n";
  printFormatedStringMatch(text, pattern, kmpResults);
  std::cout << "-------------------------------------------\n";

  std::cout << "------------- Z-Array Results -------------\n";
  printFormatedStringMatch(text, pattern, z_arrayResults);
  std::cout << "-------------------------------------------\n";

  std::cout << "Results Match: " << (resultsEqual(kmpResults, z_arrayResults) ? "Yes" : "No") << "\n";
  std::cout << "===========================================\n\n";


  const int ITERATIONS = 10;
  std::streambuf* orig_cout = std::cout.rdbuf();

  std::cout << "================ Benchmark ================\n";
  std::cout << "Running " << ITERATIONS << " iterations each...\n\n";
  std::cout.rdbuf(nullptr); // Used to suppress debug in console

  // Generate Test Cases
  std::vector<std::pair<std::string, std::string>> testCases = generateRandomTestCases(ITERATIONS);

  // KMP Benchmark
  std::vector<std::vector<std::pair<int, int>>> kmp_all_results;
  kmp_all_results.reserve(ITERATIONS);

  auto start_kmp = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < ITERATIONS; i++){
    kmp_all_results.push_back(kmp::stringMatch(testCases[i].first, testCases[i].second));
  }
  auto end_kmp = std::chrono::high_resolution_clock::now();
  auto duration_kmp = std::chrono::duration_cast<std::chrono::microseconds>(end_kmp - start_kmp);

  // Z-Array Benchmark
  std::vector<std::vector<std::pair<int, int>>> z_all_results;
  z_all_results.reserve(ITERATIONS);

  auto start_z = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < ITERATIONS; i++){
    z_all_results.push_back(z_array::stringMatch(testCases[i].first, testCases[i].second));
  }
  auto end_z = std::chrono::high_resolution_clock::now();
  auto duration_z = std::chrono::duration_cast<std::chrono::microseconds>(end_z - start_z);

  std::cout.rdbuf(orig_cout); // Restore the console

  // Verify correctness across all test cases
  int correctResults = 0;
  for(int i = 0; i < ITERATIONS; i++) {
    if(resultsEqual(kmp_all_results[i], z_all_results[i])) {
      correctResults++;
    }
  }

  std::cout << "-------------- KMP Benchmark --------------\n";
  std::cout << "KMP Total Time: " << duration_kmp.count() << " microseconds\n";
  std::cout << "KMP Average Time: " << (double)duration_kmp.count() / ITERATIONS << " microseconds per call\n";
  std::cout << "-------------------------------------------\n";

  std::cout << "------------ Z-Array Benchmark ------------\n";
  std::cout << "Z-Array Total Time: " << duration_z.count() << " microseconds\n";
  std::cout << "Z-Array Average Time: " << (double)duration_z.count() / ITERATIONS << " microseconds per call\n";
  std::cout << "-------------------------------------------\n";
  std::cout << "===========================================\n\n";


  std::cout << "=========== Performance Summary ===========\n";
  std::cout << "Iterations: " << ITERATIONS << "\n";
  std::cout << "Correctness: " << correctResults << "/" << ITERATIONS << " tests gave the same result\n";
  
  if(duration_kmp.count() < duration_z.count()){
    double speedup = (double)duration_z.count() / duration_kmp.count();
    std::cout << "KMP is " << speedup << "x faster than Z-Array\n";
  } else {
    double speedup = (double)duration_kmp.count() / duration_z.count();
    std::cout << "Z-Array is " << speedup << "x faster than KMP\n";
  }
  std::cout << "===========================================\n\n";


  std::cout << "============ Last 3 Test Cases ============\n";
  int startIndex = std::max(0, ITERATIONS - 3);
  for(int i = startIndex; i < ITERATIONS; i++) {
    std::cout << "Test Case #" << (i + 1) << ":\n";
    std::cout << "Text: \"" << testCases[i].first << "\"\n";
    std::cout << "Pattern: \"" << testCases[i].second << "\"\n";
    
    std::cout << "--------------- KMP Results ---------------\n";
    printFormatedStringMatch(testCases[i].first, testCases[i].second, kmp_all_results[i]);
    std::cout << "-------------------------------------------\n";

    std::cout << "------------- Z-Array Results -------------\n";
    printFormatedStringMatch(testCases[i].first, testCases[i].second, z_all_results[i]);
    std::cout << "-------------------------------------------\n";

    std::cout << "Results Match: " << (resultsEqual(kmp_all_results[i], z_all_results[i]) ? "Yes" : "No") << "\n";
    if(i < ITERATIONS - 1) std::cout << "\n";
  }
  std::cout << "===========================================\n";
  return 0;
}