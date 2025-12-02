#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

// Global Varibles
int i, j;
float Pen_Coordinate_X = 0;
float Pen_Coordinate_Y = 0;
char MyBuffer[256];
float X, Y;
int P;

// FONT SIZE 
float FontSize(void);
float Font_Size;
float Font_Size_Fraction;

// READ NEXT WORD
void ReadAndSendNextWord();
int CharactersInWord;
char Character;
int Word[256];

// WORD LENGTH
float WordLength();
float Character_Length_X;
float Word_Length_X = 0;

// SETTING NEW LINE
void SetNewLine();
int NewLine = 0;

// CHARACTER GCODE
void CharacterGCode();

// SETTING THE ROBOT
void SetRobot();

