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
        if (strcmp(key, "vol_up") == 0) return "Freccia SU: Aumenta Volume";
        if (strcmp(key, "vol_down") == 0) return "Freccia GIU': Diminuisci Volume";
        if (strcmp(key, "mute") == 0) return "TRIANGOLO: Muta Console";
        if (strcmp(key, "shutdown") == 0) return "CERCHIO: Spegni la console";
        if (strcmp(key, "disable_avls") == 0) return "QUADRATO: Disabilita AVLS (non permanente)";
        if (strcmp(key, "reboot") == 0) return "CROCE: Riavvia e applica";
        if (strcmp(key, "exit") == 0) return "START: Esci dall'app";
        if (strcmp(key, "vol_increased") == 0) return "Volume aumentato!";
        if (strcmp(key, "vol_decreased") == 0) return "Volume diminuito!";
        if (strcmp(key, "muted") == 0) return "Console muta.";
        if (strcmp(key, "already_max") == 0) return "Volume al massimo.";
        if (strcmp(key, "already_min") == 0) return "Volume al minimo.";
        if (strcmp(key, "rebooting") == 0) return "Riavvio in corso...";
        if (strcmp(key, "shutting_down") == 0) return "Spegnimento in corso...";
        if (strcmp(key, "exiting") == 0) return "Uscita dall'app...";
        if (strcmp(key, "avls_disabled") == 0) return "AVLS disabilitato (non permanente).";
        if (strcmp(key, "lang_changed") == 0) return "Lingua cambiata.";
        if (strcmp(key, "change_lang") == 0) return "SELECT: Cambia lingua (IT/EN)";
    } else {
        if (strcmp(key, "title") == 0) return "--- Vita Volume by inthecatsdreams ---";
        if (strcmp(key, "vol_up") == 0) return "ARROW-UP: Increase Volume";
        if (strcmp(key, "vol_down") == 0) return "ARROW-DOWN: Decrease Volume";
        if (strcmp(key, "mute") == 0) return "TRIANGLE: Mute Console";
        if (strcmp(key, "shutdown") == 0) return "CIRCLE: Shutdown your Vita";
        if (strcmp(key, "disable_avls") == 0) return "SQUARE: Disable AVLS (not permanent)";
        if (strcmp(key, "reboot") == 0) return "CROSS: Apply settings and reboot";
        if (strcmp(key, "exit") == 0) return "START: Exit app";
        if (strcmp(key, "vol_increased") == 0) return "Volume increased!";
        if (strcmp(key, "vol_decreased") == 0) return "Volume decreased!";
        if (strcmp(key, "muted") == 0) return "Console muted.";
        if (strcmp(key, "already_max") == 0) return "Already at maximum volume.";
        if (strcmp(key, "already_min") == 0) return "Already at minimum volume.";
        if (strcmp(key, "rebooting") == 0) return "Rebooting...";
        if (strcmp(key, "shutting_down") == 0) return "Shutting down...";
        if (strcmp(key, "exiting") == 0) return "Exiting app...";
        if (strcmp(key, "avls_disabled") == 0) return "AVLS disabled (not permanent).";
        if (strcmp(key, "lang_changed") == 0) return "Language changed.";
        if (strcmp(key, "change_lang") == 0) return "SELECT: Change language (IT/EN)";
    }
    return "";
}

void saveLanguage() {
    FILE* f = fopen("ux0:data/language.txt", "w");
    if (f) {
        fprintf(f, "%s", lang);
        fclose(f);
    }
}

void loadLanguage() {
    FILE* f = fopen("ux0:data/language.txt", "r");
    if (f) {
        char buffer[4];
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\r\n")] = 0; // Rimuove newline
        if (strcmp(buffer, "it") == 0) lang = "it";
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

void showMessage(const char* msg) {
    clearScreen();
    printf("\n%s\n", msg);
    sceKernelDelayThread(2000000);  // 2 secondi
}

void drawScreen() {
    clearScreen();
    printf("%s\n", _( "title" ));
    printf("%s\n", _( "vol_up" ));
    printf("%s\n", _( "vol_down" ));
    printf("%s\n", _( "mute" ));
    printf("%s\n", _( "shutdown" ));
    printf("%s\n", _( "disable_avls" ));
    printf("%s\n", _( "reboot" ));
    printf("%s\n", _( "exit" ));
    printf("%s\n\n", _( "change_lang" ));
    int vol = getCurrentVolume();
    printf("Volume: %d %s\n", vol, getVolumeSymbol(vol));
    printf("AVLS: %s\n", getAVLSStatus() ? "ON" : "OFF");
    printf("Lingua: %s\n", strcmp(lang, "it") == 0 ? "Italiano" : "English");
}

int main() {
    psvDebugScreenInit();
    loadLanguage();
    drawScreen();

    while (1) {
        int key = get_key(0);
        if (key) {
            int vol = getCurrentVolume();

            switch (key) {
                case SCE_CTRL_UP:
                    if (vol < 30) {
                        setVolume(vol + 1);
                        showMessage(_("vol_increased"));
                    } else showMessage(_("already_max"));
                    break;

                case SCE_CTRL_DOWN:
                    if (vol > 0) {
                        setVolume(vol - 1);
                        showMessage(_("vol_decreased"));
                    } else showMessage(_("already_min"));
                    break;

                case SCE_CTRL_TRIANGLE:
                    setVolume(0);
                    showMessage(_("muted"));
                    break;

                case SCE_CTRL_SQUARE:
                    disableAVLS();
                    showMessage(_("avls_disabled"));
                    break;

                case SCE_CTRL_CROSS:
                    showMessage(_("rebooting"));
                    scePowerRequestColdReset();
                    break;

                case SCE_CTRL_CIRCLE:
                    showMessage(_("shutting_down"));
                    scePowerRequestStandby();
                    break;

                case SCE_CTRL_START:
                    showMessage(_("exiting"));
                    sceKernelExitProcess(0);
                    break;

                case SCE_CTRL_SELECT:
                    lang = (strcmp(lang, "it") == 0) ? "en" : "it";
                    saveLanguage();
                    showMessage(_("lang_changed"));
                    break;

                default:
                    break;
            }

            drawScreen();
        }

        sceKernelDelayThread(100000);
    }

    return 0;
}
