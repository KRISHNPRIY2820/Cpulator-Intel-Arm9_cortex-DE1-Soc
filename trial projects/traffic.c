

#define LEDR_BASE 0xFF200000
#define MPCORE_PRIV_TIMER 0xFFFEC600

void wait_for_timer(volatile int * A9_priv_timer_ptr) {
    int status;

    do {
        status = *(A9_priv_timer_ptr + 3); // Read timer status
    } while (status == 0);

    *(A9_priv_timer_ptr + 3) = status; // Reset timer flag bit
}

int main(void) {
    volatile int * LEDR_ptr = (int *) LEDR_BASE;
    volatile int * A9_priv_timer_ptr = (int *) MPCORE_PRIV_TIMER;

    // Timer setup: timeout = 1 second (200 million clock cycles at 200 MHz)
    *A9_priv_timer_ptr = 200000000;
    *(A9_priv_timer_ptr + 2) = 0b011; // Mode = 1 (auto), enable = 1

    while (1) {
        // Red light
        *LEDR_ptr = 0b001; // Turn on red LED
        wait_for_timer(A9_priv_timer_ptr); // Wait for 1 second

        // Green light
        *LEDR_ptr = 0b010; // Turn on green LED
        wait_for_timer(A9_priv_timer_ptr); // Wait for 1 second

        // Yellow light
        *LEDR_ptr = 0b100; // Turn on yellow LED
        wait_for_timer(A9_priv_timer_ptr); // Wait for 1 second
    }

    return 0;
}
