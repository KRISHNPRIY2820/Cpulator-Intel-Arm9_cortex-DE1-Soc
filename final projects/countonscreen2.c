

#define HEX3_HEX0_BASE        0xFF200020
#define LEDR_BASE             0xFF200000
#define VGA_BASE              0xFF203020
#define MPCORE_PRIV_TIMER     0xFFFEC600
volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
volatile int pixel_buffer_start;
volatile int *char_ctrl_ptr = (int *)0xFF203030;
volatile int char_buffer_start;
char seg7[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};
void plot_pixel(int x, int y, short int line_color) {
    volatile short int *pixel_buffer = (short int *)(pixel_buffer_start + (y << 10) + (x << 1));
    *pixel_buffer = line_color;
}

void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (char_buffer_start + (y<<7) + x);
  *character_buffer = c;
}
void display_number_on_vga(volatile int *vga_ptr, int number) {
    char str[2];
    str[0] = '0' + number; // Convert number to character
    str[1] = '\0'; // Null-terminate the string
    vga_ptr[0] = str[0]; // Display the character on the VGA screen
}

void clear_screen() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
	  plot_pixel(x,y,0);
	}
  }
}

void wait_for_timer(volatile int * A9_priv_timer_ptr) {
    int status;

    do {
        status = *(A9_priv_timer_ptr + 3); // Read timer status
    } while (status == 0);

    *(A9_priv_timer_ptr + 3) = status; // Reset timer flag bit
}

int main(void) {
	 pixel_buffer_start = *pixel_ctrl_ptr;
	char_buffer_start = *char_ctrl_ptr;
    clear_screen();
	
    volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
    volatile int * LEDR_ptr = (int *) LEDR_BASE;
    volatile int * VGA_ptr = (int *) VGA_BASE;
    volatile int * A9_priv_timer_ptr = (int *) MPCORE_PRIV_TIMER;

    int count = 0;
    int led_state = 0;

    // Timer setup: timeout = 1/(200 MHz) x 20x10^6 = 0.1 sec
    *A9_priv_timer_ptr = 200000000;
    *(A9_priv_timer_ptr + 2) = 0b011; // Mode = 1 (auto), enable = 1

    while (1) {
		clear_screen();
        
		// Display the current count on the 7-segment display
		
        *HEX3_HEX0_ptr = seg7[count];
		
        
        // Toggle the LED state and display it on the LED
        led_state = !led_state;
        *LEDR_ptr = led_state;
		char ascii_char = (char)(count + 48);
        // Display the current count on the VGA screen
        //display_number_on_vga(VGA_ptr, count);
		write_char(10,10,ascii_char);
        // Increment the count
        count = (count + 1) % 10;

        // Wait for the timer
        wait_for_timer(A9_priv_timer_ptr);
    }

    return 0;
}
