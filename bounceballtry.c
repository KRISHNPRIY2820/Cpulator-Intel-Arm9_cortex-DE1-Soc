
#define HEX3_HEX0_BASE        0xFF200020
#define LEDR_BASE             0xFF200000
#define VGA_BASE              0xFF203020
#define MPCORE_PRIV_TIMER     0xFFFEC600

volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
volatile int pixel_buffer_start;

void wait_for_timer(volatile int * A9_priv_timer_ptr) {
    int status;

    do {
        status = *(A9_priv_timer_ptr + 3); // Read timer status
    } while (status == 0);

    *(A9_priv_timer_ptr + 3) = status; // Reset timer flag bit
}
void plot_pixel(int x, int y,  short int line_color) {
    volatile short int *pixel_buffer = ( short int *)(pixel_buffer_start + (y << 10) + (x << 1));
    *pixel_buffer = line_color;
}
void draw_box(int x, int y,  int color) { 
	int i=0;
	for (int j=0;j<=y;j++){
			plot_pixel(  i,  j, color);}
	int j=0;
		for(i=0;i<=x;i++)
			plot_pixel(  i,  j, color);
	 i=127;
	for (int j=0;j<=y;j++){
			plot_pixel(  i,  j, color);}
	 j=127;
		for(i=0;i<=x;i++)
			plot_pixel(  i,  j, color);
	
		 }

void clear_screen() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
	  plot_pixel(x,y,0);
	}
  }
}


int main(void) {
    volatile int * A9_priv_timer_ptr = (int *) MPCORE_PRIV_TIMER;
    pixel_buffer_start = *pixel_ctrl_ptr;
	
    clear_screen();
	
    int x = 64;
    int y = 64;
    int x_direction = 1;
    int y_direction = 1;
	draw_box(127,127,0xF800);
	*A9_priv_timer_ptr = 10000000;
    *(A9_priv_timer_ptr + 2) = 0b011; // Mode = 1 (auto), enable = 1
	
    while (1) {
        // Clear the screen
        clear_screen();

        // Draw the ball
        plot_pixel( x, y, 0xFF00); // Yellow color
		
        // Update the ball position
        x += x_direction;
        y += y_direction;

        // Bounce off the edges
        if (x >= 127 || x <= 0) {
            x_direction = -x_direction;
        }
        if (y >= 127 || y <= 0) {
            y_direction = -y_direction;
        }

        // Wait for the timer
        wait_for_timer(A9_priv_timer_ptr);
    // Timer setup: timeout = 0.05 second (10 million clock cycles at 200 MHz)
    
	}

    return 0;
}
