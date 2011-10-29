// frame.h - Frame buffer handling
//
// Copyright (c) 2011 John Graham-Cumming

#ifndef INCLUDED_FRAME
#define INCLUDED_FRAME

#include "protocol.h"

// Structure that contains the color and brightness of a single pixel

typedef struct _pixel
{
  byte r; // Red (0-15)
  byte g; // Green (0-15)
  byte b; // Blue (0-15)
  byte i; // Brightness (0-0xCC)
} pixel;

// This is the 'working frame' where changes are made to the display, this
// will be mirrored to the real display whose current state is stored in
// live
//
// Note that this is double wide so that we can place additional pixels
// to the right of the display.  This is used for left scrolling of
// text (or anything).

extern pixel frame[DISPLAY_WIDTH*2][DISPLAY_HEIGHT];

void frame_init();
void frame_mirror();
void frame_scroll();

#endif // INCLUDED_FRAME
