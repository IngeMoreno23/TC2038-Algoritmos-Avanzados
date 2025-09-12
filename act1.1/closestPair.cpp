#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

struct Point {
  double x;
  double y;

  Point() {
    x = 0;
    y = 0;
  }

  Point(double x, double y) {
    this->x = x;
    this->y = y;
  }

  friend std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
  }
};

struct PairData {
  Point a;
  Point b;

  PairData() = default;

  PairData(Point a, Point b) {
    this->a = a;
    this->b = b;
  }

  double squaredDist() {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return dx * dx + dy * dy;
  }
  double dist() { return std::sqrt(squaredDist()); }

  static double squaredDist(const Point &p1, const Point &p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return dx * dx + dy * dy;
  }

  static double dist(const Point &p1, const Point &p2) {
    return std::sqrt(squaredDist(p1, p2));
  }
};

/*
Regresa el par en un struct en donde se puede calcular la distancia.
Si el vector tiene 1 o menos elementos, genera un error.
Complejidad: O(n^2).
*/
PairData getClosestPair(std::vector<Point> vecPoints) {
  // Edge cases: vecPoints length <= 1 fails to get a pair.
  int n = vecPoints.size();

  if (n <= 1)
    throw std::runtime_error("Unable to get Pair in getClosestPair. Less than "
                             "2 points in the vector were given.");

  PairData closestPair = PairData(vecPoints[0], vecPoints[1]); // Set example

  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {

      double squaredDistance =
          PairData::squaredDist(vecPoints[i], vecPoints[j]);

      // Debug ---
      // std::cout << "i: " << i << " : " << vecPoints[i] << "\n";
      // std::cout << "j: " << j << " : " << vecPoints[j] << "\n";
      /// std::cout << "squaredDistance: " << squaredDistance << "\n";
      // ---------

      if (squaredDistance < closestPair.squaredDist()) {
        closestPair.a = vecPoints[i];
        closestPair.b = vecPoints[j];
        // Debug ---
        // std::cout << "!!! Current closestPair: " << closestPair.squaredDist()
        // <<
        // "\n";
        // ---------
      }
    }
  }

  return closestPair;
}

std::vector<Point> readPointsFromFile(std::ifstream &file) {
  if (!file.is_open()) {
    throw std::runtime_error("Unexpected File. Couldn't load.");
  }

  int numPoints;

  if (!(file >> numPoints) || numPoints <= 0) {
    throw std::runtime_error("Strange Header. Must be a positive number.");
  }

  std::vector<Point> vecPoints;

  int i = 0;
  double x, y;
  while (i < numPoints && (file >> x >> y)) {
    vecPoints.emplace_back(x, y); // Performance ;)
    i++;
  }

  if (i < numPoints)
    std::cerr
        << "Current number of points is less than in the file's header.\n";

  return vecPoints;
}

int main(int argc, char *argv[]) {
  std::cout << "Closest-Pair\n";

  std::string instancesPath = "./act1.1/closestPair";

  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(instancesPath)) {
      if (entry.is_regular_file()) {
        std::cout << "Reading file: " << entry.path() << "\n";

        std::ifstream file(entry.path());

        // Read File into a vector
        std::vector<Point> vec = readPointsFromFile(file);

        // Get closest pair
        PairData closestPair = getClosestPair(vec);

        std::cout << "Closest-pair is between: " << closestPair.a << " - "
                  << closestPair.b
                  << ".\nWith lenght of: " << closestPair.dist() << "\n\n";
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << "\n";
  }

  return 0;
}
