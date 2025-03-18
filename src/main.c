#include "eadk.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Hello World";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;


void show_menu_screen(){
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);

  eadk_display_draw_string("This is a presentation of many function\nI added for the calculator", (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);

  eadk_display_draw_string("Press 1 to see random", (eadk_point_t){0, 70}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press 2 to see input", (eadk_point_t){0, 90}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press 3 to see input_int", (eadk_point_t){0, 110}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press 4 to see decode_and_draw_image", (eadk_point_t){0, 130}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press 5 to see crash_calculator", (eadk_point_t){0, 150}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press 6 to see chrono", (eadk_point_t){0, 170}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press 7 to see prime_factor", (eadk_point_t){0, 190}, false, eadk_color_black, eadk_color_white);
}


void show1() {
  const char *list[] = {"yes", "no", "maybe", "perhaps"};

  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);
  eadk_display_draw_string("randint(0, 10)", (eadk_point_t){110, 0}, true, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("choice(list, 2)", (eadk_point_t){110, 20}, true, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);

  while (true) {
    eadk_display_push_rect_uniform((eadk_rect_t){0, 0, 100, EADK_SCREEN_HEIGHT}, eadk_color_white);

    int nbr = randint(0, 10);
    char nbr_str[12]; // Enough for 32 bits number
    sprintf(nbr_str, "%d", nbr); // Int to String

    const char *chosen = choice(list, 2); // Random choice in list with the first 2 elements ("yes" and "no")

    eadk_display_draw_string(nbr_str, (eadk_point_t){0, 0}, true, eadk_color_black, eadk_color_white);
    eadk_display_draw_string(chosen, (eadk_point_t){0, 20}, true, eadk_color_black, eadk_color_white);

    eadk_timing_msleep(500);


    // Break loop if home key is pressed
    eadk_keyboard_state_t state = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(state, eadk_key_home)) {
      show_menu_screen();
      break;
    }
  }
}





void show2() {
  char* user_input = "";
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);
  eadk_display_draw_string("Write something", (eadk_point_t){80, 120}, true, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press EXE to finish", (eadk_point_t){80, 140}, true, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("user_input = input(input_point, true, true);", (eadk_point_t){0, 50}, false, eadk_color_black, eadk_color_white);
  eadk_point_t input_point = {0, 30};
  user_input = input(input_point, true, true); // text at 0, 30, draw_string = true, large_font = true
  if (user_input != "") {
    eadk_display_draw_string("Your input was :", (eadk_point_t){0, 80}, true, eadk_color_black, eadk_color_white);
    eadk_display_draw_string(user_input, (eadk_point_t){0, 100}, true, eadk_color_black, eadk_color_white);
    eadk_display_draw_string("Wait 5 seconds", (eadk_point_t){0, 220}, true, eadk_color_black, eadk_color_white);
    eadk_timing_msleep(5000);
  }
  show_menu_screen();
}


void show3() {
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);
  eadk_display_draw_string("Write something", (eadk_point_t){80, 120}, true, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press EXE to finish", (eadk_point_t){80, 140}, true, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("user_input = input_int(input_point, true, true);", (eadk_point_t){0, 50}, false, eadk_color_black, eadk_color_white);

  eadk_point_t input_point = {0, 30};
  int user_input = input_int(input_point, true, true); // text at 0, 30, draw_string = true, large_font = true
  if (user_input != 0) {
    char user_input_str[12];
    sprintf(user_input_str, "%d", user_input);
    eadk_display_draw_string("Your input was :", (eadk_point_t){0, 80}, true, eadk_color_black, eadk_color_white);
    eadk_display_draw_string(user_input_str, (eadk_point_t){0, 100}, true, eadk_color_black, eadk_color_white);
    eadk_display_draw_string("Wait 5 seconds", (eadk_point_t){0, 220}, true, eadk_color_black, eadk_color_white);
    eadk_timing_msleep(5000);
  }
  show_menu_screen();
}


const char *image_code[] = {
  "R502SR142zekfR4gh2FR4POPR141ek3R3Fh3gR3QO3QR139zk4zR2gh4R3PO3PR139Sk4zR2gh4R3PO3PR140ek2eR3Fh3gR3QO3QR140zekfR4gh2FR4POPR158SR617S160M160L4813M2L158O2ML157O3NL156O4NL38NO2NL2NO2NL48MO3ML5MO3ML44O5NL37NO3L2O3NL48MO3ML5MO3ML44O6NL14M5L2M5L10MNO2L2MO2NL49NO2ML5MNO2ML3M3L38O7NL12MO5L2O5ML10MO2L3NONL50O2ML7O2ML3O2ML38O9L11MO5L2O5ML10MO2L3NONL50O2ML7O2ML3O2ML38O10L12NONL4NO2L12MO2L3NONL50O2ML7O2ML3O2ML38MO9NL11NONL4NONL6ML5MO2L3NONL5M2L28M2L10ML2O2ML4M2LO2ML3O2ML39MO8NL11NONL4NO2L4NO3ML3MO2L3NONL4NO3NL8MO3NLNOM2O3NL2MO4ML2MO3MONL2O2ML3NO5ML3O2ML41O7NL11NONL4NO2L3MO5ML2MO2L3NONL3NO5NL7MO4LO2NMO4LMO6L2MO4PNL2O2ML2NO6ML3O2ML42NO5NL11NO9L3O2NLNONL2MO2L3NONL2MO2NLNO2L8NO2M2O2NLNO2MLNO2M2O2NLMNO4NL2O2ML2O2NM2O2ML3O2ML43NO4NL11NO9L2MO2L3O2L2MO2L3NONL2NO2L3O2ML8O2LNO3LMOML2O2ML2NO2L3O2ML4O2MLMO2L3O2ML3O2L43NO5NL11NO2N4O3L2MO2N3O2L2MO2L3NONL2NONL3O2NL8O2LNO3MNOMLMO2ML2MO2L2MO2L5O2MLNO2L3O2ML3O2L42NO6NL11NONL4MO2L2MO7MLMO2L3NONL2NONL3NONL8NOMO2NOMNOL2MO2L3MO2L2MO2L5O2MLNONL3O2ML3O2L40MO8NL11NONL4NO2L2MO2N5L2MO2L3NONL2NONL3NONL8MONONMONO2L2MO2L3MO2L2MO2ML4O2MLNO2L3O2ML3N2L39MO9NL11NONL4MONL2MO2L7MO2L3NONL2NONL3O2MLML6MO3NLO3NL3O2ML2MO2L3O2L5O2MLMO2L3O2ML4ML39O10ML10MO2NML2MNO2ML2O2NL2MNL2NO2MLMNONMLMO2MLNO2LNOML6O3MLO3ML3O2NL2O2NL2MO2ML3MO2NLMO2NLMO2NL3O2ML38O9L11MO5L2O5MLNO5NLNO4LNO4L2O6NLO2NL6NO2L2NO2ML3MO6MLMO4MLMO4NLNO7NLMO2NL38O8L12MO5L2O5ML2NO4MLNO4LNO4L2MO4NL2O2NL6MONL2MO2L5NO4ML2MO4MLMO4ML2O4NO2NL2O2ML38O6NL14M5L2M5L4M3L4M4LM5L4M3L3NOML7ML4M2L6M3L5M4L3M4L4M2L2M2L3M2LM9L29O5NL58O2L56NO6PNL29O4NL58MOML56MN7ML29O3NL60ML95O2NL157M2L7665",
  "L22NOP2ONL42NOP6ONL38MOP10OML34MOP14OML30MOP18OML26MOP22OML22MOP26OML18KOP30OKL15OP34OL12OP38OL8OP17S2R4S2P17OL4NP16SR12SP16NLNP15R18P15MP14SR20SP11ON2P13R24P8O2N3P12R26P5O2N5P11SR27P2O2N7P10SR13S2R13O2N9P10R11P8R8SON11P9SR9SP10SR4SON13P9R9SOP10OSRSON15P9R9P13O2N17P8SR8P12O2N19P8SR8P10O2N21P8R9P8O2N23P8R9P6ON26P8SR8P4O2N27P8SR8P2O2N29P9R8SN14ON17P9R9OMN10MTRSON15P9SR9TN10TR4SON13P10R10SON6OSR7SON11P9OTR12SW2SR12ON10P8ONMSR26SN11P6ON5SR24SN12P4ON8SR22SN13P2ON11TR20TN14MN14OSR16SON14ML2N15OSR12WON15ML4MN16OTS6TON16ML8MN38ML12MN34ML16MN30ML20MN26ML24MN22ML28MN18ML32MN14ML36N12L40MN6ML44N4L23",
};

void show4() {
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT}, eadk_color_white);
  eadk_display_draw_string("decode_and_draw_image((eadk_rect_t)\n{20, 90, 160, 120}, image_code[0])", (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("decode_and_draw_image((eadk_rect_t)\n{220, 110, 50, 50}, image_code[1])", (eadk_point_t){0, 40}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);

  decode_and_draw_image((eadk_rect_t){20, 90, 160, 120}, image_code[0]); 
  decode_and_draw_image((eadk_rect_t){220, 110, 50, 50}, image_code[1]); 

  while (true) {
    eadk_keyboard_state_t state = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(state, eadk_key_home)) {
      show_menu_screen();
      break;
    }
  }
}

void show5() {
  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);
  eadk_display_draw_string("crash_calculator();", (eadk_point_t){0, 0}, true, eadk_color_white, eadk_color_black);
  eadk_display_draw_string("This will crash and Reset the calculator\nAll the data will be deleted", (eadk_point_t){0, 200}, false, eadk_color_white, eadk_color_black);
  eadk_display_draw_string("Presse Home to cancel", (eadk_point_t){60, 80}, true, eadk_color_white, eadk_color_black);
  eadk_display_draw_string("Presse 9 to crash", (eadk_point_t){60, 100}, true, eadk_color_white, eadk_color_black);

  while (1) {
    eadk_keyboard_state_t state = eadk_keyboard_scan();

    if (eadk_keyboard_key_down(state, eadk_key_home)) {
      show_menu_screen();
      break;
    } else if (eadk_keyboard_key_down(state, eadk_key_nine)) {
      crash_calculator();
    }
  }
}

void show6(){
  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("chrono((eadk_point_t){10, 50}, true, 120);", (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);
  chrono((eadk_point_t){10, 50}, true, 120);
  show_menu_screen();
}

void show7(){

  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
  eadk_display_draw_string("Press home to quit", (eadk_point_t){180, 220}, false, eadk_color_black, eadk_color_white);
  eadk_display_draw_string("char* factors = prime_factors(number);", (eadk_point_t){0, 0}, false, eadk_color_black, eadk_color_white);

  int number = 327272400;
  char* factors = prime_factors(number);

  eadk_point_t point = {10, 50};
  eadk_display_draw_string("The prime factors of ", point, true, eadk_color_black, eadk_color_white);
  point.y += 20;
  char number_str[20];
  sprintf(number_str, "%d", number);
  eadk_display_draw_string(number_str, point, true, eadk_color_black, eadk_color_white);
  point.y += 20;
  eadk_display_draw_string("are: ", point, true, eadk_color_black, eadk_color_white);
  point.y += 20;
  eadk_display_draw_string(factors, point, true, eadk_color_black, eadk_color_white);

  while (true) {
    eadk_keyboard_state_t state = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(state, eadk_key_home)) {
      show_menu_screen();
      break;
    }
  }
}

int main() {
  show_menu_screen();

  while (true) {

    eadk_keyboard_state_t state = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(state, eadk_key_home)) {
      break;
    }
    else if (eadk_keyboard_key_down(state, eadk_key_one)) {
      show1();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_two)) {
      show2();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_three)) {
      show3();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_four)) {
      show4();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_four)) {
      show4();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_five)) {
      show5();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_six)) {
      show6();
      eadk_timing_msleep(500);
    }
    else if (eadk_keyboard_key_down(state, eadk_key_seven)) {
      show7();
      eadk_timing_msleep(500);
    }
  }

  return 0;
}
