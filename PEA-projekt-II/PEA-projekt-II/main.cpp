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

int bfsAlgorithm(int* graph, int size) {
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
	for (int i = 0; i <= size; i++) {
		shortestCycle[i] = -1;
	}

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
			delete queue->getFirstItem();
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
		
		delete queue->getFirstItem();
		queue->dequeue();
	}
	/*
	for (int i = 0; i <= size; i++) {
		std::cout << shortestCycle[i] << " ";
	}
	std::cout << "\n" << shortestCycleLength << std::endl;
	*/
	delete[] path;
	delete[] visited;
	delete queue;
	delete[] shortestCycle;

	return shortestCycleLength;
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
		if (shortestEdge > 0 && shortestEdge < INT_MAX) {
			lowerBound += shortestEdge;
			for (int j = 0; j < size; j++) {
				if (graph[i * size + j] > 0) {
					graph[i * size + j] -= shortestEdge;
				}
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
		if (shortestEdge > 0 && shortestEdge < INT_MAX) {
			lowerBound += shortestEdge;
			for (int j = 0; j < size; j++) {
				if (graph[j * size + i] > 0) {
					graph[j * size + i] -= shortestEdge;
				}
			}
		}
	}

	return lowerBound;
}

bool isCycle(NodeBestFS* node, int size, int x, int y) {
	if (node->getNumberOfEdges() + 1 == size) {
		//std::cout << "Last edge\n";
		return false;
	}
	int begin = x;
	while (true) {
		bool isPath = false;
		for (int j = 0; j < size; j++) {
			if (node->getEdges()[0 * size + j] == begin) {
				begin = node->getEdges()[1 * size + j];
				isPath = true;
				break;
			}
		}
		if (!isPath) {
			break;
		}
		if (begin == y) {
			//std::cout << "Cycle\n";
			return true;
		}
	}
	return false;
}

int bestfsAlgorithm(int* graph, int size) {
	int* originalGraph = new int[size * size];
	int* edges = new int[2 * size];
	bool* visited = new bool[2 * size];

	int shortestCycleLength = INT_MAX;
	int* shortestCycle = new int[size + 1];
	for (int i = 0; i <= size; i++) {
		shortestCycle[i] = -1;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			originalGraph[i * size + j] = graph[i * size + j];
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < size; j++) {
			edges[i * size + j] = -1;
			visited[i * size + j] = false;
		}
	}

	int bound = reduceGraph(graph, size);
	NodeBestFS* root = new NodeBestFS(bound, graph, visited, edges, 0, size);

	Queue<NodeBestFS>* queue = new Queue<NodeBestFS>();

	queue->enqueue(root);

	while (!queue->empty()) {
		NodeBestFS* node = queue->getFirstItem();
		queue->dequeue();
		while (node->getNumberOfEdges() < size && node->getBound() < shortestCycleLength) {
			/*
			for (int i = 0; i < size; i++) {
				std::cout << std::endl;
				for (int j = 0; j < size; j++) {
					std::cout << node->getGraph()[i * size + j] << " ";
				}
			}
			std::cout << std::endl;
			*/
			int bestPenalty = -1;
			int posX = -1;
			int posY = -1;
			int valueX = -1;
			int valueY = -1;
			if (size - node->getNumberOfEdges() > 1) {
				for (int i = 0; i < size; i++) {
					if (!node->getVisited()[1 * size + i]) {
						for (int j = 0; j < size; j++) {
							if (!node->getVisited()[0 * size + j]) {
								if (node->getGraph()[i * size + j] == 0) {
									int x = -1;
									int y = -1;
									for (int k = 0; k < size; k++) {
										if (node->getGraph()[i * size + k] > y) {
											y = node->getGraph()[i * size + k];
										}
										if (node->getGraph()[k * size + j] > x) {
											x = node->getGraph()[k * size + j];
										}
									}
									if (x + y > bestPenalty) {
										bestPenalty = x + y;
										valueX = x;
										valueY = y;
										posX = j;
										posY = i;
									}
								}
							}
						}
					}
				}
			}
			else {
				for (int i = 0; i < size; i++) {
					if (!node->getVisited()[0 * size + i]) {
						posX = i;
					}
					if (!node->getVisited()[1 * size + i]) {
						posY = i;
					}
				}
			}
			//std::cout << posY << ", " << posX << std::endl;

			if (posX == -1 || posY == -1) {
				break;
			}

			int* newGraphB = new int[size * size];
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					newGraphB[i * size + j] = node->getGraph()[i * size + j];
				}
			}
			newGraphB[posY * size + posX] = -1;
			int newBound = reduceGraph(newGraphB, size);
			queue->enqueue(new NodeBestFS(node->getBound() + newBound, newGraphB, node->getVisited(), node->getEdges(), node->getNumberOfEdges(), size));

			delete[] newGraphB;

			if (!isCycle(node, size, posX, posY)) {
				int* newGraphA = new int[size * size];
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						if (i == posY || j == posX) {
							newGraphA[i * size + j] = -1;
						}
						else if (i == posX && j == posY) {
							newGraphA[i * size + j] = -1;
						}
						else {
							newGraphA[i * size + j] = node->getGraph()[i * size + j];
						}
					}
				}
				newBound = reduceGraph(newGraphA, size);
				int* newEdges = new int[2 * size];
				bool* newVisited = new bool[2 * size];
				for (int i = 0; i < size; i++) {
					newEdges[0 * size + i] = node->getEdges()[0 * size + i];
					newEdges[1 * size + i] = node->getEdges()[1 * size + i];
					newVisited[0 * size + i] = node->getVisited()[0 * size + i];
					newVisited[1 * size + i] = node->getVisited()[1 * size + i];
				}
				newEdges[0 * size + node->getNumberOfEdges()] = posY;
				newEdges[1 * size + node->getNumberOfEdges()] = posX;
				newVisited[0 * size + posX] = true;
				newVisited[1 * size + posY] = true;
				NodeBestFS* oldNode = node;
				node = new NodeBestFS(node->getBound() + newBound, newGraphA, newVisited, newEdges, node->getNumberOfEdges() + 1, size);

				delete oldNode;
				delete[] newGraphA;
				delete[] newEdges;
			}
			else {
				break;
			}
		}
		if (node->getNumberOfEdges() == size) {
			int length = 0;
			for (int i = 0; i < size; i++) {
				//std::cout << node->getEdges()[0 * size + i] << "->" << node->getEdges()[1 * size + i] << ": " << originalGraph[node->getEdges()[0 * size + i] * size + node->getEdges()[1 * size + i]] << ", ";
				if (originalGraph[node->getEdges()[0 * size + i] * size + node->getEdges()[1 * size + i]] >= 0) {
					length += originalGraph[node->getEdges()[0 * size + i] * size + node->getEdges()[1 * size + i]];
				}
				else {
					length = INT_MAX;
					break;
				}
			}
			//std::cout << std::endl;
			if (length < shortestCycleLength) {
				shortestCycleLength = length;
				shortestCycle[0] = 0;
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						if (node->getEdges()[0 * size + j] == shortestCycle[i]) {
							shortestCycle[i + 1] = node->getEdges()[1 * size + j];
							break;
						}
					}
				}
			}
		}
		delete node;
	}
	/*
	for (int i = 0; i <= size; i++) {
		std::cout << shortestCycle[i] << " ";
	}
	std::cout << "\n" << shortestCycleLength << std::endl;
	*/
	delete[] originalGraph;
	delete[] shortestCycle;
	delete[] edges;

	return shortestCycleLength;
}

int main() {
	srand(time(0));

	int numberOfErrors = 0;

	int size = 10;

	for (int i = 0; i < 100; i++) {
		int* graph = new int[size * size];

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i == j) {
					graph[i * size + j] = -1;
				}
				else {
					graph[i * size + j] = rand() % 90 + 10;
				}
			}
		}
		/*
		for (int i = 0; i < size; i++) {
			std::cout << std::endl;
			for (int j = 0; j < size; j++) {
				std::cout << graph[i * size + j] << " ";
			}
		}
		std::cout << std::endl;
		*/
		int result1 = bfsAlgorithm(graph, size);

		int result2 = bestfsAlgorithm(graph, size);

		if (!(result1 == result2)) {
			numberOfErrors++;

			std::cout << result1 << " != " << result2 << std::endl;
		}

		delete[] graph;
	}
	std::cout << numberOfErrors << std::endl;
	return 0;
}