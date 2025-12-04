#include "CheckingWordAndGCodeCommands.h"
#include "LoadingDataStructs.h"
#include "rs232.h"
#include "serial.h"
#include "main.h"

float FontSize(void) //Make it so letters don't crash the system.
{
    printf("Please input any value between 4 and 10 to select a font size\n");
    scanf("%f", &Font_Size);
    if (Font_Size >= 4 && Font_Size <=10) // Checks to see if the number scanned is an integer between 4 and 10
    {
        getchar();
        printf("\nYou have selected the font size: %f\n\n", Font_Size);
        return Font_Size; // Returns the users selected font size to the main
    }
    if (!Font_Size)
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
        while(1) // Either a space or \n denotes the end of a word and therefore the while loop break when one of these is scanned to allow it to be drawn before moving onto the next word
        {
            if (fscanf(Sentence, "%c", &Character) != 1)
            {
                EndOfSentence = 1; 
                break;
            }
            if (Character == '\n')
            {
                NewLine = 1;
                break;
            }
            Word[CharactersInWord++] = Character;
            if (Character == ' ') 
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
    for(i = 0; i < CharactersInWord; i++) // A Loop that sums each of the last X value for each character to work out the size of the word
    {
        struct AsciiLine *Line = &FontArray[(int)Word[i]];
        char *LastLine = Line->Lines[Line->NumberOfLines];
        sscanf(LastLine, "%f", &Character_Length_X);
        Word_Length_X = Word_Length_X + Character_Length_X * Font_Size_Fraction;
    }
    return Word_Length_X;
}

void SetNewLine(void)
{   
        Pen_Coordinate_X = 0;
        Pen_Coordinate_Y = Pen_Coordinate_Y - 2*Font_Size;
        sprintf(MyBuffer, "G0 X0 Y");
        sprintf(MyBuffer+strlen(MyBuffer), "%f", Pen_Coordinate_Y);
        if(Pen_Coordinate_Y < -43)
        {
            printf("You have reached the last line on the page, therefore the robot won't draw anymore words");
            exit(0);
        }
        //printf("%s\n", MyBuffer);
        SendCommands(MyBuffer);
}

void CharacterGCode(void)
{
    for(i = 0; i < CharactersInWord; i++)
    {
        struct AsciiLine *Line = &FontArray[(int)Word[i]];
        for(j = 0; j <= Line->NumberOfLines; j++)
        {
        char *Stroke = Line->Lines[j];
        sscanf(Stroke, "%f %f %d", &X, &Y, &P);
        Character_Length_X = X*Font_Size_Fraction;
        X = Pen_Coordinate_X + X*Font_Size_Fraction;
        Y = Pen_Coordinate_Y + Y*Font_Size_Fraction;
        if(P == 1)
        {
            sprintf(MyBuffer,"S1000\n");
            //printf(MyBuffer);
            SendCommands(MyBuffer);
        }
        if(P == 0)
        {
            sprintf(MyBuffer,"S0\n");
            //printf(MyBuffer);
            SendCommands(MyBuffer);
        }
        sprintf(MyBuffer, "G%d X%f Y%f\n", P, X, Y);
        //printf("%s", MyBuffer);
        SendCommands(MyBuffer);
        }
        Pen_Coordinate_X = Pen_Coordinate_X + Character_Length_X;
    }
}

void SetRobot(void)
{
    sprintf (MyBuffer, "G1 X0 Y0 F1000\n");
    //printf("%s\n", MyBuffer);
    SendCommands(MyBuffer);

    sprintf (MyBuffer, "M3\n");
    //printf("%s\n", MyBuffer);
    SendCommands(MyBuffer);

    sprintf (MyBuffer, "S0\n");
    //printf("%s\n", MyBuffer);
    SendCommands(MyBuffer);

    SetNewLine();
}

