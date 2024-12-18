#include "TeamKNOxLib.h"

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

void drawCusorColorSelect(u16 aCursorPosition, u16 aColor) {
	DrawBoxEmpty(COLOR_SELECT_CURSOR_POS_X,
		 aCursorPosition * COLOR_SELECT_CURSOR_STEP + COLOR_SELECT_CURSOR_OFFSET,
		 COLOR_SELECT_CURSOR_WIDTH,
		 COLOR_SELECT_CURSOR_HEIGHT,
		 aColor,
		 VRAM_ADDRESS);
}

void ViewColorSelect() {
	BitBltMaskedComp(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (u16*)ColorSelection_Map, OFF_SCREEN_ADDRESS);
#if OFF_SCREEN
	Off2VRAM(OFF_SCREEN_ADDRESS);
#endif

	u16 cursorPosY = gMyColor;
	bool first = true;

	while(true){
		key_poll();

		if(key_hit(KEY_UP)){
			if(cursorPosY > 0){
				drawCusorColorSelect(cursorPosY, BG_COLOR);
				cursorPosY--;
			}
		}
		if(key_hit(KEY_DOWN)){
			if(cursorPosY < 1){
				drawCusorColorSelect(cursorPosY, BG_COLOR);
				cursorPosY++;
			}
		}
		if(first || key_hit(KEY_UP|KEY_DOWN)) {
			drawCusorColorSelect(cursorPosY, COLOR_SELECT_CURSOR_COLOR);
			first = false;
		}

		if(key_hit(KEY_RIGHT|KEY_A)){
			break;
		}

		VBlankIntrWait();
	}

	gViewNumber = KViewLevelSelect;
	gMyColor = cursorPosY;
}

// EOF
