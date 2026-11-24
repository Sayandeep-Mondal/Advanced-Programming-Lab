#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;
class MinHeap {
private:
vector<int> heap;
long long comparisons = 0;
long long swaps = 0;
void heapifyDown(int i) {
int n = heap.size();
while (true) {
int smallest = i;
int left = 2 * i + 1;
int right = 2 * i + 2;
if (left < n) {
comparisons++;
if (heap[left] < heap[smallest])
smallest = left;
}
if (right < n) {
comparisons++;
if (heap[right] < heap[smallest])
smallest = right;
}
if (smallest == i)
break;
swap(heap[i], heap[smallest]);
swaps++;
i = smallest;
}
}
void heapifyUp(int i) {
while (i > 0) {
int parent = (i - 1) / 2;
comparisons++;
if (heap[i] < heap[parent]) {

swap(heap[i], heap[parent]);
swaps++;
i = parent;
} else {
break;
}
}
}
public:
// Bottom-up construction: O(N)
MinHeap(const vector<int>& arr) {
heap = arr;
for (int i = (int)heap.size() / 2 - 1; i >= 0; i--)
heapifyDown(i);
}
MinHeap() {}
// Reset performance counters
void resetCounters() {
comparisons = 0;
swaps = 0;
}
long long getComparisons() const {
return comparisons;
}
long long getSwaps() const {
return swaps;
}
// Insert: O(log N)
void insert(int value) {
heap.push_back(value);
heapifyUp(heap.size() - 1);
}
// Find minimum: O(1)
int findMin() const {
if (heap.empty()) {
throw runtime_error("Heap is empty!");

}
return heap[0];
}
// Extract minimum: O(log N)
int extractMin() {
if (heap.empty()) {
throw runtime_error("Heap is empty!");
}
int minimum = heap[0];
if (heap.size() == 1) {
heap.pop_back();
return minimum;
}
heap[0] = heap.back();
heap.pop_back();
heapifyDown(0);
return minimum;
}
// Search: O(N)
int search(int value) {
for (int i = 0; i < (int)heap.size(); i++) {
comparisons++;
if (heap[i] == value)
return i;
}
return -1;
}
// Arbitrary deletion: O(log N) after search
bool deleteElement(int value) {
int index = search(value);
if (index == -1)
return false;

int last = heap.size() - 1;
if (index == last) {
heap.pop_back();
return true;
}
heap[index] = heap[last];
heap.pop_back();
if (index > 0 &&
heap[index] < heap[(index - 1) / 2]) {
heapifyUp(index);
} else {
heapifyDown(index);
}
return true;
}
// Decrease key using the element's old value
bool decreaseKey(int oldValue, int newValue) {
int index = search(oldValue);
if (index == -1 || newValue > oldValue)
return false;
heap[index] = newValue;
heapifyUp(index);
return true;
}
// Display heap
void display() const {
cout << "Heap: ";
for (int x : heap)
cout << x << " ";
cout << endl;
}
int size() const {
return heap.size();

}
};
// Kth smallest element using Min Heap
int kthSmallest(const vector<int>& arr, int k) {
if (k < 1 || k > (int)arr.size())
throw invalid_argument("Invalid value of K");
MinHeap h(arr);
int result = -1;
for (int i = 0; i < k; i++)
result = h.extractMin();
return result;
}
// Top-down heap construction
MinHeap buildTopDown(const vector<int>& arr) {
MinHeap h;
for (int x : arr)
h.insert(x);
return h;
}
// Generate input datasets
vector<int> generateInput(int n, string type) {
vector<int> arr(n);
if (type == "Random") {
mt19937 rng(42 + n);
uniform_int_distribution<int> dist(1, n * 10);
// Generate distinct integers
for (int i = 0; i < n; i++)
arr[i] = i + 1;
shuffle(arr.begin(), arr.end(), rng);
}
else if (type == "Ascending") {
for (int i = 0; i < n; i++)
arr[i] = i + 1;

}
else if (type == "Descending") {
for (int i = 0; i < n; i++)
arr[i] = n - i;
}
return arr;
}
// Benchmark function
void benchmark(ofstream& file, const vector<int>& arr,
string inputType) {
int n = arr.size();
cout << "\nTesting N = " << n
<< " | Type = " << inputType << endl;
// Bottom-up construction
MinHeap bottomHeap;
auto start = high_resolution_clock::now();
bottomHeap = MinHeap(arr);
auto stop = high_resolution_clock::now();
double bottomTime =
duration<double, milli>(stop - start).count();
file << n << "," << inputType
<< ",Bottom-Up," << bottomTime << ","
<< bottomHeap.getComparisons() << ","
<< bottomHeap.getSwaps() << "\n";
// Top-down construction
MinHeap topHeap;
start = high_resolution_clock::now();
topHeap = buildTopDown(arr);
stop = high_resolution_clock::now();
double topTime =
duration<double, milli>(stop - start).count();

file << n << "," << inputType
<< ",Top-Down," << topTime << ","
<< topHeap.getComparisons() << ","
<< topHeap.getSwaps() << "\n";
// Insertion of N elements
MinHeap insertHeap;
start = high_resolution_clock::now();
for (int x : arr)
insertHeap.insert(x);
stop = high_resolution_clock::now();
double insertTime =
duration<double, milli>(stop - start).count();
file << n << "," << inputType
<< ",Insertion," << insertTime << ","
<< insertHeap.getComparisons() << ","
<< insertHeap.getSwaps() << "\n";
// Repeated Extract-Min
MinHeap extractHeap(arr);
extractHeap.resetCounters();
start = high_resolution_clock::now();
while (extractHeap.size() > 0)
extractHeap.extractMin();
stop = high_resolution_clock::now();
double extractTime =
duration<double, milli>(stop - start).count();
file << n << "," << inputType
<< ",Extract-Min," << extractTime << ","
<< extractHeap.getComparisons() << ","
<< extractHeap.getSwaps() << "\n";
// Search existing element
MinHeap searchHeap(arr);

searchHeap.resetCounters();
int existing = arr[n / 2];
start = high_resolution_clock::now();
searchHeap.search(existing);
stop = high_resolution_clock::now();
double searchTime =
duration<double, nano>(stop - start).count();
file << n << "," << inputType
<< ",Search-Existing," << searchTime << ","
<< searchHeap.getComparisons() << ","
<< searchHeap.getSwaps() << "\n";
// Search non-existing element
searchHeap.resetCounters();
start = high_resolution_clock::now();
searchHeap.search(-1);
stop = high_resolution_clock::now();
double unsuccessfulTime =
duration<double, nano>(stop - start).count();
file << n << "," << inputType
<< ",Search-Non-Existing," << unsuccessfulTime << ","
<< searchHeap.getComparisons() << ","
<< searchHeap.getSwaps() << "\n";
}
// Main function
int main() {
cout << "===== BINARY MIN HEAP =====\n";
vector<int> arr = {40, 10, 30, 50, 20};
MinHeap h(arr);

cout << "\nInitial Min Heap:\n";
h.display();
cout << "\nInsert 5:\n";
h.insert(5);
h.display();
cout << "\nFind Minimum:\n";
cout << h.findMin() << endl;
cout << "\nExtract Minimum:\n";
cout << h.extractMin() << endl;
h.display();
cout << "\nDelete element 30:\n";
if (h.deleteElement(30))
cout << "Element deleted.\n";
else
cout << "Element not found.\n";
h.display();
cout << "\nDecrease key 40 to 8:\n";
if (h.decreaseKey(40, 8))
cout << "Key decreased.\n";
else
cout << "Operation failed.\n";
h.display();
cout << "\nKth Smallest Element:\n";
vector<int> test = {40, 10, 30, 50, 20};
int k = 3;
cout << k << "th smallest element = "
<< kthSmallest(test, k) << endl;
// Performance experiments
int sizes[] = {1000, 5000, 10000, 50000, 100000};
string types[] = {

"Random",
"Ascending",
"Descending"
};
ofstream file("heap_results.csv");
file << "N,Input_Type,Operation,Time,Comparisons,Swaps\n";
for (string type : types) {
for (int n : sizes) {
vector<int> data = generateInput(n, type);
benchmark(file, data, type);
}
}
file.close();
cout << "\n====================================\n";
cout << "Experiments completed successfully!\n";
cout << "Results saved in heap_results.csv\n";
cout << "====================================\n";
return 0;
}
