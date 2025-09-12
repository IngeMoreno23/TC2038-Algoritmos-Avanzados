#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <queue>

struct KnapsackItem{
	int value = 0;
	int weight = 0;

    double ratio() const {
        return (weight == 0) ? 0.0 : static_cast<double>(value) / weight;
    }

    KnapsackItem(int value, int weight) : value(value), weight(weight) {}

    KnapsackItem operator+(const KnapsackItem& other) const {
        return KnapsackItem(value + other.value, weight + other.weight);
    }

    KnapsackItem& operator+=(const KnapsackItem& other) {
        value += other.value;
        weight += other.weight;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const KnapsackItem& item) {
        os << "(value: " << item.value << ", weight: " << item.weight << ")";
        return os;
    }
};

struct Comparador {
    bool operator()(const KnapsackItem& a, const KnapsackItem& b) const {
        return a.ratio() < b.ratio(); 
    }
};

class Knapsack{
    int _itemCount;
    int _maxWeight;

    std::vector<int> _values;
    std::vector<int> _weights;

	public:
	    Knapsack(std::ifstream& file){
            if (!file.is_open()) {
                throw std::runtime_error("Unexpected File. Couldn't load.");
            }

            int itemCount_;
            int maxWeight_;

            if (!(file >> itemCount_ >> maxWeight_) || itemCount_ <= 0) {
                throw std::runtime_error("Strange Header. Must be a positive number.");
            }

            //Skip first line
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::vector<int> values;
            values.reserve(itemCount_);
            std::vector<int> weights;
            weights.reserve(itemCount_);

            for(int i = 0; i < itemCount_; i++){
                std::string line;
                if(std::getline(file, line)){
                    std::istringstream iss(line);
                    int v;
                    int w;
                    if (!(iss >> v >> w)){
                        throw std::runtime_error("Incomplete Row. | v" + std::to_string(i) + " w" + std::to_string(i) + ".");
                    }
                    values.push_back(v);
                    weights.push_back(w);
                } else {
                  throw std::runtime_error("Incomplete File.");
                }
            }

            if  ((int)values.size() != itemCount_ || (int)weights.size() != itemCount_) throw std::runtime_error("Error reading items.");

            _itemCount = itemCount_;
            _maxWeight = maxWeight_;

            _values = values;
            _weights = weights;
        }

        void PrintValues(){
            std::cout << "Item Count: " << _itemCount << "\n";
            std::cout << "Max Weight (Capacity): " << _maxWeight << "\n";
            std::cout << "Values Waights\n";
            for(int i = 0; i < _itemCount; i++){
                std::cout << _values[i] << " " << _weights[i] << "\n";
            }
        }

        /*
        Selecciona los items con mejor ratio solo si no exceden del peso máximo.
        Devuelve la suma de valor y peso de los items con mejor ratio.
        Complejidad: O(n)
        */
        KnapsackItem GreedyResult(bool debugOptimal){
            KnapsackItem accum(0, 0);
            std::priority_queue<KnapsackItem, std::vector<KnapsackItem>, Comparador> queue;

            for(int i = 0; i < _itemCount; i++){
                queue.emplace(_values[i], _weights[i]);
            }
            
            while(!queue.empty()){
                KnapsackItem item = queue.top();
                queue.pop();

                if(_maxWeight < accum.weight + item.weight) continue;
                accum += item;
            }

            return accum;
        }
};

int main(){
	std::cout << "Greedy Knapsack\n";

    bool debugOptimal = true;

	std::string instancesPath = "./act1.6/knapsackInstances";

    try {
        for (const auto &entry : std::filesystem::directory_iterator(instancesPath)) {
            if (entry.is_regular_file()) {
            std::cout << "Reading file: " << entry.path() << "\n";

            std::ifstream file(entry.path());

            Knapsack knapsack(file);
            knapsack.PrintValues();
            KnapsackItem result = knapsack.GreedyResult(debugOptimal);
        
        
            std::cout << "The Greedy approach result is: " << result;
            std::cout << "\n\n";
        }
    }
    } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << "\n";
    }

	return 0;
}