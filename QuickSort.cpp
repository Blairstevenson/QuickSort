#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <time.h>
using namespace std;

/* This is the node of our linked list */
template <class T>
class ListItem
{
private:
	T* item;
	ListItem<T>* prev;
	ListItem<T>* next;
public:
	ListItem<T>(T* item);
	ListItem<T>* getPrev();
	void setPrev(ListItem<T>* prev);
	ListItem<T>* getNext();
	void setNext(ListItem<T>* next);
	T* getItem();
	void setItem(T* prev);
};

/* This is our doubly linked list */
template <class T>
class LinkedList
{
private:
	ListItem<T>* first; // point to first item
	ListItem<T>* last; // point to last item
public:
	LinkedList<T>();
	// functions to find data in the list
	// please note that T must properly implement/override operator = (equal)
	// in order for these functions  to work
	ListItem<T>* Find(T*);
	ListItem<T>* Find(T);
	void Insert(T*);
	void Insert(T);
	// Delete() can only work if its input is an actual node of this list, or null.
	// So we should only pass result of Find() into this function.
	void Delete(ListItem<T>*);
	// swap items
	void Swap(ListItem<T>*, ListItem<T>*);
	// insert an item after another item
	void Insert(ListItem<T>*, ListItem<T>*);
	ListItem<T>* getFirst();
	ListItem<T>* getLast();
	T Maximum();
	T Minimum();
	// print the entire list, for testing purpose
	void Print(ListItem<T>*);
};

/* Insertion sort algorithm */
template <class T>
void InsertionSort(LinkedList<T>* list)
{
	ListItem<T>* current = list->getFirst();
	if (list == nullptr)
	{
		// the list is empty, no sorting needed
		return;
	}
	current = current->getNext();
	// loop through the list
	while (current != nullptr)
	{
		ListItem<T>* back = current->getPrev();
		while (back != nullptr)
		{
			// sort ascending
			if (*(back->getItem()) > *(current->getItem()))
			{
				// if "back" still bigger than "current", move back more
				back = back->getPrev();
			}
			else
			{
				// "back" is now smaller (or equal) with "current", insert
				// "current" between "back" and next node of "back"

				// please note that if "current" is right after "back",
				// no insertion is needed.
				if (back->getNext() != current)
				{
					list->Delete(current);
					list->Insert(current, back);
					//list->Swap(back, current);
				}
				break;
			}
		}
		if (back == nullptr)
		{
			// insert "current" to the first position
			list->Delete(current);
			list->Insert(current, nullptr);
		}
		current = current->getNext();
	}
}

template <class T>
void QuickSort(LinkedList<T>* list)
{
	if (list->getFirst() == nullptr)
	{
		// the list is empty, no sorting needed
		return;
	}
	QuickSort(list, list->getFirst(), list->getLast());
}

// recursive call of quicksort
template <class T>
void QuickSort(LinkedList<T>* list, ListItem<T>* start, ListItem<T>* end)
{
	if (start == end)
	{
		// done sorting
		return;
	}

	// select "start" as pivot, partition the list in-place
	ListItem<T>* left = start;
	ListItem<T>* right = end;
	int crossed = 0;
	while (!crossed)
	{
		// move left pointer
		while (*(left->getItem()) <= *(start->getItem()))
		{
			if (left != end)
			{
				left = left->getNext();
				// check crossing
				if (left == right)
				{
					crossed = 1;
				}
			}
			else
			{
				// reached the end of current partition
				break;
			}
		}
		// move right pointer
		while (*(right->getItem()) > *(start->getItem()))
		{
			if (right != start)
			{
				right = right->getPrev();
				// check crossing
				if (left == right)
				{
					crossed = 1;
				}
			}
			else
			{
				// reached the start of current partition
				break;
			}
		}
		// if the two pointers haven't crossed each other yet, swap the two values
		if (!crossed)
		{
			list->Swap(left, right);
		}
	}
	// "right" is the final position of "start" (the pivot)
	list->Swap(start, right);
	// recursive
	if (right != start)
	{
		// left partition
		QuickSort(list, start, right->getPrev());
	}
	if (right != end)
	{
		// right partition
		QuickSort(list, right->getNext(), end);
	}
}

/* Main function to test our sorting algorithm */
int main(int argc, char* argv[])
{
	cout << endl;
	if (argc != 3)
	{
		cout << "Wrong number of arguments. There should be two." << endl;
		cout << "The first argument is input file name, the 2nd one is an integer (1 or 2)." << endl;
		getchar();
		return 0;
	}

	// from main parameters
	char* fileName = argv[1];
	char* mode = argv[2];
	int algorithm = mode[0] == '2' ? 2 : 1;
	if (algorithm == 1)
	{
		cout << "Quicksort selected"<< endl;
	}
	else if (algorithm == 2)
	{
		cout << "Insertion sort selected" << endl;
	}

	// for storing input
	LinkedList<LinkedList<int>> inputList;
	int inputCount = 0;
	
	// read input
	ifstream infile(fileName);
	LinkedList<int>* first = new LinkedList<int>();
	inputList.Insert(first);
	int number;
	while (infile >> number)
	{
		first->Insert(number);
		inputCount++;
	}
	if (inputCount <= 0)
	{
		cout << "File " << fileName << " contains no input!" << endl;
		getchar();
		return 1;
	}
	else
	{
		cout << "Input count: " << inputCount << endl << endl;
	}
	// calculate number of input and its clones, in order to obtain execution time
	int inputSetCount = 100000 / inputCount;
	if (inputSetCount <= 0)
	{
		inputSetCount = 1;
	}
	// create lists for cloning input
	for (int i = 1; i < inputSetCount; i++)
	{
		inputList.Insert(new LinkedList<int>);
	}
	cout << "Created " << inputSetCount << " input set for measuring runtime" << endl << endl;
	// clone input
	ListItem<LinkedList<int>>* currentSet = inputList.getFirst()->getNext();
	while (currentSet != nullptr)
	{
		ListItem<int>* origin = first->getFirst();
		while (origin != nullptr)
		{
			currentSet->getItem()->Insert(*(origin->getItem()));
			origin = origin->getNext();
		}

		currentSet = currentSet->getNext();
	}
	cout << "Input cloning completed" << endl << endl;
	// perform sorting
	clock_t t1, t2;
	currentSet = inputList.getFirst();
	// start counting time
	cout << "Start counting time... " << endl << endl;
	t1 = clock();
	while(currentSet != nullptr)
	{
		if (algorithm == 1)
		{
			QuickSort(currentSet->getItem());
		}
		else if (algorithm == 2)
		{
			InsertionSort(currentSet->getItem());
		}
		currentSet = currentSet->getNext();
	}
	// finish counting time
	t2 = clock();
	cout << "Finished counting time " << endl << endl;
	float diff((float)t2 - (float)t1);
	cout << "Runtime for one set: " << diff / 1000 / inputSetCount << " s" << endl << endl;

	cout << "Result list: " << endl;
	inputList.getFirst()->getItem()->Print(nullptr);

	getchar();
	return 0;
}

/*********************************/
/* ListItem function declaration */
/*********************************/
template <class T>
ListItem<T>::ListItem(T* item)
{
	this->item = item;
	this->next = nullptr;
	this->prev = nullptr;
}

template <class T>
ListItem<T>* ListItem<T>::getPrev()
{
	return this->prev;
}

template <class T>
void ListItem<T>::setPrev(ListItem<T>* prev)
{
	this->prev = prev;
}

template <class T>
ListItem<T>* ListItem<T>::getNext()
{
	return this->next;
}

template <class T>
void ListItem<T>::setNext(ListItem<T>* next)
{
	this->next = next;
}

template <class T>
T* ListItem<T>::getItem()
{
	return this->item;
}

template <class T>
void ListItem<T>::setItem(T* item)
{
	this->item = item;
}

/***********************************/
/* LinkedList function declaration */
/***********************************/
template <class T>
LinkedList<T>::LinkedList()
{
	// remember to initialize our pointer
	this->first = nullptr;
	this->last = nullptr;
}

template <class T>
ListItem<T>* LinkedList<T>::Find(T* data)
{
	// start with the first item in the list
	// please note that it can be null
	ListItem<T>* temp = this->first;
	// loop through the list using next pointer
	while (temp != nullptr && *(temp->getItem()) != *data)
	{
		temp = temp->getNext();
	}
	// if temp is null here, the data is not in the list
	return temp;
}

template <class T>
ListItem<T>* LinkedList<T>::Find(T data)
{
	return this->Find(&data);
}

template <class T>
void LinkedList<T>::Insert(T* data)
{
	if (this->first == nullptr)
	{
		// the list is empty
		this->first = new ListItem<T>(data);
		this->last = this->first;
	}
	else
	{
		// create new item
		ListItem<T>* newItem = new ListItem<T>(data);
		// point next pointer of current last item to the new item
		this->last->setNext(newItem);
		// point prev pointer of the new item to current last item
		newItem->setPrev(this->last);
		// move last pointer to the new item
		this->last = newItem;
	}
}

template <class T>
void LinkedList<T>::Insert(T data)
{
	T* dataPtr = new T(data);
	this->Insert(dataPtr);
}

template <class T>
void LinkedList<T>::Delete(ListItem<T>* node)
{
	// the node returned from Find() can be null
	if (node != nullptr)
	{
		ListItem<T>* prev = node->getPrev();
		ListItem<T>* next = node->getNext();

		if (prev == nullptr && next == nullptr)
		{
			// the node is the only one in the list
			this->first = this->last = nullptr;
		}
		else if (prev == nullptr)
		{
			// the node is the first one
			// point first pointer to next node
			this->first = this->first->getNext();
			// clear prev pointer of the next node (which is now the first one)
			this->first->setPrev(nullptr);
		}
		else if (next == nullptr)
		{
			// the node is the last one
			// point last pointer to previous node
			this->last = this->last->getPrev();
			// clear next pointer of the previous node (which is now the last one)
			this->last->setNext(nullptr);
		}
		else
		{
			// the node is in the middle of the list
			// point the next node and the previous node toward each other
			next->setPrev(prev);
			prev->setNext(next);
		}
	}
}

template <class T>
void LinkedList<T>::Swap(ListItem<T>* node1, ListItem<T>* node2)
{
	T* temp = node1->getItem();
	node1->setItem(node2->getItem());
	node2->setItem(temp);
}

template <class T>
ListItem<T>* LinkedList<T>::getFirst()
{
	return this->first;
}

template <class T>
ListItem<T>* LinkedList<T>::getLast()
{
	return this->last;
}

template <class T>
void LinkedList<T>::Insert(ListItem<T>* item, ListItem<T>* node)
{
	if (node == nullptr)
	{
		// insert to the first position
		this->first->setPrev(item);
		item->setNext(this->first);
		item->setPrev(nullptr);
		this->first = item;
	}
	else
	{
		ListItem<T>* after = node->getNext();
		node->setNext(item);
		if (after != nullptr)
		{
			after->setPrev(item);
		}
		else
		{
			// update last node
			this->last = item;
		}
		item->setPrev(node);
		item->setNext(after);
	}
}

template <class T>
T LinkedList<T>::Minimum()
{
	T min;
	ListItem<T>* temp = this->first;
	if (temp != nullptr)
	{
		min = *(temp->getItem());
		temp = temp->getNext();
	}
	// loop through the list using next pointer
	while (temp != nullptr)
	{
		if (min > *(temp->getItem()))
		{
			min = *(temp->getItem());
		}
		// move to next node
		temp = temp->getNext();
	}
	return min;
}

template <class T>
T LinkedList<T>::Maximum()
{
	T max;
	ListItem<T>* temp = this->first;
	if (temp != nullptr)
	{
		max = *(temp->getItem());
		temp = temp->getNext();
	}
	// loop through the list using next pointer
	while (temp != nullptr)
	{
		if (max < *(temp->getItem()))
		{
			max = *(temp->getItem());
		}
		// move to next node
		temp = temp->getNext();
	}
	return max;
}

template <class T>
void LinkedList<T>::Print(ListItem<T>* anchor)
{
	cout << "< ";
	// start with the first item in the list
	// please note that it can be null
	ListItem<T>* temp = this->first;
	// loop through the list using next pointer
	while (temp != nullptr)
	{
		if (temp == anchor)
		{
			// hightlight the anchor
			cout << "(" << *(temp->getItem()) << ") ";
		}
		else
		{
			cout << *(temp->getItem()) << " ";
		}
		// move to next node
		temp = temp->getNext();
	}
	cout << ">" << endl;
}
