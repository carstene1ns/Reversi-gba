#include "gba.h"

#include "TeamKNOx/extTeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

// Application Specfic part
// Graphic(Bitmap) data
#include "bitmaps/ColorSelection.c"


extern u16 gViewNumber;
extern u16 gMyColor;

#define COLOR_SELECT_CURSOR_POS_X	60
#define COLOR_SELECT_CURSOR_STEP	31
#define COLOR_SELECT_CURSOR_OFFSET	58
#define COLOR_SELECT_CURSOR_WIDTH	120
#define COLOR_SELECT_CURSOR_HEIGHT	24
#define COLOR_SELECT_CURSOR_COLOR	0x7C00

void drawCusorColorSelect(u16 aCursorPosition, u16 aColor)
{

	DrawBoxEmpty(COLOR_SELECT_CURSOR_POS_X,
		 aCursorPosition * COLOR_SELECT_CURSOR_STEP + COLOR_SELECT_CURSOR_OFFSET,
		 COLOR_SELECT_CURSOR_WIDTH,
		 COLOR_SELECT_CURSOR_HEIGHT,
		 aColor,
		 VRAM_ADDRESS);
}

u16 ViewColorSelect()
{
	u16 keyWk;					// current key data
	u16 lastKeyWk;				// prevous key data
	u16 stayThisView;
	u16 cursorPosY;

	BitBltMaskedComp(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, (u16*)ColorSelection_Map, OFF_SCREEN_ADDRESS);

// #ifdef OFF_SCREEN
	Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
// #endif

	cursorPosY = gMyColor;

	lastKeyWk = keyWk = 0;
	stayThisView = 1;
	while(stayThisView){

		keyWk = *KEYS;

		if(lastKeyWk ^ keyWk){
			if(!(*KEYS & KEY_UP)){
				if(cursorPosY > 0){
					drawCusorColorSelect(cursorPosY, BG_COLOR);
					cursorPosY--;
				}
			}
			if(!(*KEYS & KEY_DOWN)){
				if(cursorPosY < 1){
					drawCusorColorSelect(cursorPosY, BG_COLOR);
					cursorPosY++;
				}
			}
			if(!(*KEYS & KEY_LEFT)){
			}
			if(!(*KEYS & KEY_RIGHT)){
				stayThisView = 0;
			}
			if(!(*KEYS & KEY_A)){
				stayThisView = 0;
			}
			drawCusorColorSelect(cursorPosY, COLOR_SELECT_CURSOR_COLOR);
		}

		lastKeyWk = keyWk;

		WaitForVsync(); // Wait VBL

	}
	gViewNumber = KViewLevelSelect;
	gMyColor = cursorPosY;

	return 0;
}

// EOF
