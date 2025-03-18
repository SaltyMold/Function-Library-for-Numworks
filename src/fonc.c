#include "eadk.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/

// Random int between min and max
int randint(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    uint32_t random_value = eadk_random();
    return min + (random_value % (max - min + 1));
}

/*---------------------------------------------------------------------------*/

// Random choice in list
const char* choice(const char *list[], size_t size) {
    int index = randint(0, size - 1);
    return list[index];
}

/*---------------------------------------------------------------------------*/

// Instant crash and reset of the calculator
void crash_calculator() {
    volatile int *ptr = (int *)0xFFFFFFFF;
    *ptr = 0;
}

/*---------------------------------------------------------------------------*/

// All function for the input : 

// Mapping of keys to characters
typedef struct {
    eadk_key_t key;
    char normal;
    char alpha;
} KeyMapping;

// Mapping of keys to characters
static const KeyMapping key_mappings[] = {
    {eadk_key_one, '1', 'w'},
    {eadk_key_two, '2', 'x'},
    {eadk_key_three, '3', 'y'},
    {eadk_key_four, '4', 'r'},
    {eadk_key_five, '5', 's'},
    {eadk_key_six, '6', 't'},
    {eadk_key_seven, '7', 'm'},
    {eadk_key_eight, '8', 'n'},
    {eadk_key_nine, '9', 'o'},
    {eadk_key_zero, '0', '?'},
    {eadk_key_exp, '[', 'a'},
    {eadk_key_ln, ']', 'b'},
    {eadk_key_log, '{', 'c'},
    {eadk_key_imaginary, '}', 'd'},
    {eadk_key_comma, '_', 'e'},
    {eadk_key_power, ',', 'f'},
    {eadk_key_pi, '=', 'j'},
    {eadk_key_sqrt, '<', 'k'},
    {eadk_key_square, '>', 'l'},
    {eadk_key_left_parenthesis, '(', 'p'},
    {eadk_key_right_parenthesis, ')', 'q'},
    {eadk_key_multiplication, '*', 'u'},
    {eadk_key_division, '/', 'v'},
    {eadk_key_plus, '+', 'z'},
    {eadk_key_minus, '-', ' '},
    {eadk_key_dot, '.', '!'},
    {eadk_key_sine, 0, 'g'},
    {eadk_key_cosine, 0, 'h'},
    {eadk_key_tangent, 0, 'i'}
};

// refresh the screen with the buffer and the alpha mode for input()
void refresh_input(const char *buffer, eadk_point_t point, bool is_alpha, bool draw_string, bool large_font) {
    if (draw_string) {
        eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);
        eadk_display_draw_string(buffer, point, large_font, eadk_color_black, eadk_color_white);
        eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);
        if (is_alpha) {
            eadk_display_draw_string("ALPHA", (eadk_point_t){0, 0}, large_font, eadk_color_black, eadk_color_white);
        }
        else {
            eadk_display_draw_string("NO ALPHA", (eadk_point_t){0, 0}, large_font, eadk_color_black, eadk_color_white);
        }
    }
    eadk_timing_msleep(300);
}

// Return the input of the user with point, draw_string and large_font in argument
char* input(eadk_point_t point, bool draw_string, bool large_font) {
    static char buffer[100] = ""; 
    size_t index = 0;
    bool is_alpha = false;

    memset(buffer, 0, sizeof(buffer));

    if (draw_string) {
        eadk_display_draw_string("NO ALPHA", (eadk_point_t){0, 0}, large_font, eadk_color_black, eadk_color_white);
    }

    while (eadk_keyboard_scan() != 0) {
        eadk_timing_msleep(100);
    }

    while (true) {
        eadk_keyboard_state_t state = eadk_keyboard_scan();

        if (eadk_keyboard_key_down(state, eadk_key_home)) {
            return ""; // Exit input on 'HOME' key press
        } 
        else if (eadk_keyboard_key_down(state, eadk_key_backspace)) {
            if (index > 0) {
                buffer[index - 1] = 0; // Remove the last character
                index--;
                refresh_input(buffer, point, is_alpha, draw_string, large_font);
            }
        } 
        else if (eadk_keyboard_key_down(state, eadk_key_alpha)) {
            is_alpha = !is_alpha; // Toggle alpha mode on 'ALPHA' key press
            refresh_input(buffer, point, is_alpha, draw_string, large_font);
        }
        else if (eadk_keyboard_key_down(state, eadk_key_exe)) {
            refresh_input(buffer, point, is_alpha, draw_string, large_font);
            break; // Return the buffer on 'EXE' key press
        } 
        else {
            for (size_t i = 0; i < sizeof(key_mappings) / sizeof(KeyMapping); i++) {
                if (eadk_keyboard_key_down(state, key_mappings[i].key)) {
                    char character = is_alpha ? key_mappings[i].alpha : key_mappings[i].normal;
                    if (character != 0) { // Only add character if it's not 0
                        buffer[index++] = character;
                        refresh_input(buffer, point, is_alpha, draw_string, large_font);
                    }
                    break;
                }
            }
        }
    }
    return buffer;
}

/*---------------------------------------------------------------------------*/

void refresh_input_int(const char *buffer, eadk_point_t point, bool draw_string, bool large_font) {
    if (draw_string) {
        eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);
        eadk_display_draw_string(buffer, point, large_font, eadk_color_black, eadk_color_white);
        eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);
    }
    eadk_timing_msleep(300);
}

// Mapping of keys to characters
static const KeyMapping key_mappings_int[] = {
    {eadk_key_one, '1'},
    {eadk_key_two, '2'},
    {eadk_key_three, '3'},
    {eadk_key_four, '4'},
    {eadk_key_five, '5'},
    {eadk_key_six, '6'},
    {eadk_key_seven, '7'},
    {eadk_key_eight, '8'},
    {eadk_key_nine, '9'},
    {eadk_key_zero, '0'}
};

// Return the int input of the user with point, draw_string and large_font in argument
int input_int(eadk_point_t point, bool draw_string, bool large_font) {
    static char buffer[100] = ""; 
    size_t index = 0;

    memset(buffer, 0, sizeof(buffer));

    while (eadk_keyboard_scan() != 0) {
        eadk_timing_msleep(100);
    }

    while (true) {
        eadk_keyboard_state_t state = eadk_keyboard_scan();

        if (eadk_keyboard_key_down(state, eadk_key_home)) {
            return 0; // Exit input on 'HOME' key press
        } 
        else if (eadk_keyboard_key_down(state, eadk_key_backspace)) {
            if (index > 0) {
                buffer[index - 1] = 0; // Remove the last character
                index--;
                refresh_input_int(buffer, point, draw_string, large_font);
            }
        } 
        else if (eadk_keyboard_key_down(state, eadk_key_exe)) {
            refresh_input_int(buffer, point, draw_string, large_font);
            break; // Return the buffer on 'EXE' key press
        } 
        else {
            for (size_t i = 0; i < sizeof(key_mappings_int) / sizeof(KeyMapping); i++) {
                if (eadk_keyboard_key_down(state, key_mappings_int[i].key)) {
                    char character = key_mappings_int[i].normal;
                    if (character != 0) { // Only add character if it's not 0
                        buffer[index++] = character;
                        refresh_input_int(buffer, point, draw_string, large_font);
                    }
                    break;
                }
            }
        }
    }
    return atoi(buffer); // Convert the buffer to an integer and return it
}

/*---------------------------------------------------------------------------*/

#define EADK_COLOR(r, g, b) ((eadk_color_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)))

eadk_color_t palette[] = {
    EADK_COLOR(63, 67, 40),
    EADK_COLOR(95, 113, 50),
    EADK_COLOR(148, 173, 57),
    EADK_COLOR(194, 214, 79),
    EADK_COLOR(239, 243, 124),
    EADK_COLOR(227, 230, 172),
    EADK_COLOR(165, 198, 124),
    EADK_COLOR(115, 154, 112),
    EADK_COLOR(77, 102, 89),
    EADK_COLOR(52, 63, 65),
    EADK_COLOR(40, 46, 59),
    EADK_COLOR(26, 31, 46),
    EADK_COLOR(30, 49, 75),
    EADK_COLOR(47, 76, 108),
    EADK_COLOR(61, 128, 163),
    EADK_COLOR(99, 196, 204),
    EADK_COLOR(154, 229, 213),
    EADK_COLOR(229, 239, 239),
    EADK_COLOR(186, 201, 205),
    EADK_COLOR(141, 153, 164),
    EADK_COLOR(105, 111, 128),
    EADK_COLOR(65, 68, 83),
    EADK_COLOR(184, 161, 194),
    EADK_COLOR(126, 101, 155),
    EADK_COLOR(92, 58, 111),
    EADK_COLOR(57, 39, 94),
    EADK_COLOR(47, 25, 62),
    EADK_COLOR(78, 26, 73),
    EADK_COLOR(123, 35, 76),
    EADK_COLOR(178, 54, 87),
    EADK_COLOR(209, 105, 116),
    EADK_COLOR(237, 170, 163),
    EADK_COLOR(238, 203, 144),
    EADK_COLOR(225, 168, 69),
    EADK_COLOR(197, 120, 53),
    EADK_COLOR(141, 72, 48),
    EADK_COLOR(228, 114, 89),
    EADK_COLOR(195, 60, 64),
    EADK_COLOR(141, 54, 73),
    EADK_COLOR(92, 43, 52),
    EADK_COLOR(60, 37, 43),
    EADK_COLOR(104, 64, 57),
    EADK_COLOR(130, 86, 70),
    EADK_COLOR(183, 120, 98),
    EADK_COLOR(125, 89, 93),
    EADK_COLOR(83, 59, 65),
    EADK_COLOR(63, 51, 59),
    EADK_COLOR(43, 34, 42),
    EADK_COLOR(109, 78, 75),
    EADK_COLOR(134, 112, 102),
    EADK_COLOR(180, 157, 126),
    EADK_COLOR(196, 198, 184)
};

// Copy this code in the main.c file
/*
const char *image_code[] = {
    "", // Add your image code 
};
*/

// Decode and draw an image
//ex : decode_and_draw_image((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, image_code[0]); 
void decode_and_draw_image(eadk_rect_t rect, const char *code) {
    int x = rect.x, y = rect.y;
    while (*code) {
        char c = *code++;
        int count = 0;
        while (*code >= '0' && *code <= '9') {
            count = count * 10 + (*code++ - '0');
        }
        if (count == 0) {
            count = 1;
        }
        eadk_color_t color;
        if (c >= 'A' && c <= 'Z') {
            color = palette[c - 'A'];
        } else if (c >= 'a' && c <= 'z') {
            color = palette[c - 'a' + 26];
        }
        for (int i = 0; i < count; i++) {
            eadk_display_push_rect((eadk_rect_t){x, y, 1, 1}, &color);
            x++;
            if (x >= rect.x + rect.width) {
                x = rect.x;
                y++;
                if (y >= rect.y + rect.height) {
                    return;
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*/

void chrono(eadk_point_t point, bool large_font, int duration) {
    int minutes = duration / 60;
    int seconds = duration % 60;
    int tenths = 0;
    char buffer[8]; // Buffer to hold the time string in "MM:SS.T" format

    for (int i = duration * 10; i >= 0; i--) {
        snprintf(buffer, sizeof(buffer), "%02d:%02d.%d", minutes, seconds, tenths);
        eadk_display_draw_string(buffer, point, large_font, eadk_color_black, eadk_color_white);
        eadk_timing_msleep(100); // Wait for 100 milliseconds

        // Check if the home key is pressed
        eadk_keyboard_state_t state = eadk_keyboard_scan();
        if (eadk_keyboard_key_down(state, eadk_key_home)) {
            return; // Exit the function if home key is pressed
        }

        if (tenths == 0) {
            if (seconds == 0) {
                if (minutes == 0) {
                    break;
                }
                minutes--;
                seconds = 59;
            } else {
                seconds--;
            }
            tenths = 9;
        } else {
            tenths--;
        }
    }
}

/*---------------------------------------------------------------------------*/

// Factorial function
unsigned long long factorial(unsigned int n) {
    if (n == 0) {
        return 1;
    }
    unsigned long long result = 1;
    for (unsigned int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

/*---------------------------------------------------------------------------*/

// Return true if n is a prime number
bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

/*---------------------------------------------------------------------------*/

// Function to decompose a number into its prime factors and return a string
char* prime_factors(int n) {
    static char result[256]; // Fixed size for the result string
    char buffer[20]; // Buffer to convert numbers to strings
    result[0] = '\0'; // Initialize the result string

    // Divide by 2 until n is odd
    while (n % 2 == 0) {
        sprintf(buffer, "%d*", 2);
        strcat(result, buffer);
        n /= 2;
    }

    // n must be odd at this point, so we can skip even numbers
    for (int i = 3; i * i <= n; i += 2) {
        // While i divides n, add i and divide n
        while (n % i == 0) {
            sprintf(buffer, "%d*", i);
            strcat(result, buffer);
            n /= i;
        }
    }

    // This condition is for the case when n is a prime number
    // greater than 2
    if (n > 2) {
        sprintf(buffer, "%d", n);
        strcat(result, buffer);
    } else {
        // Remove the last '*' if present
        size_t len = strlen(result);
        if (len > 0 && result[len - 1] == '*') {
            result[len - 1] = '\0';
        }
    }

    return result;
}
