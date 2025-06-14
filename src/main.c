#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vitasdk.h>

#include "ctrl.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf
#define clearScreen psvDebugScreenClear

// === Funzioni di sistema ===

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
    sceRegMgrSetKeyInt("/CONFIG/SOUND/", "main_volume", vol);
}

void disableAVLS() {
    sceRegMgrSetKeyInt("/CONFIG/SOUND/", "avls", 0);
}

void drawScreen() {
    clearScreen();
    printf("--- Vita Volume by inthecatsdreams ---\n");
    printf("ARROW-UP: Increase Volume\n");
    printf("ARROW-DOWN: Decrease Volume\n");
    printf("TRIANGLE: Mute Console\n");
    printf("CIRCLE: Shutdown your Vita\n");
    printf("SQUARE: Disable AVLS (use a kernel plugin to make it stick)\n");
    printf("CROSS: Apply settings and reboot\n");
    printf("START: Exit app\n\n");
    printf("Current Volume: %d\n", getCurrentVolume());
    printf("AVLS Status: %s\n", getAVLSStatus() ? "Enabled" : "Disabled");
}

// === Main ===

int main() {
    psvDebugScreenInit();
    drawScreen();

    while (1) {
        int key = get_key(0);

        if (key) {
            int vol = getCurrentVolume();

            switch (key) {
                case SCE_CTRL_UP:
                    if (vol < 30) {
                        setVolume(vol + 1);
                        printf("\nVolume increased to %d", vol + 1);
                    } else {
                        printf("\nAlready at maximum volume.");
                    }
                    break;

                case SCE_CTRL_DOWN:
                    if (vol > 0) {
                        setVolume(vol - 1);
                        printf("\nVolume decreased to %d", vol - 1);
                    } else {
                        printf("\nAlready at minimum volume.");
                    }
                    break;

                case SCE_CTRL_TRIANGLE:
                    setVolume(0);
                    printf("\nConsole muted.");
                    break;

                case SCE_CTRL_SQUARE:
                    disableAVLS();
                    printf("\nAVLS disabled (not permanent).");
                    break;

                case SCE_CTRL_CROSS:
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

            sceKernelDelayThread(1000000);
            drawScreen(); // aggiorna schermata dopo ogni azione
        }

        sceKernelDelayThread(100000); // piccola attesa per non sovraccaricare CPU
    }

    return 0;
}
