#define BOARD                 "DE1-SoC"

/* Memory */
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define KEY_BASE              0xFF200050
#define MPCORE_PRIV_TIMER     0xFFFEC600

/* 7-segment display patterns for digits 0-9 */
char seg7[] = {
    0b0111111,  // 0
    0b0000110,  // 1
    0b1011011,  // 2
    0b1001111,  // 3
    0b1100110,  // 4
    0b1101101,  // 5
    0b1111101,  // 6
    0b0000111,  // 7
    0b1111111,  // 8
    0b1101111   // 9
};

void display_number(int number) {
    volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
    volatile int * HEX5_HEX4_ptr = (int *) HEX5_HEX4_BASE;

    /* Display the current number on the 7-segment displays */
    *HEX5_HEX4_ptr = 0; // Clear HEX5-HEX4
    *HEX3_HEX0_ptr = seg7[number]; // Display number on HEX0
}

void wait_for_timer(volatile int * A9_priv_timer_ptr) {
    int status;

    do {
        status = *(A9_priv_timer_ptr + 3); // Read timer status
    } while (status == 0);

    *(A9_priv_timer_ptr + 3) = status; // Reset timer flag bit
}

int main(void) {
    volatile int * A9_priv_timer_ptr = (int *) MPCORE_PRIV_TIMER;
    int countdown = 9;
    int flag = 0, edge_capture;
    volatile int * KEYS_ptr = (int *) KEY_BASE;

    // Timer setup: timeout = 1/(200 MHz) x 40x10^6 = 0.2 sec
    *A9_priv_timer_ptr = 40000000;
    *(A9_priv_timer_ptr + 2) = 0b011; // Mode = 1 (auto), enable = 1

    while (1) {
        // Check for key press to start/stop the countdown
        edge_capture = *(KEYS_ptr + 3); // Read keys edge capture register
        if (edge_capture) {
            *(KEYS_ptr + 3) = edge_capture; // Clear edge capture register
            flag ^= 1; // Toggle flag to start/stop countdown
        }

        if (flag) {
            display_number(countdown); // Display the current countdown number

            if (countdown == 0) // Check if countdown has reached 0
                countdown = 9; // Reset countdown to 9
            else
                --countdown; // Decrement the countdown number

            wait_for_timer(A9_priv_timer_ptr); // Wait for the timer
        }
    }

    return 0;
}
