#include <stdlib.h>
#include "TeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

extern void ViewOpening();
extern void ViewColorSelect();
extern void ViewLevelSelect();
extern void ViewGame();

u16 gViewNumber;
u16 gMyColor;
u16 gGameLevel;

#if OFF_SCREEN
u16 *shadow_vram;
#endif

int main(void) {
#if OFF_SCREEN
	shadow_vram = malloc(M3_WIDTH*M3_HEIGHT);
#endif

	gMyColor = 0;
	gGameLevel = 1;

	// gfx
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	gViewNumber = KViewOpening;
	while(true) {
		switch (gViewNumber) {
			case KViewColorSelect:
				ViewColorSelect();
				break;
			case KViewLevelSelect:
				ViewLevelSelect();
				break;
			case KViewGame:
				ViewGame();
				break;

			case KViewOpening:
			default:
				ViewOpening();
				break;
		}

	}
#if OFF_SCREEN
	free(shadow_vram);
#endif

	return 0;
}

// EOF
