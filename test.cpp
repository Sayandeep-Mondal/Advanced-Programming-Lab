#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

// Linear Search
int linearSearch(int arr[], int n, int key, int &comparisons)
{
    comparisons = 0;
    for (int i = 0; i < n; i++)
    {
        comparisons++;
        if (arr[i] == key)
            return i;
    }
    return -1;
}

// Binary Search
int binarySearch(int arr[], int n, int key, int &comparisons)
{
    int low = 0, high = n - 1;
    comparisons = 0;

    while (low <= high)
    {
        int mid = (low + high) / 2;
        comparisons++;

        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main()
{
    srand(time(0));

    // Input sizes up to 10000
    int sizes[] = {10, 50, 100, 500, 1000, 2000, 5000, 10000};
    int m = 8;

    ofstream file("search_graph.csv");

    if (!file)
    {
        cout << "File cannot be created!" << endl;
        return 1;
    }

    file << "InputSize,LinearWorst,BinaryWorst\n";

    cout << "InputSize\tLinearWorst\tBinaryWorst\n";

    for (int i = 0; i < m; i++)
    {
        int n = sizes[i];

        // Array size increased to 10000
        int arr[10000];

        // Generate sorted array
        arr[0] = rand() % 10;
        for (int j = 1; j < n; j++)
            arr[j] = arr[j - 1] + (rand() % 10 + 1);

        // Worst case: last element
        int key = arr[n - 1];

        int linearComp, binaryComp;

        linearSearch(arr, n, key, linearComp);
        binarySearch(arr, n, key, binaryComp);

        cout << n << "\t\t"
             << linearComp << "\t\t"
             << binaryComp << endl;

        file << n << ","
             << linearComp << ","
             << binaryComp << endl;
    }

    file.close();

    cout << "\nCSV file 'search_graph.csv' created successfully!" << endl;

    return 0;
}