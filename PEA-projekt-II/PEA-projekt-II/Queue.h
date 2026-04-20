#pragma once
#include "QueueItem.h"

template <typename T> class Queue
{
private:
	QueueItem<T>* firstItem;
	QueueItem<T>* lastItem;

public:
	Queue() {
		firstItem = nullptr;
		lastItem = nullptr;
	}

	bool empty() {
		if (firstItem == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}

	void enqueue(T* node) {
		QueueItem<T>* newItem = new QueueItem<T>(node);

		if (empty()) {
			firstItem = newItem;
			lastItem = newItem;
		}
		else {
			lastItem->setNextItem(newItem);
			lastItem = newItem;
		}
	}

	void dequeue() {
		QueueItem<T>* deletedItem = firstItem;

		if (!empty()) {
			firstItem = deletedItem->getNextItem();
		}

		delete deletedItem;
	}

	T* getFirstItem() {
		return firstItem->getNode();
	}
};