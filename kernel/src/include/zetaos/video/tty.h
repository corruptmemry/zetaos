#pragma once
#include <zetaos/zetalib.h>

extern Framebuffer* framebuffer;
extern PSF1_FONT* psf1_font;
extern Point CursorPosition;
extern void Print(unsigned int colour, char* str);
extern void init_video(Framebuffer* fb, PSF1_FONT* fnt);
extern void PutPix(uint32_t x, uint32_t y, uint32_t colour);
extern void ClearChar();
extern void putChar(unsigned int colour, char chr, unsigned int xOff, unsigned int yOff);
extern void PutChar(char chr);
extern void Clear();