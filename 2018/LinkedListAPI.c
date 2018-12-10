#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.h"
#include "assert.h"

// Function to initialize the list metadata head to the appropriate function pointers. Allocates memory to the struct.
List initializeList(char *(*printFunction)(void *toBePrinted), void (*deleteFunction)(void *toBeDeleted), int (*compareFunction)(const void *first, const void *second))
{
	List tmpList;

	//Asserts create a partial function...
	//assert(printFunction != NULL);
	//assert(deleteFunction != NULL);
	//assert(compareFunction != NULL);

	tmpList.head = NULL;
	tmpList.tail = NULL;
	tmpList.length = 0;
	tmpList.deleteData = deleteFunction;
	tmpList.compare = compareFunction;
	tmpList.printData = printFunction;

	return tmpList;
}

// Deletes the entire linked list, freeing all memory.
void clearList(List *list)
{

	if (list == NULL)
	{
		return;
	}

	if (list->head == NULL && list->tail == NULL)
	{
		return;
	}

	Node *tmp;

	while (list->head != NULL)
	{
		list->deleteData(list->head->data);
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}

	list->head = NULL;
	list->tail = NULL;
}

// Function for creating a node for the linked list.
Node *initializeNode(void *data)
{
	Node *tmpNode;

	tmpNode = (Node *)malloc(sizeof(Node));

	if (tmpNode == NULL)
	{
		return NULL;
	}

	tmpNode->data = data;
	tmpNode->previous = NULL;
	tmpNode->next = NULL;

	return tmpNode;
}

// Inserts a Node at the front of a linked list.  List metadata is updated
void insertBack(List *list, void *toBeAdded)
{
	if (list == NULL || toBeAdded == NULL)
	{
		return;
	}

	Node *newNode = initializeNode(toBeAdded);

	if (list->head == NULL && list->tail == NULL)
	{
		list->head = newNode;
		list->tail = list->head;
		list->length++;
	}
	else
	{
		newNode->previous = list->tail;
		list->tail->next = newNode;
		list->tail = newNode;
		list->length++;
	}
}

// Inserts a Node at the front of a linked list.  List metadata is updated
void insertFront(List *list, void *toBeAdded)
{
	if (list == NULL || toBeAdded == NULL)
	{
		return;
	}

	Node *newNode = initializeNode(toBeAdded);

	if (list->head == NULL && list->tail == NULL)
	{
		list->head = newNode;
		list->tail = list->head;
		list->length++;
	}
	else
	{
		newNode->next = list->head;
		list->head->previous = newNode;
		list->head = newNode;
		list->length++;
	}
}

// Returns a pointer to the data at the front of the list. Does not alter list structure.
void *getFromFront(List list)
{

	if (list.head == NULL)
	{
		return NULL;
	}

	return list.head->data;
}

// Returns a pointer to the data at the back of the list. Does not alter list structure.
void *getFromBack(List list)
{

	if (list.tail == NULL)
	{
		return NULL;
	}

	return list.tail->data;
}

// Removes data from from the list, deletes the node and frees the memory,
void *deleteDataFromList(List *list, void *toBeDeleted)
{
	if (list == NULL || toBeDeleted == NULL)
	{
		return NULL;
	}

	Node *tmp = list->head;

	while (tmp != NULL)
	{
		if (list->compare(toBeDeleted, tmp->data) == 0)
		{
			//Unlink the node
			Node *delNode = tmp;

			if (tmp->previous != NULL)
			{
				tmp->previous->next = delNode->next;
			}
			else
			{
				list->head = delNode->next;
			}

			if (tmp->next != NULL)
			{
				tmp->next->previous = delNode->previous;
			}
			else
			{
				list->tail = delNode->previous;
			}

			void *data = delNode->data;
			free(delNode);
			list->length--;
			return data;
		}
		else
		{
			tmp = tmp->next;
		}
	}

	return NULL;
}

// Uses the comparison function pointer to place the element in the List
void insertSorted(List *list, void *toBeAdded)
{
	if (list == NULL || toBeAdded == NULL)
	{
		return;
	}

	if (list->head == NULL)
	{
		insertBack(list, toBeAdded);
		return;
	}

	if (list->compare(toBeAdded, list->head->data) <= 0)
	{
		insertFront(list, toBeAdded);
		return;
	}

	if (list->compare(toBeAdded, list->tail->data) > 0)
	{
		insertBack(list, toBeAdded);
		return;
	}

	Node *currNode = list->head;

	while (currNode != NULL)
	{
		if (list->compare(toBeAdded, currNode->data) <= 0)
		{

			// Debug?
			// char* currDescr = list->printData(currNode->data);
			// char* newDescr = list->printData(toBeAdded);

			// printf("Inserting %s before %s\n", newDescr, currDescr);

			// free(currDescr);
			// free(newDescr);

			Node *newNode = initializeNode(toBeAdded);
			newNode->next = currNode;
			newNode->previous = currNode->previous;
			currNode->previous->next = newNode;
			currNode->previous = newNode;
			list->length++;

			return;
		}

		currNode = currNode->next;
	}

	return;
}

//Returns a string that contains a string representation of the list traversed from  head to tail.
char *toString(List list)
{
	ListIterator iter = createIterator(list);
	char *str;

	str = malloc(sizeof(char));
	strcpy(str, "");

	void *elem;
	while ((elem = nextElement(&iter)) != NULL)
	{
		list.printData(elem);
		// char* currDescr = list.printData(elem);
		// int newLen = strlen(str)+50+strlen(currDescr);
		// str = realloc(str, newLen);
		// strcat(str, "\n");
		// strcat(str, currDescr);

		// free(currDescr);
	}
	return str;
}

ListIterator createIterator(List list)
{
	ListIterator iter;

	iter.current = list.head;

	return iter;
}

void *nextElement(ListIterator *iter)
{
	Node *tmp = iter->current;

	if (tmp != NULL)
	{
		iter->current = iter->current->next;
		return tmp->data;
	}
	else
	{
		return NULL;
	}
}

// Function that searches for an element in the list using a comparator function.
void *findElement(List list, bool (*customCompare)(const void *first, const void *second), const void *searchRecord)
{

	if (list.head == NULL)
	{
		return NULL;
	}

	Node *tempNode = list.head;

	while (tempNode != NULL)
	{
		if (customCompare(tempNode->data, searchRecord) == true)
		{
			return tempNode->data;
		}
		tempNode = tempNode->next;
	}

	return NULL;
}

// Returns the number of elements in the list.
int getLength(List list)
{

	if (list.deleteData == NULL || list.compare == NULL || list.printData == NULL)
	{
		return -1;
	}

	return list.length;
}
