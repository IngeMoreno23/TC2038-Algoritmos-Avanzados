#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>

struct PartialSolution{
    int index;
    int sum;
    std::vector<int> vec;
    const std::vector<int> values;

    // Root Constructor
    PartialSolution(const std::vector<int> &values): values(values){
        index = -1;
        sum = 0;
        vec = std::vector<int>(values.size(), -1);
    }

    PartialSolution next(int zeroOrOne){
        if(zeroOrOne != 0 && zeroOrOne != 1){
            throw std::runtime_error("Unexpected value!");
        }
        
        PartialSolution next(*this); // copy construct
        
        next.index += 1;
        next.vec[next.index] = zeroOrOne;
        next.sum += next.vec[next.index] * next.values[next.index];

        return next;
    }

    std::vector<int> decode(){
        std::vector<int> result;
        for(int i = 0; i < vec.size(); i++){
            if(vec[i] == 1) result.push_back(values[i]);
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const PartialSolution& obj){
        for(int i = 0; i < obj.vec.size(); i++){
            os << obj.vec[i] << " ";
        }
        return os;
    }
};

std::vector<PartialSolution> x(std::vector<int> &values, int target){
    int n = values.size();
    std::queue<PartialSolution> queue;
    std::vector<PartialSolution> acepted{};

    queue.push(PartialSolution(values));

    while(!queue.empty())
    {
        PartialSolution current = queue.front();
        queue.pop();

        if(current.index >= (int)current.vec.size()-1) {
            continue;
        }

        for(int i = 0; i <= 1; i++){
            PartialSolution next = current.next(i);

            if (next.sum < target) queue.push(next);
            if (next.sum == target) acepted.push_back(next);
        }
    }
    
    return acepted;
}

int main(){
    std::cout << "Backtracking\n";

    std::vector<int> values{1, 2, 5, 6, 8};
    std::vector<PartialSolution> solutions = x(values, 8);

    for(int i = 0; i < solutions.size(); i++){
        std::cout << solutions[i] << "\n";
        std::vector<int> decoded = solutions[i].decode();
        std::cout << "{";
        for(int j = 0; j < decoded.size(); j++){
            std::cout << decoded[j] << " ";
        }
        std::cout << "}\n";
    }
    
    return 0;
}