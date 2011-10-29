// face.h - Draw a face with various moods
//
// Copyright (c) 2011 John Graham-Cumming

#ifndef INCLUDED_FACE
#define INCLUDED_FACE

#include "WProgram.h"

// Expressions for the first parameter of face_show

#define FACE_NORMAL 0
#define FACE_LEFT   1
#define FACE_RIGHT  2
#define FACE_SING   3
#define FACE_SMILE  4
#define FACE_SAD    5
#define FACE_SHOUT  6

void face_show( int e, byte r, byte g, byte b, byte in );

#endif // INCLUDED_FACE
