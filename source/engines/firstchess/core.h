#ifndef CORE_H
#define CORE_H

/*
        FirstChess - Freeware, by Pham Hong Nguyen
        Version: beta
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************
* Some definitions                                                         *
****************************************************************************
*/
#define	PAWN    0
#define	KNIGHT  1
#define	BISHOP  2
#define	ROOK    3
#define	QUEEN   4
#define	KING    5
#define	EMPTY   6
#define	WHITE   0
#define	BLACK   1

#define	VALUE_PAWN      100
#define	VALUE_KNIGHT    300
#define	VALUE_BISHOP    300
#define	VALUE_ROOK      500
#define	VALUE_QUEEN     900
#define	VALUE_KING      10000

#define	MATE            10000 /* equal value of King, losing King==mate */

#define FIRSTCHESS_COL(pos) ((pos)&7)
#define FIRSTCHESS_ROW(pos) (((unsigned)pos)>>3)
/*
****************************************************************************
* Board representation and main varians                                    *
****************************************************************************
*/
/* Board representation */
extern int FIRSTCHESS_piece[64];
extern int FIRSTCHESS_color[64];
int side;           /* side to move, value = BLACK or WHITE */

/* For move generation */
#define MOVE_TYPE_NONE                  0
#define MOVE_TYPE_NORMAL                1
#define MOVE_TYPE_CASTLE                2
#define MOVE_TYPE_ENPASANT              3
#define MOVE_TYPE_PROMOTION_TO_QUEEN    4
#define MOVE_TYPE_PROMOTION_TO_ROOK     5
#define MOVE_TYPE_PROMOTION_TO_BISHOP   6
#define MOVE_TYPE_PROMOTION_TO_KNIGHT   7

typedef struct tag_MOVE {
    int             from,
                    dest,
                    type;
}               MOVE;

/* For storing all moves of game */
typedef struct tag_HIST {
    MOVE            m;
    int             cap;
}               HIST;

HIST            hist[6000];     /* Game length < 6000 */

int             hdp;            /* Current move order */

/* For searching */
int             nodes;          /* Count all visited nodes when searching */
int             ply;            /* ply of search */

/*
****************************************************************************
* Move generator                                                           *
* Lack: no enpassant, no castle                                            *
****************************************************************************
*/
void Gen_Push(int from, int dest, int type, MOVE * pBuf, int *pMCount);

void Gen_PushNormal(int from, int dest, MOVE * pBuf, int *pMCount);

/* Pawn can promote */
void Gen_PushPawn(int from, int dest, MOVE * pBuf, int *pMCount);

/* Gen all moves of current_side to move and push them to pBuf, return number of moves */
int Gen(int current_side, MOVE * pBuf);

/*
****************************************************************************
* Evaluation for current position - main "brain" function                  *
* Lack: almost no knowlegde                                                *
****************************************************************************
*/
int Eval();

/*
****************************************************************************
* Make and Take back a move, IsInCheck                                     *
****************************************************************************
*/
/* Check and return 1 if side is in check */
int IsInCheck(int current_side);

int MakeMove(MOVE m);

/* undo what MakeMove did */
void TakeBack();

/*
****************************************************************************
* Search function - a typical alphabeta, main search function              *
* Lack: no any technique for move ordering                                 *
****************************************************************************
*/

int Search(int alpha, int beta, int depth, MOVE * pBestMove);

MOVE ComputerThink(int max_depth);

#ifdef __cplusplus
}
#endif

#endif 
