/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(int board[][d], int d);
void draw(int board[][d]);
bool move(int tile, int board[][d], int d);
bool won(int board[][d], int d);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();
    
    int board[d][d];

    // initialize the board
    init(board, d);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(board);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(board, d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile,board,d))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int board[][d], int d)
{
    int k = 1;
    // Fills board locations with integers (dimension^2 - k)
    // Starting in top left corner
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = (d*d)-k;
            k++;
        }
    }
    
    int temp;
    // If even board dimension, swap 1 and 2
    if(d%2 == 0)
    {
        temp = board[d-1][d-2];
        board[d-1][d-2] = board[d-1][d-3];
        board[d-1][d-3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(int board[][d])
{
    printf("Board state:\n");
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // 0 is chosen as integer for empty tile
            // Print instead underscore _ to highlight the tile
            if(board[i][j] == 0)
            {
                printf(" _  ");
            }
            else
            {
                printf("%2i  ", board[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile, int board[][d], int d)
{
    // Finding input tile location
    int tileRow = -1;
    int tileCol;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // Searching for tile value
            if(board[i][j] == tile)
            {
                tileRow = i;
                tileCol = j;
                break;
            }
        }
        if(tileRow != -1)
        {
            break;
        }
    }
    
    // Finding open slot location
    int openRow = -1;
    int openCol;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // Searching for 0 value
            // representing open slot
            if(board[i][j] == 0)
            {
                openRow = i;
                openCol = j;
                break;
            }
        }
        if(openRow != -1)
        {
            break;
        }
    }
    
    // If there is a 1 tile row difference and no column difference
    // between open slot and chosen tile
    // swap the values and return true
    if(abs(openRow-tileRow) == 1 && (openCol-tileCol) == 0)
    {
        board[openRow][openCol] = tile;
        board[tileRow][tileCol] = 0;
        return true;
    }
    
    // If there is a 1 tile column difference and no row difference
    // between open slot and chosen tile
    // swap the values and return true
    if(abs(openCol-tileCol) == 1 && (openRow-tileRow) == 0)
    {
        board[openRow][openCol] = tile;
        board[tileRow][tileCol] = 0;
        return true;
    }
    
    // If tile input is not valid, return false, error is printed in main
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int board[][d], int d)
{
    int tileValue = 1;
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // If each location do not match the won state
            // return false
            if((board[i][j] != tileValue) && tileValue < d*d)
            {
                return false;
            }
            tileValue++;
            // If tile value count has reached dim*dim without any
            // wrong tiles, return true
            if(tileValue == d*d)
            {
                return true;
            }
        }
    }
    return false;
}
