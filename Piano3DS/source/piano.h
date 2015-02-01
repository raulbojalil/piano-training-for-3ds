#ifndef _PIANO_H_
#define _PIANO_H_ 

#include <3ds.h>
#include "bmp.h"
#include "rect.h"
#include "draw.h"
#include "keys_bmp.h"
#include "clefs_bmp.h"
#include "note_bmp.h"
#include "treblebtn_bmp.h"
#include "treblebtnpressed_bmp.h"
#include "bassbtn_bmp.h"
#include "bassbtnpressed_bmp.h"

#define C2_POSITION_INDEX 0 
#define E2_POSITION_INDEX 2 
#define A3_POSITION_INDEX 12
#define C4_POSITION_INDEX 14

typedef enum
{
	PIANO_KEY_C2        = 1 << 0,
	PIANO_KEY_CSHARP2	= 1 << 1,
	PIANO_KEY_D2		= 1 << 2,
	PIANO_KEY_DSHARP2	= 1 << 3,
	PIANO_KEY_E2		= 1 << 4,
	PIANO_KEY_F2		= 1 << 5,
	PIANO_KEY_FSHARP2	= 1 << 6,
	PIANO_KEY_G2		= 1 << 7,
	PIANO_KEY_GSHARP2	= 1 << 8,
	PIANO_KEY_A2		= 1 << 9,
	PIANO_KEY_ASHARP2	= 1 << 10,
	PIANO_KEY_B2		= 1 << 11,
	PIANO_KEY_C3		= 1 << 12,
	PIANO_KEY_CSHARP3	= 1 << 13,
	PIANO_KEY_D3		= 1 << 14,
	PIANO_KEY_DSHARP3	= 1 << 15,
	PIANO_KEY_E3		= 1 << 16,
	PIANO_KEY_F3		= 1 << 17,
	PIANO_KEY_FSHARP3	= 1 << 18,
	PIANO_KEY_G3		= 1 << 19,
	PIANO_KEY_GSHARP3	= 1 << 20,
	PIANO_KEY_A3		= 1 << 21,
	PIANO_KEY_ASHARP3	= 1 << 22,
	PIANO_KEY_B3		= 1 << 23,
	PIANO_KEY_C4		= 1 << 24

} PianoKey;

u32 randMinMax(u32 min, u32 max);
void nextPianoKey();
void pianoInit();
void pianoUpdate();
void drawNoteLine(u16 y);
void drawStaff();
void drawNote();
void drawKeys();
void pianoDraw();

#endif