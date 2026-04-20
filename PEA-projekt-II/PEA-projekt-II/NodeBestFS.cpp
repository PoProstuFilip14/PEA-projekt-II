#include <limits.h>
#include "NodeBestFS.h"

NodeBestFS::NodeBestFS() {
	bound = INT_MAX;
	graph = nullptr;
	visited = nullptr;
	edges = nullptr;
	numberOfEdges = 0;
}

NodeBestFS::NodeBestFS(int bound, int* graph, bool* visited, int* edges, int numberOfEdges, int size) {
	this->bound = bound;
	this->numberOfEdges = numberOfEdges;
	this->graph = new int[size * size];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			this->graph[i * size + j] = graph[i * size + j];
		}
	}
	this->edges = new int[2 * size];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < size; j++) {
			this->edges[i * size + j] = edges[i * size + j];
		}
	}
	this->visited = new bool[2 * size];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < size; j++) {
			this->visited[i * size + j] = visited[i * size + j];
		}
	}
}

int* NodeBestFS::getGraph() {
	return graph;
}

int* NodeBestFS::getEdges() {
	return edges;
}

int NodeBestFS::getNumberOfEdges() {
	return numberOfEdges;
}

int NodeBestFS::getBound() {
	return bound;
}

bool* NodeBestFS::getVisited() {
	return visited;
}

NodeBestFS::~NodeBestFS() {
	delete[] graph;
	delete[] edges;
	delete[] visited;
}