#include <stdio.h>
#define LEDR_BASE 0xFF200000
#define MPCORE_PRIV_TIMER     0xFFFEC600
void wait_for_timer(volatile int * A9_priv_timer_ptr) {
    int status;

    do {
        status = *(A9_priv_timer_ptr + 3); // Read timer status
    } while (status == 0);

    *(A9_priv_timer_ptr + 3) = status; // Reset timer flag bit
}	
// Function to generate Fibonacci sequence up to n terms
void generateFibonacci(int n,volatile int * A9_priv_timer_ptr) {
    int t1 = 0, t2 = 1, nextTerm;

    printf("Fibonacci Sequence: ");
	volatile int * LEDR_ptr = (int *) LEDR_BASE;
    for (int i = 1; i <= n; i++) {
		printf("%d ",t1);
       *LEDR_ptr=t1;
		wait_for_timer(A9_priv_timer_ptr);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
   printf("\n");
}


int main() {
    int n=5;
volatile int * A9_priv_timer_ptr = (int *) MPCORE_PRIV_TIMER;
	*A9_priv_timer_ptr = 200000000;
    *(A9_priv_timer_ptr + 2) = 0b011; // Mode = 1 (auto), enable = 1

    

    generateFibonacci(n,A9_priv_timer_ptr);

    return 0;
}
