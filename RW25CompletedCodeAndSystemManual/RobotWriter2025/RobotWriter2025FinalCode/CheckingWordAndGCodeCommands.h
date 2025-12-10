#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

#ifndef CHECKINGWORDANDGCODECOMMANDS_H_INCLUDED
#define CHECKINGWORDANDGCODECOMMANDS_H_INCLUDED

// Global Varibles
extern int i, j;
extern float Pen_Coordinate_X;
extern float Pen_Coordinate_Y;
extern char MyBuffer[256];
extern float X, Y;
extern int P;

// READ NEXT WORD
void ReadNextWord();

// WORD LENGTH
void WordLength();

// SETTING NEW LINE
void SetNewLine();

// CHARACTER GCODE
void CharacterGCode();

#endif