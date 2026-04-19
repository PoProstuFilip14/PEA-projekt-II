#include <Queue>
#include <vector>
#include <time.h>
#include <iostream>
#include "Node.h"
#include "NodeBestFS.h"
#include "Queue.h"

//implementation of NN algorithm
int nnAlgorithm(int* graph, int size) {
	int* perm = new int[size];
	int* shortestCycle = new int[size];
	int shortestCycleLength = INT_MAX;
	int* shortestEdges = new int[size];
	bool* isVisited = new bool[size];

	perm[0] = 0;

	//prepering data structures for algorithm
	for (int i = 0; i < size; i++) {
		shortestEdges[i] = INT_MAX;
		if (i == 0) {
			isVisited[i] = true;
		}
		else {
			isVisited[i] = false;
		}
	}
	//loop controlling the number of chosen edges
	for (int i = 0; i >= 0;) {
		bool isNewPerm = false;
		//checking if the algorithm is going forward or backward through the current permutation
		if (shortestEdges[i] == INT_MAX) {
			//choosing the best available edge
			for (int j = 0; j < size; j++) {
				if (!isVisited[j] && graph[perm[i] * size + j] < shortestEdges[i]) {
					perm[i + 1] = j;
					shortestEdges[i] = graph[perm[i] * size + j];
					isNewPerm = true;
				}
			}
		}
		else {
			//checking if there is an edge available that is as good as the one chosen earlier
			for (int j = perm[i + 1] + 1; j < size; j++) {
				if (!isVisited[j] && graph[perm[i] * size + j] == shortestEdges[i]) {
					perm[i + 1] = j;
					shortestEdges[i] = graph[perm[i] * size + j];
					isNewPerm = true;
				}
			}
		}
		//checking if the next edge have been found
		if (isNewPerm) {
			i++;
			isVisited[perm[i]] = true;
		}
		if (!isNewPerm) {
			isVisited[perm[i]] = false;
			shortestEdges[i] = INT_MAX;
			i--;
		}
		//checking if cycle is finished
		if (i == size - 1) {
			int length = 0;
			for (int j = 0; j < size - 1; j++) {
				length += shortestEdges[j];
			}
			length += graph[perm[size - 1] * size + perm[0]];
			if (length < shortestCycleLength) {
				shortestCycleLength = length;
				for (int j = 0; j < size; j++) {
					shortestCycle[j] = perm[j];
				}
			}
		}
	}

	delete[] shortestCycle;
	delete[] shortestEdges;
	delete[] perm;
	delete[] isVisited;

	return shortestCycleLength;
}

Node** generateChildren(Node* vertex, int* graph, int size) {
	Node** children = new Node * [size - vertex->getLevel()];
	int numberOfChildren = 0;

	for (int i = 0; i < size; i++) {
		if (!vertex->getVisited()[i]) {
			int* path = new int[size + 1];
			bool* visited = new bool[size];
			int cost = vertex->getCost() + graph[vertex->getPath()[vertex->getLevel() - 1] * size + i];
			for (int j = 0; j < size; j++) {
				path[j] = vertex->getPath()[j];
				visited[j] = vertex->getVisited()[j];
			}
			path[size] = vertex->getPath()[size];
			path[vertex->getLevel()] = i;
			int level = vertex->getLevel() + 1;
			visited[i] = true;

			children[numberOfChildren] = new Node(path, visited, cost, level, graph, size);

			delete[] path;
			delete[] visited;

			numberOfChildren++;
		}
	}

	return children;
}

void bfsAlgorithm(int* graph, int size) {
	Queue<Node>* queue = new Queue<Node>();

	int* path = new int[size + 1];
	bool* visited = new bool[size];
	path[0] = 0;
	path[size] = -1;
	visited[0] = true;
	for (int i = 1; i < size; i++) {
		path[i] = -1;
		visited[i] = false;
	}
	int cost = 0;
	int level = 1;

	Node* root = new Node(path, visited, cost, level, graph, size);

	queue->enqueue(root);

	int shortestCycleLength = nnAlgorithm(graph, size);
	int* shortestCycle = new int[size + 1];

	while (!queue->empty()) {
		Node* vertex = queue->getFirstItem();

		//std::cout << "Last vertex: " << vertex->getPath()[vertex->getLevel() - 1] << " Bound: " << vertex->getBound() << " Cost: " << vertex->getCost() << " Level: " << vertex->getLevel() << std::endl;

		if (vertex->getLevel() == size) {
			vertex->setCost(vertex->getCost() + graph[vertex->getPath()[vertex->getLevel() - 1] * size + 0]);
			vertex->getPath()[size] = 0;

			if (vertex->getCost() <= shortestCycleLength) {
				shortestCycleLength = vertex->getCost();
				for (int i = 0; i <= size; i++) {
					//std::cout << vertex->getPath()[i] << std::endl;
					shortestCycle[i] = vertex->getPath()[i];
					//std::cout << shortestCycle[i] << std::endl;
				}
			}
			queue->dequeue();
			continue;
		}

		Node** children = generateChildren(vertex, graph, size);

		for (int i = 0; i < size - vertex->getLevel(); i++) {
			if (children[i]->getBound() <= shortestCycleLength) {
				queue->enqueue(children[i]);
			}
			else {
				delete children[i];
			}
		}

		delete[] children;

		queue->dequeue();
	}

	for (int i = 0; i <= size; i++) {
		std::cout << shortestCycle[i] << " ";
	}
	std::cout << "\n" << shortestCycleLength;

	delete[] path;
	delete[] visited;
	delete[] queue;
	delete[] shortestCycle;
}

int reduceGraph(int* graph, int size) {
	int lowerBound = 0;
	for (int i = 0; i < size; i++) {
		int shortestEdge = INT_MAX;
		for (int j = 0; j < size; j++) {
			if (graph[i * size + j] < shortestEdge && graph[i * size + j] >= 0) {
				shortestEdge = graph[i * size + j];
			}
		}
		lowerBound += shortestEdge;
		for (int j = 0; j < size; j++) {
			if (i != j) {
				graph[i * size + j] -= shortestEdge;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		int shortestEdge = INT_MAX;
		for (int j = 0; j < size; j++) {
			if (graph[j * size + i] < shortestEdge && graph[j * size + i] >= 0) {
				shortestEdge = graph[j * size + i];
			}
		}
		lowerBound += shortestEdge;
		for (int j = 0; j < size; j++) {
			if (i != j) {
				graph[j * size + i] -= shortestEdge;
			}
		}
	}

	return lowerBound;
}

void bestfsAlgorithm(int* graph, int size) {
	int lowerBound = reduceGraph(graph, size);
}

int main() {
	srand(time(0));

	int size = 4;

	int* graph = new int[size * size];

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j) {
				graph[i * size + j] = -1;
			}
			else {
				graph[i * size + j] = rand() % 9 + 1;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		std::cout << std::endl;
		for (int j = 0; j < size; j++) {
			std::cout << graph[i * size + j] << " ";
		}
	}
	std::cout << std::endl;

	bfsAlgorithm(graph, size);

	bestfsAlgorithm(graph, size);

	delete[] graph;

	return 0;
}