// TeamKNOxLib.h

#ifndef TEAMKNOX_LIB
#define TEAMKNOX_LIB

#include "bkg.c"
#include "SIO.h"

s8 gWorkStr[8];

void num02str(u8 number)
{
  u16 m;
  u8 i, n, f;
  char work[4];

  f = 0; m = 10;
  for( i=0; i<2; i++ ) {
    n = number / m; number = number % m; m = m / 10;
    if( (n==0)&&(f==0) ) {
//      work[i] = ' ';		// ' '
      work[i] = '0';		// '0'
    } else {
      f = 1;
      work[i] = n + '0';	//'0' - '9'
    }
  }
  work[i] = 0x00;

  i = n = 0;
  while(work[n]){
  	while(work[n] == ' '){
  		n++;
  	}
  	gWorkStr[i] = work[n];
  	i++;
  	n++;
  }
  gWorkStr[i] = 0x00;
}

void num04str(u16 number)
{
  u16 m;
  u8 i, n, f;
  char work[6];

  f = 0; m = 1000;
  for(i = 0;i < 4;i++){
    n = number / m; number = number % m; m = m / 10;
    if( (n==0)&&(f==0) ) {
//      work[i] = ' ';		// ' '
      work[i] = '0';		// '0'
    } else {
      f = 1;
      work[i] = n + '0';	//'0' - '9'
    }
  }
  work[i] = 0x00;

  i = n = 0;
  while(work[n]){
  	while(work[n] == ' '){
  		n++;
  	}
  	gWorkStr[i] = work[n];
  	i++;
  	n++;
  }
  gWorkStr[i] = 0x00;
}


void WaitForVsync(void)
{
	while (*(volatile u16*)0x4000006 >= 160) {};
	while (*(volatile u16*)0x4000006 < 160) {};
}

void GetRGB(u8 *arRed, u8 *arGreen, u8 *arBlue, u16 aPosScreen, u32 aVRAM)
{
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;
	*arRed = ScreenBuffer[aPosScreen] & 0x1F;
	*arGreen = (ScreenBuffer[aPosScreen] & 0x03E0) >> 5;
	*arBlue = (ScreenBuffer[aPosScreen] & 0x7C00) >> 10;
}


void SetPixel(u16 aX, u16 aY, u16 aColor, u32 aVRAM)
{
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;
	ScreenBuffer[aY * SCREEN_SIZE_X + aX] = aColor;
}


void DrawLine(u16 aX0, u16 aY0, u16 aX1, u16 aY1, u16 aColor, u32 aVRAM)
{
	s16 x, y = aY0;
	s16 dx = (aX1 - aX0) * 2;
	s16 dy = (aY1 - aY0) * 2;

	s16 dydx = dy - dx;
	s16 d = dy - dx / 2;

	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;
	for(x = aX0;x <= aX1; x++){
		ScreenBuffer[y * SCREEN_SIZE_X + x] = aColor;
		if(d < 0){
			d = d + dy;
		}
		else{
			y++;
			d = d + dydx;
		}
	}
}


void DrawCircle(u16 aX, u16 aY, u16 aRadius, u16 aColor, u32 aVRAM)
{
	s16 x, y, e;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;

	x = aRadius;
	y = 0;
	e = 3 - 2 * aRadius;

	while(y <= x){
		ScreenBuffer[aX + x + (aY + y) * SCREEN_SIZE_X] = aColor;
		ScreenBuffer[aX + x + (aY - y) * SCREEN_SIZE_X] = aColor;
		ScreenBuffer[aX - x + (aY + y) * SCREEN_SIZE_X] = aColor;
		ScreenBuffer[aX - x + (aY - y) * SCREEN_SIZE_X] = aColor;

		ScreenBuffer[aX + y + (aY + x) * SCREEN_SIZE_X] = aColor;
		ScreenBuffer[aX + y + (aY - x) * SCREEN_SIZE_X] = aColor;
		ScreenBuffer[aX - y + (aY + x) * SCREEN_SIZE_X] = aColor;
		ScreenBuffer[aX - y + (aY - x) * SCREEN_SIZE_X] = aColor;

		if(e < 0){
			e = e + 4 * y + 6;
		}
		else{
			e = e + 4 * (y - x) + 10;
			x--;
		}
		y++;
	}
}


void DrawBox(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u32 aVRAM)
{
	u16 x, y;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;
	for(y = 0;y < aHeight;y++){
		for(x = 0;x < aWidth;x++){
			ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] = aColor;
		}
	}
}

void DrawBoxEmpty(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u32 aVRAM)
{
	DrawBox(aX, aY, aWidth + 2, 2, aColor, aVRAM);
	DrawBox(aX + aWidth, aY + 2, 2, aHeight, aColor, aVRAM);
	DrawBox(aX, aY + aHeight, aWidth, 2, aColor, aVRAM);
	DrawBox(aX, aY, 2, aHeight, aColor, aVRAM);
}

void DrawBoxHalf(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u32 aVRAM)
{
	u16 x, y, posScreen;
	u16* ScreenBuffer;

	u8 red, green, blue;

	ScreenBuffer = (u16*)aVRAM;
	for(y = 0;y < aHeight;y++){
		for(x = 0;x < aWidth;x++){
			posScreen = (y + aY) * SCREEN_SIZE_X + x + aX;
			GetRGB(&red, &green, &blue, posScreen, aVRAM);
			red = red >> 1;
			green = green >> 1;
			blue = blue >> 1;
			ScreenBuffer[posScreen] = RGB(red, green, blue);
		}
	}
	DrawBoxEmpty(aX, aY, aWidth, aHeight, RGB(20, 20, 20), aVRAM);
}

void BitBltSRCCOPY(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM)
{
	u16 x, y;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;

	for(y = 0;y < aHeight;y++){
		for(x = 0;x < aWidth;x++){
			ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] = aImage[y * aWidth + x];
		}
	}
}


void BitBltOR(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM)
{
	u16 x, y;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;

	for(y = 0;y < aHeight;y++){
		for(x = 0;x < aWidth;x++){
			ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] = ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] | aImage[y * aWidth + x];
		}
	}
}


void BitBltAND(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM)
{
	u16 x, y;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;

	for(y = 0;y < aHeight;y++){
		for(x = 0;x < aWidth;x++){
			ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] = ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] & aImage[y * aWidth + x];
		}
	}
}


void BitBltExOR(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM)
{
	u16 x, y;
	u16* ScreenBuffer;

	ScreenBuffer = (u16*)aVRAM;

	for(y = 0;y < aHeight;y++){
		for(x = 0;x < aWidth;x++){
			ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] = ScreenBuffer[(y + aY) * SCREEN_SIZE_X + x + aX] ^ aImage[y * aWidth + x];
		}
	}
}

void BitBltMasked(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aMaskImage, u32 aVRAM)
{
	u16 i, bitbltWork[32*32];

	for(i = 0;i < aWidth * aHeight;i++){
		bitbltWork[i] = ~aMaskImage[i];
	}

	BitBltOR(aX, aY, aWidth, aHeight, (u16*)bitbltWork, aVRAM);
	BitBltAND(aX, aY, aWidth, aHeight, (u16*)aImage, aVRAM);
}

void BitBltMaskedComp(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM)
{
	u16 x, y, i, size, rgb[2], a, j, k;
	u16* ScreenBuffer;
	u8 count[2];
	ScreenBuffer = (u16*)aVRAM;

	size = aWidth * aHeight;
	i = 0;
	a = 0;
	while(a < size){
		count[0] = (u8)(aImage[i]>>8);
		count[1] = (u8)(aImage[i]&0xFF);
		i++;
		for (j = 0; j < 2; j++){
			rgb[j] = aImage[i];
			i++;
		}
		for (k = 0; k < 2; k++){
			for (j = 0; j < count[k];j++){
				if ((rgb[k]&0x8000)!=0){
					y = (int)(a / aWidth + 1);
					x = a % aWidth;
					ScreenBuffer[((aHeight - y) + aY) * SCREEN_SIZE_X + x + aX] = (rgb[k]&0x7FFF);
				}
				a++;
			}
		}
	}
}

void DrawTextInit()
{
}

#define CHARCTER_SIZE	8
void DrawText(u16 aX, u16 aY, s8* aStrings, u16 aStringColor, u16 aBGColor, u16 aRectMask, u32 aVRAM)
{
	int i, j, k;
	u16 CharBuffer[CHARCTER_SIZE*CHARCTER_SIZE], MaskBuffer[CHARCTER_SIZE*CHARCTER_SIZE], textPosX, temp;
	u8 charCode;
	u16 maskPattern[CHARCTER_SIZE*CHARCTER_SIZE];

	for(i = 0;i < CHARCTER_SIZE*CHARCTER_SIZE;i++){
		maskPattern[i] = aBGColor;
	}


	textPosX = aX;
	while(*aStrings){
		charCode = *aStrings;

		k = 0;
		for(j = 0;j < CHARCTER_SIZE;j++){
			temp = bkg_Map[charCode * CHARCTER_SIZE + j];

			for(i = 0;i < CHARCTER_SIZE;i++){
				temp = temp << 1;
				if (temp & 0x0100){
					CharBuffer[k] = aStringColor;
					MaskBuffer[k] = 0x0000;
				}
				else{
					CharBuffer[k] = aBGColor;
					MaskBuffer[k] = aBGColor;
				}
				k++;
			}
		}

	if(aRectMask){
		BitBltSRCCOPY(textPosX, aY, CHARCTER_SIZE, CHARCTER_SIZE, (u16*)maskPattern, aVRAM);
	}
		BitBltMasked(textPosX, aY, CHARCTER_SIZE, CHARCTER_SIZE, (u16*)CharBuffer, (u16*)MaskBuffer, aVRAM);
		textPosX = textPosX + CHARCTER_SIZE;
		aStrings++;
	}
}

void Off2VRAM(u32 aSourceAddress, u32 aDistAddress)
{
/*
	int x, y;
	u16* ScreenBuffer; 
	u16* offScreenBuffer;

	offScreenBuffer = (u16*)aSourceAddress;
	ScreenBuffer = (u16*)aDistAddress;

	for(y = 0;y < SCREEN_SIZE_Y;y++){
		for(x = 0;x < SCREEN_SIZE_X;x++){
			ScreenBuffer[y * SCREEN_SIZE_X + x] = offScreenBuffer[y * SCREEN_SIZE_X + x];
		}
	}


*/
    // DMA3
	REG_DM3SAD = aSourceAddress;	// Source
	REG_DM3DAD = aDistAddress;			// Destination
	REG_DM3CNT = 0x84000000 + 38400 / 2; // “]‘—ƒ[ƒh(32,16bit)”
}


// Random number generator

u32 gRandomSeed;

/*
------------------------------------------------------------------------------
 Global variables
------------------------------------------------------------------------------
*/
static u32 cur_value;

/*
------------------------------------------------------------------------------
 Initialize ramdom number generator
------------------------------------------------------------------------------
*/
void seed_lc(u32 v)
{
  cur_value = v;

  return;
}

/*
------------------------------------------------------------------------------
 Linear congruential method random number generator
------------------------------------------------------------------------------
*/
u32 random_lc(void)
{
  /* 2^31 = 2147483648 */;
  cur_value = (1103515245UL * cur_value + 12345) % 2147483648UL;

  return cur_value;
}

void InitUART(u16 parameter)
{
	REG_RCNT = 0; // SIO Enable
	REG_SIOCNT = 0; // SIO Reset
	REG_SIOCNT = UART_MODE
	           | SEND_ENABLE
	           | RECV_ENABLE
               | CTS_ENABLE
	           | DATA_LENGTH_8
               | parameter;
}

void SendByte(u8 data)
{
	REG_SIODATA8 = data;
}


void Wait(u32 aWaitingTime)
{
	while(aWaitingTime--);
}

void AWait()
{
	while(1) {
		WaitForVsync();
		if(!(*KEYS & KEY_A))
			break;
	}
}



#endif
