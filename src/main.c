#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vitasdk.h>

#include "ctrl.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf
int ret;



void increaseVolume(int vol){
	psvDebugScreenClear();
	if (vol == 30){
		psvDebugScreenPrintf("You are alredy at the maxium.");
		main();
	}
	else {
		int ret  = sceRegMgrSetKeyInt("/CONFIG/SOUND/", "main_volume", vol + 1);
		main();
	}
}
int getCurrentVolume(){
	int val = 0;
	int buff = sceRegMgrGetKeyInt("/CONFIG/SOUND/", "main_volume", &val);
	return val;
}

void decreaseVolume(int vol){
	psvDebugScreenClear();
	if (vol == 5){
		psvDebugScreenPrintf("You are alredy at the minimum.");
		main();
	}
	else {
		int ret  = sceRegMgrSetKeyInt("/CONFIG/SOUND/", "main_volume", vol - 1);
		printf("Volume has been decreased.");
		main();
	}
}

int main() {
		psvDebugScreenInit();
        psvDebugScreenClear(0);
		psvDebugScreenPrintf("--- Vita Volume by inthecatsdreams ---\n");
        psvDebugScreenPrintf("CROSS: Increase Volume\n");
        psvDebugScreenPrintf("CIRCLE: Decrease Volume\n");
        psvDebugScreenPrintf("SQUARE: Apply settings and reboot\n");
		psvDebugScreenPrintf("Current Volume: %d\n", getCurrentVolume());
		int currentVolume = getCurrentVolume();
		
        
        sceKernelDelayThread(100000);
        while(1)
        {
        switch(get_key(0)) {
                case SCE_CTRL_CROSS:
                    increaseVolume(currentVolume);
                    break;
                case SCE_CTRL_CIRCLE:
                    decreaseVolume(currentVolume);
                    break;
                case SCE_CTRL_SQUARE:
					scePowerRequestColdReset();
                    break;
                default:
                    break;
                }
        }
        

        
        return 0;
}
