/**
 * @author Lucas Balda
 * @date 23.02.2021
 * @brief This dive-screen-class provides a ui for show dive-data while diving underwater using lvgl.
 */

#include "DiveScreen.h"

// Screen-Object-Declaration
lv_obj_t* DiveScreen::screenObj;

// Depth-Bar-Declaration
lv_obj_t* DiveScreen::barDepthObj;

// SpO2-Arc-Declaration
lv_obj_t* DiveScreen::arcSpO2Obj;

// Image-Declarations
lv_obj_t* DiveScreen::imgTimeObj;
lv_obj_t* DiveScreen::imgDepthObj;
lv_obj_t* DiveScreen::imgSpO2Obj;
lv_obj_t* DiveScreen::imgHeartFrequencyObj;

// Label-Declarations
lv_obj_t* DiveScreen::lblTimeObj;
lv_obj_t* DiveScreen::lblDepth;
lv_obj_t* DiveScreen::lblSpO2Obj;
lv_obj_t* DiveScreen::lblHeartFrequency;

// dive-screen setup for ui-layout & -elements
// Here you can edit the dive-screen-ui-elements to fit it to your preferences
// Container-table-layout: currently there are 4 container to display
// the layout will be calculated automatically to your configuration
// if you want to add new container you can change it here but don't forget to
// initialize your own elements below the configuration-section
void DiveScreen::setup() {
    
    // CONTAINER-CONFIGURATION
    // Container per Row & Colum to dynamically calculate the ui-element-sizes
    // ATTENTION!: currently you have to use RowCount*ColumCount sections
    int contRowCount = 2;
    int contColumnCount = 2;

    // total spacing between the containers in percent to the screen-size
    // that means if you have 3 spacings and 0.06f total-spacing in percent
    // each spacing have 0.02f spacing in percent to the screen-size
    float contTotalSpacingWidthPercent = 0.05f;
    float contTotalSpacingHeightPercent = 0.05f;

    // Border configuration of the containers
    int contBorderRadius = 1;
    int contBorderWidth = 2;

    // FONT-CONFIGURATION
    // the general font for the labels
    lv_font_t& lblFont = lv_font_montserrat_40;

    // the digital clock font for the dive-time-label
    lv_font_t& lblDigitalClockFont = digital_numbers_40px;

    // IMAGE-CONFIGURATION
    // the img-source of each image in the corner of the containers
    const lv_img_dsc_t& imgHeartFrequencySource = heart_image;
    const lv_img_dsc_t& imgTimeSource = sandclock_image;
    const lv_img_dsc_t& imgDepthSource = depth_image;
    const lv_img_dsc_t& imgSpO2Source = lungs_image;

    // the spacing between the container-edge and the image in percent to the container-size
    float imgCornerSpacingPercent = 0.02f;

    // DEPTH-BAR-CONFIGURATION
    // size of the depthbar in percent
    float depthBarHeightPercent = 0.9f;
    float depthBarWidthPercent = 0.05f;
    
    // the max/min-value of the depth
    int depthBarMin = 0;
    int depthBarMax = 20;

    // SpO2-ARC-CONFIGURATION
    // Size of the arc in percent to the container-size
    float arcSizePercent = 1.20f;

    // the max/min-value of the SpO2
    int arcMinRange = 0;
    int arcMaxRange = 100;

    // Arc-Position-Offset to the center-alignment in percent to the container-size
    float arcPositionOffsetXPercent = 0.0f;
    float arcPositionOffsetYPercent = 0.05f;

    // ATTENTION: CONFIGRATION ENDS HERE
    // DO NOT CHANGE ANYTHING HERE IF YOU DONT KNOW EXACTLY WHAT YOU ARE DOING

    // CALCULATIONS FOR FUTURE USE
    int spacingRowCount = contRowCount + 1;
    int spacingColumnCount = contColumnCount + 1;
    int16_t contWidth = SCREEN_WIDTH * (1.0f - contTotalSpacingWidthPercent) / contRowCount;
    int16_t contHeight = SCREEN_HEIGHT * (1.0f - contTotalSpacingHeightPercent) / contColumnCount;
    int16_t spacingX = SCREEN_WIDTH * (contTotalSpacingWidthPercent / spacingRowCount);
    int16_t spacingY = SCREEN_HEIGHT * (contTotalSpacingHeightPercent / spacingColumnCount);

    // use smallest size of the container-sides
    // this is used because this does changes the center-alignment offset-position
    // if arcWidth is different to the arcHeight
    int arcSize = std::min(contWidth, contHeight) * arcSizePercent;

    // Create screen-object
    screenObj = lv_obj_create(NULL, NULL);
    // Change background-color
    lv_obj_set_style_local_bg_color(screenObj, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARK);

    // TIME-SECTION
    // Time-Container-Setup
    lv_obj_t* contTime = lv_cont_create(screenObj, NULL);
    lv_obj_set_size(contTime, contWidth, contHeight);
    lv_obj_set_pos(contTime, spacingX, spacingY);
    lv_obj_set_style_local_border_width(contTime, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderWidth);
    lv_obj_set_style_local_radius(contTime, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderRadius);
    lv_obj_set_style_local_bg_color(contTime, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(contTime, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    // Time-Label-Setup
    lblTimeObj = lv_label_create(contTime, NULL);
    lv_obj_set_style_local_text_font(lblTimeObj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lblDigitalClockFont);
    lv_label_set_align(lblTimeObj, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lblTimeObj, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_text_color(lblTimeObj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    // Time-Image-Setup
    imgTimeObj = lv_img_create(contTime, NULL);
    lv_img_set_src(imgTimeObj, &imgTimeSource);
    lv_obj_align(imgTimeObj, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -contWidth * imgCornerSpacingPercent, -contHeight * imgCornerSpacingPercent);

    // DEPTH-SECTION
    // Depth-Container-Setup
    lv_obj_t* contDepth = lv_cont_create(screenObj, NULL);
    lv_obj_set_size(contDepth, contWidth, contHeight);
    lv_obj_set_pos(contDepth, spacingX * 2 + contWidth, spacingY);
    lv_obj_set_style_local_border_width(contDepth, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderWidth);
    lv_obj_set_style_local_radius(contDepth, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderRadius);
    lv_obj_set_style_local_bg_color(contDepth, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(contDepth, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    // Depth-Bar-Setup
    barDepthObj = lv_bar_create(contDepth, NULL);
    lv_obj_set_size(barDepthObj, contWidth * depthBarWidthPercent, contHeight * depthBarHeightPercent);
    lv_obj_align(barDepthObj, NULL, LV_ALIGN_IN_RIGHT_MID, -contWidth * depthBarWidthPercent, 0);
    lv_bar_set_range(barDepthObj, depthBarMin, depthBarMax);
    lv_bar_set_anim_time(barDepthObj, 100);
    lv_bar_set_value(barDepthObj, 0, LV_ANIM_ON);
    lv_obj_set_style_local_bg_color(barDepthObj, LV_BAR_PART_BG , LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_set_style_local_bg_color(barDepthObj, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_NORMAL);
    lv_obj_set_style_local_radius(barDepthObj, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, 0);

    // Depth-Label-Setup
    lblDepth = lv_label_create(contDepth, NULL);
    lv_obj_set_size(lblDepth, contWidth, contHeight);
    lv_obj_set_style_local_text_font(lblDepth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lblFont);
    lv_label_set_align(lblDepth, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lblDepth, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_text_color(lblDepth, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    // Depth-Image-Setup
    imgDepthObj = lv_img_create(contDepth, NULL);
    lv_img_set_src(imgDepthObj, &imgDepthSource);
    lv_obj_align(imgDepthObj, NULL, LV_ALIGN_IN_BOTTOM_LEFT, contWidth * imgCornerSpacingPercent, -contHeight * imgCornerSpacingPercent);

    // SpO2-SECTION
    // SpO2-Container-Setup
    lv_obj_t* contSpO2 = lv_cont_create(screenObj, NULL);
    lv_obj_set_size(contSpO2, contWidth, contHeight);
    lv_obj_set_pos(contSpO2, spacingX, spacingY * 2 + contHeight);
    lv_obj_set_style_local_border_width(contSpO2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderWidth);
    lv_obj_set_style_local_radius(contSpO2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderRadius);
    lv_obj_set_style_local_bg_color(contSpO2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(contSpO2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    
    // SpO2-Arc-Setup
    arcSpO2Obj = lv_arc_create(contSpO2, NULL);
    lv_obj_set_size(arcSpO2Obj, arcSize, arcSize);
    lv_obj_set_style_local_text_color(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_set_style_local_bg_color(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_line_color(arcSpO2Obj, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    lv_obj_set_style_local_line_color(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_NORMAL);
    lv_obj_set_style_local_border_width(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(arcSpO2Obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_arc_set_range(arcSpO2Obj, arcMinRange, arcMaxRange);
    lv_obj_align(arcSpO2Obj, NULL, LV_ALIGN_CENTER, contWidth * arcPositionOffsetXPercent, contHeight * arcPositionOffsetYPercent);
    
    // SpO2-Label-Setup
    lblSpO2Obj = lv_label_create(contSpO2, NULL);
    lv_obj_set_style_local_text_font(lblSpO2Obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lblFont);
    lv_label_set_align(lblSpO2Obj, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lblSpO2Obj, NULL, LV_ALIGN_CENTER, contWidth * arcPositionOffsetXPercent, contHeight * arcPositionOffsetYPercent);
    lv_obj_set_style_local_text_color(lblSpO2Obj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    //SpO2-Image-Setup
    imgSpO2Obj = lv_img_create(contSpO2, NULL);
    lv_img_set_src(imgSpO2Obj, &imgSpO2Source);
    lv_obj_align(imgSpO2Obj, NULL, LV_ALIGN_IN_TOP_RIGHT, -contWidth * imgCornerSpacingPercent, contHeight * imgCornerSpacingPercent);

    // HEART-FREQUENCY-SECTION
    // Heart-Frequency-Container-Setup
    lv_obj_t* contHeartFrequency = lv_cont_create(screenObj, NULL);
    lv_obj_set_size(contHeartFrequency, contWidth, contHeight);
    lv_obj_set_pos(contHeartFrequency, spacingX * 2 + contWidth, spacingY * 2 + contHeight);
    lv_obj_set_style_local_border_width(contHeartFrequency, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderWidth);
    lv_obj_set_style_local_radius(contHeartFrequency, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, contBorderRadius);
    lv_obj_set_style_local_bg_color(contHeartFrequency, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_SECONDARY_COLOR_DARKER);
    lv_obj_set_style_local_border_color(contHeartFrequency, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);
    
    // Heart-Frequency-Label-Setup
    lblHeartFrequency = lv_label_create(contHeartFrequency, NULL);
    lv_obj_set_style_local_text_font(lblHeartFrequency, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lblFont);
    lv_label_set_align(lblHeartFrequency, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lblHeartFrequency, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_text_color(lblHeartFrequency, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, THEME_PRIMARY_COLOR_NORMAL);

    // Heart-Frequency-Image-Setup
    imgHeartFrequencyObj = lv_img_create(contHeartFrequency, NULL);
    lv_img_set_src(imgHeartFrequencyObj, &imgHeartFrequencySource);
    lv_obj_align(imgHeartFrequencyObj, NULL, LV_ALIGN_IN_TOP_LEFT, contWidth * imgCornerSpacingPercent, contHeight * imgCornerSpacingPercent);
}

// Update data & load dive-screen
void DiveScreen::showScreen() {
    dataUpdate();
    lv_scr_load(DiveScreen::screenObj);
}

// Handle button-presses on dive-screen
void DiveScreen::processButtonPress(ButtonType buttonType) {
    // Put here code for handling button-presses on the dive-screen
}


// Show & Update dive-data on the screen
void DiveScreen::dataUpdate() {

    // Some minute/second calculations
    int8_t s = UISystem::currentDiveData.time % 60;
    int8_t m = (UISystem::currentDiveData.time - s) / 60;

    // Update o2satuation-arc
    lv_arc_set_value(arcSpO2Obj, UISystem::currentDiveData.o2saturation);

    // Update depth-bar
    lv_bar_set_value(barDepthObj, 20 - UISystem::currentDiveData.depth, LV_ANIM_ON);
    
    //Update labels of each container
    lv_label_set_text_fmt(lblTimeObj, "%02d:%02d", m, s);
    lv_label_set_text_fmt(lblDepth, "%d", UISystem::currentDiveData.depth);
    lv_label_set_text_fmt(lblSpO2Obj, "%d", UISystem::currentDiveData.o2saturation);
    lv_label_set_text_fmt(lblHeartFrequency, "%d", UISystem::currentDiveData.heartFrequency);

    // Recalculate the center position because of new text-length
    lv_obj_realign(lblTimeObj);
    lv_obj_realign(lblSpO2Obj);
    lv_obj_realign(lblTimeObj);
    lv_obj_realign(lblHeartFrequency);
    lv_obj_realign(lblDepth);
}

// Update to new current dive-data & react to depth/time value for switching screens
void DiveScreen::update() {

    // Update display
    dataUpdate();

    // Check if diver is gone out of the water
    if(UISystem::currentDiveData.depth < 1) {
        // Check if dive-time exceeds data-collecting-threshold
        if(UISystem::currentDiveData.time >= 10) {
            // diver was diving long enough so show stat-screen
            UISystem::setScreen(STAT_SCREEN);
        } else {
            // diver was not diving enough so go back to idle-screen
            UISystem::setScreen(IDLE_SCREEN);
        }
    }
}