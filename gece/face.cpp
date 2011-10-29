// face.cpp - Draw a face with various moods
//
// Copyright (c) 2011 John Graham-Cumming

#include "face.h"
#include "frame.h"

byte smile[7][7] = { { B0111110, B1000001, B1010101, B1000101, B1010101, B1000001, B0111110 }, // Normal
                     { B0111110, B1010001, B1000101, B1010101, B1000101, B1000001, B0111110 }, // Look left
                     { B0111110, B1000001, B1000101, B1010101, B1000101, B1010001, B0111110 }, // Look right
                     { B0111110, B1000001, B1010101, B1001011, B1010101, B1000001, B0111110 }, // Sing
                     { B0111110, B1000101, B1010011, B1000011, B1010011, B1000101, B0111110 }, // Smile
                     { B0111110, B1000011, B1010101, B1000101, B1010101, B1000011, B0111110 }, // Sad
                     { B0111110, B1000101, B1011011, B1001011, B1011011, B1000101, B0111110 }, // Shout
                   };

// face_show: show a 'smiley' face on the display
void face_show( int e,                            // The experssion to use (see face.h)
                byte r, byte g, byte b, byte in ) // Color and brightness for the face
{
  for (int i = 0; i < 7; ++i ) {
    byte c = smile[e][i];
    
    for ( int j = 0; j < 7; ++j ) {
      frame[i][j].r = (c & 0x1)?r:0;
      frame[i][j].g = (c & 0x1)?g:0;
      frame[i][j].b = (c & 0x1)?b:0;
      frame[i][j].i = (c & 0x1)?in:0;
             
      c >>= 1;
    }
  }
  
  frame_mirror();
}

