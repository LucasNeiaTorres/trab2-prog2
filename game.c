#include "jewel.h"
#include "game.h"
#include "structs.h"
#include "states.h"
#include "config.h"
#include <time.h>
#include <stdio.h>

void draw_board(game_t* game) {
    int l, c, px, py;

    al_draw_scaled_bitmap(game->background, 0, 0, al_get_bitmap_width(game->background), al_get_bitmap_height(game->background),
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    for(l=0; l<ROWS; l++) {
        for(c=0; c<COLS; c++) {
            if(game->board[l][c]->selected == 0)
                al_draw_bitmap(game->board[l][c]->image, game->board[l][c]->px, game->board[l][c]->py, 0);
            else 
                al_draw_scaled_bitmap(game->board[l][c]->image, 0, 0, al_get_bitmap_width(game->board[l][c]->image), al_get_bitmap_height(game->board[l][c]->image),
                game->board[l][c]->px-5, game->board[l][c]->py-5, JEWEL_WIDTH+1, JEWEL_HEIGHT+1, 0);
        }
    }
    al_draw_textf(game->fonts->font_medium, al_map_rgb(255, 255, 255), 165, 140, ALLEGRO_ALIGN_CENTER, "%d", game->score);
    
    al_draw_textf(game->fonts->font_small, al_map_rgb(255, 255, 255), 165, 50, ALLEGRO_ALIGN_CENTER, "%dX", game->nivel);

    al_draw_textf(game->fonts->font_small, al_map_rgb(255, 255, 255), (SCREEN_WIDTH-BOARD_WIDTH)+(BOARD_WIDTH/2), 50, ALLEGRO_ALIGN_CENTER, "HIGHSCORE: %d", get_highscore(game));

}

int calcula_px(jewel_t*** board, int row, int col) {
    return (SCREEN_WIDTH-BOARD_WIDTH - MARGIN_RIGHT) + (col*JEWEL_WIDTH);
}

int calcula_py(jewel_t*** board, int row, int col) {
    return (SCREEN_HEIGHT-BOARD_HEIGHT - MARGIN_BOTTOM) + (row*JEWEL_HEIGHT);
}

void add_jewel(jewel_t*** board, int row, int col, sprite_t* sprites) {
    srand(clock());
    
    int id = 1 + random() % N_JEWELS;
    while(!validate_row(board, row, col, id) || !validate_col(board, row, col, id))
        id = 1 + random() % N_JEWELS; 
    // testar quando cair 
    board[row][col]->id_jewel = id;
    if(board[row][col]->id_jewel == 1) 
        board[row][col]->image = sprites->black;
    else if(board[row][col]->id_jewel == 2)
        board[row][col]->image = sprites->cyan;
    else if(board[row][col]->id_jewel == 3) 
        board[row][col]->image = sprites->white;
    else if(board[row][col]->id_jewel == 4) 
        board[row][col]->image = sprites->red;
    else if(board[row][col]->id_jewel == 5) 
        board[row][col]->image = sprites->orange;
    if(!board[row][col]->image)
    {
        printf("couldn't load the jewels image\n");
        exit(1);
    }
    board[row][col]->level = 1;
    board[row][col]->selected = 0;
    board[row][col]->row = row;
    board[row][col]->col = col;
    board[row][col]->px = calcula_px(board, row, col);
    board[row][col]->py = calcula_py(board, row, col);
}

jewel_t*** fill_board(sprite_t* sprites) {
    int l, c, i, j;
    jewel_t*** board = malloc(sizeof(jewel_t**) * ROWS);
    for(i=0; i<ROWS; i++) {
        board[i] = malloc(sizeof(jewel_t*) * COLS);
        for (j = 0; j < COLS; j++)
            board[i][j] = malloc(sizeof(jewel_t));
    } 
    
    for(l=0; l<ROWS; l++) {
        for(c=0; c<COLS; c++) {
            add_jewel(board, l, c, sprites);
        }
    }

    return board;
}

bool validate_row(jewel_t*** board, int row, int col, int id) {
    if(col == 0 || col == 1)
        return true;
    if((id == board[row][col-1]->id_jewel) && (id == board[row][col-2]->id_jewel)) 
        return false;
    return true;
}

bool validate_col(jewel_t*** board, int row, int col, int id) {
    if(row == 0 || row == 1)
        return true;
    if((id == board[row-1][col]->id_jewel) && (id == board[row-2][col]->id_jewel)) 
        return false;
    return true;
}


jewel_t find_position(jewel_t*** board, float mx, float my) {
    jewel_t aux;
    int col = (int)((mx-(SCREEN_WIDTH-BOARD_WIDTH-MARGIN_RIGHT))/JEWEL_WIDTH);
    int row = (int)((my-(SCREEN_HEIGHT-BOARD_HEIGHT-MARGIN_BOTTOM))/JEWEL_HEIGHT);

    aux.id_jewel = board[row][col]->id_jewel;
    aux.image = board[row][col]->image;
    aux.level = board[row][col]->level;
    aux.row = row;
    aux.col = col;
    aux.px = board[row][col]->px;
    aux.py = board[row][col]->py;
    return aux;
}

bool check_change(jewel_t first, jewel_t second) {
    if((first.row == second.row) && ((second.col == first.col+1) || (second.col == first.col-1)) 
    || (first.col == second.col) && ((second.row == first.row+1) || (second.row == first.row-1)) )
        return true;
    return false;
}

bool check_matches(jewel_t*** board) {
    int l, c;
    for(l=0; l<ROWS; l++) {
        for(c=0; c<COLS; c++) {
            if(!validate_row(board, l, c, board[l][c]->id_jewel) || !validate_col(board, l, c, board[l][c]->id_jewel))
                return true;
        }
    }
    return false;
}

void change(jewel_t*** board, jewel_t first, jewel_t second) {
    jewel_t* aux = board[second.row][second.col];
    board[second.row][second.col] = board[first.row][first.col];
    board[first.row][first.col] = aux;

    board[first.row][first.col]->row = first.row;
    board[first.row][first.col]->col = first.col;
    board[first.row][first.col]->px = first.px;
    board[first.row][first.col]->py = first.py;
    board[second.row][second.col]->row = second.row;
    board[second.row][second.col]->col = second.col;
    board[second.row][second.col]->px = second.px;
    board[second.row][second.col]->py = second.py;
}  

void replace(jewel_t*** board, jewel_t replaced, jewel_t original) {
    // printf("%d, %d - %d || %d, %d - %d\n", replaced.row, replaced.col, board[replaced.row][replaced.col].id_jewel, original.row, original.col, board[original.row][original.col].id_jewel);
    board[replaced.row][replaced.col]->id_jewel = original.id_jewel;
    board[replaced.row][replaced.col]->image = original.image;
    board[replaced.row][replaced.col]->level = original.level;
}

void falling(jewel_t first, jewel_t final, int number_rows, int number_cols, game_t* game) {
    int i, c;
    // ajusta posicionamento 
    for(c=0; c<number_cols; c++)
        for(i=final.row; i>=0; i--)
            game->board[i][first.col-c]->py = game->board[i][first.col-c]->py - (number_rows*JEWEL_HEIGHT);

    while(final.py != game->board[final.row][final.col]->py) {
        for(i=final.row; i>=0; i--)
            for(c=0; c<number_cols; c++) {
                if(game->board[i][first.col-c]->py + 2 > final.py)
                    game->board[i][first.col-c]->py++;
                else
                    game->board[i][first.col-c]->py += 2;
            }
        state_select(game);
    }
}

void col_falls(game_t* game, int row_start, int col) {
    int l, py1;
    jewel_t* aux;

    add_jewel(game->board, row_start, col, game->sprites);

    for(l=row_start; l>0; l--) {
        aux = game->board[l][col];
        py1 = game->board[l][col]->py;
        game->board[l][col] = game->board[l-1][col];
        game->board[l][col]->row = l;
        game->board[l-1][col] = aux;
        game->board[l-1][col]->row = l-1;
        game->board[l-1][col]->py = game->board[l][col]->py;
        game->board[l][col]->py = py1;
    }
}

void break_jewel(game_t* game) {
    int l, c, i;
    jewel_t c1, c2;

    for(l=0; l<ROWS; l++) {
        for(c=0; c<COLS; c++) {

            if(!validate_row(game->board, l, c, game->board[l][c]->id_jewel)) {
                // desce 1 em cada coluna
                al_play_sample(game->audios->breaks, 0.1, 0.0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                for(i=0; i<3; i++)
                    col_falls(game, l, c-i);

                c2.row = game->board[l][c]->row;
                c2.col = game->board[l][c]->col;
                c2.py = game->board[l][c]->py;

                // se for ultima linha, row start e row final sao iguais 
                if(l == 0)
                    c1 = c2;
                else {
                    c1.row = game->board[l-1][c]->row;
                    c1.col = game->board[l-1][c]->col;    
                }

                game->score += game->nivel * 10;

                falling(c1, c2, 1, 3, game);
            }
            if(!validate_col(game->board, l, c, game->board[l][c]->id_jewel)) {
                al_play_sample(game->audios->breaks, 0.1, 0.0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                // desce 3 na mesma coluna
                for(i=2; i>=0; i--)
                    col_falls(game, l-i, c);
                
                c2.row = game->board[l][c]->row;
                c2.col = game->board[l][c]->col;
                c2.py = game->board[l][c]->py;
                
                // se for ultima linha, row start e row final sao iguais 
                if(l-3 < 0)
                    c1 = c2;
                else {
                    c1.row = game->board[l-3][c]->row;
                    c1.col = game->board[l-3][c]->col;  
                }

                game->score += game->nivel * 10;

                falling(c1, c2, 3, 1, game);
            }
            if(game->score > SCORE_INICIAL * game->nivel * 2) {
                game->nivel++;
                // game->board = fill_board(game->sprites);
            }
        }
    }
}

char* current_time() {
    time_t current_time;
    time(&current_time);
    char* string = ctime(&current_time);
    return string;
}

int get_highscore(game_t* game) {
    char* line = malloc(sizeof(char) * LINESIZE);
    char* token1, *token2;
    rewind(game->score_log);
    while(!feof(game->score_log))
        fgets(line, LINESIZE, game->score_log);
    token1 = strtok(line, " ");
    token2 = strtok(NULL, " ");
    if (!strcmp(token1, "score"))
        game->highscore = atoi(token2);
    else
        game->highscore = 0;
    
    free(line);
    return game->highscore;
}

void update_score(game_t* game) {
    char* time = current_time();
    game->highscore = get_highscore(game);

    if (game->score > game->highscore) {
        fprintf(game->score_log, "score %d - %s", game->score, time);
        game->highscore = game->score;
    }
}

int count_lines(game_t* game) {
    int nmr = 0;
    char* line = malloc(sizeof(char) * LINESIZE);
    rewind(game->score_log);
    
    while(!feof(game->score_log)) {
        fgets(line, LINESIZE, game->score_log);
        nmr++;
    }
    free(line);
    return nmr;
}

void destroy_board(jewel_t*** board) {
    int i, j;
    // destruir imagens
    for(i=0; i<ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            al_destroy_bitmap(board[i][j]->image);
            free(board[i][j]);
        }
        free(board[i]);
    }
    free(board);
}