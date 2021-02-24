/**
 * @author Lucas Balda
 * @date 24.02.2021
 * @brief This stat-screen-class provides a ui for showing dive-statistics as a graphic using lvgl.
 */

#include "StatScreen.h"

// Screen-object & ui-elments
lv_obj_t* StatScreen::screenObj;
lv_obj_t* StatScreen::chartObj;
lv_obj_t* StatScreen::lblSerieNameObj;
lv_obj_t* StatScreen::lblSecondsObj;

// Chartpages to hold the instance alive
// Needed because String will be wiped
ChartPage StatScreen::depthChartPage;
ChartPage StatScreen::temperaturChartPage;
ChartPage StatScreen::brightnessChartPage;
ChartPage StatScreen::spO2DepthChartPage;
ChartPage StatScreen::heartFrequencyDepthChartPage;
ChartPage StatScreen::heartVariabilityChartPage;

// Chart-Pages for easier iteration through all pages
std::list<ChartPage*> StatScreen::chartPages = std::list<ChartPage*>();

// Current chart-page-index in the chart-page-list
int8_t StatScreen::currentSeriesIndex = 0;

// colors of graphs to make it more appealing and readable
std::list<lv_color_t> StatScreen::chartGraphColors = std::list<lv_color_t>();

// stat-screen setup for ui-layout & -elements
// Here you can edit the stat-screen-ui-elements to fit it to your preferences
void StatScreen::setup() {

    // CHART-CONFIGURATION
    // Size of the chart in percent of the screen-size
    float chartWidthPercent = 0.95f;
    float chartHeightPercent = 0.95f;

    // Padding of the graphic
    // A bit of spacing is needed for the labels on the axis
    float chartLeftPaddingPercent = 0.2f;
    float chartRightPaddingPercent = 0.06f;
    float chartTopPaddingPercent = 0.08f;
    float chartBottomPaddingPercent = 0.15f;

    // Border configuration of the chart
    int chartBorderRadius = 1;
    int chartBorderWidth = 2;

    // Chart-Graph-Colors
    // Make sure you have at least 1 color in the list
    chartGraphColors.push_back(THEME_PRIMARY_COLOR_NORMAL);
    chartGraphColors.push_back(LV_COLOR_AQUA);

    // Chart-Position-Y-Offset
    // Is needed to take some space for the chart-page-title into account
    float chartYOffsetPercent = 0.05f;

    // Seconds-Label on the bottom
    // A spacing between the bottom and the label for aestics
    float lblSecondsYOffesetPercent = 0.075f;

    // ATTENTION: CONFIGRATION ENDS HERE
    // DO NOT CHANGE ANYTHING HERE IF YOU DONT KNOW EXACTLY WHAT YOU ARE DOING

    // Create new Screen
    screenObj = lv_obj_create(NULL, NULL);

    // Change screen-background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    // Initialize chart as line-chart with positioning & alignment
    chartObj = lv_chart_create(screenObj, NULL);
    lv_obj_set_size(chartObj, SCREEN_WIDTH * chartWidthPercent, SCREEN_HEIGHT * chartHeightPercent);
    lv_obj_align(chartObj, NULL, LV_ALIGN_IN_TOP_LEFT, SCREEN_WIDTH * (1 - chartWidthPercent) / 2.0f, SCREEN_HEIGHT * ((1 - chartHeightPercent) / 2.0f + chartYOffsetPercent));
    lv_chart_set_type(chartObj, LV_CHART_TYPE_LINE);
    lv_obj_set_style_local_bg_color(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_set_style_local_border_width(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, chartBorderWidth);
    lv_obj_set_style_local_radius(chartObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, chartBorderRadius);
    
    // Gradient transparency effect of the chart
    lv_obj_set_style_local_bg_opa(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50);
    lv_obj_set_style_local_bg_grad_dir(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255);
    lv_obj_set_style_local_bg_grad_stop(chartObj, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);

    // Chart ticks style
    lv_chart_set_x_tick_length(chartObj, 5, 2);
    lv_chart_set_y_tick_length(chartObj, 5, 2);

    // Chart padding for space of tick labels
    lv_obj_set_style_local_pad_left(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, SCREEN_WIDTH * chartLeftPaddingPercent);
    lv_obj_set_style_local_pad_bottom(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, SCREEN_HEIGHT * chartBottomPaddingPercent);
    lv_obj_set_style_local_pad_right(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, SCREEN_WIDTH * chartRightPaddingPercent);
    lv_obj_set_style_local_pad_top(chartObj,  LV_CHART_PART_BG, LV_STATE_DEFAULT, SCREEN_HEIGHT * chartTopPaddingPercent);

    // Setup series-name-label on top of the screen & changes its font-color
    lblSerieNameObj = lv_label_create(screenObj, NULL);
    lv_obj_align(lblSerieNameObj, NULL, LV_ALIGN_IN_TOP_MID, 0, 2);
    lv_obj_set_style_local_text_color(lblSerieNameObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_BRIGHTER);
    lv_label_set_align(lblSerieNameObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_realign(lblSerieNameObj);

    // Setup seconds-label on bottom of the chart & change its font-color
    lblSecondsObj = lv_label_create(screenObj, NULL);
    lv_obj_align(lblSecondsObj, NULL, LV_ALIGN_IN_TOP_MID, SCREEN_WIDTH * (chartLeftPaddingPercent - chartRightPaddingPercent) / 2.0f, SCREEN_HEIGHT * (1.0f - lblSecondsYOffesetPercent));
    lv_label_set_text(lblSecondsObj, "Seconds");
    lv_label_set_align(lblSecondsObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(lblSecondsObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_BRIGHTER);
    lv_obj_realign(lblSecondsObj);
}

// Initialize a named Series with default values with a out-parameter
// out parameter is needed because instanciating objects with Strings wipes the String values
void StatScreen::initializeNamedSerie(NamedSerie& serie, const char* name) {
    serie.name = String(name);
    serie.values = std::list<int16_t>();
    serie.min = 32767;
    serie.max = -32767;
}

// Reset/Update chart & load stat-screen
void StatScreen::showScreen() {
    // Update stats after diving
    dataUpdate();
    // Show current chart-page
    showCurrentPage();
    lv_scr_load(StatScreen::screenObj);
}


void StatScreen::processButtonPress(ButtonType buttonType) {
    // If Button One is pressed
    if(buttonType == BUTTON1) {
        // Button One was pressed so switch back to Idle-Screen
        UISystem::setScreen(IDLE_SCREEN);
    // If Button Two is pressed
    } else if(buttonType == BUTTON2) {
        // Button Two was pressed so show next chart-page
        showNextPage();
    }
}

// Updates all data got from the last dive & inserts them into a chart
void StatScreen::dataUpdate() {

    // Reset current chart-pages
    resetChartPages();

    // Setup base series
    NamedSerie depthSerie = NamedSerie();
    NamedSerie temperaturSerie = NamedSerie();
    NamedSerie brightnessSerie = NamedSerie();
    NamedSerie o2SaturationSerie = NamedSerie();
    NamedSerie heartFrequencySerie = NamedSerie();
    NamedSerie heartVariabilitySerie = NamedSerie();

    // Initialize all series with default values
    initializeNamedSerie(depthSerie, "Depth");
    initializeNamedSerie(temperaturSerie, "Temperatur");
    initializeNamedSerie(brightnessSerie, "Brightness");
    initializeNamedSerie(o2SaturationSerie, "O2-Saturation");
    initializeNamedSerie(heartFrequencySerie, "Heart-Frequency");
    initializeNamedSerie(heartVariabilitySerie, "Heart-Variablity");

    // Add dive-data-values to seperate series
    for(DiveData data : UISystem::diveDataSeries) {
        depthSerie.values.push_back(data.depth);
        temperaturSerie.values.push_back(data.temperatur);
        brightnessSerie.values.push_back(data.brightness);
        o2SaturationSerie.values.push_back(data.o2saturation);
        heartFrequencySerie.values.push_back(data.heartFrequency);
        heartVariabilitySerie.values.push_back(data.heartVariability);
    }

    // Process the min/max values of the series
    processMinMax(depthSerie);
    processMinMax(temperaturSerie);
    processMinMax(brightnessSerie);
    processMinMax(o2SaturationSerie);
    processMinMax(heartFrequencySerie);
    processMinMax(heartVariabilitySerie);

    // Lists for chart-pages with multiple graphs
    std::list<NamedSerie> spO2DepthPageSeries = std::list<NamedSerie>();
    std::list<NamedSerie> heartFrequencyDepthPageSeries = std::list<NamedSerie>();

    // Add graphs for SpO2/Depth-chart-page
    spO2DepthPageSeries.push_back(depthSerie);
    spO2DepthPageSeries.push_back(o2SaturationSerie);
    
    // Add graphs for HeartFrequency/Depth-chart-page
    heartFrequencyDepthPageSeries.push_back(depthSerie);
    heartFrequencyDepthPageSeries.push_back(heartFrequencySerie);

    // Create axis-strings for labels
    // Needed to fill tick-lables which are not used
    int16_t xGaps = 7;
    int16_t yGaps = 5;

    // Initialize/Process data to seperate chart-pages
    initializeChartPage(depthChartPage, depthSerie, xGaps, yGaps);
    initializeChartPage(temperaturChartPage, temperaturSerie, xGaps, yGaps);
    initializeChartPage(brightnessChartPage, brightnessSerie, xGaps, yGaps);
    initializeChartPage(spO2DepthChartPage, spO2DepthPageSeries, xGaps, yGaps);
    initializeChartPage(heartFrequencyDepthChartPage, heartFrequencyDepthPageSeries, xGaps, yGaps);
    initializeChartPage(heartVariabilityChartPage, heartVariabilitySerie, xGaps, yGaps);

    // Add all pages as reference into the chart-page-list
    // Is needed to hold the references to the Strings
    chartPages.push_back(&depthChartPage);
    chartPages.push_back(&temperaturChartPage);
    chartPages.push_back(&brightnessChartPage);
    chartPages.push_back(&spO2DepthChartPage);
    chartPages.push_back(&heartFrequencyDepthChartPage);
    chartPages.push_back(&heartVariabilityChartPage);
}

// Update screen
// (Currently not used)
void StatScreen::update() {
}

// Calculate the min/max-values, add next point on chart & write them into the referenced series
void StatScreen::processMinMax(NamedSerie& serie) {
    for(int16_t value : serie.values) {
        serie.max = std::max(value, serie.max);
        serie.min = std::min(value, serie.min);
    }
}

// Initialize/Process a chart-page with a single graph
void StatScreen::initializeChartPage(ChartPage& chartPage, NamedSerie& series, int16_t xGaps, int16_t yGaps) {
    std::list<NamedSerie> list = std::list<NamedSerie>();
    list.push_back(series);
    initializeChartPage(chartPage, list, xGaps, yGaps);
}

// Initialize/Process a chart-page with multiple graphs
void StatScreen::initializeChartPage(ChartPage& chartPage, std::list<NamedSerie>& series, int16_t xGaps, int16_t yGaps) {

    // Initialize default values
    chartPage.min = 32767;
    chartPage.max = -32767;
    chartPage.series = std::list<lv_chart_series_t*>();
    chartPage.title = String();
    chartPage.xAxisLabel += "0";
    chartPage.yAxisLabel = String();
    String minValueString = String();

    // Process min/max values, use them to add them to the axis-labels & create chart-page-title
    // the min/max values with highest distance between them will be used for linear-interpolation/normalization
    for(NamedSerie serie : series) {
        if(serie.max - serie.min > chartPage.max - chartPage.min) {
            chartPage.min = serie.min;
            chartPage.max = serie.max;
        }
        chartPage.title += serie.name;
        chartPage.title += "/";
        chartPage.yAxisLabel += String(serie.max);
        chartPage.yAxisLabel += "/";
        minValueString += String(serie.min);
        minValueString += "/";
    }

    // Remove last letter "/" from strings
    chartPage.title.remove(chartPage.title.length() - 1);
    chartPage.yAxisLabel.remove(chartPage.yAxisLabel.length() - 1);
    minValueString.remove(minValueString.length() - 1);

    // Fill unused x-axis-labels
    for (int i = 0; i < xGaps; i++) {
        chartPage.xAxisLabel += String("\n");
    }
    // Fill unused y-axis-labels
    for (int i = 0; i < yGaps; i++) {
        chartPage.yAxisLabel += String("\n");
    }
    
    // Append last/first values of the axis-labels
    chartPage.xAxisLabel += String((UISystem::diveDataSeries.size()));
    chartPage.yAxisLabel += minValueString;

    // Iterator for graph-colors
    std::list<lv_color_t>::iterator colorIt = chartGraphColors.begin();

    // Create normalized lv_chart_series for each graph
    for(NamedSerie serie : series) {
        // Create lv_chart_series with a color
        lv_chart_series_t* chartSeries = lv_chart_add_series(chartObj, *colorIt);
        // Next color
        colorIt++;
        // All colors used so start from beginning
        if(colorIt == chartGraphColors.end()) {
            colorIt = chartGraphColors.begin();
        }
        
        // Add data-point on lv_chart_series as normalized value
        for(int16_t value : serie.values) {
            // Linear-interpolation for normalized values
            float propotion = (float)(value - serie.min) / (float)(serie.max - serie.min);
            // Add normalized point on lv_chart_series
            lv_chart_set_next(chartObj, chartSeries, (chartPage.max - chartPage.min) * propotion);
        }

        // Hide lv_chart_series by default & add them into chart-page
        lv_chart_hide_series(chartObj, chartSeries, true);
        chartPage.series.push_back(chartSeries);
    }
}

// Load current chart-page into the chart
void StatScreen::showCurrentPage() {
    // Enable new next ChartPage
    std::list<ChartPage*>::iterator it = chartPages.begin();
    std::advance(it, currentSeriesIndex);
    ChartPage& page = **it;
    
    // Update chart-axis & -range with the new chart-page
    lv_chart_set_y_range(chartObj, LV_CHART_AXIS_PRIMARY_Y, 0, page.max - page.min);
    lv_chart_set_x_tick_texts(chartObj, page.xAxisLabel.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_y_tick_texts(chartObj, page.yAxisLabel.c_str(), 1, LV_CHART_AXIS_DRAW_LAST_TICK);

    // Show all series of the chart-page
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

    // Increase to next chart-page-index
    currentSeriesIndex++;
    // Start from beginning if index exceeds chart-page-list
    currentSeriesIndex %= chartPages.size();

    // Disaplay new chart-page
    showCurrentPage();
}

// Disable the current chart-page (used for show a new chart-page)
void StatScreen::disableCurrentPage() {
    // checking if there is actually data from last time
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

// Reset the chart-pages for new dive-data
void StatScreen::resetChartPages() {
    // Reset usage from last time
    disableCurrentPage();
    currentSeriesIndex = 0;

    // Set data count (to display the whole curve)
    lv_chart_set_point_count(chartObj, UISystem::diveDataSeries.size());

    // Create/Reset instances of the chart-pages
    depthChartPage = ChartPage();
    temperaturChartPage = ChartPage();
    brightnessChartPage = ChartPage();
    spO2DepthChartPage = ChartPage();
    heartFrequencyDepthChartPage = ChartPage();
    heartVariabilityChartPage = ChartPage();

    // Remove all series of all chart-pages from last time
    for(ChartPage* page : chartPages) {
        for(lv_chart_series_t* serie : page->series) {
            lv_chart_remove_series(chartObj, serie);
        }
    }

    // Clear chart-page-list
    chartPages.clear();
}