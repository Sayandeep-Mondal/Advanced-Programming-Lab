#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <queue>
#include <climits>
using namespace std;
using namespace chrono;

mt19937 rng(42);
// =====================================================
// RED BLACK TREE
// =====================================================
class RedBlackTree {
private:
enum Color { RED, BLACK };
struct Node {
int key;
Color color;
Node *left, *right, *parent;
Node(int value) {
key = value;
color = RED;
left = right = parent = nullptr;
}
};
Node* NIL;
Node* root;
void leftRotate(Node* x) {
Node* y = x->right;
x->right = y->left;
if (y->left != NIL)
y->left->parent = x;
y->parent = x->parent;
if (x->parent == NIL)
root = y;
else if (x == x->parent->left)
x->parent->left = y;
else
x->parent->right = y;
y->left = x;

x->parent = y;
}
void rightRotate(Node* y) {
Node* x = y->left;
y->left = x->right;
if (x->right != NIL)
x->right->parent = y;
x->parent = y->parent;
if (y->parent == NIL)
root = x;
else if (y == y->parent->right)
y->parent->right = x;
else
y->parent->left = x;
x->right = y;
y->parent = x;
}
void insertFix(Node* z) {
while (z->parent->color == RED) {
if (z->parent == z->parent->parent->left) {
Node* uncle = z->parent->parent->right;
// Case 1: Uncle is RED
if (uncle->color == RED) {
z->parent->color = BLACK;
uncle->color = BLACK;
z->parent->parent->color = RED;
z = z->parent->parent;
}
else {
// Case 2: Left-Right
if (z == z->parent->right) {

z = z->parent;
leftRotate(z);
}
// Case 3: Left-Left
z->parent->color = BLACK;
z->parent->parent->color = RED;
rightRotate(z->parent->parent);
}
}
else {
Node* uncle = z->parent->parent->left;
if (uncle->color == RED) {
z->parent->color = BLACK;
uncle->color = BLACK;
z->parent->parent->color = RED;
z = z->parent->parent;
}
else {
// Right-Left
if (z == z->parent->left) {
z = z->parent;
rightRotate(z);
}
// Right-Right
z->parent->color = BLACK;
z->parent->parent->color = RED;
leftRotate(z->parent->parent);
}
}
}
root->color = BLACK;
}
Node* minimum(Node* node) {

while (node->left != NIL)
node = node->left;
return node;
}
void transplant(Node* u, Node* v) {
if (u->parent == NIL)
root = v;
else if (u == u->parent->left)
u->parent->left = v;
else
u->parent->right = v;
v->parent = u->parent;
}
void deleteFix(Node* x) {
while (x != root && x->color == BLACK) {
if (x == x->parent->left) {
Node* w = x->parent->right;
// Case 1: Sibling is RED
if (w->color == RED) {
w->color = BLACK;
x->parent->color = RED;
leftRotate(x->parent);
w = x->parent->right;
}
// Case 2: Both sibling children are BLACK
if (w->left->color == BLACK &&
w->right->color == BLACK) {
w->color = RED;
x = x->parent;
}
else {

// Case 3
if (w->right->color == BLACK) {
w->left->color = BLACK;
w->color = RED;
rightRotate(w);
w = x->parent->right;
}
// Case 4
w->color = x->parent->color;
x->parent->color = BLACK;
w->right->color = BLACK;
leftRotate(x->parent);
x = root;
}
}
else {
Node* w = x->parent->left;
// Case 1
if (w->color == RED) {
w->color = BLACK;
x->parent->color = RED;
rightRotate(x->parent);
w = x->parent->left;
}
// Case 2
if (w->right->color == BLACK &&
w->left->color == BLACK) {
w->color = RED;
x = x->parent;
}
else {
// Case 3
if (w->left->color == BLACK) {

w->right->color = BLACK;
w->color = RED;
leftRotate(w);
w = x->parent->left;
}
// Case 4
w->color = x->parent->color;
x->parent->color = BLACK;
w->left->color = BLACK;
rightRotate(x->parent);
x = root;
}
}
}
x->color = BLACK;
}
void clearTree() {
if (root == NIL)
return;
vector<Node*> nodes;
nodes.push_back(root);
for (size_t i = 0; i < nodes.size(); i++) {
Node* p = nodes[i];
if (p->left != NIL)
nodes.push_back(p->left);
if (p->right != NIL)
nodes.push_back(p->right);
}
for (Node* p : nodes)
delete p;
root = NIL;
}
public:

RedBlackTree() {
NIL = new Node(0);
NIL->color = BLACK;
NIL->left = NIL;
NIL->right = NIL;
NIL->parent = NIL;
root = NIL;
}
~RedBlackTree() {
clearTree();
delete NIL;
}
bool search(int key) const {
Node* current = root;
while (current != NIL) {
if (key == current->key)
return true;
if (key < current->key)
current = current->left;
else
current = current->right;
}
return false;
}
void insert(int key) {
Node* z = new Node(key);
Node* y = NIL;
Node* x = root;
while (x != NIL) {

y = x;
if (key == x->key) {
delete z;
return;
}
if (key < x->key)
x = x->left;
else
x = x->right;
}
z->parent = y;
z->left = NIL;
z->right = NIL;
if (y == NIL)
root = z;
else if (key < y->key)
y->left = z;
else
y->right = z;
insertFix(z);
}
bool deleteKey(int key) {
Node* z = root;
while (z != NIL && z->key != key) {
if (key < z->key)
z = z->left;
else
z = z->right;
}
if (z == NIL)
return false;
Node* y = z;
Node* x;

Color originalColor = y->color;
if (z->left == NIL) {
x = z->right;
transplant(z, z->right);
}
else if (z->right == NIL) {
x = z->left;
transplant(z, z->left);
}
else {
y = minimum(z->right);
originalColor = y->color;
x = y->right;
if (y->parent == z) {
x->parent = y;
}
else {
transplant(y, y->right);
y->right = z->right;
y->right->parent = y;
}
transplant(z, y);
y->left = z->left;
y->left->parent = y;
y->color = z->color;
}
delete z;
if (originalColor == BLACK)
deleteFix(x);
return true;
}

void inorder() const {
vector<Node*> st;
Node* current = root;
while (current != NIL || !st.empty()) {
while (current != NIL) {
st.push_back(current);
current = current->left;
}
current = st.back();
st.pop_back();
cout << current->key << " ";
current = current->right;
}
cout << endl;
}
void writeInorder(ofstream& out) const {
vector<Node*> st;
Node* current = root;
while (current != NIL || !st.empty()) {
while (current != NIL) {
st.push_back(current);
current = current->left;
}
current = st.back();
st.pop_back();
out << current->key << " ";
current = current->right;
}
out << endl;
}

int height() const {
if (root == NIL)
return 0;
queue<pair<Node*, int>> q;
q.push({root, 1});
int h = 0;
while (!q.empty()) {
auto [node, level] = q.front();
q.pop();
h = max(h, level);
if (node->left != NIL)
q.push({node->left, level + 1});
if (node->right != NIL)
q.push({node->right, level + 1});
}
return h;
}
};

// =====================================================
// ORDINARY BINARY SEARCH TREE
// =====================================================
class BST {
private:
struct Node {
int key;
Node* left;
Node* right;
Node(int value) {
key = value;

left = right = nullptr;
}
};
Node* root = nullptr;
public:
~BST() {
clear();
}
void clear() {
if (!root)
return;
vector<Node*> nodes;
nodes.push_back(root);
for (size_t i = 0; i < nodes.size(); i++) {
Node* p = nodes[i];
if (p->left)
nodes.push_back(p->left);
if (p->right)
nodes.push_back(p->right);
}
for (Node* p : nodes)
delete p;
root = nullptr;
}
void insert(int key) {
Node* newNode = new Node(key);
if (!root) {
root = newNode;
return;
}

Node* current = root;
Node* parent = nullptr;
while (current) {
parent = current;
if (key == current->key) {
delete newNode;
return;
}
if (key < current->key)
current = current->left;
else
current = current->right;
}
if (key < parent->key)
parent->left = newNode;
else
parent->right = newNode;
}
bool search(int key) const {
Node* current = root;
while (current) {
if (key == current->key)
return true;
if (key < current->key)
current = current->left;
else
current = current->right;
}
return false;
}
bool deleteKey(int key) {

Node* current = root;
Node* parent = nullptr;
while (current && current->key != key) {
parent = current;
if (key < current->key)
current = current->left;
else
current = current->right;
}
if (!current)
return false;
if (current->left && current->right) {
Node* successorParent = current;
Node* successor = current->right;
while (successor->left) {
successorParent = successor;
successor = successor->left;
}
current->key = successor->key;
current = successor;
parent = successorParent;
}
Node* child = current->left ?
current->left : current->right;
if (!parent)
root = child;
else if (parent->left == current)
parent->left = child;
else
parent->right = child;
delete current;
return true;

}
int height() const {
if (!root)
return 0;
queue<pair<Node*, int>> q;
q.push({root, 1});
int h = 0;
while (!q.empty()) {
auto [node, level] = q.front();
q.pop();
h = max(h, level);
if (node->left)
q.push({node->left, level + 1});
if (node->right)
q.push({node->right, level + 1});
}
return h;
}
};

// =====================================================
// BENCHMARK FUNCTION
// =====================================================
void benchmark() {
vector<int> sizes = {
100, 500, 1000, 5000, 10000, 50000
};
vector<string> patterns = {
"Random", "Sorted", "Reverse"
};

ofstream csv("benchmark.csv");
csv << "Pattern,N,"
<< "RB_Height,BST_Height,"
<< "RB_Insert_us,BST_Insert_us,"
<< "RB_Delete_us,BST_Delete_us,"
<< "RB_Search_ns,BST_Search_ns\n";
cout << "\n========== PERFORMANCE BENCHMARK ==========\n";
for (string pattern : patterns) {
for (int n : sizes) {
vector<int> data(n);
for (int i = 0; i < n; i++)
data[i] = i + 1;
if (pattern == "Random") {
shuffle(data.begin(), data.end(), rng);
}
else if (pattern == "Reverse") {
reverse(data.begin(), data.end());
}
// ---------------------------------
// RED BLACK TREE INSERTION
// ---------------------------------
RedBlackTree rb;
auto start = high_resolution_clock::now();
for (int x : data)
rb.insert(x);
auto end = high_resolution_clock::now();
double rbInsert =
duration<double, micro>(end - start).count();
int rbHeight = rb.height();
// ---------------------------------

// BST INSERTION
// ---------------------------------
BST bst;
start = high_resolution_clock::now();
for (int x : data)
bst.insert(x);
end = high_resolution_clock::now();
double bstInsert =
duration<double, micro>(end - start).count();
int bstHeight = bst.height();
// ---------------------------------
// SEARCH TIME
// ---------------------------------
const int repetitions = 5;
start = high_resolution_clock::now();
for (int r = 0; r < repetitions; r++) {
for (int x : data)
rb.search(x);
}
end = high_resolution_clock::now();
double rbSearch =
duration<double, nano>(end - start).count()
/ (n * repetitions);
start = high_resolution_clock::now();
for (int r = 0; r < repetitions; r++) {
for (int x : data)
bst.search(x);
}
end = high_resolution_clock::now();

double bstSearch =
duration<double, nano>(end - start).count()
/ (n * repetitions);
// ---------------------------------
// DELETION TIME
// ---------------------------------
vector<int> deletionOrder = data;
shuffle(deletionOrder.begin(),
deletionOrder.end(), rng);
RedBlackTree rbDelete;
for (int x : data)
rbDelete.insert(x);
start = high_resolution_clock::now();
for (int x : deletionOrder)
rbDelete.deleteKey(x);
end = high_resolution_clock::now();
double rbDeleteTime =
duration<double, micro>(end - start).count();
BST bstDelete;
for (int x : data)
bstDelete.insert(x);
start = high_resolution_clock::now();
for (int x : deletionOrder)
bstDelete.deleteKey(x);
end = high_resolution_clock::now();
double bstDeleteTime =
duration<double, micro>(end - start).count();
// ---------------------------------
// WRITE CSV RESULTS

// ---------------------------------
csv << pattern << ","
<< n << ","
<< rbHeight << ","
<< bstHeight << ","
<< fixed << setprecision(3)
<< rbInsert << ","
<< bstInsert << ","
<< rbDeleteTime << ","
<< bstDeleteTime << ","
<< rbSearch << ","
<< bstSearch << "\n";
cout << "\nPattern: " << pattern
<< " | Nodes: " << n << endl;
cout << "RB Height: " << rbHeight
<< " | BST Height: " << bstHeight << endl;
cout << "RB Insert: " << rbInsert
<< " us | BST Insert: "
<< bstInsert << " us" << endl;
cout << "RB Delete: " << rbDeleteTime
<< " us | BST Delete: "
<< bstDeleteTime << " us" << endl;
cout << "RB Search: " << rbSearch
<< " ns | BST Search: "
<< bstSearch << " ns" << endl;
}
}
csv.close();
cout << "\nBenchmark completed!";
cout << "\nResults saved in benchmark.csv\n";
}

// =====================================================
// MAIN
// =====================================================

int main() {
ifstream input("input.txt");
if (!input) {
cout << "Error: input.txt not found!\n";
return 1;
}
RedBlackTree rb;
int key;
vector<int> elements;
while (input >> key) {
elements.push_back(key);
}
input.close();
if (elements.empty()) {
cout << "input.txt contains no integers.\n";
return 1;
}
// ---------------------------------
// INSERTION
// ---------------------------------
cout << "========== RED BLACK TREE ==========\n";
for (int x : elements)
rb.insert(x);
cout << "\nInorder Traversal After Insertion:\n";
rb.inorder();
cout << "\nTree Height: " << rb.height() << endl;
// ---------------------------------
// SEARCH
// ---------------------------------
int searchKey;

cout << "\nEnter element to search: ";
cin >> searchKey;
if (rb.search(searchKey))
cout << searchKey << " found in Red-Black Tree.\n";
else
cout << searchKey << " not found.\n";
// ---------------------------------
// DELETION
// ---------------------------------
int deleteKey;
cout << "\nEnter element to delete: ";
cin >> deleteKey;
if (rb.deleteKey(deleteKey))
cout << "Element deleted successfully.\n";
else
cout << "Element not found. Cannot delete.\n";
cout << "\nInorder Traversal After Deletion:\n";
rb.inorder();
cout << "\nTree Height After Deletion: "
<< rb.height() << endl;
// ---------------------------------
// OUTPUT FILE
// ---------------------------------
ofstream output("output.txt");
if (!output) {
cout << "Error creating output.txt!\n";
return 1;
}
output << "Final Red-Black Tree Inorder Traversal:\n";
rb.writeInorder(output);
output.close();

cout << "\nFinal tree saved in output.txt\n";
// ---------------------------------
// PERFORMANCE BENCHMARK
// ---------------------------------
benchmark();
cout << "\n========== PROGRAM COMPLETED ==========\n";
return 0;
}
