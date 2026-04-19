#include "QueueItem.h"
#include "Node.h"

QueueItem::QueueItem(Node* node) {
	this->node = node;
	nextItem = nullptr;
}

Node* QueueItem::getNode() {
	return node;
}

QueueItem* QueueItem::getNextItem() {
	return nextItem;
}

void QueueItem::setNextItem(QueueItem* nextItem) {
	this->nextItem = nextItem;
}

void QueueItem::deleteNode() {
	node->deleteArrays();
	delete node;
}