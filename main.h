#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <allegro.h>
#include <allegro_image.h>
#include <allegro_native_dialog.h>
#include <allegro_primitives.h>
#include <allegro_font.h>
#include <allegro_ttf.h>

#include "objects.h"
// status galaga
#define MENU 1
#define DESCRIPCION 2
#define JUEGO 3
#define GAME_OVER 4

// opciones
#define JUGAR 1
#define ACERCA 2
#define SALIR 3


#define GREEN al_map_rgb(10,200,10)
#define GRAY_SELECTED al_map_rgb(200,200,200)
#define GRAY2 al_map_rgb(150,150,150)
#define GRAY al_map_rgb(70,70,70)
#define WIDTH 640
#define HEIGHT 480

#define ENM1 al_load_bitmap("img/En01.PNG")
#define ENM2 al_load_bitmap("img/En02.PNG")
#define ENM3 al_load_bitmap("img/En03.PNG")
#define ENM4 al_load_bitmap("img/En04.PNG")
#define JEFE al_load_bitmap("img/jefe1.PNG")
#define JEFE2 al_load_bitmap("img/jefe2.PNG")
#define font(tam) al_load_ttf_font("fonts/pirulen.ttf",tam,0 )

extern int NUM_BULLETS, NUM_ENEMIES;
extern bool done, render, colision, enemyIsShooting;
extern int score,vidas,muertos;

int bbcollision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);


#endif // MAIN_H_INCLUDED
