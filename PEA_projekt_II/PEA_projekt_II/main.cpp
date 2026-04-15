#include <Queue>
#include <vector>
#include <time.h>
#include <iostream>

struct Node {
    std::vector<int> path;
    std::vector<bool> visited;
    int cost;
    int bound;
    int level;
};

int calculateBound(Node& vertex, int* graph, int size) {
	int bound = vertex.cost;

	for (int i = 0; i < size; i++) {
		if (!vertex.visited[i]) {
			int minCost = INT_MAX;
			for (int j = 0; j < size; j++) {
				if (graph[j * size + i] < minCost && i != j) {
					minCost = graph[j * size + i];
				}
			}
			bound += minCost;
		}
	}

	return bound;
}

std::vector<Node> generateChildren(Node& vertex, int* graph, int size) {
	std::vector<Node> children;

	for (int i = 0; i < size; i++) {
		if (!vertex.visited[i]) {
			Node u;
			u.path = vertex.path;
			u.path.push_back(i);
			u.visited = vertex.visited;
			u.visited[i] = true;
			u.cost = vertex.cost + graph[vertex.path.back() * size + i];
			u.level = vertex.level + 1;
			u.bound = calculateBound(u, graph, size);

			children.push_back(u);
		}
	}

	return children;
}

void bfsAlgorithm(int* graph, int size) {
	std::queue<Node> queue;

	Node root;
	root.path = { 0 };
	root.visited = std::vector<bool>(size, false);
	root.visited[0] = true;
	root.cost = 0;
	root.level = 1;
	root.bound = calculateBound(root, graph, size);

	queue.push(root);
	int shortestCycleLength = INT_MAX;
	std::vector<int> shortestCycle;

	while (!queue.empty()) {
		Node vertex = queue.front();
		queue.pop();

		if (vertex.level == size) {
			vertex.cost += graph[vertex.path.back() * size + 0];
			vertex.path.push_back(0);

			if (vertex.cost < shortestCycleLength) {
				shortestCycleLength = vertex.cost;
				shortestCycle = vertex.path;
			}
			continue;
		}

		std::vector<Node> children = generateChildren(vertex, graph, size);

		for (int i = 0; i < children.size(); i++) {
			if (children[i].bound < shortestCycleLength) {
				queue.push(children[i]);
			}
		}
	}

	for (int i = 0; i < shortestCycle.size(); i++) {
		std::cout << shortestCycle[i] << " ";
	}
	std::cout << "\n" << shortestCycleLength;
}

int main() {
	srand(time(0));

    int numberOfVertices = 5;

    int* graph = new int[numberOfVertices * numberOfVertices];

	for (int i = 0; i < numberOfVertices; i++) {
		for (int j = 0; j < numberOfVertices; j++) {
			if (i == j) {
				graph[i * numberOfVertices + j] = -1;
			}
			else {
				graph[i * numberOfVertices + j] = rand() % 9 + 1;
			}
		}
	}
	
	for (int i = 0; i < numberOfVertices; i++) {
		std::cout << std::endl;
		for (int j = 0; j < numberOfVertices; j++) {
			std::cout << graph[i * numberOfVertices + j] << " ";
		}
	}
	std::cout << std::endl;

	bfsAlgorithm(graph, numberOfVertices);

	return 0;
}