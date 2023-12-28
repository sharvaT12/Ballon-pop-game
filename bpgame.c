//
//  Author: Sharva Thakur
//  UIN: 654135206
//  Author: Daivaksh Patel
//  UIN: 663042299

//  file:  bpgame.c

#include <stdio.h>
#include <stdlib.h>
#include "bpgame.h"
#include "simpleio.h"


#include<math.h>
#include<time.h>
// creates BPGame itself with various features


/********
* FILE:  bpgame.h
* DESC:  This file specifies the interface/ADT for 
*	managing the "balloon-pop" game.
*
*	 A client/app to actually play the game must
*	 manage the user interface itself, but would
*	 use the ADT/interface here to perform the
*	 interface independent operations on the state
* 	 of the game.
*
* NOTES:  The type BPGame (and struct bpgame) are "incomplete":
*	 The actual contents of the structure to capture
*	 the state of a game instance are hidden from
*	 client programs.  Client programs work with a BPGame
*	 strictly by passing a pointer to functions of the ADT.
*
*	 This allows alternative implementation strategies
*	 while maintaining the same interface.
*
*	 A primitive client program bpop.c is provided and 
*	 illustrates how the BPGame ADT is used.
*/



/** 
    BPGame is incompletely specified as far as any client program 
	is concerned.
    A BPGame "instance" captures everything about a particular run
	of a balloon pop game:  the current state of the board, 
	the score, any additional info for the undo feature, etc.

    The definition of actual struct bpgame is hidden in bpgame.c 
**/

typedef struct game{
    char grid[MAX_ROWS][MAX_COLS];
    int score;
}game;

typedef struct bpgame{
    int rows;
    int cols;
    int total;
    int cur;
    int num;
    char type;
    game* cur_s;
}BPGame;

/**
* Function:  bp_create
* Parameters: number of rows and columns
* Description: returns pointer to newly created
*              board with given number of rows
*              and columns.
*
*              Populates board with random balloons.
*
*              If nrows or ncols is out of range, 
*              an error message is printed to stderr
*              and NULL is returned.
*
*/
extern BPGame * bp_create(int nrows, int ncols){
    srand(time(NULL));

    if(nrows > MAX_ROWS || nrows < 0){
        fprintf(stderr, "Out of range");
        return NULL;
    }
    if(ncols > MAX_COLS || ncols < 0){
        fprintf(stderr, "Out of range");
        return NULL;
    }

    BPGame *b = malloc(sizeof(BPGame));
    b->cols = ncols;
    b->rows = nrows;
    b->total = 2;
    b->cur = 0;

    b->cur_s = malloc(2 * sizeof (game));
    game temp;
    temp.score = 0;
    for(int i = 0; i < 2; i++){
        b->cur_s[i] = temp;
    }
    char elements[4] = {Red, Blue, Green,Yellow};
    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
            b->cur_s[b->cur].grid[i][j] = elements[(rand() % 4)];
        }
    }

    return b;
}

/**
* Function:  bp_create_from_mtx
* Parameters: number of rows and columns and 2D array of char
* Description: returns pointer to newly created
*              board with given number of rows
*              and columns.
*
*              Populates board contents of given 2D matrix (assumes
*	       given dimensions).
*
*	       Error conditions:
*              		If nrows or ncols is out of range, an error message 
*			is printed to stderr and NULL is returned.
*
*			If any element in the matrix is not one of the understood
*			color symbols (from the #define-s at top of this file),
*			an error message is pritnted to stderr and NULL is returned.
*			
*/
extern BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols){
    if(nrows > MAX_ROWS || nrows < 0){
        fprintf(stderr, "Out of range");
        return NULL;
    }
    if(ncols > MAX_COLS || ncols < 0){
        fprintf(stderr, "Out of range");
        return NULL;
    }
    BPGame *b = bp_create(nrows,ncols);
    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
            b->cur_s[b->cur].grid[i][j] = mtx[i][j];
        }
    }
    return b;
}

/**
* Function:  bp_destroy
* Description:  deallocates all dynamically allocated 
*  		memory associated with the board.
*		
*		Comment:  this might start out as
*		a one-liner.  
*
*/
extern void bp_destroy(BPGame * b){
    if(b !=  NULL){
        if(b->cur_s != NULL)
        free(b->cur_s);
        free(b);
    }
}


/**
* Function:  bp_display
* Description:  pretty-prints the current board.
*
*		uses '|', '+' and '-' to "draw" the boundaries
*               of the board.
*
*		For full credit rows and columns are labeled with
* 		their indices.  Column indices displayed "vertically"
*
*		Examples:
*
*  This board is full of balloons (like at the beginning of a game).
* 
*       +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = = o + o + + + ^ = |
*		  2 | ^ + ^ o + o + = = = = |
*		  3 | = ^ o o = o + + + = = |
*		    +-----------------------+
*		    0 1 2 3 4 5 6 7 8 9 0
*
*
*  This board has some empty slots represented by  '.'
*       +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = o o + o + + + ^ = |
*		  2 | ^ + . o + . + = . . = |
*		  3 | . . . o . . + + . . = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
*  Here is the same board using a space ' ' to 
*  represent an empty slot.
*
*       +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = o o + o + + + ^ = |
*		  2 | ^ +   o +   + =     = |
*		  3 |       o     + +     = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*/

extern void bp_display_STD(BPGame * b){
    for(int i = 0; i < b->rows ; i++){

        
        for(int s = 0; s < b->cols; s ++){
            printf("%c", b->cur_s[b->cur].grid[i][s]);
        }
        printf("\n");
    }
}

int can_pop(BPGame *b, char type, int r, int c){
    if(r >= MAX_ROWS || r < 0){
        return 0;
    }
    if(c >= MAX_COLS || c < 0){
        return 0;
    }
    if(type == None){
        return 0;
    }
    if(type == b->cur_s[b->cur].grid[r][c]){
        return 1;
    }
    return 0;
}

void c_pop(int r, int c, BPGame *b, int X, int Y, int *count, char type){
  if (can_pop(b, type, r +  X, c +  Y) == 1)
    {
      *count += 1;
      c_pop(r + X, c +  Y, b, X, Y, count, type);
    }
}

void Rpop(BPGame *b, int r, int c);

void Pop_x(int r, int c, BPGame * b, int Xposition,int holder){
  if (Xposition < r)
    {
      Xposition = r;
      r = holder;
    }
  for (int i = r; i <= Xposition; i++){
    for (int j = c; j <= c; j++){
        if (b->cur_s[b->cur].grid[i][j] == b->type){
            b->cur_s[b->cur].grid[i][j] = None;
            b->num++;
            Rpop(b, i, j);
        }
    }
  } 
}

void Pop_y(int r, int c, BPGame * b,  int Yposition, int holder){
    if (Yposition < c){
      Yposition = c;
      c = holder;
    }
    for (int i = r; i <= r; i++){
        for (int j = c; j <= Yposition; j++){
            if (b->cur_s[b->cur].grid[i][j] == b->type){
            b->cur_s[b->cur].grid[i][j] = None;
            b->num++;
            Rpop(b, i, j);
            }
        }
    }
}

void Rpop(BPGame *b, int r, int c){
if (b->cur_s[b->cur].grid[r][c] != None)
   b->type = b->cur_s[b->cur].grid[r][c];
   int target = 1;
  for (int i = 0; i < 2; i++){
    if (i == 1)
      target =-1;
    int X = 0;
    int Y = 0;
    c_pop(r, c, b, target, 0, &X, b->type);
    c_pop(r, c, b, 0, target, &Y, b->type);
    Pop_x(r, c, b, r + (X * target), r + (X * target));
    Pop_y(r, c, b, c + (Y * target), c + (Y * target));
  }
}

/**
* Function:  bp_pop
* Description:  (attempts to) "pop" the balloon at (r,c) (if any)
*               and its associated "color cluster"
*
*               Cluster must be of AT LEASE SIZE 2 (a balloon that
*   		is its own cluster will not be popped).
*
* Returns:  number of balloons popped (which may be zero).
*
* Side effects:  
*		locations of popped balloons become None (as in the "color"
*			None in the #define at the top of this file).
*
*		adds n*(n-1) to score if n balloons were popped.
*
* Notes:  does not require that the board be "compact".  But presumably, 
*	most client applicationw would only call on a compact board.
*
*/
extern int bp_pop(BPGame * b, int r, int c){
  b->num = 0;
  b->cur++;
  if (b->cur >= b->total){
    game * copy = malloc(b->total * 2 * sizeof(game));
    for (int i = 0; i < b->cur; i++){
      copy[i] = b->cur_s[i];
    }
     game temp;
     temp.score = 0;
    for (int i = b->cur; i < b->total * 2; i++){
    
     copy[i] = temp;
    }
    b->cur_s = copy;
    b->total *= 2;
  }
  b->cur_s[b->cur] = b->cur_s[b->cur- 1];
  Rpop(b, r, c);
  b->cur_s[b->cur].score += b->num*(b->num-1);
  return b->num;
}

/**
* Function:  bp_is_compact
* Description:  determines if all balloons are as "high" as possible.
*               Returns 1 if this is the case
*               Returns 0 otherwise.
*
*               Note:  a balloon is as high as possible if and only
*     		if all slots in its column ABOVE it are also balloons
*		(non-empty)
*
*/
extern int bp_is_compact(BPGame * b){
  int s[b->cols] ;
  for (int j = 0; j < b->cols; j++){
    s[j] = 0;
    for (int  i = 0; i < b->rows; i++){
        if (b->cur_s[b->cur].grid[i][j] != None){
        s[j] = s[j]+1;
      }
    }
  }
  for (int h = 0; h < b->cols; h++){
      int c = s[h];
    for (int  i = 0; i < c; i++){
        if (b->cur_s[b->cur].grid[i][h] == None){
            return 0;
        }
    }
  }
  return 1;
}

/**
* Function:  bp_float_one_step
* Description:  moves all balloons that that are NOT AS HIGH AS POSSIBLE
*               up one spot.  
*
*   Example:
*
*		before
*
*       +-----------------------+
*		  0 | + + + = + ^     + = = |
*		  1 | ^       + o + +   ^ = |
*		  2 | ^ +   o +   + =     = |
*		  3 |       o     + + =   = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*		after
*
*       +-----------------------+
*		  0 | + + + = + ^ + + + = = |
*		  1 | ^ +   o + o + =   ^ = |
*		  2 | ^     o +   + + =   = |
*		  3 |                     = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
* Note:  think animation by repeating this until it
*   is compact (perhaps displaying each intermediate
*   configuration).
*/
extern void bp_float_one_step(BPGame * b){
  for(int j = 0; j < b->cols ; j++){
        for(int i = 0; i < b->rows; i ++){
            if (b->cur_s[b->cur].grid[i][j] == None){
                for(int s = i; s < b->rows;s++){
                    
                    if(b->cur_s[b->cur].grid[s][j] != None){
                        b->cur_s[b->cur].grid[s-1][j] = b->cur_s[b->cur].grid[s][j];
                        b->cur_s[b->cur].grid[s][j] = None;
                        break;
                    }
                }   
            }
        }
    }
}

/**
* Function:  bp_score
* Description:  returns the current score 
*
*/
extern int bp_score(BPGame * b){
    return b->cur_s[b->cur].score;
}


/**
* Function:   bp_get_balloon
* Description:  returns color of balloon at given location as a char.
*		if (r,c) is out of range, -1 is returned.
*
*/
extern int bp_get_balloon(BPGame * b, int r, int c){
    if(r < b->rows && c < b->cols){
        return b->cur_s[b->cur].grid[r][c];
    }
    return -1;
}

/**
* Function:   bp_can_pop
* Description:  returns 1 if the player can pop some balloons;
*		        0 otherwise.
*
*		If the board is empty, of course nothing can
*			be poppsed.
*		If there are no adjacent balloons of the same
*		color, there also are no moves.
*	
* Notes:  probable usage is to determine when game is over.
*/
extern int bp_can_pop(BPGame * b){
    if(b == NULL){
        return 0;
    }
    for (int i = 0; i < b->rows; i++){
        for (int j = 0; j < b->cols; j++){
            if(can_pop(b, b->cur_s[b->cur].grid[i][j], i+1,j) == 1){
                return 1;
            }
            else if(can_pop(b, b->cur_s[b->cur].grid[i][j], i,j+1) == 1){
                return 1;
            }
            else if(can_pop(b, b->cur_s[b->cur].grid[i][j], i-1,j) == 1){
                return 1;
            }
            else if(can_pop(b, b->cur_s[b->cur].grid[i][j], i,j-1) == 1){
                return 1;
            }
        }
    }
    return 0;
}

/**
* Function:  bp_undo
* Description:  "undoes" most recent successful pop (if any), 
*		restoring the board to the exact state it was 
*		in immediately prior to that pop.
*
*               Also restores score to corresponding previous value.
*
*               Returns 1 if successful; 0 otherwise (i.e., when in the initial state --
*		no moves have been made to be undone).
*
*		NOTE:  bp_undo can be called repeatedly to back up all the way to the beginning
*		of the game.
*
*/

extern int bp_undo(BPGame * b){
    if(b->cur <= 0){
        return 0;
    }
    b->cur = b->cur - 1;
    return 1;
}
