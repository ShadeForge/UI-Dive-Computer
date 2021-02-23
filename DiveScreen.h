/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This dive-screen-class provides a ui for show dive-data while diving underwater using lvgl.
 */

#ifndef DIVE_SCREEN_H
#define DIVE_SCREEN_H

#include <lvgl.h>
#include <math.h>

#include "UISystem.h"
#include "ButtonType.h"

class DiveScreen {
    public:
        static lv_obj_t* screenObj;

        static lv_obj_t* barDepthObj;
        static lv_obj_t* arcSpO2Obj;
        
        static lv_obj_t* imgTimeObj;
        static lv_obj_t* imgDepthObj;
        static lv_obj_t* imgSpO2Obj;
        static lv_obj_t* imgHeartFrequencyObj;

        static lv_obj_t* lblTimeObj;
        static lv_obj_t* lblDepth;
        static lv_obj_t* lblSpO2Obj;
        static lv_obj_t* lblHeartFrequency;

        static void setup();
        static void showScreen();
        static void processButtonPress(ButtonType);
        static void dataUpdate();
        static void update();
};

#endif