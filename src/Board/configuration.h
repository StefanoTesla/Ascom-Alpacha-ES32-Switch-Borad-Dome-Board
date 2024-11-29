#ifndef BOARD_CONFIG
#define BOARD_CONFIG


void startupTask(){

    #ifdef DOME
        initDomeConfig();
    #endif

    #ifdef COVER_CALIBRATOR
        initCoverCConfig();
    #endif

    #ifdef SWITCH

    #endif




}

#endif