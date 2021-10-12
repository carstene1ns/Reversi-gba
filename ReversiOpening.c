#include "GBA.h"

#include "TeamKNOx/extTeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

// Application Specfic part
// Graphic(Bitmap) data
#include "bitmaps/opening.c"



extern u16 gViewNumber;
u16 ViewOpening()
{
	u16 keyWk;					// current key data
	u16 lastKeyWk;				// prevous key data
	u16 stayThisView;

	BitBltMaskedComp(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, (u16*)opening_Map, OFF_SCREEN_ADDRESS);

#if OFF_SCREEN
	Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
#endif


	lastKeyWk = keyWk = 0;
	stayThisView = 1;
	while(stayThisView){

		keyWk = *KEYS;

		if(lastKeyWk ^ keyWk){
			if(!(*KEYS & KEY_UP)){
			}
			if(!(*KEYS & KEY_DOWN)){
			}
			if(!(*KEYS & KEY_LEFT)){
			}
			if(!(*KEYS & KEY_RIGHT)){
			}
			if(!(*KEYS & KEY_A)){
				gViewNumber = KViewColorSelect;
				stayThisView = 0;
			}
			if(!(*KEYS & KEY_START)){
				gViewNumber = KViewColorSelect;
				stayThisView = 0;
			}
		}

		lastKeyWk = keyWk;
		WaitForVsync(); // Wait VBL

	}
	return 0;
}

// EOF
