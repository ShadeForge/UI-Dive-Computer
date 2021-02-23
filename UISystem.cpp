/**
 * @author Lucas Balda
 */

#include "UISystem.h"

DiveData UISystem::currentDiveData = DiveData();
bool UISystem::collectingData = false;
std::list<DiveData> UISystem::diveDataSeries = std::list<DiveData>();

float UISystem::gyroX = 0, UISystem::gyroY = 0, UISystem::gyroZ = 0;
float UISystem::accelX = 0, UISystem::accelY = 0, UISystem::accelZ = 0;
float UISystem::magnetField = 0;
long UISystem::time = 0;

#if DIVE_SIMULATION
int16_t UISystem::diveAccel = -1;
#endif

ScreenType UISystem::currentScreen;

void UISystem::setup() {
    lv_init();
    
    DiveScreen::setup();
    IdleScreen::setup();
    OptionScreen::setup();
    StatScreen::setup();

    #pragma region Dive-Simulation-Button-Initialization
    #if DIVE_SIMULATION
        initializeButtons(DiveScreen::screenObj);
        initializeButtons(IdleScreen::screenObj);
        initializeButtons(OptionScreen::screenObj);
        initializeButtons(StatScreen::screenObj);
    #endif
    #pragma endregion

    lv_task_create([](lv_task_t* task){

        #pragma region Dive-Simulation-DumpData-Generation
        #if DIVE_SIMULATION
            float sinTemp = sinf((lv_tick_get() / 10000.0f) * PI);
            float cosTemp = cosf((lv_tick_get() / 10000.0f) * PI);
            
            currentDiveData.temperatur = sinTemp * 10.0f + 10;
            currentDiveData.brightness = sinTemp * 75.0f + 25; // 0 - 255
            currentDiveData.o2saturation = sinTemp * 5.0f + 94; // 0 - 100
            currentDiveData.heartFrequency = sinTemp * 40.0f + 60; // 0 - 255
            currentDiveData.heartVariability = sinTemp * 20.0f + 60; 
            accelX = sinTemp;
            accelY = cosTemp;
            accelZ = (sinTemp + cosTemp) / 2.0f;
            gyroX = sinTemp;
            gyroY = cosTemp;
            gyroZ = (sinTemp + cosTemp) / 2.0f;
            currentDiveData.depth += diveAccel;

            if(currentDiveData.depth < 0)
                currentDiveData.depth = 0;

            if(currentDiveData.depth >= 20)
                currentDiveData.depth = 20;
        #endif
        #pragma endregion

        // ToDo: Insert here your Data Update Logic
        
        switch(currentScreen) {
            case DIVE_SCREEN:
                DiveScreen::update();
                break;
            case IDLE_SCREEN:
                IdleScreen::update();
                break;
            case OPTION_SCREEN:
                OptionScreen::update();
                break;
            case STAT_SCREEN:
                StatScreen::update();
                break;
        }
    }, 500, LV_TASK_PRIO_MID, NULL);
    
    lv_task_create([](lv_task_t* task){
        time = lv_tick_get() / 1000;

        if(collectingData) {
            currentDiveData.time++;
            diveDataSeries.push_back(currentDiveData);
        }
    }, 1000, LV_TASK_PRIO_MID, NULL);
    
    setScreen(IDLE_SCREEN);
}

void UISystem::setScreen(ScreenType screenType) {
    
    switch(screenType) {
        case IDLE_SCREEN:
            IdleScreen::showScreen();
            break;
        case DIVE_SCREEN:
            currentDiveData.time = 0;
            collectingData = true;
            diveDataSeries.clear();
            diveDataSeries.push_back(currentDiveData);
            DiveScreen::showScreen();
            break;
        case OPTION_SCREEN:
            OptionScreen::showScreen();
            break;
        case STAT_SCREEN:
            collectingData = false;
            StatScreen::showScreen();
            break;
    }

    currentScreen = screenType;
}

void UISystem::start() {
    
}

#pragma region Dive-Simulation-Buttons
#if DIVE_SIMULATION
void UISystem::initializeButtons(lv_obj_t* screen) {

    float buttonWidthPercent = 0.35f;
    float buttonHeightPercent = 0.35f;
    int buttonWidth = SCREEN_WIDTH * buttonWidthPercent;
    int buttonHeight = DS_AREA_HEIGHT * buttonHeightPercent;
    int xSpacing = SCREEN_WIDTH * (1.0 - buttonWidthPercent * 2) / 3;
    int ySpacing = DS_AREA_HEIGHT * (1.0 - buttonHeightPercent * 2) / 3;

    lv_obj_t * btn1 = lv_btn_create(screen, NULL);
    lv_obj_set_pos(btn1, xSpacing, SCREEN_HEIGHT + ySpacing);
    lv_obj_set_size(btn1, buttonWidth, buttonHeight);
    lv_obj_set_event_cb(btn1, [](lv_obj_t * btn, lv_event_t event){
        if(event == LV_EVENT_RELEASED) {
            switch(currentScreen) {
                case IDLE_SCREEN:
                    IdleScreen::processButtonPress(BUTTON1);
                    break;
                case DIVE_SCREEN:
                    DiveScreen::processButtonPress(BUTTON1);
                    break;
                case OPTION_SCREEN:
                    OptionScreen::processButtonPress(BUTTON1);
                    break;
                case STAT_SCREEN:
                    StatScreen::processButtonPress(BUTTON1);
                    break;
            }
        }
    });
    
    lv_obj_t * label1 = lv_label_create(btn1, NULL);
    lv_label_set_text(label1, "Button1");
    
    lv_obj_t * btn2 = lv_btn_create(screen, NULL);
    lv_obj_set_pos(btn2, xSpacing * 2 + buttonWidth, SCREEN_HEIGHT + ySpacing);
    lv_obj_set_size(btn2, buttonWidth, buttonHeight);
    lv_obj_set_event_cb(btn2, [](lv_obj_t * btn, lv_event_t event){
        if(event == LV_EVENT_RELEASED) {
            switch(currentScreen) {
                case IDLE_SCREEN:
                    IdleScreen::processButtonPress(BUTTON2);
                    break;
                case DIVE_SCREEN:
                    DiveScreen::processButtonPress(BUTTON2);
                    break;
                case OPTION_SCREEN:
                    OptionScreen::processButtonPress(BUTTON2);
                    break;
                case STAT_SCREEN:
                    StatScreen::processButtonPress(BUTTON2);
                    break;
            }
        }
    });

    lv_obj_t * label2 = lv_label_create(btn2, NULL);
    lv_label_set_text(label2, "Button2");
    
    lv_obj_t * btn3 = lv_btn_create(screen, NULL);
    lv_obj_set_pos(btn3, xSpacing, SCREEN_HEIGHT + ySpacing * 2 + buttonHeight);
    lv_obj_set_size(btn3, buttonWidth, buttonHeight);
    lv_obj_set_event_cb(btn3, [](lv_obj_t * btn, lv_event_t event){
        diveAccel = 1;
    });

    lv_obj_t * label3 = lv_label_create(btn3, NULL);
    lv_label_set_text(label3, "Dive down");
    
    lv_obj_t * btn4 = lv_btn_create(screen, NULL);
    lv_obj_set_pos(btn4, xSpacing * 2 + buttonWidth , SCREEN_HEIGHT + ySpacing * 2 + buttonHeight);
    lv_obj_set_size(btn4, buttonWidth, buttonHeight);
    lv_obj_set_event_cb(btn4, [](lv_obj_t * btn, lv_event_t event){
        diveAccel = -1;
    });

    lv_obj_t * label4 = lv_label_create(btn4, NULL);
    lv_label_set_text(label4, "Dive up");
}
#endif
#pragma endregion