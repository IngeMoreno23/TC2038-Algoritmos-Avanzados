#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <cassert>
#include <climits>

class Graph{
private:
	int nextID = 0;
	// bidirectional map
	std::unordered_map<std::string, int> left; // string -> id
	std::unordered_map<int, std::string> right; // id -> string

	std::unordered_map<int, std::unordered_map<int ,int>> graph; // id -> map pairs {id: weight}

	void tryAddToMap(const std::string& s){
		if(left.contains(s)) return; // exists
		left[s] = nextID;
		right[nextID] = s;
		graph[nextID] = {};
		nextID++;
	}

public:
	Graph() {}

	// Creates the nodes if doesnt exist
	// If the edge does not exist, creates one, if does, replaces weight
	void addEdge(const std::string& s1, const std::string& s2, int weight, bool directed){
		tryAddToMap(s1); tryAddToMap(s2);
		int id1 = left.at(s1);
        int id2 = left.at(s2);

		auto& edgeMap = graph.at(id1);
		edgeMap[id2] = weight;

		if(!directed){
			auto& edgeMap2 = graph.at(id2);
			edgeMap2[id1] = weight;
		}
	}

	int getWeight(const std::string& s1, const std::string& s2){
		if(!left.contains(s1) || !left.contains(s2)) return -1;

		int id1 = left[s1];
		int id2 = left[s2];

		return graph.at(id1).at(id2);
	}

	/*
	brief:
		Calcula la ruta más corta entre 2 nodos con el algoritmo de dijkstra
		Devuelve la distancia y la ruta de nodos tomada
		Complejidad: O((v + e)*log(v)) : cada nodo y arista que recorre, y se toma el más corto log(v) con pririty_queue
			v : vertex - nodos
			e : edges - aristas
	params:
		const std::string& s1 : nodo inicial - origen
		const std::string& s2 : nodo final - destino
		std::vector<std::string>& path : orden del recorrido de los nodos
	return:
		int : -1 si no existe una ruta, de lo contrario la suma mínima de los pesos de s1 a s2.
	*/
	int dijkstra(const std::string& s1, const std::string& s2, std::vector<std::string>& path){
		if(!left.contains(s1) || !left.contains(s2)) return -1;
		assert(("maps dont have the same size!!!", left.size() == right.size() && right.size() == graph.size()));

		struct Node{
			int id;
			int dist;
		    bool operator<(const Node& other) const {
				return dist > other.dist;
			}
		};

		int n = graph.size();

		int originID = left[s1];
		int destinationID = left[s2];

		int* distances = new int[n];
		for(int i = 0; i < n; i++) distances[i] = INT_MAX;
		distances[originID] = 0;
		int* processed = new int[n]();
		int* predecessor = new int[n];
		for(int i = 0; i < n; i++) predecessor[i] = -1;

		std::priority_queue<Node> queue{};
		queue.push({originID, distances[originID]});

		while(!queue.empty()){
			Node current = queue.top();
			queue.pop();
			processed[current.id] = 1;
			for(auto& [id2, w] : graph.at(current.id)){
				int newDistance = distances[current.id] + w;
				if(newDistance < distances[id2]){
					distances[id2] = newDistance;
					predecessor[id2] = current.id;
					if(processed[id2] == 0) queue.push({id2, distances[id2]});
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
				path.insert(path.begin(), right[current]);
				current = predecessor[current];
			}
		}

		// for(auto& [id1, map] : graph){
		// 	std::cout << id1 << " " << distances[id1] << " " << processed[id1] << " " << predecessor[id1] << "\n";
		// }

		delete[] distances;
		delete[] processed;
		delete[] predecessor;

		return result;
	}

	void print(){
		for(auto& [id1, edgeMap] : graph){
			std::cout << right.at(id1) << " : [ ";
			for(auto it = edgeMap.begin(); it != edgeMap.end(); it++){
				auto& id2 = it->first;
				auto& w = it->second;
				std::cout << "{" << right.at(id2) << ", " << w << "}";
				if(std::next(it) != edgeMap.end()) std::cout << ", ";
			}
			std::cout << " ]\n";
		}
	}
};

struct TestCases{
	std::string name;
	std::string origin;
	std::string destination;
	int speed; // m/min
};

struct TiempoLlegada{
	std::string name;
	float time;
	bool operator<(const TiempoLlegada& other) const {
		return time > other.time;
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

		graph.addEdge(s1, s2, weight, false);
	}

	//graph.print();

	std::vector<TestCases> tests = {
		{"Carlos", "(7, 4)", "(2, 3)", 30},
		{"Ana", "(0, 19)", "(2, 3)", 40},
		{"Marcela", "(8, 12)", "(2, 3)", 25},
		{"Katia", "(5, 17)", "(2, 3)", 32},
		{"Marcos", "(17, 15)", "(2, 3)", 20}
	};

	std::priority_queue<TiempoLlegada> queue{};

	// Distancias
	for(size_t i = 0; i < tests.size(); i++){
		std::vector<std::string> path{};
		int distance = graph.dijkstra(tests[i].origin, tests[i].destination, path);
		if(distance < 0){
			std::cout << "Path not found for frined: " << tests[i].name << "\n";
			continue;
		}
		float time = distance/(float)tests[i].speed;
		std::cout << tests[i].name << ": " << distance << " metros en total: " << tests[i].speed << " m/min: " << time << " minutos en total\n";
		std::cout << "Longitud de la ruta (nodos/cuadras recorridos): " << path.size() << "\n";
		std::string prev = "";
		for(size_t i = 0; i < path.size(); i++){
			std::cout << path[i];
			if(!prev.empty()) std::cout << " : " << graph.getWeight(prev, path[i]) << "m";
			std::cout << "\n";
			prev = path[i];
		}
		std::cout << "\n";
		queue.push({tests[i].name, time});
	}

	// Secuencia
	TiempoLlegada current;

	std::cout << "Orden de llegadas:\n";
	while(!queue.empty()){
		current = queue.top();
		queue.pop();

		std::cout << current.name << " : " << current.time << " mins\n";

	}
	int horas = 17;
	int minutes = 0;

	horas += (int)current.time / 60;
	minutes += (int)current.time % 60;
	if (minutes >= 60) {
		horas += minutes / 60;
		minutes = minutes % 60;
	}
	horas = horas % 24;

	std::string period = (horas >= 12) ? "PM" : "AM";
	int displayHour = horas % 12;
	if (displayHour == 0) displayHour = 12;

	std::cout << "Todos listos a las: " << (horas < 10 ? "0" : "") << displayHour << ":" << (minutes < 10 ? "0" : "") << minutes << period << "\n";

	return 0;
}