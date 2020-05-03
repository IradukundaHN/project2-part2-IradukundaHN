/*
    Hugues Nelson Iradukunda	hiraduk1@umbc.edu 	github username:IradukundaHN

    chessboard.h -- program to implement Chess board, validity of moves and chess game logic for kernel module.

*/

/*Import headers and libraries*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>


//default const declarations 
#define BOARD_SIZE 8
#define EMPTY '**'
#define ROOK_WHITE "WR"
#define ROOK_BLACK "BR"
#define KNIGHT_WHITE "WK"
#define KNIGHT_BLACK "BK"
#define BISHOP_WHITE "WB"
#define BISHOP_BLACK "BB"
#define KING_WHITE "WK"
#define KING_BLACK "BK"
#define QUEEN_WHITE "WQ"
#define QUEEN_BLACK "BQ"
#define PAWN_WHITE "WP"
#define PAWN_BLACK "BP"

void display_board();

void move_piece(int , int, int , int);

