// CS250 Lab 10 - Task 2: Hospital ER Wait-Time Monitor
// Student Name : Amina Ahmad
// Roll No: 542601

#include <iostream>
#include <iomanip>
using namespace std;

/*
 * Simple array-based heap that can act as either MaxHeap or MinHeap
 * controlled by the 'isMax' flag.
 * All operations (push / pop / peek) are O(log n).
 */
class Heap {
private:
    int* arr;
    int size;
    int capacity;
    bool isMax;   // true = max-heap, false = min-heap

    void resize() {
        capacity *= 2;
        int* newArr = new int[capacity];
        for (int i = 0; i < size; i++)
            newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

    // Returns true if a should be higher in the heap than b
    bool better(int a, int b) const {
        return isMax ? (a > b) : (a < b);
    }

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (!better(arr[i], arr[parent]))
                break;
            int temp = arr[i];
            arr[i] = arr[parent];
            arr[parent] = temp;
            i = parent;
        }
    }

    void siftDown(int i) {
        while (true) {
            int best = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < size && better(arr[left], arr[best]))
                best = left;
            if (right < size && better(arr[right], arr[best]))
                best = right;

            if (best == i)
                break;

            int temp = arr[i];
            arr[i] = arr[best];
            arr[best] = temp;
            i = best;
        }
    }

public:
    Heap(bool maxHeap, int initialCapacity = 16) {
        isMax = maxHeap;
        capacity = initialCapacity;
        size = 0;
        arr = new int[capacity];
    }

    ~Heap() {
        delete[] arr;
    }

    // Push a value. O(log n)
    void push(int value) {
        if (size == capacity)
            resize();
        arr[size] = value;
        siftUp(size);
        size++;
    }

    // Pop the root. O(log n)
    int pop() {
        if (size == 0)
            return -1; // should never happen in this problem
        int root = arr[0];
        arr[0] = arr[size - 1];
        size--;
        if (size > 0)
            siftDown(0);
        return root;
    }

    // Peek at the root without removing. O(1)
    int peek() const {
        return (size == 0) ? -1 : arr[0];
    }

    int getSize() const { return size; }
    bool empty() const { return size == 0; }
};

/*
 * MedianFinder using two heaps:
 * - lower  : Max-heap (smaller half)
 * - upper  : Min-heap (larger half)
 *
 * Invariant: lower.size() == upper.size()  or  lower.size() == upper.size() + 1
 * Median is therefore O(1).
 * Each addWait is O(log n).
 */
class MedianFinder {
private:
    Heap lower;   // max-heap
    Heap upper;   // min-heap

public:
    MedianFinder() : lower(true), upper(false) {}

    // Insert a new wait time and rebalance. O(log n)
    void addWait(int x) {
        // Decide which heap to insert into
        if (lower.empty() || x <= lower.peek()) {
            lower.push(x);
        }
        else {
            upper.push(x);
        }

        // Rebalance so that sizes differ by at most 1
        // and lower (max-heap) is allowed to have one extra element
        if (lower.getSize() > upper.getSize() + 1) {
            upper.push(lower.pop());
        }
        else if (upper.getSize() > lower.getSize()) {
            lower.push(upper.pop());
        }
    }

    // Return current median. O(1)
    double getMedian() const {
        if (lower.getSize() > upper.getSize()) {
            return static_cast<double>(lower.peek());
        }
        else {
            // equal sizes → average of two roots
            return (static_cast<double>(lower.peek()) + upper.peek()) / 2.0;
        }
    }

    int getLowerRoot() const { return lower.empty() ? -1 : lower.peek(); }
    int getUpperRoot() const { return upper.empty() ? -1 : upper.peek(); }
};

/*
 * Answers to questions (also required in report):
 *
 * 7. Why is the median a better alert measure than the average?
 *    Using arrival 6 (waits: 45,20,60,15,30,90):
 *    Average would be pulled strongly upward by the single extreme value 90,
 *    while the median stays at 37.5 and correctly reflects the “typical” wait.
 *    One extreme case does not distort the figure that the duty manager sees.
 *
 * 8. What goes wrong if the two heap sizes differ by two or more?
 *    The two roots no longer sit at the middle of the ordered data.
 *    Consequently getMedian() returns an incorrect value (not the true median).
 *
 * 9. How many total operations does repeated sorting need compared with the heap method?
 *    Repeated full sort after every arrival: O(n² log n) total.
 *    Two-heap method: O(n log n) total for n patients.
 */

int main() {
    // Sample input
    int waits[] = { 45, 20, 60, 15, 30, 90, 10, 55 };
    int n = sizeof(waits) / sizeof(waits[0]);
    const double ALERT_THRESHOLD = 35.0;

    MedianFinder mf;

    cout << fixed << setprecision(1);
    cout << "Arrival\tWait\tMax heap root\tMin heap root\tMedian\tStatus" << endl;

    for (int i = 0; i < n; i++) {
        mf.addWait(waits[i]);

        double med = mf.getMedian();
        int maxRoot = mf.getLowerRoot();
        int minRoot = mf.getUpperRoot();

        cout << (i + 1) << "\t" << waits[i] << "\t";
        if (maxRoot == -1) cout << "none";
        else               cout << maxRoot;
        cout << "\t\t";
        if (minRoot == -1) cout << "none";
        else               cout << minRoot;
        cout << "\t\t" << med << "\t";

        if (med > ALERT_THRESHOLD)
            cout << "ALERT: call extra staff.";
        else
            cout << "Normal";
        cout << endl;
    }

    return 0;
}