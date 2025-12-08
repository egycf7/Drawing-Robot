#ifndef SETTINGUP_H_INCLUDED
#define SETTINGUP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

//LOAD DATA
extern int FontCount;

struct AsciiLine
{
    int AsciiValue;
    int NumberOfLines;
    char *Lines[50];
};

struct AsciiLine* LoadData(int *OutFontCount);
extern struct AsciiLine *FontArray;

// FONT SIZE 
float FontSize(void);
extern float Font_Size;
extern float Font_Size_Fraction;

// SETTING THE ROBOT
void SetRobot();

#endif