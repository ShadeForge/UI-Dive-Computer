/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This idle-screen-class provides a ui for showing a digital-clock and switch to other screens using lvgl.
 */

#ifndef IDLE_SCREEN_H
#define IDLE_SCREEN_H

#include <lvgl.h>

#include "UISystem.h"
#include "ButtonType.h"

class IdleScreen {
    public:
        static lv_obj_t* screenObj;
        static lv_obj_t* lblTime;

        static void setup();
        static void showScreen();
        static void processButtonPress(ButtonType);
        static void dataUpdate();
        static void update();
};

#endif