/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This idle-screen-class provides a ui for showing a digital-clock and switch to other screens using lvgl.
 */

#include "IdleScreen.h"

// Screen-Object-Declaration
lv_obj_t* IdleScreen::screenObj;

// Time-Label-Declaration
lv_obj_t* IdleScreen::lblTime;

// idle-screen setup for ui-layout & -elements
void IdleScreen::setup() {

    // CLOCK-CONFIGURATION
    // Size of the digital-clock in percent of the screen-size
    float clockWidthPercent = 0.6f;
    float clockHeightPercent = 0.35f;

    // Border configuration of the containers
    int contBorderRadius = 1;
    int contBorderWidth = 2;

    // the digital clock font for the dive-time-label
    lv_font_t& lblDigitalClockFont = digital_numbers_40px;

    // Create screen-object
    screenObj = lv_obj_create(NULL, NULL);

    // ATTENTION: CONFIGRATION ENDS HERE
    // DO NOT CHANGE ANYTHING HERE IF YOU DONT KNOW EXACTLY WHAT YOU ARE DOING

    // Change background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    // DIGITAL-CLOCK-SECTION
    // Digital-Clock-Container-Setup
    static lv_obj_t* contTimeObj = lv_cont_create(screenObj, NULL);
    lv_obj_set_size(contTimeObj, SCREEN_WIDTH * clockWidthPercent, SCREEN_HEIGHT * clockHeightPercent);
    lv_obj_set_pos(contTimeObj, (SCREEN_WIDTH * (1.0f - clockWidthPercent)) / 2, (SCREEN_HEIGHT * (1.0f - clockHeightPercent)) / 2);
    lv_obj_set_style_local_bg_color(contTimeObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_radius(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, contBorderRadius);
    lv_obj_set_style_local_border_width(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, contBorderWidth);
    lv_obj_set_style_local_border_color(contTimeObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    // Shadow-Configuration
    lv_obj_set_style_local_shadow_color(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_shadow_opa(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
    lv_obj_set_style_local_shadow_width(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_shadow_ofs_x(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_shadow_ofs_y(contTimeObj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, -3);

    // Digital-Clock-Label-Setup
    lblTime = lv_label_create(contTimeObj, NULL);
    lv_obj_set_style_local_text_font(lblTime, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lblDigitalClockFont);
    lv_obj_set_style_local_text_color(lblTime, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_align(lblTime, LV_ALIGN_CENTER);
    lv_obj_align(lblTime, NULL, LV_ALIGN_CENTER, 0, 0);
}

// Update time & load idle-screen
void IdleScreen::showScreen() {
    dataUpdate();
    lv_scr_load(IdleScreen::screenObj);
}

// Handle button-presses on idle-screen
void IdleScreen::processButtonPress(ButtonType buttonType) {
    // If Button One is pressed
    if(buttonType == BUTTON1) {
        // If legit-dive-data is available
        if(UISystem::currentDiveData.time > 10) {
            // Button One is pressed & there is legit dive-data so show stat-screen
            UISystem::setScreen(STAT_SCREEN);
        }
    // If Button Two is pressed
    } else if(buttonType == BUTTON2) {
        // Button Two is pressed so switch to option-screen
        UISystem::setScreen(OPTION_SCREEN);
    }
}

// update clock
void IdleScreen::dataUpdate() {
    // Some time-calculations
    long time = UISystem::time;
    long s = time % 60;
    // 60s * 60m = 3600s
    long m = (time % 3600) / 60;
    // 60s * 60m * 24h = 86400s
    long h = (time % 86400) / 3600;
    // Show current time on label
    lv_label_set_text_fmt(lblTime, "%02d:%02d:%02d", h, m, s);
    // Recalculate alignment with new text-size
    lv_obj_realign(lblTime);
}

// Update to new current dive-data & react to depth/time value for switching screens
void IdleScreen::update() {
    // update clock
    dataUpdate();

    // if diver starts diving
    if(UISystem::currentDiveData.depth >= 1) {
        // diver dives so switch to dive-screen
        UISystem::setScreen(DIVE_SCREEN);
    }
}