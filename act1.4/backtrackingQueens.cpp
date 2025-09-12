#include <iostream>
#include <queue>
#include <vector>

struct PartialSolution{
    private:
        int currentRow = 0;
        std::vector<std::vector<bool>> board;

    public:
        PartialSolution(int queens){
            board = std::vector<std::vector<bool>>(queens, std::vector<bool>(queens, false));
        }
    
        void print(){
            for(int i = 0; i < board.size(); i++){
                for(int j = 0; j < board[i].size(); j++){
                    std::cout << (board[i][j] ? "Q" : "-") << " ";
                }
                std::cout << "\n";
            }
        }
    
        /*
        Tests if a queen in `currentRow` and column could be placed.
        If succedes, returns true and advances 1 row
        if not, stais and returns false
        */
        bool setQueenAtColumn(int column){
            if(column < 0 || column >= board.size()) throw std::runtime_error("Column Out of Bound");
    
            // Column Check - Each row in the same column
            for(int i = 0; i < currentRow; i++){
                if (board[i][column]) return false;
            }

            // Diagonal Check
            for(int i = 0; i < currentRow; i++){
                // Diagonal in range
                int columnIndex = column + currentRow - i;
                if (columnIndex < 0 || columnIndex >= board.size()) continue;
                
                if (board[i][columnIndex]) return false;
            }

            // Antidiagonal Check
            for(int i = 0; i < currentRow; i++){
                // Diagonal in range
                int columnIndex = column - currentRow + i;
                if (columnIndex < 0 || columnIndex >= board.size()) continue;

                if (board[i][columnIndex]) return false;
            }

            board[currentRow][column] = true;
            currentRow++;
            
            return true;
        }

        bool isFull(){
            return currentRow == board.size();
        }
};

/*
Itera en forma de un arbol sobre todas las posiciones de reina en el tablero.
Avanza de arriba hacia abajo y se asume 1 reina en cada fila.
Regresa todas las soluciones posibles, incluyendo copias por rotacion-reflejo.
Complejidad: O(n!)
*/
std::vector<PartialSolution> nQueens(int queens){
    if (queens < 1) throw std::runtime_error("Number of Queens must be positive!");
    std::queue<PartialSolution> queue{};
    std::vector<PartialSolution> solutions{};

    queue.push(PartialSolution(queens));

    while(!queue.empty())
        {
            PartialSolution current = queue.front();
            queue.pop();
    
            for(int i = 0; i < queens; i++){
                PartialSolution candidate = current;
                
                // Discard if does not fit
                if(!candidate.setQueenAtColumn(i)) continue;

                if(candidate.isFull()){
                    solutions.push_back(candidate);
                    continue;
                }
                
                queue.push(candidate);
            }
        }
    
    return solutions;
}

int main(){
    std::cout << "Backtracking n-Queens:\n";
    std::cout << "Number of Queens: ";
    int queens;
    
    if (!(std::cin >> queens)) throw std::runtime_error("Invalid Input!");
    if (queens < 1) throw std::runtime_error("Number of Queens must be positive!");

    std::cout << "Solving for " << queens << "-Queens:\n";
    
    std::vector<PartialSolution> solutions = nQueens(queens);

    if (solutions.size() == 0){
        std::cout << "No Solution Found :(\n";
    } else {
        std::cout << solutions.size() << " Solutions Found :D\n\n";
        for(int i = 0; i < solutions.size(); i++){
            solutions[i].print();
            std::cout << "\n";
        }
    }
    
    return 0;
}