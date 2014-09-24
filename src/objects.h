#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED
#include "main.h"

// STRUCTS ========================
struct SpaceShip
{
    float x;
	float y;
    int w;
	int h;
	int speed;
	ALLEGRO_BITMAP *image;
};

struct Bullet
{
	int x;
	int y;
	int r;
	bool live;
	int speed;
};

struct Sprite
{
	float x;
	float y;
    int w;
	int h;
	ALLEGRO_BITMAP *image;
};

struct Enemy
{
	int x;
	int y;
	int x_fin;
	int y_fin;
    int w;
	int h;
	bool live;
	int speed;
	ALLEGRO_BITMAP *image;
};


// METHODS ==========================
void InitShip(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Enemy enemies[], int eSize);


void InitEnemies(Enemy enemies[],int size);
void DrawEnemies(Enemy enemies[], int size);

void AnimacionEntrada(Enemy enemies[], int size);

#endif // OBJECTS_H_INCLUDED
