#include "piano.h"

BmpFile* keysImage = (BmpFile*)keys_bmp;
BmpFile* clefsImage = (BmpFile*)clefs_bmp;
BmpFile* noteImage = (BmpFile*)note_bmp;
BmpFile* trebleButtonImage = (BmpFile*)treblebtn_bmp;
BmpFile* trebleButtonPressedImage = (BmpFile*)treblebtnpressed_bmp;
BmpFile* bassButtonImage = (BmpFile*)bassbtn_bmp;
BmpFile* bassButtonPressedImage = (BmpFile*)bassbtnpressed_bmp;

Rect screenRect = { 0, 0, 400, SCREEN_HEIGHT };
Rect bassRectSrc = { 44, 0, 47, 116 };
Rect trebleRectSrc = { 0, 0, 44, 116 };
Rect clefRectDst = { 0, 56, 44, 116 };
Rect buttonSrc = { 0, 0, 100 , 100 };
Rect trebleButtonDest = { 40, 135, 100 , 100 };
Rect bassButtonDest = { 180, 135, 100 , 100 };

u8 trebleButtonPressed = 1;
u8 bassButtonPressed = 0;
u8 bassMode = 0;

s16 pressedWhiteKeyIndex = -1;
s16 pressedBlackKeyIndex = -1;
u8 wrongKey = 0;
PianoKey currentPianoKey;
PianoKey lastPianoKey;
u8 random = 2;
u8 lastTouched = 0;

u32 lfsr = 10;

PianoKey whiteKeys[15] = { PIANO_KEY_C2, PIANO_KEY_D2, PIANO_KEY_E2, PIANO_KEY_F2, PIANO_KEY_G2, PIANO_KEY_A2, PIANO_KEY_B2,
						PIANO_KEY_C3, PIANO_KEY_D3, PIANO_KEY_E3, PIANO_KEY_F3, PIANO_KEY_G3, PIANO_KEY_A3, PIANO_KEY_B3, PIANO_KEY_C4 };

PianoKey blackKeys[10] = { PIANO_KEY_CSHARP2, PIANO_KEY_DSHARP2, PIANO_KEY_FSHARP2, PIANO_KEY_GSHARP2, PIANO_KEY_ASHARP2, 
						 PIANO_KEY_CSHARP3, PIANO_KEY_DSHARP3, PIANO_KEY_FSHARP3, PIANO_KEY_GSHARP3, PIANO_KEY_ASHARP3 };

u32 notePositions[15] = { PIANO_KEY_C2 | PIANO_KEY_CSHARP2, PIANO_KEY_D2 | PIANO_KEY_DSHARP2, PIANO_KEY_E2, PIANO_KEY_F2 | PIANO_KEY_FSHARP2,
					     PIANO_KEY_G2 | PIANO_KEY_GSHARP2, PIANO_KEY_A2 | PIANO_KEY_ASHARP2, PIANO_KEY_B2, PIANO_KEY_C3 | PIANO_KEY_CSHARP3,
						 PIANO_KEY_D3 | PIANO_KEY_DSHARP3, PIANO_KEY_E3, PIANO_KEY_F3 | PIANO_KEY_FSHARP3, PIANO_KEY_G3 | PIANO_KEY_GSHARP3,
						 PIANO_KEY_A3 | PIANO_KEY_ASHARP3, PIANO_KEY_B3, PIANO_KEY_C4 };


u32 randMinMax(u32 min, u32 max)
{
    u32 bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    lfsr = (lfsr >> 1) | (bit << 15);
	return lfsr%(max+1-min)+min;
}

void nextPianoKey()
{
	lastPianoKey = currentPianoKey;

	while(lastPianoKey == currentPianoKey)
	{
		currentPianoKey = (PianoKey)(1 << randMinMax(0,24));
	}
}

void pianoInit()
{
	lfsr = svcGetSystemTick();
	nextPianoKey();
}

void pianoUpdate()
{
	u32 kDown = hidKeysHeld();

	if(kDown & KEY_TOUCH)
	{
		touchPosition touchPos;
		hidTouchRead(&touchPos);

		if(touchPos.py > 128)
		{
			if(touchPos.px > trebleButtonDest.x && touchPos.px < trebleButtonDest.x + trebleButtonDest.w &&
				touchPos.py > trebleButtonDest.y && touchPos.py < trebleButtonDest.y + trebleButtonDest.h)
				bassMode = 0;
			else if(touchPos.px > bassButtonDest.x && touchPos.px < bassButtonDest.x + bassButtonDest.w &&
				touchPos.py > bassButtonDest.y && touchPos.py < bassButtonDest.y + bassButtonDest.h)
				bassMode = 1;

			trebleButtonPressed = bassMode ? 0 : 1;
			bassButtonPressed = bassMode ? 1 : 0;
			return;
		}

		pressedWhiteKeyIndex = touchPos.px / 21;
		pressedBlackKeyIndex = -1;

		PianoKey key = whiteKeys[pressedWhiteKeyIndex];

		if(touchPos.py < 85)
		{
			u8 i, counter=0;
			u8 blackKeyCounter=2;
			u16 x=14;

			for(i=0; i < 10; i++)
			{
				if(touchPos.px >= x && touchPos.px <= (x+14))
				{
					pressedWhiteKeyIndex = -1;
					pressedBlackKeyIndex = i;
					key = blackKeys[pressedBlackKeyIndex];
					break;
				}

				counter++;

				if(counter == blackKeyCounter)
				{
					x += (21*2);
					counter = 0;
					blackKeyCounter = (blackKeyCounter == 2 ? 3 : 2);
				}
				else
					x += 21;
			}
		}

		if(!lastTouched)
		{
			if(key == currentPianoKey)
			{
				wrongKey = 0;
				nextPianoKey();
			}
			else
				wrongKey = 1;
		} 

		lastTouched = 1;
	}
	else
	{
		pressedWhiteKeyIndex = -1;
		pressedBlackKeyIndex = -1;
		lastTouched = 0;
	}

}

void drawNoteLine(u16 y)
{
	drawLine(180 - 7, 180 + 21 + 7, y, 1);
}

void drawStaff()
{
	drawRect(SCREEN_TOP_LEFT, &screenRect,255,255,255);

	drawLine(0, 400, (5*16) + (16*0), 1);
	drawLine(0, 400, (5*16) + (16*1), 1);
	drawLine(0, 400, (5*16) + (16*2), 1);
	drawLine(0, 400, (5*16) + (16*3), 1);
	drawLine(0, 400, (5*16) + (16*4), 1);
	
	drawImage(SCREEN_TOP_LEFT, clefsImage, bassMode ? &bassRectSrc : &trebleRectSrc, &clefRectDst);
}

void drawNote()
{
	u8 i=0;

	u8 notePositionIndex = 0;
	for(i =0; i < 15; i++){
		if(notePositions[i] & currentPianoKey)
		{
			notePositionIndex = i;
			break;
		}
	}

	Rect noteRectSrc = { 0, 0, 21, 16 };
	Rect noteRect = { 180, (5*16) + (16*4) + 8 - (notePositionIndex*8) + (bassMode ? 16 : 0), 21, 16 };

	drawImage(SCREEN_TOP_LEFT, noteImage, &noteRectSrc, &noteRect);

	u8 bottomNotePosIndex = bassMode ? E2_POSITION_INDEX : C2_POSITION_INDEX;
	u8 topNotePosIndex = bassMode ? C4_POSITION_INDEX : A3_POSITION_INDEX;

	if(notePositionIndex <= bottomNotePosIndex){
		
		if((bottomNotePosIndex - notePositionIndex) % 2 == 0)
			drawNoteLine(noteRect.y + 8); //línea central

		if(bottomNotePosIndex > 0)
		{
			notePositionIndex += ((bottomNotePosIndex - notePositionIndex) % 2 == 0) ? 1 : 0;

			u8 y = 0;
			for(y = notePositionIndex; y < bottomNotePosIndex; y+=2)
			{
				drawNoteLine((5*16) + (16*4) + 8 + (y*8));
				//drawNoteLine((5*16) + (16*4) + 8 - (y*8)); //líneas superiores
			}
		}

	}
	else if(notePositionIndex >= topNotePosIndex){

		if((notePositionIndex - topNotePosIndex) % 2 == 0)
			drawNoteLine(noteRect.y + 8); //línea central

		if(notePositionIndex >= topNotePosIndex+1) //B3
		{
			notePositionIndex -= ((notePositionIndex - topNotePosIndex) % 2 == 0) ? 1 : 0;

			u8 y = 0;
			for(y = notePositionIndex; y > topNotePosIndex; y-=2)
			{
				drawNoteLine((5*16) + (16*4) + 8 - (y*8) + 16); //líneas de abajo
			}
		}
	}

	u8 blackKey = 0;
	for(i =0; i < 10; i++){
		if(blackKeys[i] == currentPianoKey)
		{
			blackKey = 1;
			break;
		}
	}

	if(blackKey)
	{
		noteRect.x -= 20;
		noteRect.y -= 7;
		noteRectSrc.x += 21;
		noteRectSrc.w = 11;
		noteRectSrc.h = 32;
		drawImage(SCREEN_TOP_LEFT, noteImage, &noteRectSrc, &noteRect);
	}
}

void drawKeys()
{
	Rect whiteKeyRectSrc = { 0, 0, 21, 128 };
	Rect whiteKeyRect = { 0, 0, 21, 128 };
	Rect blackKeyRectSrc = { 42, 0, 14, 85 };
	Rect blackKeyRect = { 14, 0, 14, 85 };

	u8 i=0;

	for(i=0; i < 15; i++)
	{
		if(pressedWhiteKeyIndex == i)
		{
			if(!wrongKey)
			{
				whiteKeyRectSrc.x = 21;
				drawImage(SCREEN_BOTTOM, keysImage, &whiteKeyRectSrc, &whiteKeyRect);
			}
			else
			{
				whiteKeyRectSrc.x = 42;
				drawImage(SCREEN_BOTTOM, keysImage, &whiteKeyRectSrc, &whiteKeyRect);
			}
		}
		else
		{
			whiteKeyRectSrc.x = 0;
			drawImage(SCREEN_BOTTOM, keysImage, &whiteKeyRectSrc, &whiteKeyRect);
		}

		whiteKeyRect.x += 21;
	}

	u8 counter = 0;
	u8 blackKeyCount = 2;

	for(i=0; i < 10; i++)
	{
		if(pressedBlackKeyIndex == i)
		{
			if(!wrongKey)
			{
				blackKeyRectSrc.x = 77;
				drawImage(SCREEN_BOTTOM, keysImage, &blackKeyRectSrc, &blackKeyRect);
			}
			else
			{
				blackKeyRectSrc.x = 91;
				drawImage(SCREEN_BOTTOM, keysImage, &blackKeyRectSrc, &blackKeyRect);
			}
		}
		else
		{	
			blackKeyRectSrc.x = 63;
			drawImage(SCREEN_BOTTOM, keysImage, &blackKeyRectSrc, &blackKeyRect);
		}

		counter++;

		if(counter == blackKeyCount)
		{
			blackKeyRect.x += (21*2);
			counter = 0;
			blackKeyCount = (blackKeyCount == 2 ? 3 : 2);
		}
		else
			blackKeyRect.x += 21;
	}


	drawImage(SCREEN_BOTTOM, trebleButtonPressed ? trebleButtonPressedImage : trebleButtonImage, &buttonSrc, &trebleButtonDest);
	drawImage(SCREEN_BOTTOM, bassButtonPressed ? bassButtonPressedImage : bassButtonImage, &buttonSrc, &bassButtonDest);

}

void pianoDraw()
{
	drawStaff();
	drawNote();
	drawKeys();
}