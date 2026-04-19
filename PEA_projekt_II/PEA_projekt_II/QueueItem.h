#pragma once
#include "Node.h"

class QueueItem
{
	private:
		Node* node;
		QueueItem* nextItem;
	public:
		QueueItem(Node* node);

		Node* getNode();

		QueueItem* getNextItem();

		void setNextItem(QueueItem* nextItem);

		void deleteNode();
};

