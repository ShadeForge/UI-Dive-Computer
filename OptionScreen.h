/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This option-screen-class provides a ui for showing configurations and switching options using lvgl.
 */

#ifndef OPTION_SCREEN_H
#define OPTION_SCREEN_H

#include <lvgl.h>

#include "UISystem.h"
#include "ButtonType.h"

class OptionScreen {
    public:
        static lv_obj_t* screenObj;
        
        static lv_obj_t* buttons[4];

        static int16_t selectionIndex;

        static int16_t btnSelectedBorderWidth;
        static int16_t btnUnselectedBorderWidth;

        static void setup();
        static void showScreen();
        static void processButtonPress(ButtonType);
        static void dataUpdate();
        static void update();
};

#endif