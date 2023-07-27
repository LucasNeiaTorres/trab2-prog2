#ifndef _STRUCTS_
#define _STRUCTS_

#include "jewel.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <stdio.h>

typedef enum {BEGIN, HELP, SELECT, SECOND_CLICK, CHANGING, SCORE} state_t;

typedef struct {
    ALLEGRO_FONT* font_big;
    ALLEGRO_FONT* font_medium;
    ALLEGRO_FONT* font_small;
} font_t; 

typedef struct {
    ALLEGRO_BITMAP* red;
    ALLEGRO_BITMAP* white;
    ALLEGRO_BITMAP* black;
    ALLEGRO_BITMAP* cyan;
    ALLEGRO_BITMAP* orange;
} sprite_t; 

typedef struct audio_t
{
	ALLEGRO_SAMPLE* pop;
	ALLEGRO_SAMPLE* breaks;
    ALLEGRO_SAMPLE* music;
    ALLEGRO_SAMPLE* play;
} audio_t;

typedef struct {
    int score;
    int highscore;
    int nivel;
    jewel_t*** board;
    ALLEGRO_BITMAP* background;
    font_t* fonts;
    sprite_t* sprites;
    FILE* score_log;
    audio_t* audios;
} game_t; 

#endif