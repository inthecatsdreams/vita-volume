#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vitasdk.h>

#include "ctrl.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf
#define clearScreen psvDebugScreenClear

char *lang = "en"; // default language

const char* _(const char* key) {
    if (strcmp(lang, "it") == 0) {
        if (strcmp(key, "title") == 0) return "--- Vita Volume di inthecatsdreams ---";
        if (strcmp(key, "vol_up") == 0) return "Aumenta Volume";
        if (strcmp(key, "vol_down") == 0) return "Diminuisci Volume";
        if (strcmp(key, "mute") == 0) return "Disattiva volume";
        if (strcmp(key, "shutdown") == 0) return "Spegni la console";
        if (strcmp(key, "disable_avls") == 0) return "Disabilita AVLS";
        if (strcmp(key, "reboot") == 0) return "Riavvia e applica";
        if (strcmp(key, "exit") == 0) return "Esci dall'app";
        if (strcmp(key, "language") == 0) return "Lingua: Italiano";
        if (strcmp(key, "vol_increased") == 0) return "Volume aumentato!";
        if (strcmp(key, "vol_decreased") == 0) return "Volume diminuito!";
        if (strcmp(key, "muted") == 0) return "Console muta.";
        if (strcmp(key, "already_max") == 0) return "Volume al massimo.";
        if (strcmp(key, "already_min") == 0) return "Volume al minimo.";
        if (strcmp(key, "rebooting") == 0) return "Riavvio in corso...";
        if (strcmp(key, "shutting_down") == 0) return "Spegnimento in corso...";
        if (strcmp(key, "exiting") == 0) return "Uscita dall'app...";
        if (strcmp(key, "avls_disabled") == 0) return "AVLS disabilitato.";
    } else {
        if (strcmp(key, "title") == 0) return "--- Vita Volume by inthecatsdreams ---";
        if (strcmp(key, "vol_up") == 0) return "Increase Volume";
        if (strcmp(key, "vol_down") == 0) return "Decrease Volume";
        if (strcmp(key, "mute") == 0) return "Mute volume";
        if (strcmp(key, "shutdown") == 0) return "Shutdown console";
        if (strcmp(key, "disable_avls") == 0) return "Disable AVLS";
        if (strcmp(key, "reboot") == 0) return "Reboot and apply";
        if (strcmp(key, "exit") == 0) return "Exit app";
        if (strcmp(key, "language") == 0) return "Language: English";
        if (strcmp(key, "vol_increased") == 0) return "Volume increased!";
        if (strcmp(key, "vol_decreased") == 0) return "Volume decreased!";
        if (strcmp(key, "muted") == 0) return "Console muted.";
        if (strcmp(key, "already_max") == 0) return "Already at maximum volume.";
        if (strcmp(key, "already_min") == 0) return "Already at minimum volume.";
        if (strcmp(key, "rebooting") == 0) return "Rebooting...";
        if (strcmp(key, "shutting_down") == 0) return "Shutting down...";
        if (strcmp(key, "exiting") == 0) return "Exiting app...";
        if (strcmp(key, "avls_disabled") == 0) return "AVLS disabled.";
    }
    return "";
}

void loadLanguage() {
    FILE* f = fopen("ux0:data/language.txt", "r");
    if (f) {
        char buffer[4];
        fgets(buffer, sizeof(buffer), f);
        if (strncmp(buffer, "it", 2) == 0) lang = "it";
        fclose(f);
    }
}

void saveLanguage() {
    FILE* f = fopen("ux0:data/language.txt", "w");
    if (f) {
        fputs(lang, f);
        fclose(f);
    }
}

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

const char* getVolumeSymbol(int vol) {
    if (vol == 0) return "[🔇]";
    if (vol < 20) return "[🔉]";
    return "[🔊]";
}

// dichiarazione anticipata
const char* keyFromIndex(int i);

void drawMenu(int selected) {
    clearScreen();
    printf("%s\n\n", _("title"));

    for (int i = 0; i < 8; i++) {
        if (i == selected)
            printf("> %s\n", _(keyFromIndex(i)));
        else
            printf("  %s\n", _(keyFromIndex(i)));
    }

    int vol = getCurrentVolume();
    printf("\nVolume: %d %s\n", vol, getVolumeSymbol(vol));
    printf("AVLS: %s\n", getAVLSStatus() ? "ON" : "OFF");
}

void showMessage(const char* msg) {
    printf("\n%s\n", msg);
    sceKernelDelayThread(2000000);
}

const char* keyFromIndex(int i) {
    switch (i) {
        case 0: return "vol_up";
        case 1: return "vol_down";
        case 2: return "mute";
        case 3: return "disable_avls";
        case 4: return "reboot";
        case 5: return "shutdown";
        case 6: return "exit";
        case 7: return "language";
        default: return "";
    }
}

int main() {
    psvDebugScreenInit();
    loadLanguage();

    int selected = 0;
    drawMenu(selected);

    while (1) {
        int key = get_key(0);

        if (key & SCE_CTRL_UP) {
            if (selected > 0) selected--;
            drawMenu(selected);
        } else if (key & SCE_CTRL_DOWN) {
            if (selected < 7) selected++;
            drawMenu(selected);
        } else if (key & SCE_CTRL_CROSS) {
            int vol = getCurrentVolume();
            switch (selected) {
                case 0:
                    if (vol < 30) {
                        setVolume(vol + 1);
                        showMessage(_("vol_increased"));
                    } else showMessage(_("already_max"));
                    break;
                case 1:
                    if (vol > 0) {
                        setVolume(vol - 1);
                        showMessage(_("vol_decreased"));
                    } else showMessage(_("already_min"));
                    break;
                case 2:
                    setVolume(0);
                    showMessage(_("muted"));
                    break;
                case 3:
                    disableAVLS();
                    showMessage(_("avls_disabled"));
                    break;
                case 4:
                    showMessage(_("rebooting"));
                    scePowerRequestColdReset();
                    break;
                case 5:
                    showMessage(_("shutting_down"));
                    scePowerRequestStandby();
                    break;
                case 6:
                    showMessage(_("exiting"));
                    sceKernelExitProcess(0);
                    break;
                case 7:
                    if (strcmp(lang, "it") == 0)
                        lang = "en";
                    else
                        lang = "it";
                    saveLanguage();
                    drawMenu(selected);
                    break;
            }
            drawMenu(selected);
        }

        sceKernelDelayThread(100000);
    }

    return 0;
}
