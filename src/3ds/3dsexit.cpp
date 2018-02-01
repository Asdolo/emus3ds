#include <stdlib.h>
#include <3ds.h>

#include "3dsexit.h"
#include "3dsgpu.h"
#include "3dssound.h"
#include "3dsemu.h"
#include "3dsmain.h"

aptHookCookie hookCookie;
int appExiting = 0;
int appSuspended = 0;

void handleAptHook(APT_HookType hook, void* param)
{
	switch (hook)
	{
        case APTHOOK_ONEXIT:
            // Let's turn on the bottom screen just in case it's turned off
            gspLcdInit();
            GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTTOM);
            gspLcdExit();
            appExiting = 1;
            break;
        case APTHOOK_ONSUSPEND:
            // Let's turn on the bottom screen just in case it's turned off
            gspLcdInit();
            GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTTOM);
            gspLcdExit();
            appSuspended = 1;
            if (emulator.emulatorState == EMUSTATE_EMULATE) {
                snd3dsStopPlaying();
            }
            break;
        case APTHOOK_ONSLEEP:
            appSuspended = 1;
            if (emulator.emulatorState == EMUSTATE_EMULATE) {
                snd3dsStopPlaying();
            }
            break;
        case APTHOOK_ONRESTORE:
        case APTHOOK_ONWAKEUP:
            gspLcdInit();
            if (bottom_screen_buffer == NULL)
            {
                // There's no bottom screen image, let's turn off the bottom screen
                GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTTOM);
            }
            else
            {
                GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTTOM);
            }
            gspLcdExit();
            appSuspended = 1;
            break;
    }
}

void enableAptHooks()
{
	aptHook(&hookCookie, handleAptHook, NULL);
}