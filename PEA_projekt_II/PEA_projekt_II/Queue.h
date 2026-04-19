#pragma once
#include "QueueItem.h"

class Queue
{
	private:
		QueueItem* firstItem;
		QueueItem* lastItem;
	public:
		Queue();

		bool empty();

		void enqueue(Node* node);

		void dequeue();

		Node* getFirstItem();
};

