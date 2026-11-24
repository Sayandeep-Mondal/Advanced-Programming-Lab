#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>
#include <climits>
using namespace std;
using namespace chrono;

// ======================================================
// METRICS
// ======================================================
struct Metrics {
long long comparisons = 0;
long long links = 0;
long long swaps = 0;
void reset() {
comparisons = 0;
links = 0;
swaps = 0;
}
};
// ======================================================
// BINOMIAL MIN HEAP
// Left-child, right-sibling representation
// ======================================================
struct Node {
int key;
int degree;
Node* parent;
Node* child;
Node* sibling;
Node(int value) {
key = value;
degree = 0;
parent = nullptr;
child = nullptr;
sibling = nullptr;
}
};
class BinomialHeap {
private:
Node* head = nullptr;
Node* mergeRoots(Node* a, Node* b) {
if (!a) return b;
if (!b) return a;
Node dummy(0);
Node* tail = &dummy;
while (a && b) {
if (a->degree <= b->degree) {

tail->sibling = a;
a = a->sibling;
} else {
tail->sibling = b;
b = b->sibling;
}
tail = tail->sibling;
}
tail->sibling = a ? a : b;
return dummy.sibling;
}
void linkTrees(Node* child, Node* parent) {
child->parent = parent;
child->sibling = parent->child;
parent->child = child;
parent->degree++;
metrics.links++;
}
void consolidate() {
if (!head) return;
Node* prev = nullptr;
Node* curr = head;
Node* next = curr->sibling;
while (next) {
if (curr->degree != next->degree ||
(next->sibling &&
next->sibling->degree == curr->degree)) {
prev = curr;
curr = next;
}
else {
metrics.comparisons++;
if (curr->key <= next->key) {
curr->sibling = next->sibling;
linkTrees(next, curr);
}
else {
if (prev)

prev->sibling = next;
else
head = next;
linkTrees(curr, next);
curr = next;
}
}
next = curr->sibling;
}
}
Node* findNode(Node* root, int key) {
while (root) {
metrics.comparisons++;
if (root->key == key)
return root;
Node* found = findNode(root->child, key);
if (found)
return found;
root = root->sibling;
}
return nullptr;
}
void destroy(Node* node) {
while (node) {
Node* next = node->sibling;
destroy(node->child);
delete node;
node = next;
}
}
public:
Metrics metrics;
BinomialHeap() = default;
BinomialHeap(const BinomialHeap&) = delete;
BinomialHeap& operator=(const BinomialHeap&) = delete;

BinomialHeap(BinomialHeap&& other) noexcept {
head = other.head;
other.head = nullptr;
metrics = other.metrics;
}
BinomialHeap& operator=(BinomialHeap&& other) noexcept {
if (this != &other) {
destroy(head);
head = other.head;
other.head = nullptr;
metrics = other.metrics;
}
return *this;
}
~BinomialHeap() {
destroy(head);
}
void insert(int key) {
BinomialHeap temp;
temp.head = new Node(key);
unite(move(temp));
}
void unite(BinomialHeap&& other) {
head = mergeRoots(head, other.head);
other.head = nullptr;
consolidate();
}
int findMin() {
if (!head)
return INT_MAX;
Node* current = head;
int minimum = current->key;
while (current) {
metrics.comparisons++;
if (current->key < minimum)
minimum = current->key;

current = current->sibling;
}
return minimum;
}
int extractMin() {
if (!head)
return INT_MAX;
Node* minNode = head;
Node* minPrev = nullptr;
Node* prev = nullptr;
Node* curr = head;
while (curr) {
metrics.comparisons++;
if (curr->key < minNode->key) {
minNode = curr;
minPrev = prev;
}
prev = curr;
curr = curr->sibling;
}
if (minPrev)
minPrev->sibling = minNode->sibling;
else
head = minNode->sibling;
Node* children = minNode->child;
Node* reversed = nullptr;
while (children) {
Node* next = children->sibling;
children->parent = nullptr;
children->sibling = reversed;
reversed = children;
children = next;
}
int answer = minNode->key;
delete minNode;

BinomialHeap temp;
temp.head = reversed;
unite(move(temp));
return answer;
}
bool search(int key) {
return findNode(head, key) != nullptr;
}
bool decreaseKey(int oldKey, int newKey) {
if (newKey > oldKey)
return false;
Node* node = findNode(head, oldKey);
if (!node)
return false;
node->key = newKey;
while (node->parent) {
metrics.comparisons++;
if (node->key >= node->parent->key)
break;
swap(node->key, node->parent->key);
metrics.swaps++;
node = node->parent;
}
return true;
}
bool deleteKey(int key) {
Node* node = findNode(head, key);
if (!node)
return false;
while (node->parent) {
swap(node->key, node->parent->key);
metrics.swaps++;
node = node->parent;

}
extractMin();
return true;
}
void displayTree(Node* root, int depth = 0) {
while (root) {
for (int i = 0; i < depth; i++)
cout << " ";
cout << root->key << "\n";
displayTree(root->child, depth + 1);
root = root->sibling;
}
}
void display() {
Node* root = head;
while (root) {
cout << "Degree B" << root->degree
<< " | Root: " << root->key << "\n";
displayTree(root);
root = root->sibling;
}
}
void resetMetrics() {
metrics.reset();
}
};
// ======================================================
// BINARY MIN HEAP FOR COMPARISON
// ======================================================
class BinaryHeap {
private:
vector<int> heap;
void heapifyDown(int i) {
int n = (int)heap.size();
while (true) {

int smallest = i;
int left = 2 * i + 1;
int right = 2 * i + 2;
if (left < n) {
metrics.comparisons++;
if (heap[left] < heap[smallest])
smallest = left;
}
if (right < n) {
metrics.comparisons++;
if (heap[right] < heap[smallest])
smallest = right;
}
if (smallest == i)
break;
swap(heap[i], heap[smallest]);
metrics.swaps++;
i = smallest;
}
}
public:
Metrics metrics;
BinaryHeap() = default;
explicit BinaryHeap(const vector<int>& values) {
build(values);
}
void build(const vector<int>& values) {
heap = values;
for (int i = (int)heap.size() / 2 - 1; i >= 0; i--)
heapifyDown(i);
}
void unite(const BinaryHeap& other) {
heap.insert(
heap.end(),
other.heap.begin(),

other.heap.end()
);
for (int i = (int)heap.size() / 2 - 1; i >= 0; i--)
heapifyDown(i);
}
int extractMin() {
if (heap.empty())
return INT_MAX;
int answer = heap[0];
heap[0] = heap.back();
heap.pop_back();
if (!heap.empty())
heapifyDown(0);
return answer;
}
bool search(int key) {
for (int value : heap) {
metrics.comparisons++;
if (value == key)
return true;
}
return false;
}
void resetMetrics() {
metrics.reset();
}
};
// ======================================================
// INPUT GENERATION
// ======================================================
vector<int> generateInput(int n, string type, int seed) {
vector<int> values(n);
for (int i = 0; i < n; i++)
values[i] = i + 1;

if (type == "Random") {
mt19937 rng(seed);
shuffle(values.begin(), values.end(), rng);
}
else if (type == "Descending") {
reverse(values.begin(), values.end());
}
return values;
}
// ======================================================
// BUILD BINOMIAL HEAP BY REPEATED INSERTION
// ======================================================
BinomialHeap buildRepeated(const vector<int>& values) {
BinomialHeap heap;
for (int x : values)
heap.insert(x);
return heap;
}
// ======================================================
// BUILD USING ONE-NODE HEAPS AND PAIRWISE UNION
// ======================================================
BinomialHeap buildPairwise(const vector<int>& values) {
vector<BinomialHeap> heaps;
heaps.reserve(values.size());
for (int x : values) {
BinomialHeap temp;
temp.insert(x);
temp.resetMetrics();
heaps.push_back(move(temp));
}
while (heaps.size() > 1) {
vector<BinomialHeap> nextRound;
nextRound.reserve((heaps.size() + 1) / 2);
for (size_t i = 0; i < heaps.size(); i += 2) {
if (i + 1 < heaps.size()) {
heaps[i].unite(move(heaps[i + 1]));

nextRound.push_back(move(heaps[i]));
} else {
nextRound.push_back(move(heaps[i]));
}
}
heaps = move(nextRound);
}
if (heaps.empty())
return BinomialHeap();
return move(heaps[0]);
}
// ======================================================
// BENCHMARK RESULT
// ======================================================
struct Result {
int N;
string inputType;
string operation;
double timeMs;
double comparisons;
double links;
double swaps;
};
vector<Result> results;
// ======================================================
// SAVE ONE RESULT
// ======================================================
void addResult(
int n,
const string& type,
const string& operation,
double timeMs,
double comparisons,
double links,
double swaps
) {
results.push_back({
n, type, operation,
timeMs, comparisons, links, swaps
});

}
// ======================================================
// RUN ONE FULL EXPERIMENT
// ======================================================
void runExperiments() {
const int sizes[] = {
1000, 5000, 10000, 50000, 100000
};
const string types[] = {
"Random", "Ascending", "Descending"
};
const int repetitions = 3;
results.clear();
cout << "\nStarting performance experiments...\n";
cout << "This may take some time.\n\n";
for (int n : sizes) {
for (const string& type : types) {
cout << "Testing N = " << n
<< " | Input = " << type << "\n";
double insertTime = 0;
double insertComp = 0;
double insertLinks = 0;
double pairTime = 0;
double pairComp = 0;
double pairLinks = 0;
double extractTime = 0;
double extractComp = 0;
double extractLinks = 0;
double binUnionTime = 0;
double binUnionComp = 0;
double binUnionLinks = 0;
double binaryUnionTime = 0;
double binaryUnionComp = 0;
double binaryUnionSwaps = 0;
double searchYesTime = 0;

double searchYesComp = 0;
double searchNoTime = 0;
double searchNoComp = 0;
for (int rep = 0; rep < repetitions; rep++) {
vector<int> values =
generateInput(n, type, 1234 + rep);
// --------------------------------------
// 1. REPEATED INSERTION
// --------------------------------------
{
BinomialHeap heap;
auto start = high_resolution_clock::now();
for (int x : values)
heap.insert(x);
auto stop = high_resolution_clock::now();
insertTime += duration<double, milli>(
stop - start
).count();
insertComp += heap.metrics.comparisons;
insertLinks += heap.metrics.links;
}
// --------------------------------------
// 2. PAIRWISE CONSTRUCTION
// --------------------------------------
{
auto start = high_resolution_clock::now();
BinomialHeap heap = buildPairwise(values);
auto stop = high_resolution_clock::now();
pairTime += duration<double, milli>(
stop - start
).count();
pairComp += heap.metrics.comparisons;

pairLinks += heap.metrics.links;
}
// --------------------------------------
// 3. REPEATED EXTRACT-MIN
// Build heap before timing.
// --------------------------------------
{
BinomialHeap heap = buildRepeated(values);
heap.resetMetrics();
auto start = high_resolution_clock::now();
while (heap.findMin() != INT_MAX)
heap.extractMin();
auto stop = high_resolution_clock::now();
extractTime += duration<double, milli>(
stop - start
).count();
extractComp += heap.metrics.comparisons;
extractLinks += heap.metrics.links;
}
// --------------------------------------
// 4. BINOMIAL HEAP UNION
// Build two heaps before timing.
// --------------------------------------
{
vector<int> left(
values.begin(),
values.begin() + n / 2
);
vector<int> right(
values.begin() + n / 2,
values.end()
);
BinomialHeap h1 = buildRepeated(left);
BinomialHeap h2 = buildRepeated(right);
h1.resetMetrics();

h2.resetMetrics();
auto start = high_resolution_clock::now();
h1.unite(move(h2));
auto stop = high_resolution_clock::now();
binUnionTime += duration<double, milli>(
stop - start
).count();
binUnionComp += h1.metrics.comparisons;
binUnionLinks += h1.metrics.links;
}
// --------------------------------------
// 5. BINARY HEAP UNION
// Build two heaps before timing.
// --------------------------------------
{
vector<int> left(
values.begin(),
values.begin() + n / 2
);
vector<int> right(
values.begin() + n / 2,
values.end()
);
BinaryHeap h1(left);
BinaryHeap h2(right);
h1.resetMetrics();
h2.resetMetrics();
auto start = high_resolution_clock::now();
h1.unite(h2);
auto stop = high_resolution_clock::now();
binaryUnionTime += duration<double, milli>(
stop - start
).count();

binaryUnionComp += h1.metrics.comparisons;
binaryUnionSwaps += h1.metrics.swaps;
}
// --------------------------------------
// 6. SEARCH EXISTING ELEMENT
// --------------------------------------
{
BinomialHeap heap = buildRepeated(values);
heap.resetMetrics();
int existing = values[n / 2];
auto start = high_resolution_clock::now();
heap.search(existing);
auto stop = high_resolution_clock::now();
searchYesTime += duration<double, milli>(
stop - start
).count();
searchYesComp += heap.metrics.comparisons;
}
// --------------------------------------
// 7. SEARCH NON-EXISTING ELEMENT
// --------------------------------------
{
BinomialHeap heap = buildRepeated(values);
heap.resetMetrics();
int missing = n + 1000000;
auto start = high_resolution_clock::now();
heap.search(missing);
auto stop = high_resolution_clock::now();
searchNoTime += duration<double, milli>(
stop - start
).count();
searchNoComp += heap.metrics.comparisons;

}
}
// ------------------------------------------
// STORE AVERAGE RESULTS
// ------------------------------------------
double r = repetitions;
addResult(n, type, "Repeated Insertion",
insertTime / r, insertComp / r,
insertLinks / r, 0);
addResult(n, type, "Pairwise Construction",
pairTime / r, pairComp / r,
pairLinks / r, 0);
addResult(n, type, "Repeated Extract-Min",
extractTime / r, extractComp / r,
extractLinks / r, 0);
addResult(n, type, "Binomial Heap Union",
binUnionTime / r, binUnionComp / r,
binUnionLinks / r, 0);
addResult(n, type, "Binary Heap Union",
binaryUnionTime / r, binaryUnionComp / r,
0, binaryUnionSwaps / r);
addResult(n, type, "Search Existing",
searchYesTime / r, searchYesComp / r,
0, 0);
addResult(n, type, "Search Non-Existing",
searchNoTime / r, searchNoComp / r,
0, 0);
}
}
cout << "\nExperiments completed.\n";
}
// ======================================================
// EXPORT CSV FILES
// ======================================================
void exportCSV() {

ofstream mainFile("binomial_results.csv");
ofstream timeFile("execution_time.csv");
ofstream comparisonFile("comparisons.csv");
ofstream linkSwapFile("links_swaps.csv");
if (!mainFile || !timeFile ||
!comparisonFile || !linkSwapFile) {
cout << "Error: Could not create CSV files.\n";
return;
}
mainFile << "N,Input_Type,Operation,Time_ms,"
<< "Comparisons,Links,Swaps\n";
timeFile << "N,Input_Type,Operation,Time_ms\n";
comparisonFile << "N,Input_Type,Operation,Comparisons\n";
linkSwapFile << "N,Input_Type,Operation,Links,Swaps\n";
mainFile << fixed << setprecision(6);
timeFile << fixed << setprecision(6);
comparisonFile << fixed << setprecision(2);
linkSwapFile << fixed << setprecision(2);
for (const Result& r : results) {
mainFile
<< r.N << ","
<< r.inputType << ","
<< r.operation << ","
<< r.timeMs << ","
<< r.comparisons << ","
<< r.links << ","
<< r.swaps << "\n";
timeFile
<< r.N << ","
<< r.inputType << ","
<< r.operation << ","
<< r.timeMs << "\n";
comparisonFile
<< r.N << ","
<< r.inputType << ","
<< r.operation << ","

<< r.comparisons << "\n";
linkSwapFile
<< r.N << ","
<< r.inputType << ","
<< r.operation << ","
<< r.links << ","
<< r.swaps << "\n";
}
mainFile.close();
timeFile.close();
comparisonFile.close();
linkSwapFile.close();
cout << "\nCSV files exported successfully!\n";
cout << "1. binomial_results.csv\n";
cout << "2. execution_time.csv\n";
cout << "3. comparisons.csv\n";
cout << "4. links_swaps.csv\n";
}
// ======================================================
// MAIN PROGRAM
// ======================================================
int main() {
cout << "=====================================\n";
cout << " BINOMIAL MIN HEAP PERFORMANCE TEST\n";
cout << "=====================================\n";
runExperiments();
exportCSV();
cout << "\nAll experiments and CSV exports finished.\n";
return 0;
}
