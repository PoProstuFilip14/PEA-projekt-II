#include "Queue.h"
#include "Node.h"

Queue::Queue() {
	firstItem = nullptr;
	lastItem = nullptr;
}

bool Queue::empty() {
	if (firstItem == nullptr) {
		return true;
	}
	else {
		return false;
	}
}

void Queue::enqueue(Node* node) {
	QueueItem* newItem = new QueueItem(node);

	if (empty()) {
		firstItem = newItem;
		lastItem = newItem;
	}
	else {
		lastItem->setNextItem(newItem);
		lastItem = newItem;
	}
}

void Queue::dequeue() {
	QueueItem* deletedItem = firstItem;

	if (!empty()) {
		firstItem = deletedItem->getNextItem();
	}

	delete deletedItem;
}

Node* Queue::getFirstItem() {
	return firstItem->getNode();
}