// ReversiConstants.h

#ifndef REVERSI_CONSTANTS
#define REVERSI_CONSTANTS

#define BOARD_START_POSITION_X	12
#define BOARD_START_POSITION_Y	12
#define BOARD_GRID_SIZE	17
#define BOARD_GRID_NUMBER	8
#define BOARD_GRID_LENGTH	BOARD_GRID_SIZE*BOARD_GRID_NUMBER
#define BOARD_GRID_LINE_WIDTH	1

#define BOARD_COLOR	0x0200
#define BOARD_GRID_LINE_COLOR	0x00

#define STONE_SIZE_XY	16

#define SUB_TITLE_PANE_SIZE_X	80
#define SUB_TITLE_PANE_SIZE_Y	60

#define CURSOR_SIZE_XY	16
#define CURSOR_FIXED_OFFSET 8

#define STONE_BLACK	1
#define STONE_WHITE	0

#define BG_COLOR	0x6318

enum ViewStatus
{
	KViewOpening = 0,
	KViewColorSelect,
	KViewLevelSelect,
	KViewGame,
	KViewGameEnd

} gViewStatus;

#define ISSIKI 0

#define CURSOR_FINGER	1
#define CURSOR_HOUR_GLASS	0


// use off screen
#define OFF_SCREEN 1


#if OFF_SCREEN
	#define OFF_SCREEN_ADDRESS	0x2020000
#else
	#define OFF_SCREEN_ADDRESS	VRAM_ADDRESS
#endif


#define TRANSPARENT_ON	0
#define TRANSPARENT_OFF	1


#define MULTIBOOT volatile const u8 __gba_multiboot;
MULTIBOOT



#endif
