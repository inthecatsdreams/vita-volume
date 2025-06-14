#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vitasdk.h>

#include "ctrl.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf
#define clearScreen psvDebugScreenClear
#define VOLUME_PATH "ux0:data/volume.txt"

// === System Functions ===

int getCurrentVolume() {
    int val = 0;
    sceRegMgrGetKeyInt("/CONFIG/SOUND/", "main_volume", &val);
    return val;
}

int getAVLSStatus() {
    int val = 0;
    sceRegMgrGetKeyInt("/CONFIG/SOUND/", "avls", &val);
    return val;
}

void setVolume(int vol) {
    if (vol < 0) vol = 0;
    if (vol > 30) vol = 30;
    sceRegMgrSetKeyInt("/CONFIG/SOUND/", "main_volume", vol);
}

void disableAVLS() {
    sceRegMgrSetKeyInt("/CONFIG/SOUND/", "avls", 0);
}

void saveVolume(int vol) {
    FILE *f = fopen(VOLUME_PATH, "w");
    if (f) {
        fprintf(f, "%d\n", vol);
        fclose(f);
    }
}

int loadSavedVolume() {
    FILE *f = fopen(VOLUME_PATH, "r");
    int v = -1;
    if (f) {
        fscanf(f, "%d", &v);
        fclose(f);
    }
    return v;
}

void drawVolumeBar(int vol) {
    printf("Volume: [");
    for (int i = 0; i < 30; i++) {
        printf(i < vol ? "#" : "-");
    }
    printf("] %d/30\n", vol);
}

void drawScreen() {
    clearScreen();
    printf("--- Vita Volume by inthecatsdreams ---\n");
    printf("UP / DOWN   : Increase / Decrease Volume\n");
    printf("L / R       : Set Min / Max Volume\n");
    printf("TRIANGLE    : Mute Console\n");
    printf("SQUARE      : Disable AVLS (not permanent)\n");
    printf("CIRCLE      : Shutdown your Vita\n");
    printf("CROSS       : Reboot\n");
    printf("START       : Exit\n\n");

    int vol = getCurrentVolume();
    int avls = getAVLSStatus();
    drawVolumeBar(vol);
    printf("AVLS Status: %s\n", avls ? "ENABLED" : "DISABLED");
}

// === Main ===

int main() {
    psvDebugScreenInit();

    int savedVol = loadSavedVolume();
    if (savedVol >= 0) {
        setVolume(savedVol);
    }

    drawScreen();

    while (1) {
        int key = get_key(0);

        if (key) {
            int vol = getCurrentVolume();

            switch (key) {
                case SCE_CTRL_UP:
                    if (vol < 30) setVolume(vol + 1);
                    break;

                case SCE_CTRL_DOWN:
                    if (vol > 0) setVolume(vol - 1);
                    break;

                case SCE_CTRL_LTRIGGER:
                    setVolume(0);
                    break;

                case SCE_CTRL_RTRIGGER:
                    setVolume(30);
                    break;

                case SCE_CTRL_TRIANGLE:
                    setVolume(0);
                    break;

                case SCE_CTRL_SQUARE:
                    disableAVLS();
                    break;

                case SCE_CTRL_CROSS:
                    saveVolume(getCurrentVolume());
                    printf("\nRebooting...");
                    sceKernelDelayThread(1000000);
                    scePowerRequestColdReset();
                    break;

                case SCE_CTRL_CIRCLE:
                    printf("\nShutting down...");
                    sceKernelDelayThread(1000000);
                    scePowerRequestStandby();
                    break;

                case SCE_CTRL_START:
                    printf("\nExiting app...");
                    sceKernelDelayThread(1000000);
                    sceKernelExitProcess(0);
                    break;

                default:
                    break;
            }

            saveVolume(getCurrentVolume());
            drawScreen();
        }

        sceKernelDelayThread(100000); // prevent high CPU usage
    }

    return 0;
}
