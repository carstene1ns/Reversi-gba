// ReversiEngine.h

#ifndef REVERSI_ENGINE
#define REVERSI_ENGINE

#define ID_STONE_NONE	0
#define ID_STONE_BLACK	1
#define ID_STONE_WHITE	2

#define GAME_STATUS_NORMAL	0
#define GAME_STATUS_END		1
#define GAME_STATUS_BLACK_PASS	2
#define GAME_STATUS_WHITE_PASS	3
#define GAME_STATUS_BLACK_PUT	4
#define GAME_STATUS_WHITE_PUT	5

#define GAME_TURN_WHITE	1
#define GAME_TURN_BLACK	0

extern u16 brdBaseInfo[BOARD_GRID_NUMBER][BOARD_GRID_NUMBER];
extern void InitBaseInfo();
extern u16 CheckPosition(u16* brd, u16 aStonePosX, u16 aStonePosY, u16 aMyColor, u16 aWithReverse);
extern u16 GameStatusCheck(u16 aNextColor);
extern u16 ReversiEngine(u16 aComColor, u16 aComLevel);

#endif
