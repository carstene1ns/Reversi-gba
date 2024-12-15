#include "gba.h"

#include "TeamKNOx/extTeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

// Application Specfic part
// Graphic(Bitmap) data
#include "bitmaps/LevelSelection.c"

extern u16 gViewNumber;
extern u16 gGameLevel;

#define LEVEL_SELECT_CURSOR_POS_X	60
#define LEVEL_SELECT_CURSOR_STEP	25
#define LEVEL_SELECT_CURSOR_OFFSET	50
#define LEVEL_SELECT_CURSOR_WIDTH	120
#define LEVEL_SELECT_CURSOR_HEIGHT	24
#define LEVEL_SELECT_CURSOR_COLOR	0x7C00

void drawCusorLevelSelect(u16 aCursorPosition, u16 aColor)
{

	DrawBoxEmpty(LEVEL_SELECT_CURSOR_POS_X,
		 aCursorPosition * LEVEL_SELECT_CURSOR_STEP + LEVEL_SELECT_CURSOR_OFFSET,
		 LEVEL_SELECT_CURSOR_WIDTH,
		 LEVEL_SELECT_CURSOR_HEIGHT,
		 aColor,
		 VRAM_ADDRESS);
}

u16 ViewLevelSelect()
{
	u16 keyWk;					// current key data
	u16 lastKeyWk;				// prevous key data
	u16 stayThisView;
	u16 cursorPosY;

	BitBltMaskedComp(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, (u16*)LevelSelection_Map, OFF_SCREEN_ADDRESS);

#ifdef OFF_SCREEN
	Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
#endif

	cursorPosY = gGameLevel;

	lastKeyWk = keyWk = 0;
	stayThisView = 1;
	while(stayThisView){

		keyWk = *KEYS;

		if(lastKeyWk ^ keyWk){
			if(!(*KEYS & KEY_UP)){
				if(cursorPosY > 0){
					drawCusorLevelSelect(cursorPosY, BG_COLOR);
					cursorPosY--;
				}
			}
			if(!(*KEYS & KEY_DOWN)){
				if(cursorPosY < 2){
					drawCusorLevelSelect(cursorPosY, BG_COLOR);
					cursorPosY++;
				}
			}
			if(!(*KEYS & KEY_LEFT)){
				gViewNumber = KViewColorSelect;
				stayThisView = 0;
			}
			if(!(*KEYS & KEY_RIGHT)){
				gViewNumber = KViewGame;
				stayThisView = 0;
			}
			if(!(*KEYS & KEY_A)){
				gViewNumber = KViewGame;
				stayThisView = 0;
			}
			if(!(*KEYS & KEY_B)){
				gViewNumber = KViewColorSelect;
				stayThisView = 0;
			}
			drawCusorLevelSelect(cursorPosY, LEVEL_SELECT_CURSOR_COLOR);
		}

		lastKeyWk = keyWk;
		WaitForVsync(); // Wait VBL

	}
	gGameLevel = cursorPosY;

	return 0;
}

// EOF
