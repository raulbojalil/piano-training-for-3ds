#ifndef _DRAW_H_
#define _DRAW_H_ 

#include <3ds.h>
#include "rect.h"
#include "bmp.h"

#define SCREEN_HEIGHT 240
#define SCREEN_TOP_WIDTH 400
#define SCREEN_BOTTOM_WIDTH 320
#define SCREEN_TOP_LEFT 0
#define SCREEN_TOP_RIGHT 1
#define SCREEN_BOTTOM 2

#define CONFIG_3D_SLIDERSTATE (*(float*)0x1FF81080)

typedef enum {
	TEXT_ALIGN_LEFT        = 0,
	TEXT_ALIGN_CENTER      = 1,
	TEXT_ALIGN_RIGHT       = 2
} TextAlign;

typedef enum {
	FONTFAMILY_VERDANA14		      = 0,
	FONTFAMILY_VERDANA14_BOLD         = 1,
	FONTFAMILY_VERDANA11              = 2
} FontFamily;


void updateFramebuffers();
u8* getFramebuffer(u8 screen);

void drawPixelBuf(u8* framebuffer, u16 X, u16 Y, u8 R, u8 G, u8 B);
void drawPixel(u8 screen, u16 X, u16 Y, u8 R, u8 G, u8 B);
void drawPixel3D(u16 X, u16 Y, u8 R, u8 G, u8 B, u8 d);

void drawRectBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, Rect* rect, u8 R, u8 G, u8 B);
void drawRect(u8 screen, Rect* rect, u8 R, u8 G, u8 B);
void drawRect3D(Rect* rect, u8 R, u8 G, u8 B, u8 d);

void drawImageBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, BmpFile* pBmp, Rect* src, Rect* dest);
void drawImage(u8 screen, BmpFile* pBmp, Rect* src, Rect* dest);
void drawImage3D(BmpFile* pBmp, Rect* src, Rect* dest, u8 d);

/*void drawStringBuf(u8 *framebuffer, FontFamily fontFamily, u32 bufWidth, u32 bufHeight, u16 X, u16 Y, const char *str, TextAlign textAlign);
void drawString(u8 screen, FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign);
void drawString3D(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, u8 d);*/

void drawLine(u16 startx, u16 endx, u16 y, u8 width);

#endif
