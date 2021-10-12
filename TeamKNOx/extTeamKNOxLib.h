// Extern declration

extern s8 gWorkStr[8];
extern void num02str(u8 number);
extern void num04str(u16 number);
extern void WaitForVsync(void);
extern void SetPixel(u16 aX, u16 aY, u16 aColor, u32 aVRAM);
extern void DrawLine(u16 aX0, u16 aY0, u16 aX1, u16 aY1, u16 aColor, u32 aVRAM);
extern void DrawCircle(u16 aX, u16 aY, u16 aRadius, u16 aColor, u32 aVRAM);
extern void DrawBox(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u32 aVRAM);
extern void DrawBoxHalf(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u32 aVRAM);
extern void BitBltSRCCOPY(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM);
extern void BitBltOR(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM);
extern void BitBltAND(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM);
extern void BitBltExOR(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM);
extern void BitBltMasked(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u16* aMaskImage, u32 aVRAM);
extern void BitBltMaskedComp(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16* aImage, u32 aVRAM);
extern void DrawTextInit();
extern void DrawText(u16 aX, u16 aY, s8* aStrings, u16 aStringColor, u16 aBGColor, u16 aRectMask, u32 aVRAM);
extern void Off2VRAM(u32 aSourceAddress, u32 aDistAddress);
extern void DrawBoxEmpty(u16 aX, u16 aY, u16 aWidth, u16 aHeight, u16 aColor, u32 aVRAM);
extern void seed_lc(u32 v);
extern u32 random_lc(void);
extern u32 gRandomSeed;
extern void InitUART(u16 parameter);
extern void SendByte(u8 data);
extern void AWait();
extern void Wait(u32 aWaitingTime);


