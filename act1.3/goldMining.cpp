#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

class Minefield{
  int n;
  std::vector<std::vector<int>> field;

  int max(const int &a, const int &b){
    return (a >= b) ? a : b;
  }

  /*
  Genera una matriz cuyos indices son la suma acumulada
  más alta obtenida desde la esquina superior izquierda.
  Devuelve la suma final que es la suma del mejor pathing.
  Complejidad(n^2): n = lado de matriz/field
  */
  int getMaxCoinCollection(std::vector<std::vector<int>> &tab){
    tab = field;
  
    for(int i=0; i < tab.size(); i++){
      for(int j=0; j < tab.size(); j++){
        // If borders
        if (i == 0 && j == 0) continue;
        if (i == 0){
          tab[i][j] += tab[i][j-1];
          continue;
        }
        if (j == 0){
          tab[i][j] += tab[i-1][j];
          continue;
        }
        tab[i][j] += max(tab[i-1][j], tab[i][j-1]);
      }
    }
  
    return tab[n-1][n-1];
  }

  public:
    Minefield(std::ifstream &file){
      if (!file.is_open()) {
        throw std::runtime_error("Unexpected File. Couldn't load.");
      }

      int n_;
      if (!(file >> n_) || n_ <= 0) {
        throw std::runtime_error("Strange Header. Must be a positive number.");
      }
      //Skip first line
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::vector<std::vector<int>> field_;
      field_.reserve(n_);

      for(int i = 0; i < n_; i++){
        std::string line;
        if(std::getline(file, line)){
          std::istringstream iss(line);
          std::vector<int> row;
          row.reserve(n_);
          for(int j = 0; j < n_; j++){
            int square;
            if(iss >> square){
              row.push_back(square);
            } else {
              throw std::runtime_error("Incomplete Row.");
            }
          }
          field_.push_back(std::move(row));
        } else {
          throw std::runtime_error("Incomplete Matrix.");
        }
      }

      n = n_;
      field = field_;
    }

    int size(){
      return n;
    }

    int getMaxCoinCollection(bool debug){
      std::vector<std::vector<int>> tab;
      int result = getMaxCoinCollection(tab);

      if (debug) {
        for(int i=0; i < tab.size(); i++){
          for(int j=0; j < tab.size(); j++){
            std::cout << tab[i][j] << " ";
          }
          std::cout << "\n";
        }
      }
      return result;
    }

    void printGoldfield(){
      std::cout << n << "\n";
      for(int i = 0; i < field.size(); i++){
        for(int j = 0; j < field[i].size(); j++){
          std::cout << field[i][j] << " ";
        }
        std::cout << "\n";
      }
    }
};


int main() {
  std::cout << "Gold Mining\n";

  std::string instancesPath = "./act1.3/instancias goldMining";

  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(instancesPath)) {
      if (entry.is_regular_file()) {
        std::cout << "Reading file: " << entry.path() << "\n";

        std::ifstream file(entry.path());

        Minefield goldField(file);
        
        goldField.printGoldfield();

        bool debugging = true;
        if (debugging){
          std::cout << "Debugging: " << entry.path() << "\n";
        }
        
        int result = goldField.getMaxCoinCollection(debugging);
        
        std::cout << "The max coin collection is: " << result;
        std::cout << "\n\n";
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << "\n";
  }

  return 0;
}