// frame.cpp - Frame buffer handling
//
// Copyright (c) 2011 John Graham-Cumming
//
// Provides functionality for a frame buffer than can be manipulated
// and then mirrored to the display.  All drawing should be done in the
// frame buffer and frame functions used rather than direct display writes

#include "frame.h"

// This is the 'working frame' where changes are made to the display, this
// will be mirrored to the real display whose current state is stored in
// live

pixel frame[DISPLAY_WIDTH * 2][DISPLAY_HEIGHT];

// This reflects the state of the actual display

pixel live[DISPLAY_WIDTH][DISPLAY_HEIGHT];

// frame_mirror: mirror whatever is in frame to live making appropriate changes
// the the display
void frame_mirror()
{
  for ( byte x = 0; x < DISPLAY_WIDTH; ++x ) {
    for ( byte y = 0; y < DISPLAY_HEIGHT; ++y ) {
      if ( ( frame[x][y].r != live[x][y].r ) ||
           ( frame[x][y].g != live[x][y].g ) ||
           ( frame[x][y].b != live[x][y].b ) ||
           ( frame[x][y].i != live[x][y].i ) ) {       
        protocol_set_led_state( x, y, 
                                frame[x][y].r,
                                frame[x][y].g,
                                frame[x][y].b,
                                frame[x][y].i );
        live[x][y] = frame[x][y];
      }
    }
  }
}

// frame_init: Sets up the made frame buffer (all pixels off) and mirrors
// that to the actual display
void frame_init()
{
  for ( byte x = 0; x < DISPLAY_WIDTH * 2; ++x ) {
    for ( byte y = 0; y < DISPLAY_HEIGHT; ++y ) {
      frame[x][y].r = 0;
      frame[x][y].g = 0;
      frame[x][y].b = 0;
      frame[x][y].i = 0;
    }
  }
  
  for ( byte x = 0; x < DISPLAY_WIDTH; ++x ) {
    for ( byte y = 0; y < DISPLAY_HEIGHT; ++y ) {

      // Setting these to 255 will force frame_mirror to update the
      // display

      live[x][y].r = 255;
      live[x][y].g = 255;
      live[x][y].b = 255;
      live[x][y].i = 255;
    }
  }
  
  frame_mirror();
}

// frame_scroll: scroll the frame buffer left one column
void frame_scroll()
{
  for ( byte x = 1; x < DISPLAY_WIDTH * 2; ++x ) {
    for ( byte y = 0; y < DISPLAY_HEIGHT; ++y ) {
      frame[x-1][y] = frame[x][y];
    }
  }
  
  frame_mirror();
}

