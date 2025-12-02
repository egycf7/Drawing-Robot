#include "LoadingDataStructs.h"
#include "CheckingWordAndGCodeCommands.h" 
#include "rs232.h"
#include "serial.h"
#include "main.h"

struct AsciiLine* LoadData(int *OutFontCount)
{
    FILE *FP = fopen("SingleStrokeFont.txt", "r");
    if (!FP)
    {
        printf("The file could not be opened, please check to make sure that the is a file called SingleStrokeFont.txt\n");
        exit(0);
    }
    while (1)
    {
        int ScannedValue;
        if (fscanf(FP, "%d", &ScannedValue) != 1) // First check is to make sure there is an integer being scanned and therefore if there is nothing to scan we know the end of the file has been reached
        {
            printf("SingleStrokeFont.txt has been stored\n\n");
            break;
        }
        if (ScannedValue != 999)
        {
            continue; 
        }
        FontArray = realloc(FontArray, (size_t)(FontCount + 1) * sizeof(struct AsciiLine)); // Reallocated the size of the array to be 1 larger than it was to allow for the storage of the next ascii character's stroke lines 
        struct AsciiLine *Line = &FontArray[FontCount];
        fscanf(FP, "%d", &Line->AsciiValue); // Scans the next integer after 999 and sets this as the ascii value for the character
        fscanf(FP, "%d", &Line->NumberOfLines); // Scans the next integer after AsciiValue and set it to define the number of lines that the character has in it
        fgetc(FP);
        for (i = 0; i <= Line->NumberOfLines; i++) // A loop that reads through the lines for the specific ascii character (Number of loops = NumberOfLines)
        {
            fgets(MyBuffer, sizeof(MyBuffer), FP); 
            MyBuffer[strcspn(MyBuffer,"\n")] = '\0';
            Line->Lines[i] = malloc(strlen(MyBuffer) + 1); 
            strcpy(Line->Lines[i],MyBuffer);
        }
        FontCount++; 
    }
    fclose(FP);
    *OutFontCount = FontCount;
    return FontArray;
}