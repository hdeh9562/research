#include "StdAfx.h"
#include "Heap.h"


Heap::Heap(void)
{
}


Heap::~Heap(void)
{
}


int Heap::left(int parent)
{
    int i = ( parent << 1 ) + 1; // 2 * parent + 1
    return ( i < heap.size() ) ? i : -1;
}

int Heap::right(int parent)
{
    int i = ( parent << 1 ) + 2; // 2 * parent + 2
    return ( i < heap.size() ) ? i : -1;
}

int Heap::parent(int child)
{
    if (child != 0)
    {
        int i = (child - 1) >> 1;
        return i;
    }
    return -1;
}

void Heap::heapifyup(int index)
{    
    //cout << "index=" << index << endl;
    //cout << "parent(index)=" << parent(index) << endl;
    //cout << "heap[parent(index)]=" << heap[parent(index)] << endl;
    //cout << "heap[index]=" << heap[index] << endl;
    while ( ( index > 0 ) && ( parent(index) >= 0 ) &&
		( heap[parent(index)].release_time > heap[index].release_time ) )
    {
        NodeMachine tmp = heap[parent(index)];
        heap[parent(index)] = heap[index];
        heap[index] = tmp;
        index = parent(index);
    }
}

void Heap::heapifydown(int index)
{     
    //cout << "index=" << index << endl;
    //cout << "left(index)=" << left(index) << endl;
    //cout << "right(index)=" << right(index) << endl;
    int child = left(index);
    if ( ( child > 0 ) && ( right(index) > 0 ) &&
		( heap[child].release_time > heap[right(index)].release_time ) )
    {
        child = right(index);
    }
    if ( child > 0 )
    {
        NodeMachine tmp = heap[index];
        heap[index] = heap[child];
        heap[child] = tmp;
        heapifydown(child);
    }
}

void Heap::print()
{
    vector<NodeMachine>::iterator pos = heap.begin();
    cout << "\nHeap = ";
    while ( pos != heap.end() ) {
		cout << (*pos).machine_id << " ";
		cout << (*pos).release_time << "\n ";
        ++pos;
    }
    cout << endl;
	pos = heap.begin();
	while ( pos != heap.end() ) {
	//	cout << (*pos).machine_id << " ";
		//cout << (*pos).release_time << " ";
        ++pos;
    }
	 cout << endl;
}
void Heap::insert(NodeMachine element)
{
    heap.push_back(element);
    heapifyup(heap.size() -1);
}


NodeMachine Heap::deletemin()
{
    NodeMachine min = heap.front();
    heap[0] = heap.at(heap.size() - 1);
    heap.pop_back();
    heapifydown(0);
    return min;
}
