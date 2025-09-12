#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

class Graph {
  int vertexCount;
  std::vector<std::vector<bool>> matrix;

public:
  Graph(int vertexCount) {
    if (vertexCount < 0)
      throw std::logic_error("Unable to set a negative vertexCount");

    this->vertexCount = vertexCount;
    matrix.assign(vertexCount, std::vector<bool>(vertexCount, false));
  }

  void addEdge(int u, int v, bool directed = true) {
    if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount) {
      throw std::runtime_error("Invalid edge: (" + std::to_string(u) + ", " +
                               std::to_string(v) + ")");
    }

    matrix[u][v] = true;
    if (!directed)
      matrix[v][u] = true;
  }

  void printGraph() {
    for (int i = 0; i < vertexCount; i++) {
      for (int j = 0; j < vertexCount; j++) {
        std::cout << matrix[i][j] << " ";
      }
      std::cout << "\n";
    }
  }

  /*
  Cada iteración sobre la matriz verifica que vertice no tiene
  aristas apuntando hacia el.
  Al encontrarlo, elimina sus aristas y lo agrega a un vector de salida.
  Si en una iteración no se encuentran vertices sin aristas,
  regresa el vector de salida.
  Complejidad: O(V^3)  
  */
  std::vector<int> topologicalSort() {
    std::vector<std::vector<bool>> aux = this->matrix;
    std::vector<bool> visited = std::vector<bool>(vertexCount, false);
    std::vector<int> exit = std::vector<int>();

    for (int i = 0; i < vertexCount; i++) {
      // if `i` doesn't reset, couldn't found vertex with no edges.

      if (visited[i]) // skip
        continue;

      bool hasEdge = false;
      for (int j = 0; j < vertexCount; j++) {
        hasEdge |= aux[j][i]; // Anyone has an edge to current
      }
      if (hasEdge) // skip
        continue;

      // Remove edges from current to others and append and reset
      for (int j = 0; j < vertexCount; j++) {
        aux[i][j] = false;
      }
      visited[i] = true;
      exit.push_back(i);

      i = -1;
    }

    return exit;
  }
};

Graph readGraphFromFile(std::ifstream &file) {
  if (!file.is_open()) {
    throw std::runtime_error("Unexpected File. Couldn't load.");
  }

  int vertexCount;

  if (!(file >> vertexCount) || vertexCount <= 0) {
    throw std::runtime_error("Strange Header. Must be a positive number.");
  }

  Graph graph = Graph(vertexCount);
  int from, to;
  while (file >> from >> to) {
    graph.addEdge(from, to);
  }

  return graph;
}

int main(int argc, char *argv[]) {
  std::cout << "Ordenamiento Topológico\n";
  std::cout << "File format:\n";
  std::cout << "X - (Header) VertexCount 0 to X-1\n";
  std::cout << "U V - Edge1\n";
  std::cout << "U V - Edge2\n";
  std::cout << "U V - Edge3\n\n";

  std::string instancesPath = "./act1.2/ordenamientoTopologico";

  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(instancesPath)) {
      if (entry.is_regular_file()) {
        std::cout << "Reading file: " << entry.path() << "\n";

        std::ifstream file(entry.path());

        // Read File into a graph
        Graph graph = readGraphFromFile(file);

        std::cout << "Graph matrix:\n";
        graph.printGraph();

        // Print topoloogicalSort
        std::cout << "Topological Sort:\n";
        std::vector<int> topologicalSort = graph.topologicalSort();
        for (int i = 0; i < topologicalSort.size(); i++) {
          std::cout << topologicalSort[i] << " ";
        }
        std::cout << "\n\n";
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << "\n";
  }

  return 0;
}