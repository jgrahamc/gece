// GECE - GE Color Effects Hack
//
// Copyright (c) 2011 John Graham-Cumming
//
// I hacked a set of GE Color Effects 50 lights to make a 7x7 color display and
// this program controls the display to show different light patterns, to respond
// to sound and to scroll text

#include "protocol.h"
#include "frame.h"
#include "font.h"
#include "face.h"

void setup()
{
  protocol_init();  
  protocol_test_card();
  
  frame_init();  
}

void loop()
{
  font_scroll( "Hello ", 15, 0, 0, MAX_BRIGHTNESS );
  font_scroll( "World ",  0, 15, 0, MAX_BRIGHTNESS );
  
  delay( 1000 );
  frame_init();
  
  face_show( FACE_NORMAL, 14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_LEFT,   14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_NORMAL, 14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_RIGHT,  14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_NORMAL, 14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_SING,   14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_NORMAL, 14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_SMILE,  14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_NORMAL, 14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_SAD,    14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_NORMAL, 14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  face_show( FACE_SHOUT,  14, 14, 0, MAX_BRIGHTNESS );  
  delay( 1000 );
  
  frame_init();
  
  font_scroll( "Thanks ", 0, 0, 15, MAX_BRIGHTNESS );
  font_scroll( "GE ",  13, 13, 13, MAX_BRIGHTNESS );
  
  delay( 1000 );
  frame_init();
}

