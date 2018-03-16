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
            turn_bottom_screen(TURN_ON);
            appExiting = 1;
            break;
        case APTHOOK_ONSUSPEND:
            impl3dsClearBorderTexture();
            // Let's turn on the bottom screen just in case it's turned off
            turn_bottom_screen(TURN_ON);
            appSuspended = 1;
            if (emulator.emulatorState == EMUSTATE_EMULATE) {
                snd3dsStopPlaying();
            }
            break;
        case APTHOOK_ONSLEEP:
            impl3dsClearBorderTexture();
            appSuspended = 1;
            if (emulator.emulatorState == EMUSTATE_EMULATE) {
                snd3dsStopPlaying();
            }
            break;
        case APTHOOK_ONRESTORE:
        case APTHOOK_ONWAKEUP:
            gpu3dsFixHang();
            if (border_image_texture_buffer != NULL)
            {
                GX_DisplayTransfer((u32*)border_image_texture_buffer,GX_BUFFER_DIM(512, 256),(u32*)borderTexture->PixelData,GX_BUFFER_DIM(512, 256),
                GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GPU_RGBA8) |
                GX_TRANSFER_OUT_FORMAT((u32) GPU_RGBA8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
            }            
            
            if (bottom_screen_buffer == NULL && emulator.emulatorState == EMUSTATE_EMULATE)
            {
                // There's no bottom screen image AND the menu is closed, let's turn off the bottom screen
                turn_bottom_screen(TURN_OFF);
            }
            else
            {
                turn_bottom_screen(TURN_ON);
            }
            
            appSuspended = 1;
            break;
    }
}

void enableAptHooks()
{
	aptHook(&hookCookie, handleAptHook, NULL);
}