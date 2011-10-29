// font.cpp - 5x7 font for scrolling text
//
// Copyright (c) 2011 John Graham-Cumming

#include "font.h"
#include "frame.h"

// For compactness each letter form is stord as five bytes.  Each byte 
// corresponds to one column of the letter with 0 indicating frame off
// and 1 indicating frame on.  This wastes 1 bit per column

// The upper case alphabet

#define CHAR_WIDTH 5

byte upper[26][CHAR_WIDTH] = { { B0111111, B1001000, B1001000, B1001000, B0111111 }, // A
                               { B1111111, B1001001, B1001001, B1001001, B0110110 }, // B
                               { B0111110, B1000001, B1000001, B1000001, B0100010 }, // C
                               { B1111111, B1000001, B1000001, B1000001, B0111110 }, // D
                               { B1111111, B1001001, B1001001, B1001001, B1000001 }, // E
                               { B1111111, B1001000, B1001000, B1001000, B1000000 }, // F
                               { B0111110, B1000001, B1000001, B1001001, B0101111 }, // G
                               { B1111111, B0001000, B0001000, B0001000, B1111111 }, // H
                               { B0000000, B1000001, B1111111, B1000001, B0000000 }, // I
                               { B0000010, B0000001, B1000001, B1111110, B1000000 }, // J
                               { B1111111, B0001000, B0010100, B0100010, B1000001 }, // K
                               { B1111111, B0000001, B0000001, B0000001, B0000001 }, // L
                               { B1111111, B0100000, B0011000, B0100000, B1111111 }, // M
                               { B1111111, B0010000, B0001000, B0000100, B1111111 }, // N
                               { B0111110, B1000001, B1000001, B1000001, B0111110 }, // O
                               { B1111111, B1001000, B1001000, B1001000, B0110000 }, // P
                               { B0111110, B1000001, B1000101, B1000010, B0111101 }, // Q
                               { B1111111, B1001000, B1001100, B1001010, B0110001 }, // R
                               { B0110000, B1001001, B1001001, B1001001, B0000110 }, // S
                               { B1000000, B1000000, B1111111, B1000000, B1000000 }, // T
                               { B1111110, B0000001, B0000001, B0000001, B1111110 }, // U
                               { B1111100, B0000010, B0000001, B0000010, B1111100 }, // V
                               { B1111110, B0000001, B0001110, B0000001, B1111110 }, // W
                               { B1100011, B0010100, B0001000, B0010100, B1100011 }, // X
                               { B1110000, B0001000, B0000111, B0001000, B1110000 }, // Y
                               { B1000011, B1000101, B1001001, B1010001, B1100001 }, // Z
                             };

// The lower case alphabet

byte lower[26][CHAR_WIDTH] = { { B0001110, B0010001, B0010001, B0001010, B0011111 }, // a
                               { B1111111, B0001010, B0010001, B0010001, B0001110 }, // b
                               { B0001110, B0010001, B0010001, B0010001, B0001010 }, // c
                               { B0001110, B0010001, B0010001, B0001010, B1111111 }, // d
                               { B0001110, B0010101, B0010101, B0010101, B0001101 }, // e
                               { B0000100, B0111111, B1000100, B1000000, B0100000 }, // f
                               { B0001000, B0010101, B0010101, B0010101, B0011110 }, // g
                               { B1111111, B0001000, B0010000, B0010000, B0001111 }, // h
                               { B0000000, B0001001, B0101111, B0000001, B0000000 }, // i
                               { B0000000, B0000010, B0010001, B1011110, B0000000 }, // j
                               { B1111111, B0000100, B0001010, B0010001, B0000000 }, // k
                               { B0000000, B1111110, B0000001, B0000001, B0000000 }, // l
                               { B0011111, B0010000, B0001110, B0010000, B0001111 }, // m
                               { B0011111, B0001000, B0010000, B0010000, B0001111 }, // n
                               { B0001110, B0010001, B0010001, B0010001, B0001110 }, // o
                               { B0011111, B0010100, B0010100, B0010100, B0001000 }, // p
                               { B0001000, B0010100, B0010100, B0010100, B0011111 }, // q
                               { B0011111, B0001000, B0010000, B0010000, B0000000 }, // r
                               { B0001001, B0010101, B0010101, B0010101, B0010010 }, // s
                               { B0001000, B0111110, B0001001, B0000010, B0000000 }, // t
                               { B0011110, B0000001, B0000001, B0000010, B0011111 }, // u
                               { B0011000, B0000110, B0000001, B0000110, B0011000 }, // v
                               { B0011100, B0000011, B0001100, B0000011, B0011100 }, // w
                               { B0010001, B0001010, B0000100, B0001010, B0010001 }, // x
                               { B0011000, B0000101, B0000101, B0000101, B0011110 }, // y
                               { B0010001, B0010011, B0010101, B0011001, B0010001 }, // z
                             };

// font_write: write a letter into the frame buffer at a particular column with
// specified color and brightness.  This function returns the actual number of
// display columns written to so that the font can be proportional (letters like
// i and l take up less space)
int font_write( byte l, // Letter (0 = A, 1 = B, ... )
                byte uc, // 1 = upper, 0 = lower
                byte p, // Frame column position (0 - 8)
                byte r, byte g, byte b, byte in ) // Colour and brightness
{  
  
  // This will contain the actual number of columns written to the frame
  
  int written = 0;
  
  for (int i = 0; i < 5; ++i ) {
    byte c = uc?upper[l][i]:lower[l][i];
    
    if ( c != 0 ) {
      for ( int j = 0; j < 7; ++j ) {
        frame[p][j].r = (c & 0x1)?r:0;
        frame[p][j].g = (c & 0x1)?g:0;
        frame[p][j].b = (c & 0x1)?b:0;
        frame[p][j].i = (c & 0x1)?in:0;
      
        c >>= 1;
      }
      
      written +=1;
      p += 1;
    }
  }  
  
  return written;
}  

// font_scroll: scroll a line of text across the display
void font_scroll( char * text,                     // Text 
                 byte r, byte g, byte b, byte in ) // Colour and brightness
{ 
  while ( *text ) {
    int cols = 3;
    if ( *text != ' ' ) {
      if ( *text >= 'a' ) {
         cols = font_write( *text - 'a', 0, 7, r, g, b, in );
      } else {
         cols = font_write( *text - 'A', 1, 7, r, g, b, in );
      }
    }
    
    for ( int i = 0; i < cols + 1; ++i ) {
      frame_scroll();
      delay(100);
    }
    
    text += 1;
  }
}

                
              
