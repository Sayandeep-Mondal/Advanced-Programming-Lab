#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <climits>
#include <chrono>
#include <random>
#include <iomanip>
#include <cmath>
using namespace std;
using namespace chrono;
// Structure to store minimum and maximum
struct Node {
int minimum;
int maximum;
};
// Segment Tree Class
class SegmentTree {
private:
vector<Node> tree;
int n;
// Merge two child nodes
Node merge(Node left, Node right) {
Node result;

result.minimum = min(left.minimum, right.minimum);
result.maximum = max(left.maximum, right.maximum);
return result;
}
// Build Segment Tree
void build(int node, int start, int end,
const vector<int>& arr) {
if (start == end) {
tree[node].minimum = arr[start];
tree[node].maximum = arr[start];
return;
}
int mid = (start + end) / 2;
build(2 * node, start, mid, arr);
build(2 * node + 1, mid + 1, end, arr);
tree[node] = merge(tree[2 * node],
tree[2 * node + 1]);
}
// Range query
Node query(int node, int start, int end,
int L, int R) {
// No overlap
if (R < start || L > end) {
return {INT_MAX, INT_MIN};
}
// Complete overlap
if (L <= start && end <= R) {
return tree[node];
}
int mid = (start + end) / 2;
Node left = query(2 * node, start, mid, L, R);
Node right = query(2 * node + 1, mid + 1, end, L, R);

return merge(left, right);
}
// Point update
void update(int node, int start, int end,
int index, int value) {
if (start == end) {
tree[node].minimum = value;
tree[node].maximum = value;
return;
}
int mid = (start + end) / 2;
if (index <= mid) {
update(2 * node, start, mid, index, value);
} else {
update(2 * node + 1, mid + 1, end, index, value);
}
tree[node] = merge(tree[2 * node],
tree[2 * node + 1]);
}
public:
SegmentTree(const vector<int>& arr) {
n = arr.size();
tree.resize(4 * n);
build(1, 0, n - 1, arr);
}
Node query(int L, int R) {
return query(1, 0, n - 1, L, R);
}
void update(int index, int value) {
update(1, 0, n - 1, index, value);
}
// Display internal tree representation
void displayTree() {
cout << "\nSegment Tree Representation:\n";

for (int i = 1; i < 4 * n; i++) {
if (tree[i].minimum != 0 ||
tree[i].maximum != 0) {
cout << "Node " << i
<< " -> Min = " << tree[i].minimum
<< ", Max = " << tree[i].maximum
<< endl;
}
}
}
};
// Linear search approach
Node linearQuery(const vector<int>& arr, int L, int R) {
Node result = {INT_MAX, INT_MIN};
for (int i = L; i <= R; i++) {
result.minimum = min(result.minimum, arr[i]);
result.maximum = max(result.maximum, arr[i]);
}
return result;
}
// Calculate tree height
int treeHeight(int n) {
return ceil(log2(n)) + 1;
}
// Generate random array
vector<int> generateRandom(int n) {
vector<int> arr(n);
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist(1, 100000);
for (int i = 0; i < n; i++) {
arr[i] = dist(gen);
}

return arr;
}
// Generate sorted array
vector<int> generateSorted(int n) {
vector<int> arr(n);
for (int i = 0; i < n; i++) {
arr[i] = i + 1;
}
return arr;
}
// Generate reverse sorted array
vector<int> generateReverse(int n) {
vector<int> arr(n);
for (int i = 0; i < n; i++) {
arr[i] = n - i;
}
return arr;
}
// Benchmark experiments
void runExperiments() {
int sizes[] = {100, 500, 1000, 5000, 10000, 50000};
ofstream file("experimental_results.csv");
file << "Type,N,BuildTime_us,SegmentQuery_us,"
<< "LinearQuery_us,UpdateTime_us,TreeHeight\n";
string types[] = {"Random", "Sorted", "Reverse"};
for (string type : types) {
for (int n : sizes) {
vector<int> arr;

if (type == "Random")
arr = generateRandom(n);
else if (type == "Sorted")
arr = generateSorted(n);
else
arr = generateReverse(n);
// Construction time
auto start = high_resolution_clock::now();
SegmentTree st(arr);
auto stop = high_resolution_clock::now();
double buildTime =
duration<double, micro>(stop - start).count();
// Prepare query ranges
mt19937 gen(42);
uniform_int_distribution<int> dist(0, n - 1);
const int repetitions = 1000;
double segmentQueryTime = 0;
double linearQueryTime = 0;
volatile long long checksum = 0;
// Segment Tree query benchmark
start = high_resolution_clock::now();
for (int i = 0; i < repetitions; i++) {
int L = dist(gen);
int R = dist(gen);
if (L > R)
swap(L, R);
Node result = st.query(L, R);
checksum += result.minimum;
checksum += result.maximum;

}
stop = high_resolution_clock::now();
segmentQueryTime =
duration<double, micro>(stop - start).count()
/ repetitions;
// Linear search benchmark
start = high_resolution_clock::now();
for (int i = 0; i < repetitions; i++) {
int L = dist(gen);
int R = dist(gen);
if (L > R)
swap(L, R);
Node result = linearQuery(arr, L, R);
checksum += result.minimum;
checksum += result.maximum;
}
stop = high_resolution_clock::now();
linearQueryTime =
duration<double, micro>(stop - start).count()
/ repetitions;
// Point update benchmark
start = high_resolution_clock::now();
for (int i = 0; i < repetitions; i++) {
int index = dist(gen);
st.update(index, i);
}
stop = high_resolution_clock::now();
double updateTime =
duration<double, micro>(stop - start).count()
/ repetitions;

int height = treeHeight(n);
file << type << ","
<< n << ","
<< fixed << setprecision(4)
<< buildTime << ","
<< segmentQueryTime << ","
<< linearQueryTime << ","
<< updateTime << ","
<< height << "\n";
cout << type << " | N = " << n
<< " | Build = " << buildTime << " us"
<< " | Segment Query = " << segmentQueryTime << " us"
<< " | Linear Query = " << linearQueryTime << " us"
<< " | Update = " << updateTime << " us"
<< " | Height = " << height << endl;
(void)checksum;
}
}
file.close();
cout << "\nExperimental results saved to "
<< "experimental_results.csv\n";
}
// Main function
int main() {
ifstream input("input.txt");
if (!input) {
cout << "Error: input.txt file not found!\n";
return 1;
}
vector<int> arr;
int value;
while (input >> value) {
arr.push_back(value);
}

input.close();
if (arr.empty()) {
cout << "Input file is empty!\n";
return 1;
}
int n = arr.size();
cout << "Original Array:\n";
for (int x : arr) {
cout << x << " ";
}
cout << "\n";
// Construct Segment Tree
SegmentTree st(arr);
st.displayTree();
// Read range queries
int q;
cout << "\nEnter number of range queries: ";
cin >> q;
for (int i = 0; i < q; i++) {
int L, R;
cout << "\nEnter L and R: ";
cin >> L >> R;
if (L < 0 || R >= n || L > R) {
cout << "Invalid range!\n";
i--;
continue;
}
Node result = st.query(L, R);
cout << "\nQuery [" << L << ", " << R << "]\n";

cout << "Minimum = " << result.minimum << endl;
cout << "Maximum = " << result.maximum << endl;
}
// Point update
char choice;
cout << "\nDo you want to perform point update? (y/n): ";
cin >> choice;
if (choice == 'y' || choice == 'Y') {
int index, newValue;
cout << "Enter index: ";
cin >> index;
cout << "Enter new value: ";
cin >> newValue;
if (index < 0 || index >= n) {
cout << "Invalid index!\n";
} else {
st.update(index, newValue);
arr[index] = newValue;
cout << "\nArray after update:\n";
for (int x : arr) {
cout << x << " ";
}
Node result = st.query(0, n - 1);
cout << "\n\nAfter Update:\n";
cout << "Minimum = " << result.minimum << endl;
cout << "Maximum = " << result.maximum << endl;
}
}
// Run experimental analysis
cout << "\nRunning Experimental Analysis...\n";
runExperiments();

cout << "\nProgram completed successfully!\n";
return 0;
}
