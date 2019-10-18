#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vitasdk.h>

#include "ctrl.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf
#define clearScreen psvDebugScreenClear
int ret;

void increaseVolume(int vol){
  clearScreen();
  if (vol == 30){
      printf("You are alredy at the maxium. Nothing to do.");
      main();
    }
  else {
      int ret = sceRegMgrSetKeyInt("/CONFIG/SOUND/", "main_volume", vol + 1);
      main();
    }
}

int getCurrentVolume(){
  int val = 0;
  int buff = sceRegMgrGetKeyInt("/CONFIG/SOUND/", "main_volume", &val);
  return val;
}

void muteConsole(){
  clearScreen();
  int ret = sceRegMgrSetKeyInt("/CONFIG/SOUND/","main_volume", 0);
  main();

}

void decreaseVolume(int vol){
  clearScreen();
  if (vol == 0){
      printf("You are alredy at the minimum.");
      main();
    }
  else {
      int ret = sceRegMgrSetKeyInt ("/CONFIG/SOUND/", "main_volume", vol - 1);
      printf ("Volume has been decreased.");
      main();
    }
}

int main()
{
  psvDebugScreenInit ();
  clearScreen(0);
  printf("--- Vita Volume by inthecatsdreams ---\n");
  printf("CROSS: Increase Volume\n");
  printf("CIRCLE: Decrease Volume\n");
  printf("TRIANGLE: Mute Console\n");
  printf("SQUARE: Apply settings and reboot\n");
  printf("Current Volume: %d\n", getCurrentVolume());
  int currentVolume = getCurrentVolume();
  sceKernelDelayThread(100000);
  while(1){

      switch(get_key (0)){
	case SCE_CTRL_CROSS:
	  increaseVolume(currentVolume);
	  break;
	case SCE_CTRL_CIRCLE:
	  decreaseVolume(currentVolume);
	  break;
	case SCE_CTRL_SQUARE:
	  scePowerRequestColdReset();
	  break;
	case SCE_CTRL_TRIANGLE:
	  muteConsole();
	  break;
	default:
	  break;
	}
    }

  return 0;
}
