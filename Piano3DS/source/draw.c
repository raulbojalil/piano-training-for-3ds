#include "draw.h"
/*#include "arial10_bmp.h"
#include "arial10.h"
#include "verdana14_000000.h"
#include "verdana14_000000_bmp.h";
#include "verdana14_000000_reg.h"
#include "verdana14_000000_reg_bmp.h";
#include "verdana11_000000.h"
#include "verdana11_000000_bmp.h";

BmpFile* verdana14Image = (BmpFile*)verdana14_000000_reg_bmp;
BmpFile* verdana14BoldImage = (BmpFile*)verdana14_000000_bmp;
BmpFile* verdana11Image = (BmpFile*)verdana11_000000_bmp;*/


u8* topLeftFB = NULL;
u8* topRightFB = NULL;
u8* bottomFB = NULL;

void updateFramebuffers()
{
	topLeftFB = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	topRightFB = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	bottomFB = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
}

u8* getFramebuffer(u8 screen)
{
	if(screen == SCREEN_TOP_LEFT) return topLeftFB; else if(screen == SCREEN_TOP_RIGHT) return topRightFB;
	return bottomFB;
}

void drawPixelBuf(u8* framebuffer, u16 X, u16 Y, u8 R, u8 G, u8 B)
{
	if(framebuffer == NULL) return;

	u32 pos = (SCREEN_HEIGHT - 1 - Y) * 3 + X * 3 * SCREEN_HEIGHT;
	/*framebuffer[pos + 0] = RGB & 0xFF;
	framebuffer[pos + 1] = (RGB >> 8) & 0xFF;
	framebuffer[pos + 2] = (RGB >> 16) & 0xFF;*/
	framebuffer[pos + 0] = B;
	framebuffer[pos + 1] = G;
	framebuffer[pos + 2] = R;
}

void drawPixel(u8 screen, u16 X, u16 Y, u8 R, u8 G, u8 B)
{
	drawPixelBuf(getFramebuffer(screen), X, Y, R, G, B);
}

void drawPixel3D(u16 X, u16 Y, u8 R, u8 G, u8 B, u8 d) 
{
	drawPixel(SCREEN_TOP_LEFT, X, Y, R, G, B);

	if(CONFIG_3D_SLIDERSTATE > 0.0f)
		drawPixel(SCREEN_TOP_RIGHT, X + d, Y, R, G, B);
}


void drawRectBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, Rect* rect, u8 R, u8 G, u8 B)
{
	if(framebuffer == NULL) return;

	u32 x,y;
	for(x = rect->x; x < rect->x+rect->w; x++)
	{
		for(y = rect->y; y < rect->y+rect->h; y++)
		{
			if(x < 0 || y < 0 || x >= bufWidth || y >= bufHeight) continue;
			u32 scr_pix = (bufHeight - 1 - y) * 3 + x * 3 * bufHeight;
			framebuffer[scr_pix+0] = B;
			framebuffer[scr_pix+1] = G;
			framebuffer[scr_pix+2] = R;
		}
	}
}

void drawRect(u8 screen, Rect* rect, u8 R, u8 G, u8 B)
{
	drawRectBuf(getFramebuffer(screen), (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, rect, R, G, B);  
}

void drawRect3D(Rect* rect, u8 R, u8 G, u8 B, u8 d)
{
	drawRect(SCREEN_TOP_LEFT, rect, R, G, B);
	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		rect->x += d;
		drawRect(SCREEN_TOP_RIGHT, rect, R, G, B);
	}
}


void drawImageBuf(u8* framebuffer, u32 bufWidth, u32 bufHeight, BmpFile* pBmp, Rect* src, Rect* dest)
{
	if(framebuffer == NULL)
		return;

	if(pBmp == NULL)
		return drawRectBuf(framebuffer,bufWidth, bufHeight, dest,255,255,255);

	u32 img_width = pBmp->info.width;
	u8* img_data = pBmp->image;

	u32 x,y;
	for(x = 0; x < src->w; x++)
	{
		for(y = 0; y < src->h; y++)
		{
			u32 scr_x = dest->x + x;
			u32 scr_y = dest->y + y;
			u32 scr_pix = (scr_x*bufHeight+(bufHeight-1-scr_y))*3;
			if(scr_x < 0 || scr_y < 0 || scr_x >= bufWidth || scr_y >= bufHeight) continue;

			u32 img_x = src->x + x;                                        //0
			//u32 img_y = src->y + (src->h-y-1);        //7 pBmp->info.height-1-src->y-y   9-0-1 
			u32 img_y = pBmp->info.height-1-src->y-y;
			u32 img_pix = (img_x + img_y*pBmp->info.width)*3;

			unsigned char r = img_data[img_pix+0];
			unsigned char g = img_data[img_pix+1];
			unsigned char b = img_data[img_pix+2];

			if(!(r == 255 && g == 0 && b == 255))
			{
				framebuffer[scr_pix+0] = r;
				framebuffer[scr_pix+1] = g;
				framebuffer[scr_pix+2] = b;
			}
		}
	}
}

void drawImage(u8 screen, BmpFile* pBmp, Rect* src, Rect* dest)
{
	drawImageBuf(getFramebuffer(screen), (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, pBmp, src, dest);
}

void drawImage3D(BmpFile* pBmp, Rect* src, Rect* dest, u8 d)
{
	drawImage(SCREEN_TOP_LEFT, pBmp, src, dest);

	if(CONFIG_3D_SLIDERSTATE > 0.0f)
	{
		dest->x += d;
		drawImage(SCREEN_TOP_RIGHT, pBmp, src, dest);
	}
}

/*void drawStringBuf(u8 *framebuffer, FontFamily fontFamily, u32 bufWidth, u32 bufHeight, u16 X, u16 Y, const char *str, TextAlign textAlign) {

	Rect* fontInfo;
	BmpFile* fontImage;

	switch(fontFamily)
	{
		case FONTFAMILY_VERDANA14:
			fontInfo = verdana14_000000_reg;
			fontImage = verdana14Image;
		break;
		case FONTFAMILY_VERDANA14_BOLD:
			fontInfo = verdana14_000000;
			fontImage = verdana14BoldImage;
		break;
		case FONTFAMILY_VERDANA11:
			fontInfo = verdana11_000000;
			fontImage = verdana11Image;
		break;

	}

	if(framebuffer == NULL || str == NULL) return;

	Rect dest = {X,Y,100,100};

	if(textAlign == TEXT_ALIGN_RIGHT || textAlign == TEXT_ALIGN_CENTER)
	{
		u32 strWidth = 0;
		const char *g = str;
		while(*g) { if(*g != '\n' && *g != '\t') strWidth += fontInfo[*g].w; ++g; }

		if(textAlign == TEXT_ALIGN_RIGHT)
			dest.x -= strWidth;
		else 
			dest.x -= strWidth/2;
	}

	const char *s = str;
	while(*s) {
		
		if(*s == '\n') {
		} else if(*s == '\t') {
		} else {

			u8* img_data = fontImage->image;

			u32 x,y;
			for(x = 0; x < fontInfo[*s].w; x++)
			{
				for(y = 0; y < fontInfo[*s].h; y++)
				{
					u32 scr_x = dest.x + x;
					u32 scr_y = dest.y + y;
					u32 scr_pix = (scr_x*bufHeight+(bufHeight-1-scr_y))*3;
					if(scr_x < 0 || scr_y < 0 || scr_x >= bufWidth || scr_y >= bufHeight) continue;

					u32 img_x = fontInfo[*s].x + x; 
					u32 img_y = fontImage->info.height-1-fontInfo[*s].y-y;
					u32 img_pix = (img_x + img_y*fontImage->info.width)*3;

					unsigned char r = img_data[img_pix+0];
					unsigned char g = img_data[img_pix+1];
					unsigned char b = img_data[img_pix+2];

					if(!(r == 255 && g == 255 && b == 255))
					{
						framebuffer[scr_pix+0] = r;
						framebuffer[scr_pix+1] = g;
						framebuffer[scr_pix+2] = b;
					}
				}
			}

			//drawImageBuf(framebuffer, bufWidth, bufHeight, fontImage, &fontInfo[*s], &dest);
			dest.x += fontInfo[*s].w;
		}
		++s;
	}
}

void drawString(u8 screen, FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign) {

	drawStringBuf(getFramebuffer(screen), fontFamily, (screen == SCREEN_BOTTOM ? SCREEN_BOTTOM_WIDTH : SCREEN_TOP_WIDTH), SCREEN_HEIGHT, X, Y, str, textAlign);
}

void drawString3D(FontFamily fontFamily, u16 X, u16 Y, const char *str, TextAlign textAlign, u8 d) 
{
	drawString(SCREEN_TOP_LEFT, fontFamily, X, Y, str, textAlign);

	if(CONFIG_3D_SLIDERSTATE > 0.0f)
		drawString(SCREEN_TOP_RIGHT, fontFamily, X+d, Y, str, textAlign);
}*/

void drawLine(u16 startx, u16 endx, u16 y, u8 width)
{
	u16 x = 0;
	u8 w = 0;
	for(x=startx; x < endx; x++)
	{
		drawPixel(SCREEN_TOP_LEFT, x,y, 0, 0, 0);
		
		for(w=0; w < width; w++)
		{
			drawPixel(SCREEN_TOP_LEFT, x,y+w, 0, 0, 0);
		}
	}
}