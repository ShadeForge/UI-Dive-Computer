/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This stat-screen-class provides a ui for showing dive-statistics as a graphic using lvgl.
 */

#include "StatScreen.h"

lv_obj_t* StatScreen::screenObj;
lv_obj_t* StatScreen::chartObj;
lv_obj_t* StatScreen::lblSerieNameObj;
lv_obj_t* StatScreen::lblSecondsObj;

ChartPage StatScreen::depthChartPage;
ChartPage StatScreen::temperaturChartPage;
ChartPage StatScreen::brightnessChartPage;
ChartPage StatScreen::spO2DepthChartPage;
ChartPage StatScreen::heartFrequencyDepthChartPage;
ChartPage StatScreen::heartVariabilityChartPage;

std::list<ChartPage*> StatScreen::chartPages = std::list<ChartPage*>();
int8_t StatScreen::currentSeriesIndex = 0;

void StatScreen::setup() {
    // Create new Screen
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    // Initialize chart as line-chart with positioning & alignment
    chartObj = lv_chart_create(screenObj, NULL);
    lv_obj_set_size(chartObj, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20);
    lv_obj_align(chartObj, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 20);
    lv_chart_set_type(chartObj, LV_CHART_TYPE_LINE);
    lv_obj_set_style_local_bg_color(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    
    // Gradient transparency effect of the chart
    lv_obj_set_style_local_bg_opa(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50);
    lv_obj_set_style_local_bg_grad_dir(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_bg_grad_stop(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);

    // Chart ticks style
    lv_chart_set_x_tick_length(chartObj, 5, 2);
    lv_chart_set_y_tick_length(chartObj, 5, 2);

    // Chart padding for space of tick labels
    lv_obj_set_style_local_pad_left(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 5 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_bottom(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 3 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_right(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_top(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));

    // Setup series-name-label on top of the screen & changes its font-color
    lblSerieNameObj = lv_label_create(screenObj, NULL);
    lv_obj_align(lblSerieNameObj, NULL, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_text_color(lblSerieNameObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_BRIGHTER);
    lv_label_set_align(lblSerieNameObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_realign(lblSerieNameObj);

    // Setup seconds-label on bottom of the chart & change its font-color
    lblSecondsObj = lv_label_create(screenObj, NULL);
    lv_label_set_text(lblSecondsObj, "Seconds");
    lv_obj_set_pos(lblSecondsObj, (SCREEN_WIDTH - 7 * 8) / 2, SCREEN_HEIGHT - 24);
    lv_obj_set_style_local_text_color(lblSecondsObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_BRIGHTER);
}

void StatScreen::createNamedSerie(NamedSerie& serie, const char* name) {
    serie.name = String(name);
    serie.values = std::list<int16_t>();
    serie.min = 32767;
    serie.max = -32767;
}

void StatScreen::showScreen() {
    // Update stats after diving
    dataUpdate();
    showCurrentPage();
    lv_scr_load(StatScreen::screenObj);
}

void StatScreen::processButtonPress(ButtonType buttonType) {
    // Back to Idle-Screen or show the next series
    if(buttonType == BUTTON1) {
        UISystem::setScreen(IDLE_SCREEN);
    } else if(buttonType == BUTTON2) {
        showNextPage();
    }
}

// Updates all data got from the last dive & inserts them into a chart
void StatScreen::dataUpdate() {

    resetChartPages();

    // Setup base series    
    NamedSerie depthSerie = NamedSerie();
    NamedSerie temperaturSerie = NamedSerie();
    NamedSerie brightnessSerie = NamedSerie();
    NamedSerie o2SaturationSerie = NamedSerie();
    NamedSerie heartFrequencySerie = NamedSerie();
    NamedSerie heartVariabilitySerie = NamedSerie();

    createNamedSerie(depthSerie, "Depth");
    createNamedSerie(temperaturSerie, "Temperatur");
    createNamedSerie(brightnessSerie, "Brightness");
    createNamedSerie(o2SaturationSerie, "O2-Saturation");
    createNamedSerie(heartFrequencySerie, "Heart-Frequency");
    createNamedSerie(heartVariabilitySerie, "Heart-Variablity");

    // Add values to seperate series
    for(DiveData data : UISystem::diveDataSeries) {
        depthSerie.values.push_back(data.depth);
        temperaturSerie.values.push_back(data.temperatur);
        brightnessSerie.values.push_back(data.brightness);
        o2SaturationSerie.values.push_back(data.o2saturation);
        heartFrequencySerie.values.push_back(data.heartFrequency);
        heartVariabilitySerie.values.push_back(data.heartVariability);
    }

    // Get the min/max values of the series
    processMinMax(depthSerie);
    processMinMax(temperaturSerie);
    processMinMax(brightnessSerie);
    processMinMax(o2SaturationSerie);
    processMinMax(heartFrequencySerie);
    processMinMax(heartVariabilitySerie);

    std::list<NamedSerie> spO2DepthPageSeries = std::list<NamedSerie>();
    std::list<NamedSerie> heartFrequencyDepthPageSeries = std::list<NamedSerie>();

    spO2DepthPageSeries.push_back(depthSerie);
    spO2DepthPageSeries.push_back(o2SaturationSerie);
    
    heartFrequencyDepthPageSeries.push_back(depthSerie);
    heartFrequencyDepthPageSeries.push_back(heartFrequencySerie);

    // Create axis-strings for labels
    int16_t xGaps = 7;
    int16_t yGaps = 5;

    initializeChartPage(depthChartPage, depthSerie, xGaps, yGaps);
    initializeChartPage(temperaturChartPage, temperaturSerie, xGaps, yGaps);
    initializeChartPage(brightnessChartPage, brightnessSerie, xGaps, yGaps);
    initializeChartPage(spO2DepthChartPage, spO2DepthPageSeries, xGaps, yGaps);
    initializeChartPage(heartFrequencyDepthChartPage, heartFrequencyDepthPageSeries, xGaps, yGaps);
    initializeChartPage(heartVariabilityChartPage, heartVariabilitySerie, xGaps, yGaps);

    chartPages.push_back(&depthChartPage);
    chartPages.push_back(&temperaturChartPage);
    chartPages.push_back(&brightnessChartPage);
    chartPages.push_back(&spO2DepthChartPage);
    chartPages.push_back(&heartFrequencyDepthChartPage);
    chartPages.push_back(&heartVariabilityChartPage);
}

// For future use ...
void StatScreen::update() {

}

// Calculate the min/max-values, add next point on chart & write them into the referenced series
void StatScreen::processMinMax(NamedSerie& serie) {
    for(int16_t value : serie.values) {
        serie.max = std::max(value, serie.max);
        serie.min = std::min(value, serie.min);
    }
}

void StatScreen::initializeChartPage(ChartPage& chartPage, NamedSerie& series, int16_t xGaps, int16_t yGaps) {
    std::list<NamedSerie> list = std::list<NamedSerie>();
    list.push_back(series);
    initializeChartPage(chartPage, list, xGaps, yGaps);
}

void StatScreen::initializeChartPage(ChartPage& chartPage, std::list<NamedSerie>& series, int16_t xGaps, int16_t yGaps) {

    chartPage.min = 32767;
    chartPage.max = -32767;
    chartPage.series = std::list<lv_chart_series_t*>();
    chartPage.title = String();
    chartPage.xAxisLabel = String("0");
    chartPage.yAxisLabel = String();
    String minValueString = String();

    for(NamedSerie serie : series) {
        if(serie.max - serie.min > chartPage.max - chartPage.min) {
            chartPage.min = serie.min;
            chartPage.max = serie.max;
        }
        chartPage.title += serie.name;
        chartPage.title += String("/");
        chartPage.yAxisLabel += String(serie.max);
        chartPage.yAxisLabel += String("/");
        minValueString += String(serie.min);
        minValueString += String("/");
    }

    chartPage.title.remove(chartPage.title.length() - 1);
    chartPage.yAxisLabel.remove(chartPage.yAxisLabel.length() - 1);
    minValueString.remove(minValueString.length() - 1);

    for (int i = 0; i < xGaps; i++) {
        chartPage.xAxisLabel += String("\n");
    }
    
    for (int i = 0; i < yGaps; i++) {
        chartPage.yAxisLabel += String("\n");
    }
    
    chartPage.xAxisLabel += String((UISystem::diveDataSeries.size()));
    chartPage.yAxisLabel += minValueString;
    for(NamedSerie serie : series) {
        lv_chart_series_t* chartSeries = lv_chart_add_series(chartObj, THEME_PRIMARY_COLOR_NORMAL);
        int i = 0;
        for(int16_t value : serie.values) {
            float propotion = (float)(value - serie.min) / (float)(serie.max - serie.min);
            lv_chart_set_next(chartObj, chartSeries, (chartPage.max - chartPage.min) * propotion);
        }

        lv_chart_hide_series(chartObj, chartSeries, true);
        chartPage.series.push_back(chartSeries);
    }
}

void StatScreen::showCurrentPage() {
    // Enable new next ChartPage
    currentSeriesIndex++;
    currentSeriesIndex %= chartPages.size();
    std::list<ChartPage*>::iterator it = chartPages.begin();
    std::advance(it, currentSeriesIndex);
    ChartPage& page = **it;
    
    // Update the series on the chart
    lv_chart_set_y_range(chartObj, LV_CHART_AXIS_PRIMARY_Y, 0, page.max - page.min);
    lv_chart_set_x_tick_texts(chartObj, page.xAxisLabel.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_y_tick_texts(chartObj, page.yAxisLabel.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    for(lv_chart_series_t* serie : page.series) {
        lv_chart_hide_series(chartObj, serie, false);
    }
    
    // Update Serie-Name-Label
    lv_label_set_text(lblSerieNameObj, page.title.c_str());
    lv_obj_realign(lblSerieNameObj);
}

// Switch to next series in the series-list & show them on the chart
void StatScreen::showNextPage() {
    // Disable current ChartPage
    disableCurrentPage();

    showCurrentPage();
}

void StatScreen::disableCurrentPage() {

    if(chartPages.size() > 0) {
        // Get last shown series
        std::list<ChartPage*>::iterator it = chartPages.begin();
        std::advance(it, currentSeriesIndex);
        ChartPage& page = **it;

        // Hide last shown page
        for(lv_chart_series_t* serie : page.series) {
            lv_chart_hide_series(chartObj, serie, true);
        }
    }
}

void StatScreen::resetChartPages() {

    disableCurrentPage();
    currentSeriesIndex = 0;

    // Set data count (to display the whole curve)
    lv_chart_set_point_count(chartObj, UISystem::diveDataSeries.size());

    depthChartPage = ChartPage();
    temperaturChartPage = ChartPage();
    brightnessChartPage = ChartPage();
    spO2DepthChartPage = ChartPage();
    heartFrequencyDepthChartPage = ChartPage();
    heartVariabilityChartPage = ChartPage();

    for(ChartPage* page : chartPages) {
        for(lv_chart_series_t* serie : page->series) {
            lv_chart_remove_series(chartObj, serie);
        }
    }

    chartPages.clear();
}