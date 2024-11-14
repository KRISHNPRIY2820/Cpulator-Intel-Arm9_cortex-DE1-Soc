#include <stdbool.h>
#include <stdio.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREEN 0x07E0
#define RED 0xF800

// Function declarations
void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void draw_dino(int y);
void draw_obstacle(int x);
void draw_score(int score);
void check_collision();
void reset_game();
void delay(int n);

volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
volatile int *button_ptr = (int *)0xFF200050; // To read the button inputs
//volatile int *SW_ptr = (volatile int *)0xFF200040; // To read switch inputs
volatile int *LEDR_ptr = (volatile int *)0xFF200000; // To control LEDs
volatile int pixel_buffer_start;

int dino_y = 120;
int obstacle_x = 320;
int score = 0;
bool game_over = false;
bool jump = false;

void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
            plot_pixel(x, y, BLACK);
        }
    }
}

void plot_pixel(int x, int y, short int line_color) {
    volatile short int *pixel_buffer = (short int *)(pixel_buffer_start + (y << 10) + (x << 1));
    *pixel_buffer = line_color;
}

void draw_dino(int y) {
    int i, j;
    
    
    for (i = 15; i < 25; i++) {
        for (j = y; j < y + 10; j++) {
            plot_pixel(i, j, GREEN); // Head part
        }
    }
    
    // Body (A wider rectangle for the body)
    for (i = 10; i < 30; i++) {
      for (j = y + 10; j < y + 30; j++) {
        plot_pixel(i, j, GREEN); // Body part
      }
    }

    // Legs (Two small rectangles for legs)
    for (i = 12; i < 15; i++) {
        for (j = y + 30; j < y + 40; j++) {
            plot_pixel(i, j, GREEN); // Left leg
        }
    }
    
    for (i = 25; i < 28; i++) {
        for (j = y + 30; j < y + 40; j++) {
            plot_pixel(i, j, GREEN); // Right leg
        }
    }
}

void draw_obstacle(int x) {
    int i, j;
    
    // Drawing a cactus-like obstacle (using a wider structure)
    for (i = x; i < x + 10; i++) {
        for (j = 130; j < 150; j++) {
            plot_pixel(i, j, RED); // Main body of the cactus
        }
    }

    // Adding branches to the cactus
    for (i = x - 5; i < x ; i++) {
        for (j = 120; j < 130; j++) {
            plot_pixel(i, j, GREEN); // Left branch of the cactus
        }
    }

    for (i = x + 10; i < x + 15; i++) {
        for (j = 120; j < 130; j++) {
            plot_pixel(i, j, GREEN); // Right branch of the cactus
        }
    }
}

void clear_score_display() {
    // We will clear the score display by updating the LEDs 
    *LEDR_ptr = 0x0; // Clear the LED display
}

void draw_score(int score) {
    clear_score_display();
    
    *LEDR_ptr = score; 
}

void check_collision() {
    if (obstacle_x < 30 && obstacle_x > 10 && dino_y > 110) {
        game_over = true;
    }
}

void reset_game() {
    score = 0;
    dino_y = 120;
    obstacle_x = 320;
    game_over = false;
    jump = false;
    clear_screen();
}

void update_dino_position() {
    if (jump) {
        dino_y -= 10;
        if (dino_y <= 80) {
            jump = false;
        }
    } else if (dino_y < 120) {
        dino_y += 10;
    }
}

void delay(int n) {
    volatile int i;
    for (i = 0; i < n * 10000; i++); // Adjust the multiplier as needed
}

int main(void) {
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen();

    while (1) {
        if (!game_over) {
            clear_screen();

            if (obstacle_x < 0) {
                obstacle_x = 320;
                score++;
            }

            // Check if the button is pressed for jumping (using hardware input for buttons)
            if ((*button_ptr & 0x1) == 0x1 && dino_y == 120) { // Check for button press
                jump = true;
            }

            update_dino_position();
            draw_dino(dino_y);
            draw_obstacle(obstacle_x);
            draw_score(score);

            check_collision();

            if (game_over) {
                printf("Game Over!\n");
                reset_game();
            }

            obstacle_x -= 5; // Adjust speed if needed
            delay(5); // Slow down the game loop for better response
        }
    }
    return 0;
}
