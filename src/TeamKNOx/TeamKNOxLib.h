#ifndef TEAMKNOXLIB
#define TEAMKNOXLIB

#include <tonc.h>

// Extern declration

extern char gWorkStr[8];
extern void num02str(u8 number);
extern void DrawBox(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u16* aVRAM);
extern void DrawBoxHalf(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aVRAM);
extern void BitBltSRCCOPY(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM);
extern void BitBltOR(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM);
extern void BitBltAND(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM);
extern void BitBltMasked(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aMaskImage, u16* aVRAM);
extern void BitBltMaskedComp(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aVRAM);
extern void DrawText(u16 aX, u16 aY, char* aStrings, u16 aStringColor, u16 aBGColor, u16 aRectMask, u16* aVRAM);
extern void DrawBoxEmpty(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u16* aVRAM);

// tonc compat
#define VRAM_ADDRESS (u16*)m3_mem
static inline void Off2VRAM(void* addr) {
	//memcpy16(m3_mem, addr, M3_SIZE/2);
	dma3_cpy(m3_mem, addr, M3_SIZE);
}

#endif // TEAMKNOXLIB
