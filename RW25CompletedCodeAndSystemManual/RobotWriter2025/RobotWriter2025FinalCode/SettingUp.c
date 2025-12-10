#include "SettingUp.h"
#include "CheckingWordAndGCodeCommands.h" 
#include "main.h"

#ifdef SETTINGUP_H_INCLUDED

// FONT SIZE 
float Font_Size;
float Font_Size_Fraction;

int FontCount = 0;
struct AsciiLine *FontArray = NULL;

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
            break;
        }
        if (ScannedValue != 999)
        {
            continue; 
        }
        FontArray = realloc(FontArray, (size_t)(FontCount + 1) * sizeof(struct AsciiLine)); // Reallocated the size of the array to be 1 larger than it was to allow for the storage of the next ascii character's stroke lines 
        struct AsciiLine *Line = &FontArray[FontCount];
        fscanf(FP, "%d", &Line->AsciiValue); // Scans the next integer after 999 and sets this as the ascii value for the character
        fscanf(FP, "%d", &Line->NumberOfLines); // Scans the next integer and sets it as the number of stroke lines the character has
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

float FontSize(void)
{
    printf("Please input any value between 4 and 10 to select a font size\n");
    scanf("%f", &Font_Size);
    if (Font_Size >= 4 && Font_Size <=10) // Checks to see if the number scanned is an integer between 4 and 10
    {
        printf("\nYou have selected the font size: %f\n\n", Font_Size);
        getchar();
        return Font_Size;
    }
    if (!Font_Size) // Font_Size will be NULL if a number isn't input by te user
    {
        printf("\nPlease make sure you input a number between 4 and 10\n");
        getchar();
        return FontSize();
    }
    else 
    {
        printf("\nYour number isn't between 4 and 10, please try again:\n");
        getchar();
        return FontSize();
    }
}

void SetRobot(void) // Sends the specific commands to the robot that get it ready in the location for the first letter to be drawnn
{
    sprintf (MyBuffer, "G1 X0 Y0 F1000\n");
    SendCommands(MyBuffer);

    sprintf (MyBuffer, "M3\n");
    SendCommands(MyBuffer);

    sprintf (MyBuffer, "S0\n");
    SendCommands(MyBuffer);

    SetNewLine();
}

#endif