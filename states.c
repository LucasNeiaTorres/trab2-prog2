#include <stdio.h>
#include <stdlib.h>

#include "structs.h"
#include "states.h"
#include "game.h"
#include "jewel.h"
#include "config.h"

void state_begin(game_t* game) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_scaled_bitmap(game->background, 0, 0, al_get_bitmap_width(game->background), al_get_bitmap_height(game->background),
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    al_draw_text(game->fonts->font_big, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 70, ALLEGRO_ALIGN_CENTER, "JEWELS: AMONG US");
    al_draw_text(game->fonts->font_big, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2-35, ALLEGRO_ALIGN_CENTER, "PLAY");
    al_draw_text(game->fonts->font_big, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2+110, ALLEGRO_ALIGN_CENTER, "HELP");

    al_flip_display();
}

void state_help(game_t* game) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_scaled_bitmap(game->background, 0, 0, al_get_bitmap_width(game->background), al_get_bitmap_height(game->background),
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    al_draw_text(game->fonts->font_big, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 20, ALLEGRO_ALIGN_CENTER, "HELP");
    al_draw_text(game->fonts->font_big, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 595, ALLEGRO_ALIGN_CENTER, "EXIT");

    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 130, ALLEGRO_ALIGN_LEFT, "1. Clique com o mouse em um among us;");
    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 170, ALLEGRO_ALIGN_LEFT, "2. Selecione o outro para trocar as peças;");
    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 210, ALLEGRO_ALIGN_LEFT, "3. Caso não haja combinações com as peças ao lado, a peça retornará a sua posição de origem;");
    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 250, ALLEGRO_ALIGN_LEFT, "4. Caso haja combinações, as peças são quebradas e as suas colunas descem;");
    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 290, ALLEGRO_ALIGN_LEFT, "5. O objetivo é eliminar o máximo de peças possíveis;");
    // al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 280, ALLEGRO_ALIGN_LEFT, "6. O jogo acaba quando não houver movimentos possíveis.");

    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 390, ALLEGRO_ALIGN_LEFT, "Lucas Néia Torres");
    al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 420, ALLEGRO_ALIGN_LEFT, "GRR20210570");
    // al_draw_text(game->fonts->font_small, al_map_rgb(255, 255, 255), 120, 450, ALLEGRO_ALIGN_LEFT, "Easter egg");


    al_flip_display();
}

void state_score(game_t* game) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_scaled_bitmap(game->background, 0, 0, al_get_bitmap_width(game->background), al_get_bitmap_height(game->background),
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    al_draw_text(game->fonts->font_big, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 20, ALLEGRO_ALIGN_CENTER, "HIGHSCORE");
    
    char* line = malloc(sizeof(char) * LINESIZE);
    char* token1, *token2;
    int py;
    int n_line = 0;
    int n_lines = count_lines(game);
    int score;
    rewind(game->score_log);

    while(!feof(game->score_log)) {
        n_line++;
        fgets(line, LINESIZE, game->score_log);
        if(n_line >= n_lines - 5) {
            token1 = strtok(line, " ");
            token2 = strtok(NULL, " ");
            score = atoi(token2);
            al_draw_textf(game->fonts->font_medium, al_map_rgb(255, 255, 255), 120, ((n_lines-n_line)*100) + 200, ALLEGRO_ALIGN_LEFT, "%d", score);
        } 
    }
    free(line);
    al_flip_display();
}

void state_select(game_t* game) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_board(game);
    al_flip_display();
}

void state_changing(jewel_t first, jewel_t second, game_t* game) {
    int l, c;
    for(l=0; l<ROWS; l++) {
        for(c=0; c<COLS; c++) {
            if((l == first.row) && (c == first.col)) {   
                if(first.row == second.row) {
                    game->board[first.row][first.col]->py = calcula_py(game->board, first.row, first.col);
                    game->board[first.row][second.col]->py = game->board[first.row][first.col]->py;

                    if(first.col < second.col) { 
                        if(game->board[l][c]->px + 2 > second.px) {
                            game->board[l][c]->px++; 
                            game->board[second.row][second.col]->px--;
                        }
                        else {
                            game->board[l][c]->px += 2; 
                            game->board[second.row][second.col]->px -= 2;
                        }
                    } 
                    else {
                        if(game->board[l][c]->px - 2 < second.px) {
                            game->board[l][c]->px--; 
                            game->board[second.row][second.col]->px++;
                        }
                        else {
                            game->board[l][c]->px -= 2; 
                            game->board[second.row][second.col]->px += 2;
                        }
                    }
                }
                else {
                    game->board[first.row][first.col]->px = calcula_px(game->board, first.row, first.col);
                    game->board[second.row][first.col]->px = game->board[first.row][first.col]->px;

                    if(first.row < second.row) {
                        if(game->board[l][c]->py + 2 > second.py) {
                            game->board[l][c]->py++; 
                            game->board[second.row][second.col]->py--;
                        }
                        else {
                            game->board[l][c]->py += 2; 
                            game->board[second.row][second.col]->py -= 2;
                        }
                    }
                    else {
                        if(game->board[l][c]->py - 2 < second.py) {
                            game->board[l][c]->py--; 
                            game->board[second.row][second.col]->py++;
                        }
                        else {
                            game->board[l][c]->py -= 2; 
                            game->board[second.row][second.col]->py += 2;
                        }
                    }
                }
            }
        }
    }
    state_select(game);
}