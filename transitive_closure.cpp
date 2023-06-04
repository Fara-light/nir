// +============================================+
// | Program to find transitive closure of      |
// | fuzzy relation. Must be written overnight  |
// | by Fara_light				|
// +============================================+

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <set>
#include <map>
using namespace std;

class AdjacencyMatrix {
public:
	AdjacencyMatrix(size_t graphOrder) : graphOrder(graphOrder) {
		adjacencyMatrix = std::vector<std::vector<long double>>(graphOrder, std::vector<long double>(graphOrder, 0));
	}
	std::vector<long double>& operator [](size_t rowIndex) {
		return adjacencyMatrix.at(rowIndex);
	}
	const std::vector<long double>& operator [](size_t rowIndex) const {
		return adjacencyMatrix.at(rowIndex);
	}
	size_t size() const {
		return graphOrder;
	}
private:
	size_t graphOrder;
	std::vector<std::vector<long double>> adjacencyMatrix;
};

AdjacencyMatrix operator | (const AdjacencyMatrix& left, const AdjacencyMatrix& right) {
	// Matrix multiplication for Warshall Algorithm (logical OR instead of actual sum)
	if (left.size() != right.size()) {
		throw std::invalid_argument("can't multiply different matrices");
	}
	size_t graphOrder = left.size();
	AdjacencyMatrix result(graphOrder);
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			long double elementMultiplicationResult = 0;
			for (size_t multiplicationIndex = 0; multiplicationIndex < graphOrder; ++multiplicationIndex) {
				elementMultiplicationResult += left[rowIndex][multiplicationIndex] * right[multiplicationIndex][columnIndex];
			}
			if (elementMultiplicationResult > 0) {
				result[rowIndex][columnIndex] = 1;
			}
		}
	}
	return result;
}

AdjacencyMatrix operator || (const AdjacencyMatrix& left, const AdjacencyMatrix& right) {
	// matrix disjunction
	if (left.size() != right.size()) {
		throw std::invalid_argument("can't find disjunction of matrices with different dimentions");
	}
	size_t graphOrder = left.size();
	AdjacencyMatrix result(graphOrder);
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			result[rowIndex][columnIndex] = (left[rowIndex][columnIndex] == 1 || right[rowIndex][columnIndex] == 1 ? 1 : 0);
		}
	}
	return result;
}

std::ostream& operator << (std::ostream& os, const AdjacencyMatrix& adjacencyMatrix) {
	size_t graphOrder = adjacencyMatrix.size();
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			os << std::setw(5) << adjacencyMatrix[rowIndex][columnIndex] << " ";
		}
		os << std::endl;
	}
	return os;
}

AdjacencyMatrix getRelationTransitiveClosure(const AdjacencyMatrix& relation) {
	size_t graphOrder = relation.size();
	AdjacencyMatrix result = relation;
	AdjacencyMatrix nextPower = relation;
	for (size_t iteration = 1; iteration < graphOrder - 1; ++iteration) {
		nextPower = nextPower | relation;
		result = result || nextPower;
	}
	return result;
}

AdjacencyMatrix findNextFuzzyRelationPower(const AdjacencyMatrix& nextFuzzyRelationPower, 
					   const AdjacencyMatrix& fuzzyRelation, 
					   const AdjacencyMatrix& nextRelationPower) {
	size_t graphOrder = fuzzyRelation.size();
	AdjacencyMatrix powerResult(graphOrder);
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			if (nextRelationPower[rowIndex][columnIndex] == 1) {
				long double result = 0;
				for (size_t interimNode = 0; interimNode < graphOrder; ++interimNode) {
					result = max(result, min(nextFuzzyRelationPower[rowIndex][interimNode], fuzzyRelation[interimNode][columnIndex]));
				}	
				powerResult[rowIndex][columnIndex] = result;
			}
		}
	}
	return powerResult;
}

AdjacencyMatrix fuzzyUnion(const AdjacencyMatrix& left, const AdjacencyMatrix& right) {
	if (left.size() != right.size()) {
		throw std::invalid_argument("can't unionize adjacency_matrix of different sizes");
	}
	size_t graphOrder = left.size();
	AdjacencyMatrix result(graphOrder);
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			result[rowIndex][columnIndex] = std::max(left[rowIndex][columnIndex], right[rowIndex][columnIndex]);	
		}
	}
	return result;
}

AdjacencyMatrix getFuzzyRelationTransitiveClosure(const AdjacencyMatrix& fuzzyRelation) {
	size_t graphOrder = fuzzyRelation.size();
	AdjacencyMatrix relation(graphOrder);
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			if (fuzzyRelation[rowIndex][columnIndex] > 0) {
				relation[rowIndex][columnIndex] = 1;
			}
		}
	}
	AdjacencyMatrix result = fuzzyRelation;
	AdjacencyMatrix nextRelationPower = relation;
	AdjacencyMatrix nextFuzzyRelationPower = fuzzyRelation;
	for (size_t iteration = 1; iteration < graphOrder - 1; ++iteration) {
		nextRelationPower = nextRelationPower | relation;
		nextFuzzyRelationPower = findNextFuzzyRelationPower(nextFuzzyRelationPower, fuzzyRelation, nextRelationPower);
		result = fuzzyUnion(result, nextFuzzyRelationPower);
	}
	return result;
}

int main() {
	size_t graphOrder;
	std::cout << "Enter order of a graph of fuzzy relation: ";
	std::cin >> graphOrder;
	AdjacencyMatrix fuzzyRelation(graphOrder);
	for (size_t rowIndex = 0; rowIndex < graphOrder; ++rowIndex) {
		for (size_t columnIndex = 0; columnIndex < graphOrder; ++columnIndex) {
			std::cin >> fuzzyRelation[rowIndex][columnIndex];
		}
	}
	std::cout << "Transitive closure: " << std::endl;
	std::cout << getFuzzyRelationTransitiveClosure(fuzzyRelation) << std::endl;
	return 0;
}
