#include "gba.h"

#include "TeamKNOx/extTeamKNOxLib.h"

// Reversi constants
#include "ReversiConstants.h"

// Application Specfic part
// Graphic(Bitmap) data
#include "bitmaps/Stone_Black_16x16.c"
#include "bitmaps/Stone_White_16x16.c"

#include "bitmaps/gameBoard.c"

#include "bitmaps/FingerCursorReady.c"
#include "bitmaps/FingerCursorReady_mask.c"

#include "bitmaps/HourGlass.c"
#include "bitmaps/HourGlass_mask.c"

// Defined at ReversiEngine part...
#include "ReversiEngine.h"
extern u16 brdBaseInfo[BOARD_GRID_NUMBER][BOARD_GRID_NUMBER];
extern void InitBaseInfo();
extern u16 CheckPosition(u16* brd, u16 aStonePosX, u16 aStonePosY, u16 aMyColor, u16 aWithReverse);
extern u16 GameStatusCheck(u16 aNextColor);
extern u16 ReversiEngine(u16 aComColor, u16 aComLevel);

extern u16 gViewNumber;
extern u16 gMyColor;
extern u16 gGameLevel;


void SetUpBoard()
{
	BitBltMaskedComp(0, 0, 240, 160, (u16*)gameBoard_Map, OFF_SCREEN_ADDRESS);
}

u16 SetUpAStone(u16 aStonePosX, u16 aStonePosY)
{
	u16 positionX, positionY;

	if(aStonePosY == 8){
		return 0;
	}
	else{

	positionX = aStonePosX * BOARD_GRID_SIZE + BOARD_START_POSITION_X;
	positionY = aStonePosY * BOARD_GRID_SIZE + BOARD_START_POSITION_Y;

	switch(brdBaseInfo[aStonePosX][aStonePosY]){
		case ID_STONE_NONE:
			break;

			case ID_STONE_BLACK:
			if(ISSIKI)
					BitBltMaskedComp(positionX, positionY, STONE_SIZE_XY, STONE_SIZE_XY, (u16*)Stone_White_16x16_Map, OFF_SCREEN_ADDRESS);
			else
				BitBltMaskedComp(positionX, positionY, STONE_SIZE_XY, STONE_SIZE_XY, (u16*)Stone_Black_16x16_Map, OFF_SCREEN_ADDRESS);
			break;

		case ID_STONE_WHITE:
			BitBltMaskedComp(positionX, positionY, STONE_SIZE_XY, STONE_SIZE_XY, (u16*)Stone_White_16x16_Map, OFF_SCREEN_ADDRESS);
			break;

		default:
			break;
	}
	return 1;
	}
}


void SetUpStone()
{
	u16 i, j;

	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			SetUpAStone(i, j);
		}
	}
}

void CountUpScore(u8 *arBlackScore, u8 *arWhiteScore)
{
	u16 i, j;
	u8 blackScore, whiteScore;

	blackScore = whiteScore = 0;

	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			switch(brdBaseInfo[i][j]){
				case ID_STONE_NONE:
					break;

				case ID_STONE_BLACK:
					blackScore++;
					break;

				case ID_STONE_WHITE:
					whiteScore++;
					break;

				default:
					break;
			}
		}
	}
	*arBlackScore = blackScore;
	*arWhiteScore = whiteScore;
}

void SetUpInfo(u8 comLevel)
{
	u8 blackScore, whiteScore;

	blackScore = whiteScore = 0;

	CountUpScore(&blackScore, &whiteScore);

	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH, BOARD_START_POSITION_Y * 2 + SUB_TITLE_PANE_SIZE_Y, "LEVEL:", RGB(0, 0, 0), BG_COLOR, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);
	num02str(comLevel + 1);
	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH + 8 * 6, BOARD_START_POSITION_Y * 2 + SUB_TITLE_PANE_SIZE_Y, gWorkStr, RGB(0, 0, 0), BG_COLOR, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);

	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH, BOARD_START_POSITION_Y * 4 + SUB_TITLE_PANE_SIZE_Y, "BLACK:", RGB(0, 0, 0), BG_COLOR, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);
	num02str(blackScore);
	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH + 8 * 6, BOARD_START_POSITION_Y * 4 + SUB_TITLE_PANE_SIZE_Y, gWorkStr, RGB(0, 0, 0), BG_COLOR, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);

	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH, BOARD_START_POSITION_Y * 5 + SUB_TITLE_PANE_SIZE_Y, "WHITE:", RGB(0, 0, 10), BG_COLOR, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);
	num02str(whiteScore);
	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH + 8 * 6, BOARD_START_POSITION_Y * 5 + SUB_TITLE_PANE_SIZE_Y, gWorkStr, RGB(0, 0, 0), BG_COLOR, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);
}

void SetUpMessage(char* aStrings)
{
	DrawBox(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH - 8, BOARD_START_POSITION_Y * 6 + SUB_TITLE_PANE_SIZE_Y + 2, 80, 10, 0x001f, OFF_SCREEN_ADDRESS);
	DrawText(BOARD_START_POSITION_X * 2 + BOARD_GRID_LENGTH - 8, BOARD_START_POSITION_Y * 6 + SUB_TITLE_PANE_SIZE_Y + 3, aStrings, RGB(0, 0, 0), 0x001F, TRANSPARENT_OFF, OFF_SCREEN_ADDRESS);
}

u16 gBGSaveData[16][16];

void SetUpCursor(u16 aPositionX, u16 aPositionY, u16 aGameTurn)
{
	u16 positionX, positionY;

	positionX = aPositionX * BOARD_GRID_SIZE + BOARD_START_POSITION_X;
	positionY = aPositionY * BOARD_GRID_SIZE + BOARD_START_POSITION_Y + CURSOR_FIXED_OFFSET;

	if(aGameTurn){
		BitBltMasked(positionX, positionY, CURSOR_SIZE_XY, CURSOR_SIZE_XY, (u16*)FingerCursorReady_Map, (u16*)FingerCursorReady_mask_Map, OFF_SCREEN_ADDRESS);
		//	BitBltMaskedComp(positionX, positionY, CURSOR_SIZE_XY, CURSOR_SIZE_XY, (u16*)FingerCursorReady_Map, OFF_SCREEN_ADDRESS);
	}
	else{
		BitBltMasked(positionX, positionY, CURSOR_SIZE_XY, CURSOR_SIZE_XY, (u16*)HourGlass_Map, (u16*)HourGlass_mask_Map, OFF_SCREEN_ADDRESS);
	}

}

void SaveSprite(u16 aPositionX, u16 aPositionY, u32 aVRAM)
{
	u16 i, j;
	u16 positionX, positionY;
	u16* ScreenBuffer;

	positionX = aPositionX * BOARD_GRID_SIZE + BOARD_START_POSITION_X;
	positionY = aPositionY * BOARD_GRID_SIZE + BOARD_START_POSITION_Y + CURSOR_FIXED_OFFSET;

	ScreenBuffer = (u16*)aVRAM;

	for(j = 0;j < 16;j++){
		for(i = 0;i < 16;i++){
			gBGSaveData[i][j] = ScreenBuffer[(positionY + j) * SCREEN_SIZE_X + positionX + i];
		}
	}

}

void RestoreSprite(u16 aPositionX, u16 aPositionY, u32 aVRAM)
{
	u16 i, j;
	u16 positionX, positionY;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;

	positionX = aPositionX * BOARD_GRID_SIZE + BOARD_START_POSITION_X;
	positionY = aPositionY * BOARD_GRID_SIZE + BOARD_START_POSITION_Y + CURSOR_FIXED_OFFSET;

	for(j = 0;j < 16;j++){
		for(i = 0;i < 16;i++){
			ScreenBuffer[(positionY + j) * SCREEN_SIZE_X + positionX + i] = gBGSaveData[i][j];
		}
	}

}


u16 ViewGame()
{
	u16 keyWk;					// current key data
	u16 lastKeyWk;				// prevous key data
	u16 stayThisView;

	u16 cursorPosX, cursorPosY;

	u16 myColor, comColor, gameResult;
	u8 blackScore, whiteScore, comLevel;
	u16 gameTurn;
	u8 put;

	gameResult = GAME_STATUS_NORMAL;

	myColor = ID_STONE_BLACK;
	comColor = ID_STONE_WHITE;
	gameTurn = GAME_TURN_WHITE;
	switch(gMyColor){
		case 0:
			myColor = ID_STONE_BLACK;
			comColor = ID_STONE_WHITE;
			gameTurn = GAME_TURN_WHITE;
			break;
		case 1:
			myColor = ID_STONE_WHITE;
			comColor = ID_STONE_BLACK;
			gameTurn = GAME_TURN_BLACK;
			break;
		default:
			break;
	}

	comLevel = 1;
	switch(gGameLevel){
		case 0:
			comLevel = 2;
			break;
		case 1:
			comLevel = 1;
			break;
		case 2:
			comLevel = 0;
			break;
		default:
			break;
	}


	blackScore = whiteScore = 0;

	cursorPosX = cursorPosY = BOARD_GRID_NUMBER / 2 + 1;

	InitBaseInfo();

	SetUpBoard();
	SetUpStone();
	SetUpInfo(comLevel);
	SaveSprite(cursorPosX, cursorPosY, OFF_SCREEN_ADDRESS);
	SetUpMessage(" Start....");

	lastKeyWk = keyWk = 0;
	stayThisView = 1;

	gameTurn = 0;
	put = 0;
	while(stayThisView)
	{
		keyWk = *KEYS;
		if (gameTurn == myColor - 1)
		{
			SetUpMessage("Your Turn.");
			if(!(lastKeyWk ^ keyWk)){
				continue;
			}
			lastKeyWk = keyWk;
			RestoreSprite(cursorPosX, cursorPosY, OFF_SCREEN_ADDRESS);
			SetUpAStone(cursorPosX, cursorPosY);
			SetUpAStone(cursorPosX, cursorPosY + 1);
			if(!(*KEYS & KEY_UP)){
				if(cursorPosY > 0){
					cursorPosY--;
				}
			}
			if(!(*KEYS & KEY_DOWN)){
				if(cursorPosY < BOARD_GRID_NUMBER - 1){
					cursorPosY++;
				}
			}
			if(!(*KEYS & KEY_LEFT)){
				if(cursorPosX > 0){
					cursorPosX--;
				}
			}
			if(!(*KEYS & KEY_RIGHT)){
				if(cursorPosX < BOARD_GRID_NUMBER - 1){
					cursorPosX++;
				}
			}
			if(!(*KEYS & KEY_A)){
				if(CheckPosition(&(brdBaseInfo[0][0]), cursorPosX, cursorPosY, myColor, 1)) {
					put = 1;
				}
				SetUpStone();
				SetUpInfo(comLevel);
			}
			SetUpAStone(cursorPosX, cursorPosY);
			SaveSprite(cursorPosX, cursorPosY, OFF_SCREEN_ADDRESS);
			SetUpCursor(cursorPosX, cursorPosY, CURSOR_FINGER);
		}
		else if (gameTurn == comColor - 1)
		{
			SetUpCursor(cursorPosX, cursorPosY, CURSOR_HOUR_GLASS);
			SetUpMessage("Thinking..");
			RestoreSprite(cursorPosX, cursorPosY, OFF_SCREEN_ADDRESS);

			#if OFF_SCREEN
				Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
			#endif

			ReversiEngine(comColor, comLevel*2);
			SetUpStone();
			SetUpInfo(comLevel);
			put = 1;
		}
		if (put == 1) {
			put = 0;
			gameResult = GameStatusCheck((!gameTurn) + 1);
			switch(gameResult){
			case GAME_STATUS_NORMAL:
				gameTurn = !gameTurn;
				break;
			case GAME_STATUS_END:
				// Game End
				SetUpCursor(cursorPosX, cursorPosY, CURSOR_HOUR_GLASS);
				SetUpMessage(" Game End");
				DrawBoxHalf(BOARD_START_POSITION_X / 2, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 3, BOARD_START_POSITION_X + BOARD_GRID_SIZE * 8, BOARD_GRID_SIZE * 2, OFF_SCREEN_ADDRESS);

				CountUpScore(&blackScore, &whiteScore);
				if(blackScore > whiteScore)
					DrawText(BOARD_START_POSITION_X + BOARD_GRID_SIZE, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 4, "Black Win !!", RGB(30, 30, 30), 0x7FFF, TRANSPARENT_ON, OFF_SCREEN_ADDRESS);
				if(blackScore < whiteScore)
					DrawText(BOARD_START_POSITION_X + BOARD_GRID_SIZE, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 4, "White Win !!", RGB(30, 30, 30), 0x7FFF, TRANSPARENT_ON, OFF_SCREEN_ADDRESS);
				if(blackScore == whiteScore)
					DrawText(BOARD_START_POSITION_X + BOARD_GRID_SIZE * 2, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 4, "Draw !!", RGB(30, 30, 30), 0x7FFF, TRANSPARENT_ON, OFF_SCREEN_ADDRESS);
			#if OFF_SCREEN
				Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
			#endif

				gViewNumber = KViewOpening;
				stayThisView = 0;
//			#if OFF_SCREEN
//				Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
//			#endif
				AWait();
				break;
			case GAME_STATUS_BLACK_PASS:
				SetUpCursor(cursorPosX, cursorPosY, CURSOR_HOUR_GLASS);
				SetUpMessage("Black Pass");
				DrawBoxHalf(BOARD_START_POSITION_X / 2, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 3, BOARD_START_POSITION_X + BOARD_GRID_SIZE * 8, BOARD_GRID_SIZE * 2, OFF_SCREEN_ADDRESS);
				DrawText(BOARD_START_POSITION_X + BOARD_GRID_SIZE * 2, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 4, "Black Pass", RGB(30, 30, 30), 0x7FFF, TRANSPARENT_ON, OFF_SCREEN_ADDRESS);
			#if OFF_SCREEN
				Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
			#endif
				AWait();
				SetUpBoard();
				SetUpStone();
				SetUpInfo(comLevel);
				SetUpCursor(cursorPosX, cursorPosY, CURSOR_FINGER);
				break;
			case GAME_STATUS_WHITE_PASS:
				SetUpCursor(cursorPosX, cursorPosY, CURSOR_HOUR_GLASS);
				SetUpMessage("White Pass");
				DrawBoxHalf(BOARD_START_POSITION_X / 2, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 3, BOARD_START_POSITION_X + BOARD_GRID_SIZE * 8, BOARD_GRID_SIZE * 2, OFF_SCREEN_ADDRESS);
				DrawText(BOARD_START_POSITION_X + BOARD_GRID_SIZE * 2, BOARD_START_POSITION_Y + BOARD_GRID_SIZE * 4, "White Pass", RGB(30, 30, 30), 0x7FFF, TRANSPARENT_ON, OFF_SCREEN_ADDRESS);
			#if OFF_SCREEN
				Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
			#endif
				AWait();
				SetUpBoard();
				SetUpStone();
				SetUpInfo(comLevel);
				SetUpCursor(cursorPosX, cursorPosY, CURSOR_FINGER);
				break;
			default:
				break;
			}
		}

		#if OFF_SCREEN
			Off2VRAM(OFF_SCREEN_ADDRESS, VRAM_ADDRESS);
		#endif

		WaitForVsync(); // Wait VBL
	}

	return 0;
}

// EOF
