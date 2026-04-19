#pragma once
#include "Node.h"

template <typename T> class QueueItem
{
private:
	T* node;
	QueueItem* nextItem;
public:
	QueueItem(T* node) {
		this->node = node;
		nextItem = nullptr;
	}

	T* getNode() {
		return node;
	}

	QueueItem* getNextItem() {
		return nextItem;
	}

	void setNextItem(QueueItem* nextItem) {
		this->nextItem = nextItem;
	}

	~QueueItem() {
		delete node;
	}
};

