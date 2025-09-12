#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <queue>

enum MoveType { RIGHT, DOWN };

struct Move{
  MoveType type;
  int value;
  
  Move(MoveType t, int v) : type(t), value(v){}

  friend std::ostream& operator<<(std::ostream& os, const Move& move) {
      switch (move.type) {
          case RIGHT: os << "derecha (" << move.value << ")"; break;
          case DOWN:  os << "abajo ("   << move.value << ")"; break;
      }
      return os;
  }
};

struct PartialSolution {
  const std::vector<std::vector<int>>* field;
  int currentSum;
  int i;
  int j;

  int maxCoinValue;
  std::vector<Move> moves{};

  PartialSolution(const std::vector<std::vector<int>>& f) : field(&f) {
    i = 0;
    j = 0;
    currentSum = (*field)[i][j];
    maxCoinValue = 0;
    for(int i_ = 0; i_ < field->size(); i_++){
      for(int j_ = 0; j_ < (*field)[i_].size(); j_++){
        maxCoinValue = maxCoinValue > (*field)[i_][j_] ? maxCoinValue : (*field)[i_][j_];
      }
    }
  }

  int upperBound() const {
    int stepsRemaining = (field->size() - 1 - i) + (field->size() - 1 - j);
    return currentSum + stepsRemaining * maxCoinValue;
  }

  bool tryGoRight(){
    // Cant go deeper right
    if (j + 1 >= (int)field->size()) return false;
    j++;
    currentSum += (*field)[i][j];
    moves.push_back(Move(MoveType::RIGHT, (*field)[i][j]));
    return true;
  }

  bool tryGoDown(){
    // Cant go deeper down
    if (i + 1 >= (int)field->size()) return false;
    i++;
    currentSum += (*field)[i][j];
    moves.push_back(Move(MoveType::DOWN, (*field)[i][j]));
    return true;
  }
  
};

struct Comparador {
    bool operator()(const PartialSolution& a, const PartialSolution& b) const {
        return a.upperBound() < b.upperBound(); 
    }
};

class Minefield{
  int n;
  std::vector<std::vector<int>> field;
  int maxCoinValue;

  int max(const int &a, const int &b){
    return (a >= b) ? a : b;
  }

  /*
  Genera un arbol de desiciones y los poda si su upperbound
  es menor que el mejor ya encontrado.
  Complejidad O(2^2n) o O(4^n):
  n = un lado del campo, 2n es la profundidad del campo
  */
  int getMaxCoinCollection(std::vector<Move> &moves){
    std::priority_queue<PartialSolution, std::vector<PartialSolution>, Comparador> queue{};

    queue.push(PartialSolution(field));
    PartialSolution bestSoFar = queue.top();

    while(!queue.empty())
    {
      PartialSolution current = queue.top();
      queue.pop();

      // Poda = prune
      if (current.upperBound() <= bestSoFar.currentSum) {
          continue;
      }

      // Update bestSoFar
      if(current.i == n-1 && current.j == n-1){
          if(current.currentSum > bestSoFar.currentSum){
              bestSoFar = current;
          }
          continue;
      }

      PartialSolution nextToRight = current;
      if(nextToRight.tryGoRight()) queue.push(nextToRight);
      
      PartialSolution nextToDown = current;
      if(nextToDown.tryGoDown()) queue.push(nextToDown);
    }

    moves = bestSoFar.moves;
    return bestSoFar.currentSum;
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

      int maxCoinValue_ = 0;
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
              maxCoinValue_ = max(maxCoinValue_, square);
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
      maxCoinValue = maxCoinValue_;
    }

    int size(){
      return n;
    }

    int getMaxCoinCollection(bool showMoves){
      std::vector<Move> moves{};

      int result = getMaxCoinCollection(moves);

      if (showMoves) {
        std::cout << "Upperbound calculated with coin: " << maxCoinValue << "\n";
        if(!moves.empty()){
          std::cout << moves[0] << "";
          for(int i=1; i < moves.size(); i++){
            std::cout << ", " << moves[i];
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
  std::cout << "Branch and Bound - Gold Mining\n";

  std::string instancesPath = "./act1.5/instances";

  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(instancesPath)) {
      if (entry.is_regular_file()) {
        std::cout << "Reading file: " << entry.path() << "\n";

        std::ifstream file(entry.path());

        Minefield goldField(file);

        goldField.printGoldfield();

        bool debugging = true; // Show moves
        if (debugging){
          std::cout << "Moves for: " << entry.path() << "\n";
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