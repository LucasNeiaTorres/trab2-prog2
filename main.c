#include <stdio.h>
#include <stdlib.h>
#include "states.h"
#include "structs.h"
#include "jewel.h"
#include "config.h"
#include "game.h"

int main() {
    srand(time(NULL));

    // Inicializa Allegro, teclado e mouse
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);
    //init_test(al_init(), "Allegro");
    // init_test(al_install_keyboard(), "teclado");
    // init_test(al_install_mouse(), "mouse");

    // Inicializa timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    // Inicializa fila de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    // Inicializa display
    ALLEGRO_DISPLAY* disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if(!disp) {
      fprintf(stderr, "Falha ao criar a janela.\n");
      return -1;
    }

    // Inicializa áudios
    //audios_t* audios = audio_init();

    // Registra as fontes de eventos para a fila
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // Controla se uma das condições para o fim do programa foi atendida
    bool done = false;

    // Declara variável de eventos
    ALLEGRO_EVENT* event = malloc(sizeof(ALLEGRO_EVENT));

    al_init_font_addon();
    al_init_ttf_addon();

    game_t* game = malloc(sizeof(game_t));

    game->sprites = malloc(sizeof(sprite_t));
    game->sprites->black = al_load_bitmap("resources/among1.png");
    game->sprites->cyan = al_load_bitmap("resources/among2.png");
    game->sprites->white = al_load_bitmap("resources/among3.png");
    game->sprites->red = al_load_bitmap("resources/among4.png");
    game->sprites->orange = al_load_bitmap("resources/among5.png");
    
    game->board = fill_board(game->sprites);
    game->background = al_load_bitmap("resources/inicial.png");
    game->score = 0;
    game->highscore = 0;
    game->nivel = 1;
    game->fonts = malloc(sizeof(font_t));

    game->fonts->font_small = al_load_font("resources/DejaVuSans.ttf", 20, 0);
    game->fonts->font_medium = al_load_font("resources/DejaVuSans.ttf", 40, 0);
    game->fonts->font_big = al_load_font("resources/joffrey.ttf", 80, 0);

    game->score_log = fopen("score.log", "a+");

    game->audios = malloc(sizeof(audio_t));
    game->audios->breaks = al_load_sample("resources/kill.opus");
    game->audios->music = al_load_sample("resources/background_tue.opus");
    game->audios->pop = al_load_sample("resources/Bluezone_BC0240_interface_001.opus");
    game->audios->play = al_load_sample("resources/play.opus");

    al_play_sample(game->audios->music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    // Variáveis que armazenam a posição selecionada pelo usuário
    jewel_t first_select, second_select, aux;
    
    bool redraw = true;

    // Variáveis para posição do mouse
    float mx = 0, my = 0;
    int px1, py1, px2, py2;

    state_t state = BEGIN;
    // Inicia o timer do loop principal
    al_start_timer(timer);

    while(!done) {
        // Lê o próximo evento da fila
        al_wait_for_event(queue, event);
        switch(event->type){

            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                mx = event->mouse.x;
                my = event->mouse.y;
                break;
            
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event->keyboard.keycode == ALLEGRO_KEY_H || event->keyboard.keycode == ALLEGRO_KEY_F1) {
                    game->background = al_load_bitmap("resources/help.png");
                    state = HELP;
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:

                if(state == BEGIN) {
                    if((mx > SCREEN_WIDTH/2 - 93) && (mx < SCREEN_WIDTH/2 + 93) && (my > SCREEN_HEIGHT/2 - 63) && (my < SCREEN_HEIGHT/2 + 60)) {
                        game->background = al_load_bitmap("resources/fundo.png");
                        al_play_sample(game->audios->play, 0.4, 0.0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                        state = SELECT;
                    }
                    else if((mx > SCREEN_WIDTH/2 - 93) && (mx < SCREEN_WIDTH/2 + 93) && (my > SCREEN_HEIGHT/2 + 80) && (my < SCREEN_HEIGHT/2 + 203)) {
                        game->background = al_load_bitmap("resources/help.png");
                        state = HELP;
                    }
                }

                else if(state == HELP) {
                    if((mx > SCREEN_WIDTH/2 - 78) && (mx < SCREEN_WIDTH/2 + 78) && (my > SCREEN_HEIGHT/2 + 210) && (my < SCREEN_HEIGHT/2 + 325)) {
                        game->background = al_load_bitmap("resources/inicial.png");
                        state = BEGIN;
                    }
                }

                else if(state == SELECT) {
                    if ((mx > SCREEN_WIDTH-BOARD_WIDTH) && (mx < SCREEN_WIDTH-MARGIN_RIGHT) && (my > SCREEN_HEIGHT-BOARD_HEIGHT) && (my < SCREEN_HEIGHT-MARGIN_BOTTOM)) {
                        first_select = find_position(game->board, mx, my);
                        first_select.px = calcula_px(game->board, first_select.row, first_select.col);
                        first_select.py = calcula_py(game->board, first_select.row, first_select.col);

                        game->board[first_select.row][first_select.col]->selected = 1;
                        state_select(game);
                        state = SECOND_CLICK;
                    }
                }
                else if(state == SECOND_CLICK) {
                    if ((mx > SCREEN_WIDTH-BOARD_WIDTH) && (mx < SCREEN_WIDTH-MARGIN_RIGHT) && (my > SCREEN_HEIGHT-BOARD_HEIGHT) && (my < SCREEN_HEIGHT-MARGIN_BOTTOM)) {
                        second_select = find_position(game->board, mx, my);
                        second_select.px = calcula_px(game->board, second_select.row, second_select.col);
                        second_select.py = calcula_py(game->board, second_select.row, second_select.col);
                        
                        game->board[second_select.row][second_select.col]->selected = 1;
                        if(check_change(first_select, second_select)) {
                            state = CHANGING;
                        } else {
                            // printf("%d, %d = %d\n", first_select.row, first_select.col, game->board[first_select.row][first_select.col]->selected);
                            game->board[first_select.row][first_select.col]->selected = 0;
                            first_select = second_select;
                            game->board[first_select.row][first_select.col]->selected = 1;
                            state_select(game);
                        }
                    }
                }
        }

        if(redraw && al_is_event_queue_empty(queue)) {
            //al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "Hello world!");
            switch(state) {
                case BEGIN:
                    state_begin(game);
                    break;

                case HELP:
                    state_help(game);
                    break;

                case SCORE:
                    state_score(game);
                    break;

                case SELECT:
                    state_select(game);

                    // caso nao tenha match, jewel retorna a posicao de origem
                    if((second_select.id_jewel>0) && (second_select.id_jewel<=N_JEWELS) && (first_select.id_jewel>0) && (first_select.id_jewel<=N_JEWELS)) {
                        game->board[first_select.row][first_select.col]->selected = 0;
                        game->board[second_select.row][second_select.col]->selected = 0;
                        if(!check_matches(game->board)) {
                            // sleep(1);
                            aux = second_select;
                            second_select.row = first_select.row;
                            second_select.col = first_select.col;
                            second_select.px = first_select.px;
                            second_select.py = first_select.py;

                                
                            first_select.row = aux.row;
                            first_select.col = aux.col;
                            first_select.px = aux.px;
                            first_select.py = aux.py;
                            state = CHANGING;
                        }
                        else {
                            while(check_matches(game->board)){
                                break_jewel(game);
                            }
                                
                        }
                        first_select.id_jewel = 0;
                        second_select.id_jewel = 0;

                    }
                    break;
                case CHANGING:
                    game->board[first_select.row][first_select.col]->selected = 0;
                    game->board[second_select.row][second_select.col]->selected = 0;
                    if ((game->board[first_select.row][first_select.col]->px != second_select.px) || (game->board[first_select.row][first_select.col]->py != second_select.py)) {
                        state_changing(first_select, second_select, game);
                        draw_board(game);
                    }
                    else {
                        change(game->board, first_select, second_select);
                        state = SELECT;
                    }
                    break;
            }
            redraw = false;
        }
    }
    
    update_score(game);

    fclose(game->score_log);
    destroy_board(game->board);
    al_destroy_bitmap(game->background);
    al_destroy_font(game->fonts->font_big);
    al_destroy_font(game->fonts->font_medium);
    al_destroy_font(game->fonts->font_small);
    free(game->fonts);
    al_destroy_bitmap(game->sprites->red);
    al_destroy_bitmap(game->sprites->white);
    al_destroy_bitmap(game->sprites->black);
    al_destroy_bitmap(game->sprites->orange);
    al_destroy_bitmap(game->sprites->cyan);
    free(game->sprites);
    al_destroy_sample(game->audios->pop);
    al_destroy_sample(game->audios->music);
    al_destroy_sample(game->audios->breaks);
    al_destroy_sample(game->audios->play);
    free(game->audios);
    free(game);
    free(event);
    //audio_deinit(audios);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(disp);

    return 0;
}