// TeamKNOxLib

#include "TeamKNOxLib.h"
#include "bkg.h"

char gWorkStr[8];

void num02str(u8 number) {
  u8 i, n;
  char work[4];

  u8 f = 0;
  u16 m = 10;
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

void GetRGB(u8 *arRed, u8 *arGreen, u8 *arBlue, u16 aPosScreen, u16* aVRAM) {
	*arRed = aVRAM[aPosScreen] & 0x1F;
	*arGreen = (aVRAM[aPosScreen] & 0x03E0) >> 5;
	*arBlue = (aVRAM[aPosScreen] & 0x7C00) >> 10;
}

void DrawBox(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u16* aVRAM) {
	bmp16_rect(aX, aY, aX+aWidth, aY+aHeight, aColor, aVRAM, M3_WIDTH*2);
}

void DrawBoxEmpty(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u16* aVRAM) {
	bmp16_frame(aX, aY, aX+aWidth, aY+aHeight, aColor, aVRAM, M3_WIDTH*2);
}

void DrawBoxHalf(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aVRAM) {
	u16 x, y, posScreen;
	u8 red, green, blue;

	for(y = 1;y < aHeight-1;y++){
		for(x = 1;x < aWidth-1;x++){
			posScreen = (y + aY) * SCREEN_WIDTH + x + aX;
			GetRGB(&red, &green, &blue, posScreen, aVRAM);
			red = red >> 1;
			green = green >> 1;
			blue = blue >> 1;
			aVRAM[posScreen] = RGB15(red, green, blue);
		}
	}
	DrawBoxEmpty(aX, aY, aWidth, aHeight, RGB15(20, 20, 20), aVRAM);
}

void BitBltSRCCOPY(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM) {
	for(u16 y = 0;y < aHeight;y++){
		for(u16 x = 0;x < aWidth;x++){
			aVRAM[(y + aY) * SCREEN_WIDTH + x + aX] = aImage[y * aWidth + x];
		}
	}
}


void BitBltOR(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM) {
	for(u16 y = 0;y < aHeight;y++){
		for(u16 x = 0;x < aWidth;x++){
			aVRAM[(y + aY) * SCREEN_WIDTH + x + aX] = aVRAM[(y + aY) * SCREEN_WIDTH + x + aX] | aImage[y * aWidth + x];
		}
	}
}


void BitBltAND(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM) {
	for(u16 y = 0;y < aHeight;y++){
		for(u16 x = 0;x < aWidth;x++){
			aVRAM[(y + aY) * SCREEN_WIDTH + x + aX] = aVRAM[(y + aY) * SCREEN_WIDTH + x + aX] & aImage[y * aWidth + x];
		}
	}
}

void BitBltMasked(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aMaskImage, u16* aVRAM) {
	u16 bitbltWork[32*32];

	for(u16 i = 0;i < aWidth * aHeight;i++){
		bitbltWork[i] = ~aMaskImage[i];
	}

	BitBltOR(aX, aY, aWidth, aHeight, (u16*)bitbltWork, aVRAM);
	BitBltAND(aX, aY, aWidth, aHeight, (u16*)aImage, aVRAM);
}

void BitBltMaskedComp(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM) {
	u16 rgb[2];
	u8 count[2];

	u16 size = aWidth * aHeight;
	u16 i = 0, a = 0;
	while(a < size){
		count[0] = (u8)(aImage[i]>>8);
		count[1] = (u8)(aImage[i]&0xFF);
		i++;
		for (u16 j = 0; j < 2; j++){
			rgb[j] = aImage[i];
			i++;
		}
		for (u16 k = 0; k < 2; k++){
			for (u16 j = 0; j < count[k];j++){
				if ((rgb[k]&0x8000)!=0){
					u16 y = (int)(a / aWidth + 1);
					u16 x = a % aWidth;
					aVRAM[((aHeight - y) + aY) * SCREEN_WIDTH + x + aX] = (rgb[k]&0x7FFF);
				}
				a++;
			}
		}
	}
}

#define CHARCTER_SIZE	8
void DrawText(u16 aX, u16 aY, char* aStrings, u16 aStringColor, u16 aBGColor, u16 aRectMask, u16* aVRAM) {
	u16 CharBuffer[CHARCTER_SIZE*CHARCTER_SIZE], MaskBuffer[CHARCTER_SIZE*CHARCTER_SIZE], textPosX, temp;
	u8 charCode;
	u16 maskPattern[CHARCTER_SIZE*CHARCTER_SIZE];

	for(u16 i = 0;i < CHARCTER_SIZE*CHARCTER_SIZE;i++){
		maskPattern[i] = aBGColor;
	}

	textPosX = aX;
	while(*aStrings){
		charCode = *aStrings;

		u16 k = 0;
		for(u16 j = 0;j < CHARCTER_SIZE;j++){
			temp = bkg_Map[charCode * CHARCTER_SIZE + j];

			for(u16 i = 0;i < CHARCTER_SIZE;i++){
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
