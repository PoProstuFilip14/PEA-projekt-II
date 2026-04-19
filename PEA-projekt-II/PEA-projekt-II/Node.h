#pragma once
class Node
{
private:
	int* path;
	bool* visited;
	int cost;
	int bound;
	int level;

public:
	Node();

	Node(int* path, bool* visited, int cost, int level, int* graph, int size);

	int getLevel();

	void setLevel(int level);

	int getCost();

	void setCost(int cost);

	int getBound();

	void setBound(int bound);

	int* getPath();

	void setPath(int* path, int size);

	bool* getVisited();

	void setVisited(bool* visited, int size);

	int calculateBound(int* graph, int size);

	~Node();
};

