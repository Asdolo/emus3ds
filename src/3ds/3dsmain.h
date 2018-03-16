
#ifndef _3DSMAIN_H_
#define _3DSMAIN_H_


extern char romFileNameFullPath[];
extern char romFileNameLastSelected[];

extern uint8_t* bottom_screen_buffer;

#include <cstring>
#include "3dsgpu.h"

void renderTopScreenBorder();
extern u8* border_image_texture_buffer;
extern SGPUTexture *borderTexture;
void impl3dsClearBorderTexture();
bool impl3dsLoadBorderTexture(char *imgFilePath);

//---------------------------------------------------------
// The different emulator states.
//---------------------------------------------------------
#define EMUSTATE_EMULATE        1
#define EMUSTATE_PAUSEMENU      2
#define EMUSTATE_END            3

//---------------------------------------------------------
// FPS behavior 
//---------------------------------------------------------
#define WAIT_FULL 0
#define WAIT_HALF 1
#define WAIT_NONE 2

#define TURN_ON   true
#define TURN_OFF  false

inline void clearBottomScreen() {
    uint bytes = 0;
    switch (gfxGetScreenFormat(GFX_BOTTOM))
    {
        case GSP_RGBA8_OES:
            bytes = 4;
            break;

        case GSP_BGR8_OES:
            bytes = 3;
            break;

        case GSP_RGB565_OES:
        case GSP_RGB5_A1_OES:
        case GSP_RGBA4_OES:
            bytes = 2;
            break;
    }

    u8 *frame = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
    memset(frame, 0, 320 * 240 * bytes);
}

static void turn_bottom_screen(bool on)
{
   if (!on) clearBottomScreen();

   cfguInit();

   Handle lcd_handle;
   u8 not_2DS;
   CFGU_GetModelNintendo2DS(&not_2DS);
   if(not_2DS && srvGetServiceHandle(&lcd_handle, "gsp::Lcd") >= 0)
   {
      u32 *cmdbuf = getThreadCommandBuffer();
      cmdbuf[0] = (on ? 0x00110040 : 0x00120040);
      cmdbuf[1] = 2;
      svcSendSyncRequest(lcd_handle);
      svcCloseHandle(lcd_handle);
   }

   cfguExit();
}

#endif
