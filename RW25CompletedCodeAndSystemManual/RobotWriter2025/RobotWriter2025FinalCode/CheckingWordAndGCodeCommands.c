#include "CheckingWordAndGCodeCommands.h"
#include "SettingUp.h"
#include "main.h"

#ifdef CHECKINGWORDANDGCODECOMMANDS_H_INCLUDED

// Global Varibles
int i, j;
float Pen_Coordinate_X = 0;
float Pen_Coordinate_Y = 0;
char MyBuffer[256];
float X, Y;
int P;

// READ NEXT WORD
int CharactersInWord;
char Character;
int Word[256];

// WORD LENGTH
float Character_Length_X;
float Word_Length_X = 0;

// SETTING NEW LINE
int NewLine = 0;

void ReadNextWord(void)
{ 
    FILE *Sentence = fopen("SampleLines.txt", "r");
    if (!Sentence)
    {
        printf("The file that contains your sentence couldn't be found\n\n");
        exit(0);
    }
    int EndOfSentence = 0;
    printf("\nThe sentence file has been found and the robot is now ready to draw\n");
    while(1)
    {   
        CharactersInWord = 0;
        while(1) 
        {
            if (fscanf(Sentence, "%c", &Character) != 1) // No character to scan concludes the sentence has all been scanned
            {
                EndOfSentence = 1; 
                break;
            }
            if (Character == '\n') // \n concludes the user has moved to the next line and this is the end of the word  
            {
                NewLine = 1;
                break;
            }
            Word[CharactersInWord++] = Character;
            if (Character == ' ') // Space concludes this is the end of the word being scanned and therefore it can now be processed
            {
                break;
            }
        }
        WordLength();
        if ( Word_Length_X > (100 - Pen_Coordinate_X)) // If there isn't enough space for the word in the remaining space on the line the function sets the pen's coordinates to the line below
        {
            SetNewLine();
        }
        CharacterGCode();
        if (NewLine == 1)
        {
            SetNewLine();
            NewLine = 0;
        }
        if (EndOfSentence == 1) 
        {
            break;
        }
    }
}

float WordLength(void)
{
    Word_Length_X = 0;
    for(i = 0; i < CharactersInWord; i++) // This loop finds the ending stroke's X value in each of the characters and sums them to find the word length
    {
        struct AsciiLine *Line = &FontArray[(int)Word[i]];
        sscanf(Line->Lines[Line->NumberOfLines], "%f", &Character_Length_X);
        Word_Length_X = Word_Length_X + Character_Length_X * Font_Size_Fraction;
    }
    if(Word_Length_X > 100)
    {
        printf("The next word that is about to be drawn in your sentence is to long to fit on the page");
        exit(0);
    }
    return Word_Length_X;
}

void SetNewLine(void)
{   
    Pen_Coordinate_X = 0;
    Pen_Coordinate_Y = Pen_Coordinate_Y - 2*Font_Size;
    sprintf(MyBuffer, "G0 X0 Y%f\n", Pen_Coordinate_Y);
    SendCommands(MyBuffer);
}

void CharacterGCode(void)
{
    for(i = 0; i < CharactersInWord; i++)
    {
        struct AsciiLine *Line = &FontArray[(int)Word[i]]; 
        for(j = 1; j <= Line->NumberOfLines; j++)
        {
        sscanf(Line->Lines[j], "%f %f %d", &X, &Y, &P);
        Character_Length_X = X*Font_Size_Fraction;
        X = Pen_Coordinate_X + X*Font_Size_Fraction;
        Y = Pen_Coordinate_Y + Y*Font_Size_Fraction;
        if(P == 1)
        {
            sprintf(MyBuffer,"S1000\n");
            SendCommands(MyBuffer);
        }
        if(P == 0)
        {
            sprintf(MyBuffer,"S0\n");
            SendCommands(MyBuffer);
        }
        sprintf(MyBuffer, "G%d X%f Y%f\n", P, X, Y);
        SendCommands(MyBuffer);
        }
        Pen_Coordinate_X = Pen_Coordinate_X + Character_Length_X; // Edits the pen's X location after each character has been drawn and therefore the characters get drawn next to each other
    }
}

#endif

