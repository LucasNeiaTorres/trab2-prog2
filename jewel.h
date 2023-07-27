#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#ifndef _jewel_t_H
#define _jewel_t_H

typedef struct {
    int id_jewel;
    ALLEGRO_BITMAP* image;
    int level;
    int row, col;
    int px, py;
    int selected;
} jewel_t; 

#endif