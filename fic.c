#include <stdio.h>

// Function to generate Fibonacci sequence up to n terms
void generateFibonacci(int n) {
    int t1 = 0, t2 = 1, nextTerm;

    printf("Fibonacci Sequence: ");

    for (int i = 1; i <= n; i++) {
        printf("%d ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
    printf("\n");
}

int main() {
    int n=4;

   
    generateFibonacci(n);

    return 0;
}
