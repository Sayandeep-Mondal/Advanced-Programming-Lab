#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace chrono;

// ========================================================
// BST NODE
// ========================================================

struct Node
{
    int data;
    Node* left;
    Node* right;

    Node(int value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// ========================================================
// BST CLASS
// ========================================================

class BST
{
private:
    Node* root;

    // ---------- Recursive Inorder ----------
    void inorder(Node* node, vector<int>& result)
    {
        if (node == nullptr)
            return;

        inorder(node->left, result);
        result.push_back(node->data);
        inorder(node->right, result);
    }

    // ---------- Recursive Preorder ----------
    void preorder(Node* node, vector<int>& result)
    {
        if (node == nullptr)
            return;

        result.push_back(node->data);
        preorder(node->left, result);
        preorder(node->right, result);
    }

    // ---------- Recursive Postorder ----------
    void postorder(Node* node, vector<int>& result)
    {
        if (node == nullptr)
            return;

        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->data);
    }

    // ---------- Height ----------
    int getHeight(Node* node)
    {
        if (node == nullptr)
            return -1;

        return 1 + max(getHeight(node->left),
                       getHeight(node->right));
    }

    // ---------- Delete Tree ----------
    void destroyTree(Node* node)
    {
        if (node == nullptr)
            return;

        destroyTree(node->left);
        destroyTree(node->right);

        delete node;
    }

public:

    // ---------- Constructor ----------
    BST()
    {
        root = nullptr;
    }

    // ---------- Destructor ----------
    ~BST()
    {
        destroyTree(root);
    }

    // ====================================================
    // INSERT
    // ====================================================

    void insert(int value)
    {
        Node* newNode = new Node(value);

        if (root == nullptr)
        {
            root = newNode;
            return;
        }

        Node* current = root;

        while (true)
        {
            if (value < current->data)
            {
                if (current->left == nullptr)
                {
                    current->left = newNode;
                    return;
                }

                current = current->left;
            }
            else
            {
                if (current->right == nullptr)
                {
                    current->right = newNode;
                    return;
                }

                current = current->right;
            }
        }
    }

    // ====================================================
    // SEARCH
    // ====================================================

    bool search(int value)
    {
        Node* current = root;

        while (current != nullptr)
        {
            if (value == current->data)
                return true;

            if (value < current->data)
                current = current->left;
            else
                current = current->right;
        }

        return false;
    }

    // ====================================================
    // DELETE
    // ====================================================

    void deleteNode(int value)
    {
        Node* current = root;
        Node* parent = nullptr;

        // Find node
        while (current != nullptr && current->data != value)
        {
            parent = current;

            if (value < current->data)
                current = current->left;
            else
                current = current->right;
        }

        // Node not found
        if (current == nullptr)
            return;

        // =================================================
        // CASE 1: NODE HAS TWO CHILDREN
        // =================================================

        if (current->left != nullptr &&
            current->right != nullptr)
        {
            Node* successorParent = current;
            Node* successor = current->right;

            while (successor->left != nullptr)
            {
                successorParent = successor;
                successor = successor->left;
            }

            current->data = successor->data;

            // Delete successor
            if (successorParent->left == successor)
                successorParent->left = successor->right;
            else
                successorParent->right = successor->right;

            delete successor;
            return;
        }

        // =================================================
        // CASE 2: NODE HAS ZERO OR ONE CHILD
        // =================================================

        Node* child;

        if (current->left != nullptr)
            child = current->left;
        else
            child = current->right;

        // Deleting root
        if (parent == nullptr)
        {
            root = child;
        }
        else
        {
            if (parent->left == current)
                parent->left = child;
            else
                parent->right = child;
        }

        delete current;
    }

    // ====================================================
    // TRAVERSALS
    // ====================================================

    vector<int> getInorder()
    {
        vector<int> result;
        inorder(root, result);
        return result;
    }

    vector<int> getPreorder()
    {
        vector<int> result;
        preorder(root, result);
        return result;
    }

    vector<int> getPostorder()
    {
        vector<int> result;
        postorder(root, result);
        return result;
    }

    // ====================================================
    // HEIGHT
    // ====================================================

    int height()
    {
        return getHeight(root);
    }
};

// ========================================================
// PRINT VECTOR
// ========================================================

void printVector(const vector<int>& values)
{
    for (int value : values)
        cout << value << " ";

    cout << endl;
}

// ========================================================
// CHECK IF INORDER IS SORTED
// ========================================================

bool isSorted(const vector<int>& values)
{
    return is_sorted(values.begin(), values.end());
}

// ========================================================
// GENERATE RANDOM INPUT
// ========================================================

vector<int> generateRandom(int n)
{
    vector<int> values(n);

    for (int i = 0; i < n; i++)
        values[i] = i + 1;

    random_device rd;
    mt19937 generator(rd());

    shuffle(values.begin(), values.end(), generator);

    return values;
}

// ========================================================
// GENERATE SORTED INPUT
// ========================================================

vector<int> generateSorted(int n)
{
    vector<int> values(n);

    for (int i = 0; i < n; i++)
        values[i] = i + 1;

    return values;
}

// ========================================================
// GENERATE REVERSE-SORTED INPUT
// ========================================================

vector<int> generateReverseSorted(int n)
{
    vector<int> values(n);

    for (int i = 0; i < n; i++)
        values[i] = n - i;

    return values;
}

// ========================================================
// PERFORMANCE RESULT STRUCTURE
// ========================================================

struct Result
{
    int n;
    string type;

    double buildTime;
    int height;

    double searchTime;
    double deleteTime;
};

// ========================================================
// RUN PERFORMANCE TEST
// ========================================================

Result runTest(int n, string type, vector<int> values)
{
    BST tree;

    // ====================================================
    // BUILD TIME
    // ====================================================

    auto buildStart = high_resolution_clock::now();

    for (int value : values)
        tree.insert(value);

    auto buildEnd = high_resolution_clock::now();

    double buildTime =
        duration<double, milli>(buildEnd - buildStart).count();

    // ====================================================
    // HEIGHT
    // ====================================================

    int treeHeight = tree.height();

    // ====================================================
    // 1000 SEARCHES
    // ====================================================

    // Use fixed random generator
    mt19937 generator(12345);
    uniform_int_distribution<int> distribution(1, n);

    vector<int> searchValues(1000);

    for (int i = 0; i < 1000; i++)
        searchValues[i] = distribution(generator);

    auto searchStart = high_resolution_clock::now();

    int found = 0;

    for (int value : searchValues)
    {
        if (tree.search(value))
            found++;
    }

    auto searchEnd = high_resolution_clock::now();

    double searchTime =
        duration<double, milli>(searchEnd - searchStart).count();

    // Prevent compiler from optimizing searches away
    volatile int searchResult = found;
    (void)searchResult;

    // ====================================================
    // 500 DELETIONS
    // ====================================================

    vector<int> deleteValues = values;

    shuffle(deleteValues.begin(),
            deleteValues.end(),
            generator);

    auto deleteStart = high_resolution_clock::now();

    for (int i = 0; i < 500; i++)
    {
        tree.deleteNode(deleteValues[i]);
    }

    auto deleteEnd = high_resolution_clock::now();

    double deleteTime =
        duration<double, milli>(deleteEnd - deleteStart).count();

    return {
        n,
        type,
        buildTime,
        treeHeight,
        searchTime,
        deleteTime
    };
}

// ========================================================
// SAVE RESULTS TO CSV
// ========================================================

void saveCSV(const vector<Result>& results)
{
    // ----------------------------------------------------
    // BUILD TIME CSV
    // ----------------------------------------------------

    ofstream buildFile("bst_build_time.csv");

    buildFile << "N,Random,Sorted,Reverse-Sorted\n";

    for (int n : {1000, 5000, 10000})
    {
        double randomTime = 0;
        double sortedTime = 0;
        double reverseTime = 0;

        for (const Result& r : results)
        {
            if (r.n == n && r.type == "Random")
                randomTime = r.buildTime;

            if (r.n == n && r.type == "Sorted")
                sortedTime = r.buildTime;

            if (r.n == n && r.type == "Reverse-Sorted")
                reverseTime = r.buildTime;
        }

        buildFile << n << ","
                  << randomTime << ","
                  << sortedTime << ","
                  << reverseTime << "\n";
    }

    buildFile.close();

    // ----------------------------------------------------
    // HEIGHT CSV
    // ----------------------------------------------------

    ofstream heightFile("bst_height.csv");

    heightFile << "N,Random,Sorted,Reverse-Sorted\n";

    for (int n : {1000, 5000, 10000})
    {
        int randomHeight = 0;
        int sortedHeight = 0;
        int reverseHeight = 0;

        for (const Result& r : results)
        {
            if (r.n == n && r.type == "Random")
                randomHeight = r.height;

            if (r.n == n && r.type == "Sorted")
                sortedHeight = r.height;

            if (r.n == n && r.type == "Reverse-Sorted")
                reverseHeight = r.height;
        }

        heightFile << n << ","
                   << randomHeight << ","
                   << sortedHeight << ","
                   << reverseHeight << "\n";
    }

    heightFile.close();

    // ----------------------------------------------------
    // SEARCH TIME CSV
    // ----------------------------------------------------

    ofstream searchFile("bst_search_time.csv");

    searchFile << "N,Random,Sorted,Reverse-Sorted\n";

    for (int n : {1000, 5000, 10000})
    {
        double randomTime = 0;
        double sortedTime = 0;
        double reverseTime = 0;

        for (const Result& r : results)
        {
            if (r.n == n && r.type == "Random")
                randomTime = r.searchTime;

            if (r.n == n && r.type == "Sorted")
                sortedTime = r.searchTime;

            if (r.n == n && r.type == "Reverse-Sorted")
                reverseTime = r.searchTime;
        }

        searchFile << n << ","
                   << randomTime << ","
                   << sortedTime << ","
                   << reverseTime << "\n";
    }

    searchFile.close();

    // ----------------------------------------------------
    // DELETE TIME CSV
    // ----------------------------------------------------

    ofstream deleteFile("bst_delete_time.csv");

    deleteFile << "N,Random,Sorted,Reverse-Sorted\n";

    for (int n : {1000, 5000, 10000})
    {
        double randomTime = 0;
        double sortedTime = 0;
        double reverseTime = 0;

        for (const Result& r : results)
        {
            if (r.n == n && r.type == "Random")
                randomTime = r.deleteTime;

            if (r.n == n && r.type == "Sorted")
                sortedTime = r.deleteTime;

            if (r.n == n && r.type == "Reverse-Sorted")
                reverseTime = r.deleteTime;
        }

        deleteFile << n << ","
                   << randomTime << ","
                   << sortedTime << ","
                   << reverseTime << "\n";
    }

    deleteFile.close();

    // ----------------------------------------------------
    // COMPLETE CSV
    // ----------------------------------------------------

    ofstream completeFile("bst_results.csv");

    completeFile
        << "N,Input Type,Build Time (ms),Height,"
        << "1000 Searches (ms),500 Deletions (ms)\n";

    for (const Result& r : results)
    {
        completeFile
            << r.n << ","
            << r.type << ","
            << r.buildTime << ","
            << r.height << ","
            << r.searchTime << ","
            << r.deleteTime << "\n";
    }

    completeFile.close();
}

// ========================================================
// MAIN
// ========================================================

int main()
{
    cout << "====================================================\n";
    cout << "       BINARY SEARCH TREE IMPLEMENTATION\n";
    cout << "====================================================\n\n";

    // ====================================================
    // PART 1: BASIC BST OPERATIONS
    // ====================================================

    cout << "PART 1: BST INSERTION AND TRAVERSALS\n";
    cout << "----------------------------------------\n";

    BST tree;

    vector<int> keys =
    {
        50, 30, 70, 20, 40, 60, 80, 10
    };

    cout << "Keys inserted: ";

    for (int value : keys)
    {
        cout << value << " ";
        tree.insert(value);
    }

    cout << "\n\n";

    // Inorder
    vector<int> inorder = tree.getInorder();

    cout << "Inorder Traversal:   ";
    printVector(inorder);

    // Preorder
    vector<int> preorder = tree.getPreorder();

    cout << "Preorder Traversal:  ";
    printVector(preorder);

    // Postorder
    vector<int> postorder = tree.getPostorder();

    cout << "Postorder Traversal: ";
    printVector(postorder);

    // Sorted verification
    cout << "\nBST Property Verification:\n";

    if (isSorted(inorder))
        cout << "Inorder traversal is sorted. BST property is satisfied.\n";
    else
        cout << "Inorder traversal is NOT sorted.\n";

    // ====================================================
    // SEARCH
    // ====================================================

    cout << "\nSearch Operation:\n";

    int presentKey = 40;
    int absentKey = 100;

    cout << "Searching for " << presentKey << ": ";

    if (tree.search(presentKey))
        cout << "Key found.\n";
    else
        cout << "Key not found.\n";

    cout << "Searching for " << absentKey << ": ";

    if (tree.search(absentKey))
        cout << "Key found.\n";
    else
        cout << "Key not found.\n";

    // ====================================================
    // PART 2: DELETION
    // ====================================================

    cout << "\n\nPART 2: DELETION OPERATIONS\n";
    cout << "----------------------------------------\n";

    // ----------------------------------------------------
    // (i) DELETE LEAF NODE
    // ----------------------------------------------------

    cout << "\n(i) Deleting a leaf node: 80\n";

    tree.deleteNode(80);

    cout << "Inorder after deleting 80: ";
    printVector(tree.getInorder());

    // ----------------------------------------------------
    // (ii) DELETE NODE WITH ONE CHILD
    // ----------------------------------------------------

    cout << "\n(ii) Deleting a node with one child: 20\n";

    // Node 20 has one child: 10
    tree.deleteNode(20);

    cout << "Inorder after deleting 20: ";
    printVector(tree.getInorder());

    // ----------------------------------------------------
    // (iii) DELETE NODE WITH TWO CHILDREN
    // ----------------------------------------------------

    cout << "\n(iii) Deleting a node with two children: 50\n";

    // Root 50 has two children: 30 and 70
    tree.deleteNode(50);

    cout << "Inorder after deleting 50: ";
    printVector(tree.getInorder());

    cout << "\nBST Property after all deletions: ";

    if (isSorted(tree.getInorder()))
        cout << "Satisfied.\n";
    else
        cout << "Not satisfied.\n";

    // ====================================================
    // PART 3: PERFORMANCE ANALYSIS
    // ====================================================

    cout << "\n\nPART 3: PERFORMANCE ANALYSIS\n";
    cout << "====================================================\n";

    vector<Result> results;

    vector<int> sizes =
    {
        1000,
        5000,
        10000
    };

    for (int n : sizes)
    {
        cout << "\nRunning tests for N = " << n << "...\n";

        // ------------------------------------------------
        // RANDOM
        // ------------------------------------------------

        vector<int> randomValues = generateRandom(n);

        Result randomResult =
            runTest(n, "Random", randomValues);

        results.push_back(randomResult);

        // ------------------------------------------------
        // SORTED
        // ------------------------------------------------

        vector<int> sortedValues =
            generateSorted(n);

        Result sortedResult =
            runTest(n, "Sorted", sortedValues);

        results.push_back(sortedResult);

        // ------------------------------------------------
        // REVERSE SORTED
        // ------------------------------------------------

        vector<int> reverseValues =
            generateReverseSorted(n);

        Result reverseResult =
            runTest(n, "Reverse-Sorted", reverseValues);

        results.push_back(reverseResult);
    }

    // ====================================================
    // PRINT RESULTS TABLE
    // ====================================================

    cout << "\n\n====================================================\n";
    cout << "                 PERFORMANCE RESULTS\n";
    cout << "====================================================\n\n";

    cout << left
         << setw(8)  << "N"
         << setw(18) << "Input"
         << setw(18) << "Build(ms)"
         << setw(12) << "Height"
         << setw(20) << "Search(ms)"
         << setw(20) << "Delete(ms)"
         << endl;

    cout << string(96, '-') << endl;

    cout << fixed << setprecision(4);

    for (const Result& r : results)
    {
        cout << left
             << setw(8)  << r.n
             << setw(18) << r.type
             << setw(18) << r.buildTime
             << setw(12) << r.height
             << setw(20) << r.searchTime
             << setw(20) << r.deleteTime
             << endl;
    }

    // ====================================================
    // SAVE CSV FILES
    // ====================================================

    saveCSV(results);

    cout << "\n====================================================\n";
    cout << "CSV FILES CREATED SUCCESSFULLY\n";
    cout << "====================================================\n";

    cout << "1. bst_results.csv\n";
    cout << "2. bst_build_time.csv\n";
    cout << "3. bst_height.csv\n";
    cout << "4. bst_search_time.csv\n";
    cout << "5. bst_delete_time.csv\n";

    // ====================================================
    // DISCUSSION
    // ====================================================

    cout << "\n\n====================================================\n";
    cout << "                 DISCUSSION\n";
    cout << "====================================================\n\n";

    cout << "1. RANDOM INPUT:\n";
    cout << "   Random insertion usually produces a relatively\n";
    cout << "   balanced BST. Therefore, the height is much smaller\n";
    cout << "   than N and search/insert/delete operations are\n";
    cout << "   generally close to O(log N).\n\n";

    cout << "2. SORTED INPUT:\n";
    cout << "   Sorted insertion creates a highly unbalanced tree.\n";
    cout << "   Each new element is inserted to the right, producing\n";
    cout << "   a tree similar to a linked list. The height becomes\n";
    cout << "   approximately N-1 and operations can become O(N).\n\n";

    cout << "3. REVERSE-SORTED INPUT:\n";
    cout << "   Reverse-sorted insertion creates the same type of\n";
    cout << "   unbalanced tree, but all nodes grow toward the left.\n";
    cout << "   Its height is also approximately N-1 and operations\n";
    cout << "   can become O(N).\n\n";

    cout << "Therefore, input order has a major effect on the\n";
    cout << "performance of an ordinary Binary Search Tree.\n";

    cout << "\n====================================================\n";
    cout << "                    PROGRAM END\n";
    cout << "====================================================\n";

    return 0;
}