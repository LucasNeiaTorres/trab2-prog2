#ifndef _game_t_H
#define _game_t_H

#define COLS 8
#define ROWS 8 
#define N_JEWELS 5
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720
#define JEWEL_WIDTH 75
#define JEWEL_HEIGHT 75
#define BOARD_WIDTH JEWEL_WIDTH * COLS
#define BOARD_HEIGHT JEWEL_HEIGHT * ROWS
#define MARGIN_RIGHT 20
#define MARGIN_BOTTOM 20
#define SCORE_INICIAL 100
#define LINESIZE 1024
#include "structs.h"
#include <stdio.h>

void draw_board(game_t* game);

int calcula_px(jewel_t*** board, int row, int col);

int calcula_py(jewel_t*** board, int row, int col);

void add_jewel(jewel_t*** board, int row, int col, sprite_t* sprites);

jewel_t*** fill_board(sprite_t* sprites);

bool validate_row(jewel_t*** board, int row, int col, int id);

bool validate_col(jewel_t*** board, int row, int col, int id);

jewel_t find_position(jewel_t*** board, float mx, float my);

bool check_change(jewel_t first, jewel_t second);

void change(jewel_t*** board, jewel_t first, jewel_t second);

bool check_matches(jewel_t*** board);

void replace(jewel_t*** board, jewel_t replaced, jewel_t original);

void falling(jewel_t first, jewel_t final, int number_rows, int number_cols, game_t* game);

void col_falls(game_t* game, int row_start, int col);

void break_jewel(game_t* game);

char* current_time();

int get_highscore(game_t* game);

void update_score(game_t* game);

int count_lines(game_t* game);

void destroy_board(jewel_t*** board);

#endif