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

    /*
    Recorre el camino de inicio a fin solo recogiendo
    la moneda más grande en la posicion actual (derecha o abajo)
    Si el resultado es el mismo prioriza ir hacia la derecha
    Complejidad: O(n)
    */
    int getMaxCoinCollection(bool debugPath){
      int result = 0;

      int i = 0;
      int j = 0;

      while(i < n-1 || j < n-1){
        bool goRight = true;

        if(i+1 < n && j+1 < n){
            if (field[i+1][j] > field[i][j+1]) {
                i++;
                goRight = false;
            } else {
                j++;
                goRight = true;
            }
        } else if (i+1 < n) {
            i++;
            goRight = false;
        } else {
            j++;
            goRight = true;
        }

        if (debugPath){
            std::cout << (goRight ? "Derecha" : "Abajo") << "(" << field[i][j] << "), ";
        }

        result += field[i][j];
      }
      if(debugPath) std::cout << "\n";

      return result;
    }

    void printGoldfield(){
      std::cout << n << "\n";
      for(int i = 0; i < (int)field.size(); i++){
        for(int j = 0; j < (int)field[i].size(); j++){
          std::cout << field[i][j] << " ";
        }
        std::cout << "\n";
      }
    }
};

int main(){
	std::cout << "Greedy Coin Collecting\n";

	std::string instancesPath = "./act1.6/coinCollectingInstances";

    try {
        for (const auto &entry : std::filesystem::directory_iterator(instancesPath)) {
            if (entry.is_regular_file()) {
            std::cout << "Reading file: " << entry.path() << "\n";

            std::ifstream file(entry.path());

            Minefield minefield(file);

            int result = minefield.getMaxCoinCollection(true);
        
            std::cout << "The Greedy approach result is: " << result;
            std::cout << "\n\n";
        }
    }
    } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << "\n";
    }

	return 0;
}