#include "gba.h"

#include "TeamKNOx/TeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

extern u16 ViewOpening();
extern u16 ViewColorSelect();
extern u16 ViewLevelSelect();
extern u16 ViewGame();

u16 gViewNumber;
u16 gMyColor;
u16 gGameLevel;

int main(void)
{
	gMyColor = 0;
	gGameLevel = 1;

	SetMode( MODE_3 | BG2_ENABLE ); // Set MODE3

	gViewNumber = KViewOpening;
	ViewOpening();

	gViewNumber = KViewColorSelect;
	while(1){
		switch (gViewNumber){
			case KViewOpening:
				ViewOpening();
			case KViewColorSelect:
				ViewColorSelect();
				break;
			case KViewLevelSelect:
				ViewLevelSelect();
				break;
			case KViewGame:
				ViewGame();
				break;

			default:
				ViewOpening();
				break;
		}

	}
	return 0;

}

// EOF
