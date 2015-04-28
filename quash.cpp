//Justin Liang
//CS130A
//Due: 11/4/12
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

//Class for an element inside the hash
class HeapElement;
class HashElement
{
	public:	
		int value;
		HeapElement *toHeap;
		bool isEmpty;
		
		HashElement()
		{
			isEmpty = true;
		}
		
		int getValue()
		{
			return value;
		}
};

//Class for an element inside the heap
class HeapElement
{	
	public:
		int value;
		HashElement *toHash;
		bool isEmpty;
		
		HeapElement()
		{
			value = 0;
			isEmpty = true;
		}
};

int mod(int a, int b)
{
	int c = (((a%b) + b) % b);
	return c;
}

//Class for the quash data structure
//Contains a hash table with mod function value%43 using linear probing
//Contains a max binary heap
class Quash
{
	private:
		HashElement hash[43];
		HeapElement heap[44];
		int currentHeapSize;
		int hashPos, heapPos;
		int hashPointerPos, heapPointerPos;
		
	public:
		//Constructor for a new Quash data structure
		Quash()
		{
			hashPos = 0;
			heapPos = 1;
		}		
		
		//Taken from chapter 6.3.4, figure 6.12, percolates down
		int percolateDown(int hole)
		{
			int child;
			HeapElement temp = heap[hole];
			for( ; hole * 2 <= currentHeapSize; hole = child)
			{
				child = hole * 2;
				if((child != currentHeapSize) && (heap[child+1].value > heap[child].value))
					child++;
				if(heap[child].value > temp.value)
				{	
					heap[hole].value = heap[child].value;
					
				}
				else
				{
					break;
				}
			}
			heap[hole].value = temp.value;	
			
		}
		
		//Inserts the value into the hash and into the heap
		void insert(int val)
		{
			hashPos = mod(val,43);
			
			//Inserts into the hash						
			//If hash position is empty, set the value
			if(hash[hashPos].isEmpty == true)
			{
				hash[hashPos].value = val;
				hash[hashPos].isEmpty = false;
				hashPointerPos = hashPos;
			}			
			//If not, start linear probing
			else
			{
				bool alreadyInserted = false;
				//Checks for the next available empty slot and non-marked slot
				for(int i = hashPos; i < 43; i++)
				{
					if(hash[i].isEmpty == true)
					{
						hash[i].value = val;
						hash[i].isEmpty = false;
						hashPointerPos = i;
						alreadyInserted = true;
						break;
					}
				}
				//If linear probing did not insert it from hashPos to end of the hash, try to check from beginning of hash to hashPos
				if(alreadyInserted == false)
				{
					//Checks for empty slots from beginning of hash to hashPos
					for(int i = 0; i < hashPos; i++)
					{
						if(hash[i].isEmpty == true)
						{
							hash[i].value = val;
							hash[i].isEmpty = false;
							hashPointerPos = i;
							alreadyInserted = true;
							break;
						}
					}
				}
				if(alreadyInserted == false)
				{
					cout << "error inserting, table is full";
				}
			}
			
			//Inserts into the heap
			//Checks to see if the heap is full before inserting
			if(heapPos == 44)
			{
				cout << "heap is full";
			}
			
			//If not empty, make an empty hole and percolate up		
			else 
			{
				int hole = heapPos;
				for( ; hole > 1 && val > heap[hole/2].value; hole /= 2)
					heap[hole] = heap[hole/2];
				heap[hole].value = val;
				heap[hole].isEmpty = false;
				heapPointerPos = hole;
				
				
				//Set the pointer from hash to heap and heap to hash
				hash[hashPointerPos].toHeap = &heap[heapPointerPos];
				heap[heapPointerPos].toHash = &hash[hashPointerPos];
								
				heapPos++;
				currentHeapSize = heapPos-1;
			}
		}
		
		//Checks to see if value is in the hash, returns true if found, false if not found
		bool lookup(int val)
		{
			for(int i = 0; i < 43; i++)
			{
				if((hash[i].value == val) && (hash[i].isEmpty == false))
				{
					return true;
				}
			}			
			return false;
		}
		
		//Using the max heap, deletes the first element in the heap (the maximum element)
		//and also the element in the hash table by using the pointers
		void deleteMax()
		{
			if(heap[1].isEmpty == true)
			{
				cout << "max item not present since table is empty" << endl;
			}
			else
			{
				for(int i = 0; i < 43; i++)
				{
					if(hash[i].value == heap[1].value)
					{
						hash[i].isEmpty = true;
					}
				}
				heap[currentHeapSize].isEmpty = true;
				(*heap[1].toHash).isEmpty = true;				
				heap[1].value = heap[currentHeapSize].value;	
				currentHeapSize--;
				percolateDown(1);
			}
		}
		
		//Deletes the selected value
		void deleteValue(int val)
		{
			hashPos = mod(val,43);
			bool alreadyInserted = false;
			
			for(int i = hashPos; i < 43; i++)
			{
				if(hash[i].value == val)
				{
					(*hash[i].toHeap).isEmpty = true;
					percolateDown(i);
					hash[i].isEmpty = true;
					alreadyInserted = true;
					currentHeapSize--;
					break;
				}
			}
			
			if(alreadyInserted == false)
			{
				for(int i = 0; i < hashPos; i++)
				{
					if(hash[i].value == val)
					{
						(*hash[i].toHeap).isEmpty = true;
						percolateDown(i);
						hash[i].isEmpty = true;
						alreadyInserted = true;
						currentHeapSize--;
						break;
					}
				}
			}
			if(alreadyInserted == false)
			{
				cout << "item not present in the table" << endl;
			}
			
			for(int i = 0; i < 44; i++)
			{
				if(val == heap[i].value)
				{
					heap[i].isEmpty = true;
					currentHeapSize--;
				}
			}
		}
		//Prints the current value of the hash and heap in array format
		void print()
		{
			for(int i = 0; i < 43; i++)
			{
				if(hash[i].isEmpty == false)
					cout << hash[i].value << " ";
			}
			cout << endl;
			for(int i = 0; i < 44; i++)
			{
				if(heap[i].isEmpty == false)
					cout << heap[i].value << " ";
			}
			cout << endl;
		}
		
};

//Main function
int main()
{
	Quash *q = new Quash();
	string str;
	int val;
	cin >> str;
	while(!cin.eof())
	{
		if(str.compare("insert") == 0)
		{
			cin >> val;
			if((q->lookup(val)) == true)
				cout << "item already present" << endl;
			else			
				q->insert(val);
		}
		
		if(str.compare("lookup") == 0)
		{
			cin >> val;
			if((q->lookup(val)) == true)
				cout << "item found" << endl;
			else
				cout << "item not found" << endl;
		}
		
		if(str.compare("deleteMax") == 0)
		{			
			q->deleteMax();
		}
		
		if(str.compare("delete") == 0)
		{
			cin >> val;
			q->deleteValue(val);
		}
		
		if(str.compare("print") == 0)
		{
			q->print();
		}		
		
		cin >> str;		
	}
	return 0;
}

