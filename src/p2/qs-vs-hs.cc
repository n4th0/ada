// Nathan Rodriguez Moyses
/*
ADA. 2024-25
Practice 2: "Empirical analysis by means of program-steps account of two sorting
algorithms: Middle-Quicksort and Heapsort."
*/
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace std;

size_t PASOS;

//--------------------------------------------------------------
// Middle-Quicksort:
// The algorithm selects the middle element of the array as the "pivot".
// In a process called "partitioning", it rearranges the elements so that
// all elements smaller than the pivot are placed to its left, and
// all elements greater than the pivot are placed to its right.
// The process is then repeated recursively on the two resulting
// subarrays (left and right of the pivot).
//--------------------------------------------------------------

void middle_QuickSort(int *v, long left, long right) {
  long i, j;
  int pivot;
  if (left < right) {
    i = left;
    j = right;
    pivot = v[(i + j) / 2];
    // PASOS++;
    // pivot based partitioning:
    do {
      while (v[i] < pivot) {
        PASOS++;
        i++;
      }
      while (v[j] > pivot) {
        PASOS++;
        j--;
      }
      if (i <= j) {
        swap(v[i], v[j]);
        i++;
        j--;
      }
      PASOS++;
    } while (i <= j);
    // Repeat for each non-empty subarray:
    if (left < j)
      middle_QuickSort(v, left, j);
    if (i < right)
      middle_QuickSort(v, i, right);
  }
}

//--------------------------------------------------------------
// Heapsort:
// The algorithm works by repeatedly selecting the largest remaining element
// and placing it at the end of the vector in its correct position.
//
// To efficiently select the largest element, it builds a max-heap.
//
// The sink procedure is used for heap construction (or reconstruction).
//--------------------------------------------------------------

void sink(int *v, size_t n, size_t i)
// Sink an element (indexed by i) in a tree to maintain the heap property.
// n is the size of the heap.
{
  size_t largest;
  size_t l, r; // indices of left and right childs

  do {
    PASOS++;
    largest = i;   // Initialize largest as root
    l = 2 * i + 1; // left = 2*i + 1
    r = 2 * i + 2; // right = 2*i + 2

    // If the left child exists and is larger than the root
    if (l < n && v[l] > v[largest])
      largest = l;

    // If the right child exists and is larger than the largest so far
    if (r < n && v[r] > v[largest])
      largest = r;

    // If the largest is still the root, the process is done
    if (largest == i)
      break;

    // Otherwise, swap the new largest with the current node i and repeat the
    // process with the children
    swap(v[i], v[largest]);
    i = largest;
  } while (true);
}

//--------------------------------------------------------------
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n) {
  // Build a max-heap with the input array ("heapify"):
  // Starting from the last non-leaf node (right to left), sink each element to
  // construct the heap.
  for (size_t i = n / 2 - 1; true; i--) {
    // PASOS++;
    sink(v, n, i);
    if (i == 0)
      break; // As size_t is an unsigned type
  }

  // At this point, we have a max-heap. Now, sort the array:
  // Repeatedly swap the root (largest element) with the last element and
  // rebuild the heap.
  for (size_t i = n - 1; i > 0; i--) {
    // PASOS++;
    // Move the root (largest element) to the end by swapping it with the last
    // element.
    swap(v[0], v[i]);
    // Rebuild the heap by sinking the new root element.
    // Note that the heap size is reduced by one in each iteration (so the
    // element moved to the end stays there)
    sink(v, i, 0);
    // The process ends when the heap has only one element, which is the
    // smallest and remains at the beginning of the array.
  }
}

void cabecera() {
  cout << "# QUICKSORT VERSUS HEAPSORT.                                        "
          "         "
       << endl;
  cout << "# Average processing Msteps (millions of program steps)             "
          "         "
       << endl;
  cout << "# Number of samples (arrays of integer): 30                         "
          "         "
       << endl;
  cout << "# RANDOM ARRAYS SORTED ARRAYS REVERSE SORTED ARRAYS                 "
          "         "
       << endl;
  cout << "# ------------------- ------------------- ---------------------     "
          "         "
       << endl;
  cout << "# Size QuickSort HeapSort QuickSort HeapSort QuickSort HeapSort     "
          "         "
       << endl;
  cout << "#==================================================================="
          "========="
       << endl;
}

int main() {
  cabecera();

  srand(getpid());

  for (int exp = 15; exp <= 20; exp++) {
    float average_random = 0;
    float average2_random = 0;
    float average_sorted = 0;
    float average2_sorted = 0;
    float average_reverse = 0;
    float average2_reverse = 0;

    size_t size = size_t(pow(2, exp));
    cout << size << "\t" << std::flush;
    int *v;
    int *v2;
    v = new int[size];
    if (!v) {
      cerr << "Error, not enough memory!" << endl;
      exit(EXIT_FAILURE);
    }

    v2 = new int[size];
    if (!v2) {
      cerr << "Error, not enough memory!" << endl;
      exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < 30; i++) {

      for (size_t j = 0; j < size; j++) {
        int aux = rand();
        v[j] = aux;
        v2[j] = aux;
      }

      PASOS = 0;
      middle_QuickSort(v, 0, size);
      average_random += (float)(PASOS) / 1000000;

      PASOS = 0;
      heapSort(v, size);
      average2_random += (float)(PASOS) / 1000000;
    }

    for (size_t j = 0; j < size; j++)
      v[j] = v2[j] = j;

    PASOS = 0;
    middle_QuickSort(v, 0, size);
    average_sorted += (float)(PASOS) / 1000000;

    PASOS = 0;
    heapSort(v, size);
    average2_sorted += (float)(PASOS) / 1000000;

    for (size_t j = size; j > 0; j--)
      v[(size - j)] = v2[(size - j)] = j;

    PASOS = 0;
    middle_QuickSort(v, 0, size);
    average_reverse += (float)(PASOS) / 1000000;

    PASOS = 0;
    heapSort(v, size);
    average2_reverse += (float)(PASOS) / 1000000;

    cout << average_random / 30 << ' ' << average2_random / 30 << ' '
         << average_sorted << ' ' << average2_sorted << ' ' << average_reverse
         << ' ' << average2_reverse;
    cout << endl;

    delete[] v;
    delete[] v2;
  }
}
