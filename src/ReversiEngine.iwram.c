#include <tonc.h>

// ReversiAdvance Engine part
#include "ReversiConstants.h"

#include "ReversiEngine.h"

extern u16 gGameLevel;

u16 brdBaseInfo[BOARD_GRID_NUMBER][BOARD_GRID_NUMBER];
u16 gGameStatusChanged;

void InitBaseInfo()
{
	u16 i, j;

	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			brdBaseInfo[i][j] = ID_STONE_NONE;
		}
	}

	brdBaseInfo[3][3] = ID_STONE_WHITE;
	brdBaseInfo[3][4] = ID_STONE_BLACK;
	brdBaseInfo[4][3] = ID_STONE_BLACK;
	brdBaseInfo[4][4] = ID_STONE_WHITE;

}

u16 GetStone(u16* brd, u16 aStonePosX, u16 aStonePosY)
{
	return (u16)*(brd + aStonePosY + aStonePosX * BOARD_GRID_NUMBER);
}

u16 CheckPosition(u16* brd, u16 aStonePosX, u16 aStonePosY, u16 aMyColor, u16 aWithReverse)
	{
	s16 targetColor;
	s16 aroundNumber, dX, dY;
	s16 aroundX, aroundY;
	s16 conditionScopeX, conditionScopeY;
	s16 linedColorX, linedColorY;

	// Get Target Color
	targetColor = ID_STONE_WHITE;
	switch (aMyColor)
		{
		case ID_STONE_NONE:
			break;
		case ID_STONE_BLACK:
			targetColor = ID_STONE_WHITE;
			break;
		case ID_STONE_WHITE:
			targetColor = ID_STONE_BLACK;
			break;
		default:
			targetColor = ID_STONE_WHITE;
			break;
		}

	aroundNumber = 0;
	// Check correct position and reverse
	if(GetStone(brd,aStonePosX,aStonePosY) == ID_STONE_NONE){
		for(dY = -1;dY <= 1;dY++){
			for(dX = -1;dX <= 1;dX++){
				if((dX == 0) && (dY == 0)){
					// Point self
				}
				else{
					aroundX = aStonePosX + dX;
					aroundY = aStonePosY + dY;
					conditionScopeX = ((aroundX >= 0) && (aroundX < BOARD_GRID_NUMBER));
					conditionScopeY = ((aroundY >= 0) && (aroundY < BOARD_GRID_NUMBER));

					if(conditionScopeX && conditionScopeY){
						if(GetStone(brd,aroundX,aroundY) == targetColor){
							linedColorX = aroundX;
							linedColorY = aroundY;
							while((linedColorX >= 0) && (linedColorX < BOARD_GRID_NUMBER) &&
								(linedColorY >= 0) && (linedColorY < BOARD_GRID_NUMBER) &&
								(GetStone(brd, linedColorX, linedColorY) == targetColor)){
								linedColorX = linedColorX + dX;
								linedColorY = linedColorY + dY;
							}

							if((linedColorX >= 0) && (linedColorX < BOARD_GRID_NUMBER) &&
								(linedColorY >= 0) && (linedColorY < BOARD_GRID_NUMBER) && 
								GetStone(brd, linedColorX, linedColorY) == aMyColor){

								if(aWithReverse){
									linedColorX = aroundX;
									linedColorY = aroundY;
									while(GetStone(brd, linedColorX, linedColorY) == targetColor){
										// GetStone(brd, linedColorX, linedColorY) = aMyColor;
										*(brd + linedColorY + linedColorX * BOARD_GRID_NUMBER) = aMyColor;
										linedColorX = linedColorX + dX;
										linedColorY = linedColorY + dY;
									}
									// GetStone(brd, aStonePosX, aStonePosY) = aMyColor;
									*(brd + aStonePosY + aStonePosX * BOARD_GRID_NUMBER) = aMyColor;
								}
								aroundNumber++;
							}
						}
					}
				}
			}
		}
	}
	return aroundNumber;
}

u16 GameStatusCheck(u16 aNextColor)
{
	// Results ...
	// (0) Playing ... Possible to put stone
	// (1) No put position ... GameEnd
	// (2) Pass Black
	// (3) Pass White
	// (4) Possible to put Black
	// (5) Possible to put White

	u16 i, j, result, blackStatus, whiteStatus;

	blackStatus = whiteStatus = 0;

	result = GAME_STATUS_NORMAL;

	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			blackStatus = blackStatus + CheckPosition((u16*)brdBaseInfo, i, j, ID_STONE_BLACK, 0);
			whiteStatus = whiteStatus + CheckPosition((u16*)brdBaseInfo, i, j, ID_STONE_WHITE, 0);
		}
	}

	if(blackStatus + whiteStatus){
		if (aNextColor == ID_STONE_BLACK && blackStatus == 0)
			return GAME_STATUS_BLACK_PASS;
		if (aNextColor == ID_STONE_WHITE && whiteStatus == 0)
			return GAME_STATUS_WHITE_PASS;
	}
	else{
		return GAME_STATUS_END;
	}

	return result;
}

// ================================================================
//
//
//
// ================================================================

struct Evalution
{
	u16 x;
	u16 y;
	s16 score;
};

struct Evalution ReadNextMove(u16* brd, u16 readMoves, u16 aComColor, u16 aPutColor);
struct Evalution GetEvalute(u16* brd, u16 aComColor);
u16 CornerEvalute(u16* board, u16 aPutColor);

void CopyBoard(u16* newbrd, u16* orgbrd)
{
	u16 i, j;
	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			*(newbrd + i + j * BOARD_GRID_NUMBER) = (u16)*(orgbrd + i + j * BOARD_GRID_NUMBER);
		}
	}
}

u16 ReversiEngine(u16 aComColor, u16 aComLevel)
{
	u16 i, j;
	struct Evalution eva;
	u16 myBrd[BOARD_GRID_NUMBER][BOARD_GRID_NUMBER];

	CopyBoard(&(myBrd[0][0]), &(brdBaseInfo[0][0]));
	if (aComLevel == 0)
	{
		for(j = 0;j < BOARD_GRID_NUMBER;j++){
			for(i = 0;i < BOARD_GRID_NUMBER;i++){
				if (CheckPosition(&(brdBaseInfo[0][0]), i, j, aComColor, 1))
					return 0;
			}
		}
	}
	eva = ReadNextMove(&(myBrd[0][0]), aComLevel, aComColor, aComColor);
	CheckPosition(&(brdBaseInfo[0][0]), eva.x, eva.y, aComColor, 1);

	return 0;

}

u16 PutAbleCount(u16* brd, u16 aComColor)
{
	u16 i, j, count;
	count = 0;
	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			if(CheckPosition(brd, i, j, aComColor, 0)){
				count++;
			}
		}
	}
	return count;
}

u16 PieceCount(u16* brd, u16 aComColor)
{
	u16 i, j, count;
	count = 0;
	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			if(GetStone(brd,i, j) == aComColor){
				count++;
			}
		}
	}
	return count;
}

struct Evalution ReadNextMove(u16* brd, u16 readMoves, u16 aComColor, u16 aPutColor)
{
	u16 i, j;
	u16 myBrd[BOARD_GRID_NUMBER][BOARD_GRID_NUMBER];

	struct Evalution best;
	struct Evalution worst;
	struct Evalution eva;

	best.score = -9999;
	worst.score = 9999;
	if (readMoves == 0)
	{
		return GetEvalute(brd, aComColor);
	}
	if (PutAbleCount(brd, aPutColor) == 0)
	{
		if (PutAbleCount(brd, (3 - aPutColor)) == 0)
		{
			if (PieceCount(brd, aComColor) > PieceCount(brd, (3 - aComColor)))
			{
				eva.score = 7000 + PieceCount(brd, aComColor);
				return eva;
			} else {
				eva.score = - 7000 + PieceCount(brd, aComColor);;
				return eva;
			}
		}
		eva = ReadNextMove(&(myBrd[0][0]), readMoves - 1, aComColor, (3 - aPutColor));
		return eva;
	}
	
	CopyBoard(&(myBrd[0][0]), brd);
	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			if(CheckPosition(&(myBrd[0][0]), i, j, aPutColor, 1)){
 				eva = ReadNextMove(&(myBrd[0][0]), readMoves - 1, aComColor, (3 - aPutColor));
				if (eva.score < worst.score)
				{
					worst.score = eva.score;
					worst.x = i;
					worst.y = j;
				}
				if (eva.score > best.score)
				{
					best.score = eva.score;
					best.x = i;
					best.y = j;
				}
				CopyBoard(&(myBrd[0][0]), brd);
			}
		}
	}
	if (aComColor == aPutColor) {
		return best;
	} else {
		return worst;
	}
}

struct Evalution GetEvalute(u16* brd, u16 aComColor)
{
	s16 i, j, score;
	s16 dY, dX;
	struct Evalution eva;
	score = 0;

	for(j = 0;j < BOARD_GRID_NUMBER;j++){
		for(i = 0;i < BOARD_GRID_NUMBER;i++){
			if (GetStone(brd, i, j) == ID_STONE_NONE) {
				for(dY = -1;dY <= 1;dY++){
					for(dX = -1;dX <= 1;dX++){
						if((dX == 0) && (dY == 0)){
							// Point self
						}
						else
						{
							if ((i+dX) >= 0 && (i+dX) <= 7 && (j+dY) >= 0 && (j+dY) <= 7)
							{
								if (GetStone(brd,i + dX,j + dY) == aComColor) {
									score--;
								}
								if (GetStone(brd,i + dX,j + dY) == (3 - aComColor)) {
									score++;
								}
							}
						}
					}
				}
			}
		}
	}

	eva.score = score + CornerEvalute(brd, aComColor);
	return eva;
}

u16 CornerEvalute(u16* board, u16 aPutColor)
{
	u16 score = 0;
	if (GetStone(board, 1, 1) == (3-aPutColor)) {
		if (GetStone(board, 0, 0) == ID_STONE_NONE) {
			score = score + 120;
		}
	}
	if (GetStone(board, 6, 1) == (3-aPutColor)) {
		if (GetStone(board, 7,0) == ID_STONE_NONE) {
			score = score + 120;
		}
	}
	if (GetStone(board, 1, 7) == (3-aPutColor)) {
		if (GetStone(board, 0, 7) == ID_STONE_NONE) {
			score = score + 120;
		}
	}	
	if (GetStone(board, 6, 6) == (3-aPutColor)) {
		if (GetStone(board, 7, 7) == ID_STONE_NONE) {
			score = score + 120;
		}
	}

	if (GetStone(board, 1, 1) == aPutColor) {
		if (GetStone(board, 0, 0) == ID_STONE_NONE) {
			score = score - 120;
		}
	}
	if (GetStone(board, 6, 1) == aPutColor) {
		if (GetStone(board, 7, 0) == ID_STONE_NONE) {
			score = score - 120;
		}
	}
	if (GetStone(board, 1, 6) == aPutColor) {
		if (GetStone(board, 0, 7) == ID_STONE_NONE) {
			score = score - 120;
		}
	}

	if (GetStone(board, 6, 6) == aPutColor) {
		if (GetStone(board, 7, 7) == ID_STONE_NONE) {
			score = score - 120;
		}
	}

	if (GetStone(board, 0, 0) == aPutColor) score = score + 200;
	if (GetStone(board, 7, 0) == aPutColor) score = score + 200;
	if (GetStone(board, 0, 7) == aPutColor) score = score + 200;
	if (GetStone(board, 7, 7) == aPutColor) score = score + 200;

	if (GetStone(board, 0, 0) == (3-aPutColor)) score = score - 200;
	if (GetStone(board, 7, 0) == (3-aPutColor)) score = score - 200;
	if (GetStone(board, 0, 8) == (3-aPutColor)) score = score - 200;
	if (GetStone(board, 7, 7) == (3-aPutColor)) score = score - 200;
	

	if(CheckPosition(board, 0, 0, aPutColor, 0)) score = score + 80;
	if(CheckPosition(board, 7, 0, aPutColor, 0)) score = score + 80;
	if(CheckPosition(board, 0, 7, aPutColor, 0)) score = score + 80;
	if(CheckPosition(board, 7, 7, aPutColor, 0)) score = score + 80;

	if(CheckPosition(board, 0, 0, (3-aPutColor), 0)) score = score - 80;
	if(CheckPosition(board, 7, 0, (3-aPutColor), 0)) score = score - 80;
	if(CheckPosition(board, 0, 7, (3-aPutColor), 0)) score = score - 80;
	if(CheckPosition(board, 7, 7, (3-aPutColor), 0)) score = score - 80;

	return score;
}
