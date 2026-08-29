#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

/* =========================================================
   AVL TREE NODE
   ========================================================= */

struct AVLNode {
    int key;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int value) {
        key = value;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

/* =========================================================
   NORMAL BST NODE
   ========================================================= */

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int value) {
        key = value;
        left = nullptr;
        right = nullptr;
    }
};

/* =========================================================
   AVL TREE CLASS
   ========================================================= */

class AVLTree {
private:

    AVLNode* root;

    int height(AVLNode* node) {
        if (node == nullptr)
            return 0;

        return node->height;
    }

    int getBalance(AVLNode* node) {
        if (node == nullptr)
            return 0;

        return height(node->left) - height(node->right);
    }

    void updateHeight(AVLNode* node) {
        if (node != nullptr) {
            node->height =
                1 + max(height(node->left), height(node->right));
        }
    }

    /* ---------------------------------------------------------
       RIGHT ROTATION - LL CASE
       --------------------------------------------------------- */

    AVLNode* rightRotate(AVLNode* y) {

        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    /* ---------------------------------------------------------
       LEFT ROTATION - RR CASE
       --------------------------------------------------------- */

    AVLNode* leftRotate(AVLNode* x) {

        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    /* ---------------------------------------------------------
       AVL INSERTION
       --------------------------------------------------------- */

    AVLNode* insertNode(AVLNode* node, int key) {

        // Normal BST insertion
        if (node == nullptr)
            return new AVLNode(key);

        if (key < node->key)
            node->left = insertNode(node->left, key);

        else if (key > node->key)
            node->right = insertNode(node->right, key);

        else {
            // Duplicate values are ignored
            return node;
        }

        // Update height
        updateHeight(node);

        // Calculate balance factor
        int balance = getBalance(node);

        /* -----------------------------------------------------
           LL CASE
           ----------------------------------------------------- */

        if (balance > 1 && key < node->left->key) {
            cout << "LL Rotation at node " << node->key << endl;
            return rightRotate(node);
        }

        /* -----------------------------------------------------
           RR CASE
           ----------------------------------------------------- */

        if (balance < -1 && key > node->right->key) {
            cout << "RR Rotation at node " << node->key << endl;
            return leftRotate(node);
        }

        /* -----------------------------------------------------
           LR CASE
           ----------------------------------------------------- */

        if (balance > 1 && key > node->left->key) {
            cout << "LR Rotation at node " << node->key << endl;

            node->left = leftRotate(node->left);

            return rightRotate(node);
        }

        /* -----------------------------------------------------
           RL CASE
           ----------------------------------------------------- */

        if (balance < -1 && key < node->right->key) {
            cout << "RL Rotation at node " << node->key << endl;

            node->right = rightRotate(node->right);

            return leftRotate(node);
        }

        return node;
    }

    /* ---------------------------------------------------------
       FIND MINIMUM NODE
       --------------------------------------------------------- */

    AVLNode* minValueNode(AVLNode* node) {

        AVLNode* current = node;

        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    /* ---------------------------------------------------------
       AVL DELETION
       --------------------------------------------------------- */

    AVLNode* deleteNode(AVLNode* node, int key) {

        if (node == nullptr)
            return node;

        // Search for node
        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        }

        else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        }

        else {

            // Node with one child or no child
            if (node->left == nullptr ||
                node->right == nullptr) {

                AVLNode* child =
                    (node->left != nullptr)
                    ? node->left
                    : node->right;

                // No child
                if (child == nullptr) {

                    delete node;
                    return nullptr;
                }

                // One child
                else {

                    AVLNode* temp = node;
                    node = child;

                    delete temp;
                }
            }

            // Node has two children
            else {

                AVLNode* temp =
                    minValueNode(node->right);

                node->key = temp->key;

                node->right =
                    deleteNode(node->right, temp->key);
            }
        }

        // Update height
        updateHeight(node);

        // Calculate balance
        int balance = getBalance(node);

        /* -----------------------------------------------------
           LL
           ----------------------------------------------------- */

        if (balance > 1 &&
            getBalance(node->left) >= 0) {

            return rightRotate(node);
        }

        /* -----------------------------------------------------
           LR
           ----------------------------------------------------- */

        if (balance > 1 &&
            getBalance(node->left) < 0) {

            node->left =
                leftRotate(node->left);

            return rightRotate(node);
        }

        /* -----------------------------------------------------
           RR
           ----------------------------------------------------- */

        if (balance < -1 &&
            getBalance(node->right) <= 0) {

            return leftRotate(node);
        }

        /* -----------------------------------------------------
           RL
           ----------------------------------------------------- */

        if (balance < -1 &&
            getBalance(node->right) > 0) {

            node->right =
                rightRotate(node->right);

            return leftRotate(node);
        }

        return node;
    }

    /* ---------------------------------------------------------
       INORDER TRAVERSAL
       --------------------------------------------------------- */

    void inorder(AVLNode* node, ofstream& file) {

        if (node == nullptr)
            return;

        inorder(node->left, file);

        file << node->key << " ";

        inorder(node->right, file);
    }

    void inorderConsole(AVLNode* node) {

        if (node == nullptr)
            return;

        inorderConsole(node->left);

        cout << node->key << " ";

        inorderConsole(node->right);
    }

    /* ---------------------------------------------------------
       DISPLAY TREE DETAILS
       --------------------------------------------------------- */

    void displayDetails(AVLNode* node) {

        if (node == nullptr)
            return;

        displayDetails(node->left);

        cout << "Node: " << setw(5) << node->key
             << " | Height: " << setw(3) << height(node)
             << " | Balance Factor: "
             << setw(3) << getBalance(node)
             << endl;

        displayDetails(node->right);
    }

    /* ---------------------------------------------------------
       COUNT NODES
       --------------------------------------------------------- */

    int countNodes(AVLNode* node) {

        if (node == nullptr)
            return 0;

        return 1 +
               countNodes(node->left) +
               countNodes(node->right);
    }

public:

    AVLTree() {
        root = nullptr;
    }

    void insert(int key) {
        root = insertNode(root, key);
    }

    void remove(int key) {
        root = deleteNode(root, key);
    }

    int getHeight() {
        return height(root);
    }

    int getNodeCount() {
        return countNodes(root);
    }

    bool search(int key) {

        AVLNode* current = root;

        while (current != nullptr) {

            if (key == current->key)
                return true;

            if (key < current->key)
                current = current->left;

            else
                current = current->right;
        }

        return false;
    }

    void saveInorder(string filename) {

        ofstream file(filename);

        if (!file) {
            cout << "Error opening " << filename << endl;
            return;
        }

        inorder(root, file);

        file << endl;

        file.close();

        cout << "In-order traversal saved to "
             << filename << endl;
    }

    void displayInorder() {

        cout << "In-order traversal: ";

        inorderConsole(root);

        cout << endl;
    }

    void displayTreeDetails() {

        cout << "\n----------------------------------------\n";
        cout << "AVL TREE DETAILS\n";
        cout << "----------------------------------------\n";

        cout << "Total Nodes : "
             << getNodeCount() << endl;

        cout << "Tree Height : "
             << getHeight() << endl;

        cout << "\nNode Information:\n";

        displayDetails(root);

        cout << "----------------------------------------\n";
    }
};

/* =========================================================
   NORMAL BST FUNCTIONS
   ========================================================= */

BSTNode* bstInsert(BSTNode* root, int key) {

    if (root == nullptr)
        return new BSTNode(key);

    if (key < root->key)
        root->left = bstInsert(root->left, key);

    else if (key > root->key)
        root->right = bstInsert(root->right, key);

    return root;
}

bool bstSearch(BSTNode* root, int key) {

    BSTNode* current = root;

    while (current != nullptr) {

        if (key == current->key)
            return true;

        if (key < current->key)
            current = current->left;

        else
            current = current->right;
    }

    return false;
}

/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main() {

    cout << "=============================================\n";
    cout << "       AVL TREE IMPLEMENTATION IN C++\n";
    cout << "=============================================\n\n";

    /* ---------------------------------------------------------
       READ INPUT FILE
       --------------------------------------------------------- */

    ifstream input("input.txt");

    if (!input) {

        cout << "ERROR: input.txt not found!\n";
        cout << "Create input.txt in the same folder as the program.\n";

        return 1;
    }

    vector<int> values;

    int value;

    while (input >> value) {
        values.push_back(value);
    }

    input.close();

    if (values.empty()) {

        cout << "input.txt is empty.\n";

        return 1;
    }

    cout << "Elements read from input.txt: "
         << values.size() << endl;

    /* ---------------------------------------------------------
       CREATE AVL TREE
       --------------------------------------------------------- */

    AVLTree avl;

    /* ---------------------------------------------------------
       CREATE NORMAL BST
       --------------------------------------------------------- */

    BSTNode* bstRoot = nullptr;

    /* ---------------------------------------------------------
       CSV FILE FOR HEIGHT DATA
       --------------------------------------------------------- */

    ofstream heightFile("height_data.csv");

    if (!heightFile) {

        cout << "Error creating height_data.csv\n";

        return 1;
    }

    heightFile << "Number_of_Nodes,AVL_Height\n";

    /* ---------------------------------------------------------
       INSERT ELEMENTS
       --------------------------------------------------------- */

    cout << "\n=============================================\n";
    cout << "INSERTING ELEMENTS INTO AVL TREE\n";
    cout << "=============================================\n";

    int nodeNumber = 0;

    for (int key : values) {

        cout << "\nInserting: " << key << endl;

        avl.insert(key);

        bstRoot = bstInsert(bstRoot, key);

        nodeNumber++;

        cout << "Current AVL Height: "
             << avl.getHeight() << endl;

        heightFile
            << nodeNumber << ","
            << avl.getHeight()
            << "\n";
    }

    heightFile.close();

    /* ---------------------------------------------------------
       DISPLAY FINAL AVL TREE
       --------------------------------------------------------- */

    cout << "\n=============================================\n";
    cout << "FINAL AVL TREE\n";
    cout << "=============================================\n";

    avl.displayInorder();

    avl.displayTreeDetails();

    /* ---------------------------------------------------------
       SAVE INORDER TRAVERSAL
       --------------------------------------------------------- */

    avl.saveInorder("output.txt");

    /* ---------------------------------------------------------
       OPTIONAL DELETION
       --------------------------------------------------------- */

    cout << "\n=============================================\n";
    cout << "DELETION\n";
    cout << "=============================================\n";

    cout << "Enter element to delete "
         << "(enter -1 to skip): ";

    int deleteKey;

    cin >> deleteKey;

    if (deleteKey != -1) {

        cout << "\nDeleting " << deleteKey << "...\n";

        avl.remove(deleteKey);

        cout << "\nAVL tree after deletion:\n";

        avl.displayInorder();

        avl.displayTreeDetails();

        // Save updated tree again
        avl.saveInorder("output.txt");
    }

    /* ---------------------------------------------------------
       SEARCH TIME COMPARISON
       --------------------------------------------------------- */

    cout << "\n=============================================\n";
    cout << "AVL VS BST SEARCH TIME\n";
    cout << "=============================================\n";

    int searchKey;

    cout << "Enter element to search: ";

    cin >> searchKey;

    /*
       Repeat the search many times so that the execution
       time becomes measurable.
    */

    const int repetitions = 1000000;

    /* ---------------------------------------------------------
       AVL SEARCH
       --------------------------------------------------------- */

    auto avlStart = high_resolution_clock::now();

    bool avlFound = false;

    for (int i = 0; i < repetitions; i++) {

        avlFound = avl.search(searchKey);
    }

    auto avlEnd = high_resolution_clock::now();

    double avlTime =
        duration<double, micro>(avlEnd - avlStart).count();

    /* ---------------------------------------------------------
       BST SEARCH
       --------------------------------------------------------- */

    auto bstStart = high_resolution_clock::now();

    bool bstFound = false;

    for (int i = 0; i < repetitions; i++) {

        bstFound = bstSearch(bstRoot, searchKey);
    }

    auto bstEnd = high_resolution_clock::now();

    double bstTime =
        duration<double, micro>(bstEnd - bstStart).count();

    /* ---------------------------------------------------------
       DISPLAY SEARCH RESULTS
       --------------------------------------------------------- */

    cout << fixed << setprecision(3);

    cout << "\nSearch element: "
         << searchKey << endl;

    cout << "AVL Search Result: "
         << (avlFound ? "Found" : "Not Found")
         << endl;

    cout << "BST Search Result: "
         << (bstFound ? "Found" : "Not Found")
         << endl;

    cout << "\nAVL Search Time: "
         << avlTime << " microseconds"
         << endl;

    cout << "BST Search Time: "
         << bstTime << " microseconds"
         << endl;

    cout << "\nAverage AVL Search Time: "
         << avlTime / repetitions
         << " microseconds"
         << endl;

    cout << "Average BST Search Time: "
         << bstTime / repetitions
         << " microseconds"
         << endl;

    /* ---------------------------------------------------------
       SAVE SEARCH COMPARISON TO CSV
       --------------------------------------------------------- */

    ofstream searchFile("search_comparison.csv");

    if (searchFile) {

        searchFile << "Structure,Search_Result,"
                   << "Total_Time_microseconds,"
                   << "Average_Time_microseconds\n";

        searchFile << "AVL,"
                   << (avlFound ? "Found" : "Not Found")
                   << ","
                   << avlTime
                   << ","
                   << avlTime / repetitions
                   << "\n";

        searchFile << "BST,"
                   << (bstFound ? "Found" : "Not Found")
                   << ","
                   << bstTime
                   << ","
                   << bstTime / repetitions
                   << "\n";

        searchFile.close();

        cout << "\nSearch comparison saved to "
             << "search_comparison.csv\n";
    }

    /* ---------------------------------------------------------
       FINAL OUTPUT
       --------------------------------------------------------- */

    cout << "\n=============================================\n";
    cout << "FILES CREATED\n";
    cout << "=============================================\n";

    cout << "1. output.txt\n";
    cout << "   -> Final AVL in-order traversal\n\n";

    cout << "2. height_data.csv\n";
    cout << "   -> Number of nodes vs AVL height\n\n";

    cout << "3. search_comparison.csv\n";
    cout << "   -> AVL vs BST search time\n";

    cout << "\n=============================================\n";
    cout << "PROGRAM COMPLETED SUCCESSFULLY\n";
    cout << "=============================================\n";

    return 0;
}
