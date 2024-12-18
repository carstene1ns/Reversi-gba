#include "TeamKNOxLib.h"

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

void drawCusorLevelSelect(u16 aCursorPosition, u16 aColor) {
	DrawBoxEmpty(LEVEL_SELECT_CURSOR_POS_X,
		 aCursorPosition * LEVEL_SELECT_CURSOR_STEP + LEVEL_SELECT_CURSOR_OFFSET,
		 LEVEL_SELECT_CURSOR_WIDTH,
		 LEVEL_SELECT_CURSOR_HEIGHT,
		 aColor,
		 VRAM_ADDRESS);
}

void ViewLevelSelect() {
	BitBltMaskedComp(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (u16*)LevelSelection_Map, OFF_SCREEN_ADDRESS);
#if OFF_SCREEN
	Off2VRAM(OFF_SCREEN_ADDRESS);
#endif

	u16 cursorPosY = gGameLevel;
	bool first = true;

	while(true) {
		key_poll();

		if(key_hit(KEY_UP)){
			if(cursorPosY > 0){
				drawCusorLevelSelect(cursorPosY, BG_COLOR);
				cursorPosY--;
			}
		}
		else if(key_hit(KEY_DOWN)){
			if(cursorPosY < 2){
				drawCusorLevelSelect(cursorPosY, BG_COLOR);
				cursorPosY++;
			}
		}
		if(first || key_hit(KEY_UP|KEY_DOWN)) {
			drawCusorLevelSelect(cursorPosY, LEVEL_SELECT_CURSOR_COLOR);
			first = false;
		}

		if(key_hit(KEY_LEFT|KEY_B)){
			gViewNumber = KViewColorSelect;
			break;
		}
		else if(key_hit(KEY_RIGHT|KEY_A)){
			gViewNumber = KViewGame;
			break;
		}

		VBlankIntrWait();
	}

	gGameLevel = cursorPosY;
}

// EOF
