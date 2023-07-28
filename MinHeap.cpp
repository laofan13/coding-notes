#include <climits>
#include <iostream>
#include <utility>
#include <cassert>
#include <vector>

/** A class for Min Heap */
class MinHeap {
public:
    explicit MinHeap(int cap) 
        :capacity(cap), heap_size(0)
    {
        harr = new int[cap];
    }
    
    ~MinHeap() { delete[] harr; }

    /** Inserts a new key 'k' */
    void Push(int k) {
        if (heap_size == capacity) {
            std::cout << "\nOverflow: Could not insertKey\n";
            return;
        }

        // First insert the new key at the end
        heap_size++;
        int i = heap_size - 1;
        harr[i] = k;

        // Fix the min heap property if it is violated
        while (i != 0 && harr[parent(i)] > harr[i]) {
            std::swap(harr[i], harr[parent(i)]);
            i = parent(i);
        }
    }

    /** Returns the minimum key (key at root) from min heap */
    int Top() { return harr[0]; }

    /** to extract the root which is the minimum element */
    void Pop() {
        if (heap_size <= 0)
            return;
        if (heap_size == 1) {
            heap_size--;
            return;
        }

        // Store the minimum value, and remove it from heap
        harr[0] = harr[heap_size - 1];
        heap_size--;
        MinHeapify(0);
    }

private:
        /** to heapify a subtree with the root at given index */
    void MinHeapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < heap_size && harr[l] < harr[i])
            smallest = l;
        if (r < heap_size && harr[r] < harr[smallest])
            smallest = r;
        if (smallest != i) {
            std::swap(harr[i], harr[smallest]);
            MinHeapify(smallest);
        }
    }

    int parent(int i) { return (i - 1) / 2; }

    /** to get index of left child of node at index i */
    int left(int i) { return (2 * i + 1); }

    /** to get index of right child of node at index i */
    int right(int i) { return (2 * i + 2); }

private:
    int *harr;     
    int capacity; 
    int heap_size;
};

int main() {
    MinHeap heap(11);
    heap.Push(3);
    heap.Push(2);
    heap.Push(1);
    heap.Push(15);
    heap.Push(5);
    heap.Push(4);
    heap.Push(45);
    assert(heap.Top() == 1);
    heap.Pop();
    assert(heap.Top() == 2);

    return 0;
}
