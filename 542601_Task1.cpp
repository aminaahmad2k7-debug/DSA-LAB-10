// CS250 Lab 10 - Task 1: Max Heap Insertion and Heap Sort
// Student Name : Amina Ahmad
// Roll No: 542601

#include <iostream>
using namespace std;

/*
 * MaxHeap class
 * Stores a max-heap in a dynamic array.
 * Index arithmetic: parent = (i-1)/2, left = 2i+1, right = 2i+2
 * Time complexity of individual operations is mentioned in the method comments.
 */
class MaxHeap {
private:
    int* arr;       // dynamic array
    int size;       // current number of elements
    int capacity;   // allocated size

    // Double the capacity when full... O(n) but amortized O(1)
    void resize() {
        capacity *= 2;
        int* newArr = new int[capacity];
        for (int i = 0; i < size; i++)
            newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

    // Sift-up: restore heap property after insertion at index i.
    // Returns number of swaps performed. O(log n)
    int siftUp(int i) {
        int swaps = 0;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (arr[i] <= arr[parent])
                break;
            // swap
            int temp = arr[i];
            arr[i] = arr[parent];
            arr[parent] = temp;
            swaps++;
            i = parent;
        }
        return swaps;
    }

    // Sift-down: restore heap property starting from index i
    // for a heap of given heapSize. O(log n)
    void siftDown(int i, int heapSize) {
        while (true) {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < heapSize && arr[left] > arr[largest])
                largest = left;
            if (right < heapSize && arr[right] > arr[largest])
                largest = right;

            if (largest == i)
                break;

            // swap
            int temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            i = largest;
        }
    }

public:
    // Constructor. O(1)
    MaxHeap(int initialCapacity = 8) {
        capacity = initialCapacity;
        size = 0;
        arr = new int[capacity];
    }

    // Destructor
    ~MaxHeap() {
        delete[] arr;
    }

    // Insert a value and sift it up.
    // Prints: value, number of swaps, current heap contents.
    // Time complexity: O(log n)
    void insert(int value) {
        if (size == capacity)
            resize();

        arr[size] = value;
        int swaps = siftUp(size);
        size++;

        // Required output format
        cout << value << "\t" << swaps << "\t";
        for (int i = 0; i < size; i++)
            cout << arr[i] << (i == size - 1 ? "" : " ");
        cout << endl;
    }

    // Heap Sort: sorts the heap contents into ascending order in-place.
    // After this call the array is sorted ascending and is no longer a heap.
    // Time complexity: O(n log n)
    void heapSort() {
        // Build is already done by successive insertions.
        // Now extract max repeatedly.
        for (int end = size - 1; end > 0; end--) {
            // swap root with last element
            int temp = arr[0];
            arr[0] = arr[end];
            arr[end] = temp;
            // restore heap on the reduced range
            siftDown(0, end);
        }
    }

    // Print current contents of the array
    void printArray() const {
        for (int i = 0; i < size; i++)
            cout << arr[i] << (i == size - 1 ? "" : " ");
        cout << endl;
    }

    int getSize() const { return size; }
};

/*
 * Answers to questions (also required in report):
 *
 * 1. Worst-case number of swaps for one insertion into a heap of n elements:
 *    O(log n) ... specifically floor(log2 n) swaps (height of the tree).
 *
 * 2. Time complexity of building the heap by n successive insertions:
 *    O(n log n) in the worst case.
 *    Time complexity of heap sort: O(n log n).
 */

int main() {
    // Sample input
    int values[] = { 25, 40, 15, 60, 35, 10, 70, 50 };
    int n = sizeof(values) / sizeof(values[0]);

    MaxHeap heap;

    cout << "Insert\tSwaps\tHeap after insertion" << endl;
    for (int i = 0; i < n; i++) {
        heap.insert(values[i]);
    }

    cout << "\nFinal max heap: ";
    heap.printArray();

    // Perform heap sort (ascending)
    heap.heapSort();

    cout << "Sorted output: ";
    heap.printArray();

    return 0;
}