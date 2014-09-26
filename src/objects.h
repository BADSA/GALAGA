#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED

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
	int ID;
	bool live;
	int speed;
<<<<<<< HEAD
	ALLEGRO_BITMAP *image;
	Bullet bullet;

	ALLEGRO_BITMAP *image2;
	int cantVidas;
	bool isBoss;
};
typedef struct
{
    float x;
    float y;
}
Point2D;

// METHODS ==========================
void InitShip(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);
=======
	ALLEGRO_BITMAP *image;
};
typedef struct
{
    float x;
    float y;
}
Point2D;

// METHODS ==========================
void InitShip(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Enemy enemies[], int eSize);
>>>>>>> c05fe6ae2f39075b34aca66d3fadab340e19b84f

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Enemy enemies[], int eSize);

<<<<<<< HEAD

void InitEnemies(Enemy enemies[],int size);
void DrawEnemies(Enemy enemies[], int size);

void AnimacionEntrada(Enemy enemies[], int size);
void *curveBezierAux(void * arg);
void curveBezierAux( Point2D* cp, int numberOfPoints, Point2D *curve );
Point2D PointOnCubicBezier( Point2D* cp, float t );

=======
void InitEnemies(Enemy enemies[],int size);
void DrawEnemies(Enemy enemies[], int size);

void AnimacionEntrada(Enemy enemies[], int size);
void *curveBezierAux(void * arg);
void curveBezierAux( Point2D* cp, int numberOfPoints, Point2D *curve );
Point2D PointOnCubicBezier( Point2D* cp, float t );

>>>>>>> c05fe6ae2f39075b34aca66d3fadab340e19b84f
#endif // OBJECTS_H_INCLUDED
