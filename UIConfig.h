/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This configuration-header is for configurating the UI-System for diving.
 */

#ifndef UICONFIG_H
#define UICONFIG_H

#include <lvgl.h>

// Hera are the general-configuration of the dive-computer-ui
// You can edit images, hardspecifics (like screen-size), fonts and theme-colors

// If you changing the screen-size make sure you change it in the lv_conf.h too
// (or lv_conf_internal.h if you have the same problems as i encountered)
#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

// Declaration of the images for the dive-screen
LV_IMG_DECLARE(heart_image)
LV_IMG_DECLARE(depth_image)
LV_IMG_DECLARE(lungs_image)
LV_IMG_DECLARE(sandclock_image)

// Declaration of the digital-clock-font for the dive- & idle-screen
LV_FONT_DECLARE(digital_numbers_40px)

// Theme-Configuration
// Here you can change the colors of the theme.
// if you change the normal-colors all other colors (dark, bright, darker, brighter)
// will be automatically changed to your color (just darker, brighter).
// if you want to make some versions darker or brighter then it is
// change the last parameter to value between 0 to 255 (0 is dark, 255 is bright)

// The primary-color
// used for texts, lines, borders, important-elements
#define THEME_PRIMARY_COLOR_NORMAL lv_color_make(210, 110, 0)
#define THEME_PRIMARY_COLOR_BRIGHT lv_color_mix(THEME_PRIMARY_COLOR_NORMAL, LV_COLOR_WHITE, 255 * 0.5)
#define THEME_PRIMARY_COLOR_DARK lv_color_mix(THEME_PRIMARY_COLOR_NORMAL, LV_COLOR_BLACK, 255 * 0.5)
#define THEME_PRIMARY_COLOR_BRIGHTER lv_color_mix(THEME_PRIMARY_COLOR_NORMAL, LV_COLOR_WHITE, 255 * 0.25)
#define THEME_PRIMARY_COLOR_DARKER lv_color_mix(THEME_PRIMARY_COLOR_NORMAL, LV_COLOR_BLACK, 255 * 0.25)

// The secondary-color
// used for backgrounds, containers
#define THEME_SECONDARY_COLOR_NORMAL lv_color_make(91, 91, 91)
#define THEME_SECONDARY_COLOR_BRIGHT lv_color_mix(THEME_SECONDARY_COLOR_NORMAL, LV_COLOR_WHITE, 255 * 0.5)
#define THEME_SECONDARY_COLOR_DARK lv_color_mix(THEME_SECONDARY_COLOR_NORMAL, LV_COLOR_BLACK, 255 * 0.5)
#define THEME_SECONDARY_COLOR_BRIGHTER lv_color_mix(THEME_SECONDARY_COLOR_NORMAL, LV_COLOR_WHITE, 255 * 0.25)
#define THEME_SECONDARY_COLOR_DARKER lv_color_mix(THEME_SECONDARY_COLOR_NORMAL, LV_COLOR_BLACK, 255 * 0.25)

#endif