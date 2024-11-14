#include <stdio.h>
#define LEDR_BASE 0xFF200000
#define MPCORE_PRIV_TIMER     0xFFFEC600

	
volatile int * LEDR_ptr = (int *) LEDR_BASE;
// Function to perform insertion sort
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Function to print an array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        *LEDR_ptr=arr[i];
		
    }
    printf("\n");
}
void printArray2(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
// Main function to test the insertion sort function
int main() {
    int arr[] = {12, 11, 13, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: \n");
    printArray2(arr, n);
	  printArray(arr, n);
    insertionSort(arr, n);

    printf("Sorted array: \n");
    printArray2(arr, n);
 printArray(arr, n);
    return 0;
}
