#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>

// With adjacency? list
class Graph{
private:
	int nextID = 0;
	std::unordered_map<std::string, int> idmapper; // string -> id
	std::unordered_map<int, std::string> inversemapper; // id -> string
	std::unordered_map<int, std::vector<std::pair<int, int>>> graph; // id -> vector pairs {id: weight}
	
	// maps a string to an id
	void tryAddToMap(const std::string& s){
		if(idmapper.contains(s)) return;
		idmapper[s] = nextID;
		inversemapper[nextID] = s;
		graph[nextID] = {};
		nextID++;
	}

	class Comparator {
	public:
		bool operator()(const std::pair<int,int>& a, const std::pair<int,int>& b) const {
			return a.second > b.second;
		}
	};
	
public:
	Graph() {}

	void addEdge(const std::string& s1, const std::string& s2, int weight){
		tryAddToMap(s1); tryAddToMap(s2);
		int id1 = idmapper[s1];
        int id2 = idmapper[s2];

		auto& edges = graph[id1];
		bool found = false;

		for(auto& [neighbor, w] : edges){
			if(neighbor == id2){ // exists inside, replace
				w = weight;
				found = true;
				break;
			}
		}

		if(!found) edges.push_back({id2, weight});
	}

	int dikjstra(const std::string& s1, const std::string& s2, std::vector<std::string>& path){
		if(!idmapper.contains(s1) || !idmapper.contains(s2)) return -1;
		int n = graph.size();

		int originID = idmapper[s1];
		int destinationID = idmapper[s2];

		int* distances = new int[n];
		for(int i = 0; i < n; i++) distances[i] = INT_MAX;
		distances[originID] = 0;
		int* processed = new int[n]();
		int* predecessor = new int[n];
		for(int i = 0; i < n; i++) predecessor[i] = -1;

		std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, Comparator> queue; // id node -> distances[id];

		queue.push({originID, distances[originID]});

		while(!queue.empty()){
			auto& pair = queue.top();
			queue.pop();
			if(pair.second != distances[pair.first]) continue;
			if(processed[pair.first] == 1) continue;
			processed[pair.first] = 1;
			for(auto& [neighbor, w] : graph[pair.first]){
				int newDistance = distances[pair.first] + w;
				if(newDistance < distances[neighbor]){
					distances[neighbor] = newDistance;
					predecessor[neighbor] = pair.first;
					if(processed[neighbor] == 0) queue.push({neighbor, distances[neighbor]});
				}
			}
		}

		int result = 0;
		path.clear();
		// traceback
		if(processed[destinationID] != 0){
			result = distances[destinationID];
			int current = destinationID;
			while(current != -1){
				path.insert(path.begin(), inversemapper[current]);
				current = predecessor[current];
			}
		}

		for(int i = 0; i < n; i++){
			std::cout << distances[i] << " " << processed[i] << " " << predecessor[i] << "\n";
		}

		delete[] distances;
		delete[] processed;
		delete[] predecessor;

		return result;
	}

	int nodeWeight(const std::string& s1, const std::string& s2){
		if(!idmapper.contains(s1) || !idmapper.contains(s2)) return -1;
    
		int id1 = idmapper[s1];
		int id2 = idmapper[s2];

		for(auto& [neighbor, w] : graph[id1]){
			if(neighbor == id2) return w;
		}

		return -1;
	}

	void print(){
		for(auto& [s, id] : idmapper){
			std::cout << s << " : [ ";
			auto it = graph.find(id);
			if (it != graph.end()){ // safety skip if any error;
				auto& edges = it->second;
				for(size_t i = 0; i < edges.size(); i++){
					auto& neighbor = edges[i].first;
					auto& w = edges[i].second;
					std::cout << "{" << inversemapper[neighbor] << ", " << w << "}";
					if(i < edges.size() -1) std::cout << ", ";
				}
			}
			std::cout << " ]\n";
		}
	}
};
	

int main(){
	std::cout << "Yendo a la Casa de Damian\n";

	std::string filePath = "actClase14/city_2020.txt";

	Graph graph = Graph();

	std::ifstream file(filePath);
	if(!file.is_open()){
		std::cerr << "Could not open file: " << filePath << "\n";
		return 1;
	}

	// parsing "(node) (neighbor) weight"
	std::string line;
	while(std::getline(file, line)){
		std::string s1, s2, weightStr;
		int phase = 0;
		bool inParentesis = false;

		for(char c : line){
			if (phase > 1) {
				if(!isspace(c)) weightStr += c;
			} else if(c == '('){
				if(phase == 0) s1 += c; else if(phase == 1) s2 += c;
				inParentesis = true;
			} else if(c == ')'){
				if(phase == 0) s1 += c; else if(phase == 1) s2 += c;
				phase++;
				inParentesis = false;
			} else if(inParentesis){
				if(phase == 0) s1 += c; else if(phase == 1) s2 += c;
			}
		}
		int weight = std::stoi(weightStr);

		graph.addEdge(s1, s2, weight);
	}

	// graph.print();
	std::vector<std::string> path{};

	std::cout << graph.dikjstra("(2, 3)", "(7, 4)", path) << "\n";
	for(size_t i = 0; i < path.size(); i++){
		std::cout << path[i] << " ";
	}
	std::cout << "\n";

	return 0;
}