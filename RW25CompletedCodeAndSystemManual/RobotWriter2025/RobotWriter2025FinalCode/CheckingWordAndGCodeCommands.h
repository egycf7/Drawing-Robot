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

// FONT SIZE 
float FontSize(void);
extern float Font_Size;
extern float Font_Size_Fraction;

// READ NEXT WORD
void ReadNextWord();

// WORD LENGTH
float WordLength();

// SETTING NEW LINE
void SetNewLine();

// CHARACTER GCODE
void CharacterGCode();

// SETTING THE ROBOT
void SetRobot();

#endif