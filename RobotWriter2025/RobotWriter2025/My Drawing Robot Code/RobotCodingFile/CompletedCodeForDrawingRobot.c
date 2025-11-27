#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>



//Telling the main all the functions in the code:
// Global Varibles
int i, j;
float Pen_Coordinate_X = 0;
float Pen_Coordinate_Y = 0;

//FONT SIZE 
float FontSize(void);
float Font_Size;
float Font_Size_Fraction;

//READ NEXT WORD
void ReadAndSendNextWord();
int CharactersInWord;
char Character;
int Word[256];

//WORD LENGTH
float WordLength();
float Character_Length_X;
float Word_Length_X = 0;

// SETTING NEW LINE
void SetNewLine();
int NewLine = 0;

// CHARACTER GCODE
void CharacterGCode();
char GCode[40];

// SETTING THE ROBOT
void SetRobot();

//LOAD DATA
float X, Y;
int P;
struct AsciiLine
{
    int AsciiValue;
    int NumberOfLines;
    char *Lines[50];
};
struct AsciiLine *FontArray = NULL;
int FontCount = 0;
char buffer[256];


struct AsciiLine* LoadData(int *OutFontCount)
{
    FILE *FP = fopen("SingleStrokeFont.txt", "r");
    if (!FP)
    {
        printf("The file could not be opened, please check to make sure that the is a file called SingleStrokeFont.txt\n");
        exit(1);
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
            fgets(buffer, sizeof(buffer), FP); 
            buffer[strcspn(buffer,"\n")] = '\0';
            Line->Lines[i] = malloc(strlen(buffer) + 1); 
            strcpy(Line->Lines[i],buffer);
        }
        FontCount++; 
    }
    fclose(FP);
    *OutFontCount = FontCount;
    return FontArray;
}

int main(void)
{
    FontArray = LoadData(&FontCount); // Calls the load font data function. This scans and stores all the strokes for each ascii character 
    FontSize();
    Font_Size_Fraction = ((float)Font_Size/18.0f);
    SetRobot();
    ReadAndSendNextWord();
    exit(1);
}

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

void ReadAndSendNextWord(void)
{ 
    FILE *Sentence = fopen("SampleLines.txt", "r");
    if (!Sentence)
    {
        printf("The file that contains your sentence couldn't be found\n\n");
        exit(1);
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
        sprintf(GCode, "G0 X0 Y");
        sprintf(GCode+strlen(GCode), "%f", Pen_Coordinate_Y);
        printf("%s\n", GCode);
        //SendCommands(GCode);
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
        sprintf(GCode, "G%d X%f Y%f", P, X, Y);
        printf("%s\n", GCode);
        //SendCommands() 
        }
        Pen_Coordinate_X = Pen_Coordinate_X + Character_Length_X;
    }
}

void SetRobot(void)
{
    sprintf (GCode, "G1 X0 Y0 F1000\n");
    printf("%s\n", GCode);
    //SendCommands(GCode);

    sprintf (GCode, "M3\n");
    printf("%s\n", GCode);
    //SendCommands(GCode);

    sprintf (GCode, "S0\n");
    printf("%s\n", GCode);
    //SendCommands(GCode);

    SetNewLine();
}

// Will the sentence contain characters that aren't ascii?
// Do we need to worry about weather the sentence will fit on the page in the Y length?
// What is the difference between pen G1 and S1000 when it come to putting the pen down?
// What happens if the word is longer than 100 


