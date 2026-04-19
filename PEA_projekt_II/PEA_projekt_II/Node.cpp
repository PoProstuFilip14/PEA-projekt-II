#include <limits.h>
#include "Node.h"

Node::Node() {
	path = nullptr;
	visited = nullptr;
	cost = INT_MAX;
	level = -1;
	bound = INT_MAX;
}

Node::Node(int* path, bool* visited, int cost, int level, int* graph, int size) {
	this->path = path;
	this->visited = visited;
	this->cost = cost;
	this->level = level;
	bound = calculateBound(graph, size);
}

int Node::calculateBound(int* graph, int size) {
	int bound = cost;

	for (int i = 0; i < size; i++) {
		if (!visited[i]) {
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

int Node::getLevel() {
	return level;
}

void Node::setLevel(int level) {
	this->level = level;
}

int Node::getCost() {
	return cost;
}

void Node::setCost(int cost) {
	this->cost = cost;
}

int Node::getBound() {
	return bound;
}

void Node::setBound(int* graph, int size) {
	this->bound = calculateBound(graph, size);
}

int* Node::getPath() {
	return path;
}

void Node::setPath(int* path) {
	this->path = path;
}

bool* Node::getVisited() {
	return visited;
}

void Node::setVisited(bool* visited) {
	this->visited = visited;
}

void Node::deleteArrays() {
	delete[] path;
	delete[] visited;
}