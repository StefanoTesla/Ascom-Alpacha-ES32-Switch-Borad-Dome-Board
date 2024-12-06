#ifndef BOARD_MAIN
#define BOARN_MAIN
#include "configuration.h"


void startupTask(){


    initBoardConfig();

    #ifdef DOME
        initDomeConfig();
    #endif

    #ifdef COVER_CALIBRATOR
        initCoverCConfig();
    #endif

    #ifdef SWITCH
        initSwitchConfig();
    #endif

    printLEDChannelStatus();

}

void boardLoop(){
    if(Global.config.save.execute){
        saveBoardConfig();
        Global.config.save.execute = false;
    }
}

#endif