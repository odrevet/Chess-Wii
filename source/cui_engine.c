#include "cui_engine.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/timeb.h>


// get_ms() returns the milliseconds elapsed since midnight,
//   January 1, 1970. 

#include <sys/timeb.h>
#include <sys/time.h> 

int getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

BOOL ftime_ok = FALSE;  // does ftime return milliseconds?
int get_ms()
{/*
	struct timeb timebuffer;
	ftime(&timebuffer);
	if (timebuffer.millitm != 0)
		ftime_ok = TRUE;
	return (timebuffer.time * 1000) + timebuffer.millitm;
	*/
	ftime_ok = TRUE;
	return getTime();
}


// parse the move s (in coordinate notation) and return the move's
//   index in gen_dat, or -1 if the move is illegal 

int parse_move(char *s)
{
	int from, to, i;

	// make sure the string looks like a move 
	if (s[0] < 'a' || s[0] > 'h' ||
			s[1] < '0' || s[1] > '9' ||
			s[2] < 'a' || s[2] > 'h' ||
			s[3] < '0' || s[3] > '9')
		return -1;

	from = s[0] - 'a';
	from += 8 * (8 - (s[1] - '0'));
	to = s[2] - 'a';
	to += 8 * (8 - (s[3] - '0'));

	for (i = 0; i < first_move[1]; ++i)
		if (gen_dat[i].m.b.from == from && gen_dat[i].m.b.to == to) {

			// if the move is a promotion, handle the promotion piece;
			//   assume that the promotion moves occur consecutively in
			//   gen_dat. 
			if (gen_dat[i].m.b.bits & 32)
				switch (s[4]) {
					case 'N':
						return i;
					case 'B':
						return i + 1;
					case 'R':
						return i + 2;
					default:  // assume it's a queen 
						return i + 3;
				}
			return i;
		}

	// didn't find the move 
	return -1;
}


// move_str returns a string with move m in coordinate notation

char *move_str(move_bytes m)
{
	static char str[6];

	char c;

	if (m.bits & 32) {
		switch (m.promote) {
			case KNIGHT:
				c = 'n';
				break;
			case BISHOP:
				c = 'b';
				break;
			case ROOK:
				c = 'r';
				break;
			default:
				c = 'q';
				break;
		}
		sprintf(str, "%c%d%c%d%c",
				COL(m.from) + 'a',
				8 - ROW(m.from),
				COL(m.to) + 'a',
				8 - ROW(m.to),
				c);
	}
	else
		sprintf(str, "%c%d%c%d",
				COL(m.from) + 'a',
				8 - ROW(m.from),
				COL(m.to) + 'a',
				8 - ROW(m.to));
	return str;
}



bool calculate_coords(char coords[5], char newcoord[5], ENGINE engine)
{
	switch(engine){
		case FIRSTCHESS:
		{			
			int             from,
							dest,
							i;
			int             computer_side;
			int             max_depth;  // max depth to search 
			MOVE            moveBuf[200];
			int             movecnt;

			side = WHITE;
			max_depth = 7;
			hdp = 0;

			// maybe the user entered a move? 
			from = coords[0] - 'a';
			from += 8 * (8 - (coords[1] - '0'));
			dest = coords[2] - 'a';
			dest += 8 * (8 - (coords[3] - '0'));
			ply = 0;
			movecnt = Gen(side, moveBuf);
			// loop through the moves to see if it's legal 
			for (i = 0; i < movecnt; i++){
				if (moveBuf[i].from == from && moveBuf[i].dest == dest) {
					if (FIRSTCHESS_piece[from] == PAWN && (dest < 8 || dest > 55)) {   // Promotion move? 
						switch (coords[4]) {
							case 'q':
								moveBuf[i].type = MOVE_TYPE_PROMOTION_TO_QUEEN;
								break;

							case 'r':
								moveBuf[i].type = MOVE_TYPE_PROMOTION_TO_ROOK;
								break;

							case 'b':
								moveBuf[i].type = MOVE_TYPE_PROMOTION_TO_BISHOP;
								break;

							case 'n':
								moveBuf[i].type = MOVE_TYPE_PROMOTION_TO_KNIGHT;
								break;

							default:

								puts("promoting to a McGuffin..., I'll give you a queen");
								moveBuf[i].type = MOVE_TYPE_PROMOTION_TO_QUEEN;
						}
					}
					
					if (!MakeMove(moveBuf[i])) {
						TakeBack();
					}else{
						MOVE bestMove = ComputerThink(max_depth);
						MakeMove(bestMove);
					}
					break;
				}
			}	
			break;
		}
		case TSCP:
		{
			int m = parse_move(coords);
			if (m == -1 || !makemove(gen_dat[m].m.b)){
				//break;
			}
			else {
				ply = 0;
				gen();
			}
		
			// think about the move and make it 
			think(1);
			/*if (!pv[0][0].u) {
				break;
			}*/
			makemove(pv[0][0].b);
			ply = 0;
			gen();
		}
		break;
	}
	return true;
}
