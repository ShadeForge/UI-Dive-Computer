/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This option-screen-class provides a ui for showing configurations and switching options using lvgl.
 */

#include "OptionScreen.h"

// Screen-Object-Declaration
lv_obj_t* OptionScreen::screenObj;

// Button-Object-Declarations
lv_obj_t* OptionScreen::buttons[4];

// Current button-selected as index for the buttons-array
int16_t OptionScreen::selectionIndex;

// Button-border-width for selected & unselected buttons
int16_t OptionScreen::btnSelectedBorderWidth;
int16_t OptionScreen::btnUnselectedBorderWidth;

// idle-screen setup for ui-layout & -elements
// Here you can edit the option-screen-ui-elements to fit it to your preferences
// Button-table-layout: currently there are 4 buttons to display
// the layout will be calculated automatically to your configuration
// if you want to add new buttons you can change it here but don't forget to
// initialize your own elements below the configuration-section
void OptionScreen::setup() {

    // BUTTON-CONFIGURATION
    // Buttons per Row & Colum to dynamically calculate the ui-element-sizes
    // ATTENTION!: currently you have to use RowCount*ColumCount sections
    int btnRowCount = 2;
    int btnColumnCount = 2;
    
    // total spacing between the buttons in percent to the screen-size
    // that means if you have 3 spacings and 0.06f total-spacing in percent
    // each spacing have 0.02f spacing in percent to the screen-size
    float btnTotalSpacingWidthPercent = 0.05f;
    float btnTotalSpacingHeightPercent = 0.05f;

    // Border configuration of the buttons (selected & unselected)
    btnSelectedBorderWidth = 3;
    btnUnselectedBorderWidth = 1;
    int btnBorderRadius = 1;

    // ATTENTION: CONFIGRATION ENDS HERE
    // DO NOT CHANGE ANYTHING HERE IF YOU DONT KNOW EXACTLY WHAT YOU ARE DOING

    // CALCULATIONS FOR FUTURE USE
    int spacingRowCount = btnRowCount + 1;
    int spacingColumCount = btnColumnCount + 1;
    int16_t btnWidth = SCREEN_WIDTH * (1.0f - btnTotalSpacingWidthPercent) / btnRowCount;
    int16_t btnHeight = SCREEN_HEIGHT * (1.0f - btnTotalSpacingHeightPercent) / btnColumnCount;
    int16_t spacingX = SCREEN_WIDTH * (btnTotalSpacingWidthPercent / spacingRowCount);
    int16_t spacingY = SCREEN_HEIGHT * (btnTotalSpacingHeightPercent / spacingColumCount);

    // Create screen-object
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    // BLUETOOTH-BUTTON-SECTION
    // Bluetooth-Button-Setup
    buttons[0] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[0], spacingX, spacingY);
    lv_obj_set_size(buttons[0], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnSelectedBorderWidth);
    lv_obj_set_style_local_radius(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnBorderRadius);
    lv_obj_set_style_local_border_color(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label1 = lv_label_create(buttons[0], NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label1, LV_SYMBOL_BLUETOOTH);

    // WATER-TYPE-BUTTON-SECTION
    // Water-Type-Button-Setup
    buttons[1] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[1], spacingX * 2 + btnWidth, spacingY);
    lv_obj_set_size(buttons[1], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnUnselectedBorderWidth);
    lv_obj_set_style_local_radius(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnBorderRadius);
    lv_obj_set_style_local_border_color(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label2 = lv_label_create(buttons[1], NULL);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label2, "Salz/Suesswasser");

    // TEMPERATUR-BUTTON-SECTION
    // Temperatur-Button-Setup
    buttons[2] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[2], spacingX, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[2], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnUnselectedBorderWidth);
    lv_obj_set_style_local_radius(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnBorderRadius);
    lv_obj_set_style_local_border_color(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label3 = lv_label_create(buttons[2], NULL);
    lv_obj_set_style_local_text_color(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label3, "Temperatur");

    // EXIT-BUTTON-SECTION
    // Exit-Button-Setup
    buttons[3] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[3], spacingX * 2 + btnWidth, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[3], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnUnselectedBorderWidth);
    lv_obj_set_style_local_radius(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnBorderRadius);
    lv_obj_set_style_local_border_color(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label4 = lv_label_create(buttons[3], NULL);
    lv_obj_set_style_local_text_font(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label4, LV_SYMBOL_HOME);
}

// Setup button-selection, button-style & load option-screen
void OptionScreen::showScreen() {
    lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnUnselectedBorderWidth);
    selectionIndex = 0;
    lv_obj_set_style_local_border_width(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnSelectedBorderWidth);

    dataUpdate();
    lv_scr_load(OptionScreen::screenObj);
}

// Handle button-presses on option-screen
void OptionScreen::processButtonPress(ButtonType buttonType) {

    // If Button One is pressed
    if(buttonType == BUTTON1) {
        // Button One was pressed so delete current button-border &
        lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnUnselectedBorderWidth);

        // increment selection of button-selection &
        selectionIndex++;
        selectionIndex %= 4;

        // add border to new selected button
        lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, btnSelectedBorderWidth);

    // If Button Two is pressed
    } else if (buttonType == BUTTON2) {
        // Here you can add changes of your application-configuration
        // if a specific option-button was pressed or add new buttons
        // Select current Button
        switch(selectionIndex) {
            case 0:
                // Bluetooth-Button was pressed
                break;
            case 1:
                // Salz-/Süßwasser-Button was pressed
                break;
            case 2:
                // Temperature-Button was pressed
                break;
            case 3:
                // Exit-Button was pressed
                // Switch to idle-screen
                UISystem::setScreen(IDLE_SCREEN);
                break;
        }
    }
}

// update data
// (currently not used)
void OptionScreen::dataUpdate() {
    // Place here code for updating data if needed
}

// Update
// (currently not used)
void OptionScreen::update() {
    // Update data
    dataUpdate();
}