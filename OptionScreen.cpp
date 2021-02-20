/**
 * @author Lucas Balda
 */

#include "OptionScreen.h"

lv_obj_t* OptionScreen::screenObj;
int16_t OptionScreen::selectionIndex;
lv_obj_t* OptionScreen::buttons[4];
lv_style_t OptionScreen::styleSelected;
lv_style_t OptionScreen::styleDefault;

void OptionScreen::setup() {
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    static int16_t btnWidth = 200;
    static int16_t btnHeight = 140;
    int16_t spacingX = (SCREEN_WIDTH - btnWidth * 2) / 3;
    int16_t spacingY = (SCREEN_HEIGHT - btnHeight * 2) / 3;

    buttons[0] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[0], spacingX, spacingY);
    lv_obj_set_size(buttons[0], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_radius(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label1 = lv_label_create(buttons[0], NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label1, LV_SYMBOL_BLUETOOTH);

    buttons[1] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[1], spacingX * 2 + btnWidth, spacingY);
    lv_obj_set_size(buttons[1], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[1], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label2 = lv_label_create(buttons[1], NULL);
    //lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label2, "Salz/Suesswasser");

    buttons[2] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[2], spacingX, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[2], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[2], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label3 = lv_label_create(buttons[2], NULL);
    //lv_obj_set_style_local_text_font(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label3, "Temperatur");

    buttons[3] = lv_btn_create(screenObj, NULL);
    lv_obj_set_pos(buttons[3], spacingX * 2 + btnWidth, spacingY * 2 + btnHeight);
    lv_obj_set_size(buttons[3], btnWidth, btnHeight);
    lv_obj_set_style_local_bg_color(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_width(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(buttons[3], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_t * label4 = lv_label_create(buttons[3], NULL);
    lv_obj_set_style_local_text_font(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_40);
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_label_set_text(label4, LV_SYMBOL_HOME);
}

void OptionScreen::showScreen() {
    lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    selectionIndex = 0;
    lv_obj_set_style_local_border_width(buttons[0], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    dataUpdate();
    lv_scr_load(OptionScreen::screenObj);
}

void OptionScreen::processButtonPress(ButtonType buttonType) {
    if(buttonType == BUTTON1) {
        lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
        selectionIndex++;
        selectionIndex %= 4;
        lv_obj_set_style_local_border_width(buttons[selectionIndex], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 3);
    } else {
        switch(selectionIndex) {
            case 0:
                // Bluetooth
                break;
            case 1:
                // Salz-/Süßwasser
                break;
            case 2:
                // Temperature
                break;
            case 3:
                // Exit
                UISystem::setScreen(IDLE_SCREEN);
                break;
        }
    }
}

void OptionScreen::dataUpdate() {

}

void OptionScreen::update() {

    dataUpdate();
}