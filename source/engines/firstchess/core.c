#include "core.h"

int FIRSTCHESS_piece[64] = {
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
};

int FIRSTCHESS_color[64] = {
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
};

void Gen_Push(int from, int dest, int type, MOVE * pBuf, int *pMCount)
{
    MOVE move;
    move.from = from;
    move.dest = dest;
    move.type = type;
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void Gen_PushNormal(int from, int dest, MOVE * pBuf, int *pMCount)
{
    Gen_Push(from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
}

void Gen_PushPawn(int from, int dest, MOVE * pBuf, int *pMCount)
{
    if (dest > 7 && dest < 56)
        Gen_Push(from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
    else {
        Gen_Push(from, dest, MOVE_TYPE_PROMOTION_TO_QUEEN, pBuf, pMCount);
        Gen_Push(from, dest, MOVE_TYPE_PROMOTION_TO_ROOK, pBuf, pMCount);
        Gen_Push(from, dest, MOVE_TYPE_PROMOTION_TO_BISHOP, pBuf, pMCount);
        Gen_Push(from, dest, MOVE_TYPE_PROMOTION_TO_KNIGHT, pBuf, pMCount);
    }
}

int Gen(int current_side, MOVE * pBuf)
{
    int             i,
                    k,
                    y,
                    FIRSTCHESS_ROW,
                    FIRSTCHESS_COL,
                    movecount;
    movecount = 0;

    for (i = 0; i < 64; i++)	/* Scan all board */
        if (FIRSTCHESS_color[i] == current_side) {
            switch (FIRSTCHESS_piece[i]) {
            case PAWN:
                FIRSTCHESS_COL = FIRSTCHESS_COL(i);
                FIRSTCHESS_ROW = FIRSTCHESS_ROW(i);
                if (current_side == BLACK) {
                    if (FIRSTCHESS_color[i + 8] == EMPTY)
                        Gen_PushPawn(i, i + 8, pBuf, &movecount);
                    if (FIRSTCHESS_ROW == 1 && FIRSTCHESS_color[i + 8] == EMPTY && FIRSTCHESS_color[i + 16] == EMPTY)
                        Gen_PushNormal(i, i + 16, pBuf, &movecount);
                    if (FIRSTCHESS_COL && FIRSTCHESS_color[i + 7] == WHITE)
                        Gen_PushNormal(i, i + 7, pBuf, &movecount);
                    if (FIRSTCHESS_COL < 7 && FIRSTCHESS_color[i + 9] == WHITE)
                        Gen_PushNormal(i, i + 9, pBuf, &movecount);
                } else {
                    if (FIRSTCHESS_color[i - 8] == EMPTY)
                        Gen_PushPawn(i, i - 8, pBuf, &movecount);
                    if (FIRSTCHESS_ROW == 6 && FIRSTCHESS_color[i - 8] == EMPTY && FIRSTCHESS_color[i - 16] == EMPTY)
                        Gen_PushNormal(i, i - 16, pBuf, &movecount);
                    if (FIRSTCHESS_COL && FIRSTCHESS_color[i - 9] == BLACK)
                        Gen_PushNormal(i, i - 9, pBuf, &movecount);
                    if (FIRSTCHESS_COL < 7 && FIRSTCHESS_color[i - 7] == BLACK)
                        Gen_PushNormal(i, i - 7, pBuf, &movecount);
                }
                break;

            case QUEEN:         /* == BISHOP+ROOK */
            case BISHOP:
                for (y = i - 9; y >= 0 && FIRSTCHESS_COL(y) != 7; y -= 9) {        /* go left up */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                for (y = i - 7; y >= 0 && FIRSTCHESS_COL(y) != 0; y -= 7) {        /* go right up */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                for (y = i + 9; y < 64 && FIRSTCHESS_COL(y) != 0; y += 9) {        /* go right down */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                for (y = i + 7; y < 64 && FIRSTCHESS_COL(y) != 7; y += 7) {        /* go right down */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                if (FIRSTCHESS_piece[i] == BISHOP)
                    break;

            /* FALL THROUGH FOR QUEEN {I meant to do that!} ;-) */
            case ROOK:
                FIRSTCHESS_COL = FIRSTCHESS_COL(i);
                for (k = i - FIRSTCHESS_COL, y = i - 1; y >= k; y--) {  /* go left */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                for (k = i - FIRSTCHESS_COL + 7, y = i + 1; y <= k; y++) {      /* go right */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                for (y = i - 8; y >= 0; y -= 8) {       /* go up */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                for (y = i + 8; y < 64; y += 8) {       /* go down */
                    if (FIRSTCHESS_color[y] != current_side)
                        Gen_PushNormal(i, y, pBuf, &movecount);
                    if (FIRSTCHESS_color[y] != EMPTY)
                        break;

                }
                break;

            case KNIGHT:
                FIRSTCHESS_COL = FIRSTCHESS_COL(i);
                y = i - 6;
                if (y >= 0 && FIRSTCHESS_COL < 6 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i - 10;
                if (y >= 0 && FIRSTCHESS_COL > 1 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i - 15;
                if (y >= 0 && FIRSTCHESS_COL < 7 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i - 17;
                if (y >= 0 && FIRSTCHESS_COL > 0 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i + 6;
                if (y < 64 && FIRSTCHESS_COL > 1 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i + 10;
                if (y < 64 && FIRSTCHESS_COL < 6 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i + 15;
                if (y < 64 && FIRSTCHESS_COL > 0 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                y = i + 17;
                if (y < 64 && FIRSTCHESS_COL < 7 && FIRSTCHESS_color[y] != current_side)
                    Gen_PushNormal(i, y, pBuf, &movecount);
                break;

            case KING:
                FIRSTCHESS_COL = FIRSTCHESS_COL(i);
                if (FIRSTCHESS_COL && FIRSTCHESS_color[i - 1] != current_side)
                    Gen_PushNormal(i, i - 1, pBuf, &movecount); /* left */
                if (FIRSTCHESS_COL < 7 && FIRSTCHESS_color[i + 1] != current_side)
                    Gen_PushNormal(i, i + 1, pBuf, &movecount); /* right */
                if (i > 7 && FIRSTCHESS_color[i - 8] != current_side)
                    Gen_PushNormal(i, i - 8, pBuf, &movecount); /* up */
                if (i < 56 && FIRSTCHESS_color[i + 8] != current_side)
                    Gen_PushNormal(i, i + 8, pBuf, &movecount); /* down */
                if (FIRSTCHESS_COL && i > 7 && FIRSTCHESS_color[i - 9] != current_side)
                    Gen_PushNormal(i, i - 9, pBuf, &movecount); /* left up */
                if (FIRSTCHESS_COL < 7 && i > 7 && FIRSTCHESS_color[i - 7] != current_side)
                    Gen_PushNormal(i, i - 7, pBuf, &movecount); /* right up */
                if (FIRSTCHESS_COL && i < 56 && FIRSTCHESS_color[i + 7] != current_side)
                    Gen_PushNormal(i, i + 7, pBuf, &movecount); /* left down */
                if (FIRSTCHESS_COL < 7 && i < 56 && FIRSTCHESS_color[i + 9] != current_side)
                    Gen_PushNormal(i, i + 9, pBuf, &movecount); /* right down */
                break;
            default:
                puts("FIRSTCHESS_piece type unknown");
                assert(false);
            }
        }
    return movecount;
}

int Eval()
{
    int             value_FIRSTCHESS_piece[6] = {VALUE_PAWN, VALUE_KNIGHT, VALUE_BISHOP, VALUE_ROOK, VALUE_QUEEN, VALUE_KING};
    int             i,
                    score = 0;
    for (i = 0; i < 64; i++) {
        if (FIRSTCHESS_color[i] == WHITE)
            score += value_FIRSTCHESS_piece[FIRSTCHESS_piece[i]];
        else if (FIRSTCHESS_color[i] == BLACK)
            score -= value_FIRSTCHESS_piece[FIRSTCHESS_piece[i]];
    }
    if (side == WHITE)
        return score;
    return -score;
}

int IsInCheck(int current_side)
{
    int             k,
                    h,
                    y,
                    FIRSTCHESS_ROW,
                    FIRSTCHESS_COL,
                    xside;
    xside = (WHITE + BLACK) - current_side;     /* opposite current_side, who may be checking */
    /* Find King */
    for (k = 0; k < 64; k++)
        if (FIRSTCHESS_piece[k] == KING && FIRSTCHESS_color[k] == current_side)
            break;

    FIRSTCHESS_ROW = FIRSTCHESS_ROW(k), FIRSTCHESS_COL = FIRSTCHESS_COL(k);
    /* Check attacking of Knight */
    if (FIRSTCHESS_COL > 0 && FIRSTCHESS_ROW > 1 && FIRSTCHESS_color[k - 17] == xside && FIRSTCHESS_piece[k - 17] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL < 7 && FIRSTCHESS_ROW > 1 && FIRSTCHESS_color[k - 15] == xside && FIRSTCHESS_piece[k - 15] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL > 1 && FIRSTCHESS_ROW > 0 && FIRSTCHESS_color[k - 10] == xside && FIRSTCHESS_piece[k - 10] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL < 6 && FIRSTCHESS_ROW > 0 && FIRSTCHESS_color[k - 6] == xside && FIRSTCHESS_piece[k - 6] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL > 1 && FIRSTCHESS_ROW < 7 && FIRSTCHESS_color[k + 6] == xside && FIRSTCHESS_piece[k + 6] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL < 6 && FIRSTCHESS_ROW < 7 && FIRSTCHESS_color[k + 10] == xside && FIRSTCHESS_piece[k + 10] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL > 0 && FIRSTCHESS_ROW < 6 && FIRSTCHESS_color[k + 15] == xside && FIRSTCHESS_piece[k + 15] == KNIGHT)
        return 1;
    if (FIRSTCHESS_COL < 7 && FIRSTCHESS_ROW < 6 && FIRSTCHESS_color[k + 17] == xside && FIRSTCHESS_piece[k + 17] == KNIGHT)
        return 1;
    /* Check horizontal and vertical lines for attacking of Queen, Rook, King */
    /* go down */
    y = k + 8;
    if (y < 64) {
        if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
            return 1;
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y += 8; y < 64; y += 8) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;

            }
    }
    /* go left */
    y = k - 1;
    h = k - FIRSTCHESS_COL;
    if (y >= h) {
        if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
            return 1;
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y--; y >= h; y--) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;
            }
    }
    /* go right */
    y = k + 1;
    h = k - FIRSTCHESS_COL + 7;
    if (y <= h) {
        if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
            return 1;
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y++; y <= h; y++) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;
            }
    }
    /* go up */
    y = k - 8;
    if (y >= 0) {
        if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
            return 1;
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y -= 8; y >= 0; y -= 8) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == ROOK))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;
            }
    }
    /* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
    /* go right down */
    y = k + 9;
    if (y < 64 && FIRSTCHESS_COL(y) != 0) {
        if (FIRSTCHESS_color[y] == xside) {
            if (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP)
                return 1;
            if (current_side == BLACK && FIRSTCHESS_piece[y] == PAWN)
                return 1;
        }
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y += 9; y < 64 && FIRSTCHESS_COL(y) != 0; y += 9) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;
            }
    }
    /* go left down */
    y = k + 7;
    if (y < 64 && FIRSTCHESS_COL(y) != 7) {
        if (FIRSTCHESS_color[y] == xside) {
            if (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP)
                return 1;
            if (current_side == BLACK && FIRSTCHESS_piece[y] == PAWN)
                return 1;
        }
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y += 7; y < 64 && FIRSTCHESS_COL(y) != 7; y += 7) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;

            }
    }
    /* go left up */
    y = k - 9;
    if (y >= 0 && FIRSTCHESS_COL(y) != 7) {
        if (FIRSTCHESS_color[y] == xside) {
            if (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP)
                return 1;
            if (current_side == WHITE && FIRSTCHESS_piece[y] == PAWN)
                return 1;
        }
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y -= 9; y >= 0 && FIRSTCHESS_COL(y) != 7; y -= 9) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;

            }
    }
    /* go right up */
    y = k - 7;
    if (y >= 0 && FIRSTCHESS_COL(y) != 0) {
        if (FIRSTCHESS_color[y] == xside) {
            if (FIRSTCHESS_piece[y] == KING || FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP)
                return 1;
            if (current_side == WHITE && FIRSTCHESS_piece[y] == PAWN)
                return 1;
        }
        if (FIRSTCHESS_piece[y] == EMPTY)
            for (y -= 7; y >= 0 && FIRSTCHESS_COL(y) != 0; y -= 7) {
                if (FIRSTCHESS_color[y] == xside && (FIRSTCHESS_piece[y] == QUEEN || FIRSTCHESS_piece[y] == BISHOP))
                    return 1;
                if (FIRSTCHESS_piece[y] != EMPTY)
                    break;
            }
    }
    return 0;
}

int MakeMove(MOVE m)
{
    int             r;
    hist[hdp].m = m;
    hist[hdp].cap = FIRSTCHESS_piece[m.dest];
    FIRSTCHESS_piece[m.dest] = FIRSTCHESS_piece[m.from];
    FIRSTCHESS_piece[m.from] = EMPTY;
    FIRSTCHESS_color[m.dest] = FIRSTCHESS_color[m.from];
    FIRSTCHESS_color[m.from] = EMPTY;
    if (m.type >= MOVE_TYPE_PROMOTION_TO_QUEEN) {       /* Promotion */
        switch (m.type) {
        case MOVE_TYPE_PROMOTION_TO_QUEEN:
            FIRSTCHESS_piece[m.dest] = QUEEN;
            break;

        case MOVE_TYPE_PROMOTION_TO_ROOK:
            FIRSTCHESS_piece[m.dest] = ROOK;
            break;

        case MOVE_TYPE_PROMOTION_TO_BISHOP:
            FIRSTCHESS_piece[m.dest] = BISHOP;
            break;

        case MOVE_TYPE_PROMOTION_TO_KNIGHT:
            FIRSTCHESS_piece[m.dest] = KNIGHT;
            break;

        default:
            puts("impossible to get here...");
            assert(false);
        }
    }
    ply++;
    hdp++;
    r = !IsInCheck(side);
    side = (WHITE + BLACK) - side;      /* After making move, give turn to
                                         * opponent */
    return r;
}

void TakeBack() 
{
    side = (WHITE + BLACK) - side;
    hdp--;
    ply--;
    FIRSTCHESS_piece[hist[hdp].m.from] = FIRSTCHESS_piece[hist[hdp].m.dest];
    FIRSTCHESS_piece[hist[hdp].m.dest] = hist[hdp].cap;
    FIRSTCHESS_color[hist[hdp].m.from] = side;
    if (hist[hdp].cap != EMPTY)
        FIRSTCHESS_color[hist[hdp].m.dest] = (WHITE + BLACK) - side;
    else
        FIRSTCHESS_color[hist[hdp].m.dest] = EMPTY;
    if (hist[hdp].m.type >= MOVE_TYPE_PROMOTION_TO_QUEEN)       /* Promotion */
        FIRSTCHESS_piece[hist[hdp].m.from] = PAWN;
}

int Search(int alpha, int beta, int depth, MOVE * pBestMove)
{
    int             i,
                    value,
                    havemove,
					movecnt;
    MOVE            moveBuf[200],
                    tmpMove;
    
    nodes++; /* visiting a node, count it */
    havemove = 0;
    pBestMove->type = MOVE_TYPE_NONE;
	movecnt = Gen(side, moveBuf); /* generate all moves for current position */
    /* loop through the moves */
    for (i = 0; i < movecnt; ++i) {
        if (!MakeMove(moveBuf[i])) {
            TakeBack();
            continue;
        }
        havemove = 1;
        if (depth - 1 > 0) /* If depth is still, continue to search deeper */
            value = -Search(-beta, -alpha, depth - 1, &tmpMove);
        else /* If no depth left (leaf node), go to evalute that position */
            value = Eval(); 
        TakeBack();
        if (value > alpha) {
            /* This move is so good and caused a cutoff */
            if (value >= beta)
                return beta;
            alpha = value;
            *pBestMove = moveBuf[i]; /* so far, current move is the best reaction
									  * for current position */
        }
    }
    if (!havemove) {            /* If no legal moves, that is checkmate or
                                 * stalemate */
        if (IsInCheck(side))
            return -MATE + ply; /* add ply to find the longest path to lose or shortest path to win */
        else
            return 0;
    }
    return alpha;
}

MOVE ComputerThink(int max_depth)
{
    MOVE            m;
    int             score;
	/* reset some values before searching */
    ply = 0;
    nodes = 0;
	/* search now */
    score = Search(-MATE, MATE, max_depth, &m);
	/* after searching, print results */
    printf("Search result: move = %c%d%c%d; nodes = %d, score = %d\n",
           'a' + FIRSTCHESS_COL(m.from),
           8 - FIRSTCHESS_ROW(m.from),
           'a' + FIRSTCHESS_COL(m.dest),
           8 - FIRSTCHESS_ROW(m.dest),
           nodes,
           score
        );
    return m;
}
