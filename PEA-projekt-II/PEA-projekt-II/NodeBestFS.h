#pragma once
class NodeBestFS
{
private:
	int bound;
	int* graph;
	int* edges;
	bool* visited;
	int numberOfEdges;

public:
	NodeBestFS();

	NodeBestFS(int bound, int* graph, bool* visited, int* edges, int numberOfEdges, int size);

	int* getGraph();

	int* getEdges();

	int getNumberOfEdges();

	int getBound();

	bool* getVisited();

	~NodeBestFS();
};