#include "TeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

// Application Specfic part
// Graphic(Bitmap) data
#include "bitmaps/opening.c"

extern u16 gViewNumber;

void ViewOpening() {
	BitBltMaskedComp(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (u16*)opening_Map, OFF_SCREEN_ADDRESS);
#if OFF_SCREEN
	Off2VRAM(OFF_SCREEN_ADDRESS);
#endif

	while(true) {
		key_poll();

		if(key_hit(KEY_A|KEY_START)){
			gViewNumber = KViewColorSelect;
			PlayNote(NOTE_A, 1);
			return;
		}

		VBlankIntrWait();
	}
}

// EOF
