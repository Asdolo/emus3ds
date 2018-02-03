
#ifndef _3DSMAIN_H_
#define _3DSMAIN_H_


extern char romFileNameFullPath[];
extern char romFileNameLastSelected[];

extern uint8_t* bottom_screen_buffer;

#include "3dsgpu.h"

void turn_bottom_screen(bool on);
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

#endif
