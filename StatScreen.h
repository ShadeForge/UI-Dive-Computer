/**
 * @author Lucas Balda
 * @date 24.02.2021
 * @brief This stat-screen-class provides a ui for showing dive-statistics as a graphic using lvgl.
 */

#ifndef STAT_SCREEN_H
#define STAT_SCREEN_H

#include <lvgl.h>
#include <list>
#include <math.h>
#include <Arduino.h>

#include "DiveScreen.h"
#include "UISystem.h"
#include "ScreenType.h"
#include "ButtonType.h"

struct NamedSerie {
    String name;
    std::list<int16_t> values;
    int16_t max, min;
};

struct ChartPage {
    std::list<lv_chart_series_t*> series;
    String title, xAxisLabel, yAxisLabel; 
    int16_t min, max;
};

class StatScreen {
    public:
        static lv_obj_t* screenObj;
        static lv_obj_t* chartObj;
        static lv_obj_t* lblSerieNameObj;
        static lv_obj_t* lblSecondsObj;
        
        static std::list<ChartPage*> chartPages;
        static std::list<lv_color_t> chartGraphColors;
        static int8_t currentSeriesIndex;

        static ChartPage depthChartPage;
        static ChartPage temperaturChartPage;
        static ChartPage brightnessChartPage;
        static ChartPage spO2DepthChartPage;
        static ChartPage heartFrequencyDepthChartPage;
        static ChartPage heartVariabilityChartPage;

        static void setup();
        static void showScreen();
        static void processButtonPress(ButtonType);
        static void dataUpdate();
        static void update();
        static void initializeNamedSerie(NamedSerie& serie, const char* name);
        static void processMinMax(NamedSerie& serie);
        static void initializeChartPage(ChartPage& chartPage, NamedSerie& series, int16_t xGaps, int16_t yGaps);
        static void initializeChartPage(ChartPage& chartPage, std::list<NamedSerie>& series, int16_t xGaps, int16_t yGaps);
        static void showCurrentPage();
        static void showNextPage();
        static void disableCurrentPage();
        static void resetChartPages();
};

#endif