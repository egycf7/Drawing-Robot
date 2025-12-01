#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

//LOAD DATA
int FontCount = 0;

struct AsciiLine
{
    int AsciiValue;
    int NumberOfLines;
    char *Lines[50];
};

struct AsciiLine* LoadData(int *OutFontCount);
struct AsciiLine *FontArray = NULL;
