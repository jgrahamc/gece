// protocol.cpp - The GE Color Effects Serial protoc5ol
//
// Copyright (c) 2011 John Graham-Cumming
//
// Provides functions for initializing the display and sending color
// and brightness information to LEDs.
//
// This file is based on the work done by Robert Sun Quattlebaum and
// published on his web site.  The URL for the entry of GE hacking is:
// http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/

#include "protocol.h"

// This library of macros is used because we know the pin assignment
// beforehand and we want to minimize function calling to get accurate
// serial timing.

#include "digitalWriteFast.h"

// And we want accurate us timings so use special code and not the built
// in

#include "delay_x.h"

// The Arduino Pro pin to which the display is connected

#define DISPLAY_PIN 7

// NOTE: Macros are used instead of functions so that we have
// maximum control of timing of the serial protocol

// send_one, send_zero: send a one or zero bit to the LEDs with
// correct timing
  
// The timing for a bit one is as follows:
//
// The line is driven low for 20us then held high
// for 10us.
//
// Note that the actual values used here were determined
// by experimentation and by measurement using the logic
// analyzer.
#define send_one                         \ 
{                                        \
  digitalWriteFast( DISPLAY_PIN, LOW );  \
  _delay_us( 18 );                       \
  digitalWriteFast( DISPLAY_PIN, HIGH ); \
  _delay_us( 8 );                        \
  digitalWriteFast( DISPLAY_PIN, LOW );  \
}

// The timing for a bit zero is as follows:
//
// The line is driven low for 20us then held high
// for 10us.
//
// Note that the actual values used here were determined
// by experimentation and by measurement using the logic
// analyzer.
#define send_zero                        \
{                                        \
  digitalWriteFast( DISPLAY_PIN, LOW );  \
  _delay_us( 8 );                       \
  digitalWriteFast( DISPLAY_PIN, HIGH ); \
  _delay_us( 20 );                       \
  digitalWriteFast( DISPLAY_PIN, LOW );  \
}

// send_bits: transmits up to 8 bits to the LED sending from the MSB
// first
#define send_bits( _v, _b )            \ 
{                                      \
  int _c = _b;                         \
  while ( _c > 0 ) {                   \
    if ( _v & ( 1 << ( _c - 1 ) ) ) {  \
      send_one                         \
    } else {                           \
      send_zero                        \
    }                                  \
    _c -= 1;                           \
  }                                    \
}

// start_packet: set the line for sending a packet
// Each packet begins with 10us of bus high
#define start_packet                      \
{                                         \
  digitalWriteFast( DISPLAY_PIN, HIGH );  \
  _delay_us( 10 );                        \
  digitalWriteFast( DISPLAY_PIN, LOW );   \
}

// end_packet: called when the packet has been transmitted
// Must ensure that the bus is idle for at least 30us
// between packets
#define end_packet                       \
{                                        \
  digitalWriteFast( DISPLAY_PIN, LOW );  \
  _delay_us( 30 );                       \
}

// protocol_set_led_state: sets the state (color and brightness) of an
// LED in the display.  The (x, y) coordinates range from (0,0)
// to (6,6).  The color is (r, g, b) where each value is between
// 0 and 15.  The brightness is between 0 and 255, but notes in the
// blog post mentioned above say that 0xCC seems to be the maximum
// so I make an explicit check for that here to ensure that no
// LEDs are damaged
void protocol_set_led_state( byte x,  // X-coordinate of LED
                             byte y,  // Y-coordinate of LED
                             byte r, byte g, byte b, // Color 
                             byte i ) // Brightness
{
  
  // Ensure that brightness cannot go too high
  
  if ( i > MAX_BRIGHTNESS ) {
    i = MAX_BRIGHTNESS;
  }
  
  // The packet format is as follows:
  // 
  // Name             Bits
  // ----             ----
  //
  // Start bit           1
  // Bulb address        6
  // Brightness          8
  // Blue                4
  // Green               4
  // Red                 4
  //
  // Note that values are sent MSB first
  
  int id = y * 8 + x;
  start_packet
  send_bits( id, 6 )
  send_bits( i, 8 )
  send_bits( b, 4 )
  send_bits( g, 4 )
  send_bits( r, 4 )
  end_packet
}

// init_led: send the ID to the next LED in the chain
void init_led( byte x,     // X-coordinate of LED
               byte y ) {  // Y-coordinate of LED
  
  // Initializing the LED is actually just achieved by sending it a packet setting
  // its ID.  This has to be done at power up and is called from protocol_init
  
  protocol_set_led_state( x, y, 4, 5, 6, 100 );
  
  // Observing the start up of the actual GE lights shows that it allows 0.005s
  // between initializations.  I don't know if this is necessary, but I am going
  // to follow the same
  
  delay( 50 );
}

// protocol_init: Set up a newly powered-on string of lights.  The lights are arranged
// in an array wired diagonally starting from the bottom left.  For ease of programming
// it's better if they are given numbers which correspond to coordinates.  Given that
// each LED has a 6 bit address it's possible to address the entire array using 3 bits
// for X and 3 bits for Y (the 50th LED will be (7,0) and the display itself will be
// numbered starting (0,0) up to (6,6).
//
// For example, the first (single LED) diagonal is (0,0), the second diagonal is (1,0) and
// (0,1), the third is (2,0), (1,1) and (0,2)... the loop inside this function expolits this
// to program the LEDs.
//
// The actual display is wired as follows 
//
// 21 33 34 42 43 47 48
// 20 22 32 35 41 44 46
// 10 19 23 31 36 40 45
// 09 11 18 24 30 37 39
// 03 08 12 17 25 29 38
// 02 04 07 13 16 26 28
// 00 01 05 06 14 15 27 
//
// The initializer sets these up with the following addresses (corresponding to X in the bottom
// 3 bits and Y in the top 3 bits)
//
// 06 14 22 30 36 46 54
// 05 13 21 29 37 45 53
// 04 12 20 28 36 44 52
// 03 11 19 27 35 43 51
// 02 10 18 26 34 42 50
// 01 09 17 25 33 41 49
// 00 08 16 24 32 40 48
//
// Thus the order in which the (x, y) coordinates are programmed is:
//
// (0, 0), (0, 1), (1, 0), (2, 0), (1, 1), (0, 2), (0, 3), ...
//
// Also notice that the sum of the coorindates in each diagonal is related to the direction
// of wiring: even diagonals are wired upwards, odd diagonals downward.
//
// Initially all the LEDs are set to RGB color (0,0,0) with no brightness (i.e. they are 
// each sent an address and told to be off).
void protocol_init()
{
  pinMode( DISPLAY_PIN, OUTPUT );
  digitalWrite( DISPLAY_PIN, LOW );
  
  // This is done so we see that the bus is low before we do anything on it
  
  delay( 1000 );
    
  // See comment preceding this function for explanation of this loop
  
  for ( int sum = 0; sum <= 12; ++sum ) {
    if ( ( sum % 2 ) == 0  ) {
      int x = (sum < 8)?0:(sum-6);
      for ( int y = sum - x; y >= 0; --y, ++x ) {
        if ( x < 7 ) {
          init_led( x, y );
        }
      }
    } else {
      int x = sum;
      for ( int y = 0; ( y < 7 ) && ( x >= 0 ); ++y, --x ) {
        if ( x < 7 ) {
          init_led( x, y );
        }
      }
    }
  }
}

// protocol_broadcast: helper function that sets color and brightness
// on every LED using the broadcast functionality (ID set to 63 which 
// corresponds to (7, 7)
void protocol_broadcast( int r, int g, int b, // Color
                         int i )              // Brightness
{
    protocol_set_led_state( 7, 7, r, g, b, i );
}

// protocol_test_card: run through test images to show that the display is working
void protocol_test_card()
{
  protocol_broadcast( 0, 0, 0, 0 );
  
  // 1. Set all the LEDs to white and fade from 0 to maximum

  for ( int i = 0; i < MAX_BRIGHTNESS; i += 10 ) {
      
    // Note that 13 is used here for all the RGB values as that is 
    // apparently what GE do for white (instead of 15)

    protocol_broadcast( 13, 13, 13, i );        
    delay( 50 );
  }  

  protocol_broadcast( 0, 0, 0, 0 );

  for ( byte x = 0; x < 7; ++x ) {
    for ( byte y = 0; y < 7; ++y ) {
        protocol_set_led_state( x, y, 15, 0, 0, MAX_BRIGHTNESS );
        delay( 50 );
    }
  }

  delay( 1000 );
  protocol_broadcast( 0, 0, 0, 0 );
  
  // 2. Set alternating LEDs to red and blue and then reverse
  
  for ( byte mode = 0; mode < 2; ++mode ) {
    for ( byte x = 0; x < 7; ++x ) {
      for ( byte y = 0; y < 7; ++y ) {
        if ( ( ( x + y ) & 1 ) == mode ) {
          protocol_set_led_state( x, y, 15, 0, 0, MAX_BRIGHTNESS );
        }  else {
          protocol_set_led_state( x, y, 0, 0, 15, MAX_BRIGHTNESS );
        }
      }
    }
    
    delay( 1000 );
  }

  protocol_broadcast( 0, 0, 0, 0 );
  
  // 3. Vertical stripes of green and yellow and then reverse

  for ( byte mode = 0; mode < 2; ++mode ) {
    for ( byte x = 0; x < 7; ++x ) {
      for ( byte y = 0; y < 7; ++y ) {
        if ( ( x % 2 ) == mode ) {
          protocol_set_led_state( x, y, 0, 15, 0, MAX_BRIGHTNESS );
        }  else {
          protocol_set_led_state( x, y, 15, 15, 0, MAX_BRIGHTNESS );
        }
      }
    }
    
    delay( 1000 );
  }
     
  protocol_broadcast( 0, 0, 0, 0 );
  
  // 4. Horizontal stripes of orange and purple and then reverse

  for ( byte mode = 0; mode < 2; ++mode ) {
    for ( byte x = 0; x < 7; ++x ) {
      for ( byte y = 0; y < 7; ++y ) {
        if ( ( y % 2 ) == mode ) {
          protocol_set_led_state( x, y, 0, 7, 15, MAX_BRIGHTNESS );
        }  else {
          protocol_set_led_state( x, y, 15, 7, 0, MAX_BRIGHTNESS );
        }
      }
    }
    
    delay( 1000 );
  }
  
  protocol_broadcast( 0, 0, 0, 0 );
}
