// protocol.h - The GE Color Effects Serial protoc5ol
//
// Copyright (c) 2011 John Graham-Cumming
//
// Provides functions for initializing the display and sending color
// and brightness information to LEDs.
//
// This file is based on the work done by Robert Sun Quattlebaum and
// published on his web site.  The URL for the entry of GE hacking is:
// http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/

#ifndef INCLUDED_PROTOCOL
#define INCLUDED_PROTOCOL

#include "WProgram.h"
 
#define MAX_BRIGHTNESS 0xCC

#define DISPLAY_WIDTH  7
#define DISPLAY_HEIGHT 7

void protocol_init();
void protocol_test_card();
void protocol_set_led_state( byte x, byte y, byte r, byte g, byte b,  byte i );

#endif // INCLUDED_PROTOCOL

