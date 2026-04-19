#pragma once
class Node
{
	private:
		int* path;
		bool* visited;
		int cost;
		int bound;
		int level;

		int calculateBound(int* graph, int size);

	public:
		Node();

		Node(int* path, bool* visited, int cost, int level, int* graph, int size);

		int getLevel();

		void setLevel(int level);

		int getCost();

		void setCost(int cost);

		int getBound();

		void setBound(int* graph, int size);

		int* getPath();

		void setPath(int* path);

		bool* getVisited();

		void setVisited(bool* visited);

		void deleteArrays();
};

