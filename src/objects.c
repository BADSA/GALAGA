#include "main.h"



void MoveShipLeft(SpaceShip &ship)
{
	ship.x -= ship.speed;
	if(ship.x < 0)
		ship.x = 0;
<<<<<<< HEAD
}

void MoveShipRight(SpaceShip &ship)
{
	ship.x += ship.speed;
	if(ship.x > WIDTH)
		ship.x = WIDTH;
}

void InitShip(SpaceShip &ship){
	ship.x = WIDTH / 2;
	ship.y = 420;
	ship.speed = 5;
	ship.image = al_load_bitmap("img/zero.PNG");
    ship.w = al_get_bitmap_width(ship.image);
    ship.h = al_get_bitmap_height(ship.image);
}

void InitBullet(Bullet bullet[], int size){
	for(int i = 0; i < size; i++){
	    bullet[i].r = 2;
		bullet[i].speed = 6;
		bullet[i].live = false;
	}
}

void InitEnemies(Enemy enemies[],ALLEGRO_BITMAP *img,ALLEGRO_BITMAP *img2,bool isBoss, int size = 10,int xFinal = 0 , int yFinal =  0,int xInicial=0){
    int x_final=90+xFinal;
    for (int i =0;i<size;i++){
        enemies[i].ID=i;
        enemies[i].image = img;
        enemies[i].x_fin = x_final;
        enemies[i].x =xInicial;
        enemies[i].live = true;
        enemies[i].y_fin = yFinal;
        enemies[i].y = -5;
        enemies[i].w = al_get_bitmap_width(enemies[i].image);
        enemies[i].h = al_get_bitmap_height(enemies[i].image);
        enemies[i].speed = 2;
        if (isBoss){
            enemies[i].cantVidas = 2;
            enemies[i].image2 = img2;
            enemies[i].isBoss = true;
        }else{
            enemies[i].bullet.r = 2;
            enemies[i].bullet.speed = 5;
            enemies[i].bullet.live=false;

            enemies[i].isBoss = false;
        }
        x_final += 50;
    }
}
=======
}

void MoveShipRight(SpaceShip &ship)
{
	ship.x += ship.speed;
	if(ship.x > WIDTH)
		ship.x = WIDTH;
}

void InitShip(SpaceShip &ship){
	ship.x = WIDTH / 2;
	ship.y = 420;
	ship.speed = 5;
	ship.image = al_load_bitmap("img/zero.PNG");
    ship.w = al_get_bitmap_width(ship.image);
    ship.h = al_get_bitmap_height(ship.image);
}

void InitBullet(Bullet bullet[], int size){
	for(int i = 0; i < size; i++){
	    bullet[i].r = 2;
		bullet[i].speed = 6;
		bullet[i].live = false;
	}
}

void InitEnemies(Enemy enemies[],ALLEGRO_BITMAP *img,int size = 10,int xFinal = 0 , int yFinal =  0,int xInicial=0){
    int x_final=90+xFinal;
    for (int i =0;i<size;i++){
        enemies[i].ID=i;
        enemies[i].image = img;
        enemies[i].x_fin = x_final;
        enemies[i].x =xInicial;
        enemies[i].live = true;
        enemies[i].y_fin = yFinal;
        enemies[i].y = -5;
        enemies[i].w = al_get_bitmap_width(enemies[i].image);
        enemies[i].h = al_get_bitmap_height(enemies[i].image);
        enemies[i].speed = 3;
        x_final += 50;
    }
}
>>>>>>> c05fe6ae2f39075b34aca66d3fadab340e19b84f

void DrawEnemies(Enemy *enemies, int size){
	for( int i = 0; i < size; i++)
		if(enemies[i].live)
            al_draw_bitmap(enemies[i].image, enemies[i].x, enemies[i].y, 0);
}

void *animacionEntrada (void *arguments){
    Enemy *e =(Enemy *) arguments;
    int direc=false;
    if (e->x < e->x_fin){
        direc= true;
    }
    while (e->y < e->y_fin){
        int ran = rand();
        usleep(5000+ran%10000);
        e->y += e->speed;
        if (direc){
            while (e->x < e->x_fin){
                usleep(5000+ran%10000);
                e->x += e->speed;
<<<<<<< HEAD
            }
        }else {
            while (e->x > e->x_fin){
                usleep(5000+ran%10000);
                e->x -= e->speed;
            }
=======
            }
        }else {
            while (e->x > e->x_fin){
                usleep(5000+ran%10000);
                e->x -= e->speed;
            }
>>>>>>> c05fe6ae2f39075b34aca66d3fadab340e19b84f
        }
    }
    pthread_exit(NULL);
}
void *movTriang (void *arguments){
    sleep(2);
    Enemy *e =(Enemy *) arguments;
    while (e->live){
        int ran = rand();
        for (int i = e->y; i<e->y_fin+100; i+=e->speed){
            e->y+=e->speed;
            e->x+=e->speed;
            usleep(10000+ran%10000);
        }
        for (int i = e->x; i>e->x_fin-100;i-=e->speed){
            e->x-=e->speed;
            usleep(10000+ran%10000);
        }
        for (int i = e->y; i>e->y_fin; i-=e->speed){
            e->y-=e->speed;
            e->x+=e->speed;
            usleep(10000+ran%10000);
        }
    }
    pthread_exit(NULL);
}
void *movCirc (void *arguments){
    sleep(2);
    Enemy *e =(Enemy *) arguments;
    while (e->live){
        int ran = rand();
        for (double t = 0; t < 2*(3.14159265); t += 0.01) {
            usleep(10000+ran%10000);
            e->x =(int)( 75*cos(t) + e->x_fin);
            e->y =(int) (75*sin(t) + e->y_fin);
        }
    }
    pthread_exit(NULL);
}

void *movCuadrado (void *arguments){
    sleep(2);
    Enemy *e =(Enemy *) arguments;
    while (e->live){
        int ran = rand();
        if(e->ID<6){
            for (int i=e->x_fin; i<e->x_fin+150; i+=e->speed){e->x=i;usleep(10000+ran%10000);}; //e->x=i;
            for (int i=e->y_fin; i<e->y_fin+80; i+=e->speed){e->y=i;usleep(10000+ran%10000);}; //e->x=i;
            for (int i=e->x; i>e->x_fin; i-=e->speed){e->x=i;usleep(10000+ran%10000);}; //e->x=i;
            for (int i=e->y; i>e->y_fin; i-=e->speed){e->y=i;usleep(10000+ran%10000);}; //e->x=i;
        }else{
            for (int i=e->x_fin; i>e->x_fin-150; i-=e->speed){e->x=i;usleep(10000+ran%10000);}; //e->x=i;
            for (int i=e->y_fin; i<e->y_fin+80; i+=e->speed){e->y=i;usleep(10000+ran%10000);}; //e->x=i;
            for (int i=e->x; i<e->x_fin; i+=e->speed){e->x=i;usleep(10000+ran%10000);}; //e->x=i;
            for (int i=e->y; i>e->y_fin; i-=e->speed){e->y=i;usleep(10000+ran%10000);}; //e->x=i;
        }
<<<<<<< HEAD

    }
    pthread_exit(NULL);
}


=======

    }
    pthread_exit(NULL);
}


>>>>>>> c05fe6ae2f39075b34aca66d3fadab340e19b84f

void DrawBullet(Bullet bullet[], int size){
	for( int i = 0; i < size; i++){
		if(bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y, bullet[i].r , al_map_rgb(255, 255, 255));
	}
}

void FireBullet(Bullet bullet[], int size, SpaceShip &ship){
	for( int i = 0; i < size; i++){
		if(!bullet[i].live){
			bullet[i].x = ship.x;
			bullet[i].y = ship.y-10;
			bullet[i].live = true;
			return;
        }
	}
}

void UpdateBullet(Bullet bullet[], int size){
	for(int i = 0; i < size; i++){
		if(bullet[i].live){
			bullet[i].y -= bullet[i].speed;
			if(bullet[i].y < 0)
				bullet[i].live = false;
		}
	}
}
<<<<<<< HEAD


void CollideBullet(Bullet bullet[], int bSize, Enemy enemies[], int eSize){
	for(int i = 0; i < bSize; i++){
		if(bullet[i].live){
			for(int j =0; j < eSize; j++){
				if(enemies[j].live){
					if( bbcollision(bullet[i].x , bullet[i].y, bullet[i].r, bullet[i].r,
                                    enemies[j].x, enemies[j].y, enemies[j].w, enemies[j].h)){
						bullet[i].live = false;
						if (enemies[j].isBoss){
                            enemies[j].cantVidas--;
                            if (enemies[j].cantVidas==0){
                                enemies[j].live = false;
                            }else{
                                enemies[j].image = enemies[j].image2;
                            }
                            score += 17;
						}else{
                            enemies[j].live = false;
                            colision=true; // solo para probar
                            score += 7;
						}
						return;
					}else colision=false;
				}
			}
		}
	}
}
//============================================================================================
/*
Prueba para la curva de bezier
*/

Point2D PointOnCubicBezier( Point2D* cp, float t )
{
    float   ax, bx, cx;
    float   ay, by, cy;
    float   tSquared, tCubed;
    Point2D result;

    /* cálculo de los coeficientes polinomiales*/

    cx = 3.0 * (cp[1].x - cp[0].x);
    bx = 3.0 * (cp[2].x - cp[1].x) - cx;
    ax = cp[3].x - cp[0].x - cx - bx;

    cy = 3.0 * (cp[1].y - cp[0].y);
    by = 3.0 * (cp[2].y - cp[1].y) - cy;
    ay = cp[3].y - cp[0].y - cy - by;

     /*calculate the curve point at parameter value t*/

    tSquared = t * t;
    tCubed = tSquared * t;

    result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp[0].x;
    result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + cp[0].y;

    return result;
}

/*
 ComputeBezier fills an array of Point2D structs with the curve
 points generated from the control points cp. Caller must
 allocate sufficient memory for the result, which is
 <sizeof(Point2D) numberOfPoints>
*/

void  curveBezierAux( Point2D* cp, int numberOfPoints, Point2D *curve, Enemy *e ) {
    float   dt;
    int	  i;

    dt = 1.0 / ( numberOfPoints - 1 );

    for( i = 0; i < numberOfPoints; i++){
        curve[i] = PointOnCubicBezier( cp, i*dt );
        e->x=(int)curve[i].x; e->y=(int)curve[i].y ;
            usleep(50000);
        }
    for( i = numberOfPoints-1; i>0; i--){
        curve[i] = PointOnCubicBezier( cp, i*dt );
        e->x=(int)curve[i].x; e->y=(int)curve[i].y ;
            usleep(50000);
    }
}

void  * curveBezier( void * arg){
    Enemy *e =(Enemy *) arg;
    Point2D cp[4];
    cp[0].x=e->x_fin;
    cp[0].y=e->y_fin;
    cp[1].x=e->x_fin-50;
    cp[1].y=e->y_fin+150;
    cp[2].x=cp[1].x;
    cp[2].y=cp[1].x+50;
    cp[3].x= cp[0].x+300;
    cp[3].y= cp[0].y;
    Point2D *curve= (Point2D*)malloc(sizeof(Point2D) *50);
    curveBezierAux(cp,50, curve,e);
    pthread_exit(NULL);

}

void movEnemies(Enemy *enemies, int size,int numMov){
    srand(time(NULL));
    pthread_t h[size];
    if(numMov==1)
    for( int i = 0 ; i < size ; i++)
        pthread_create (&h[i], NULL, animacionEntrada, (void *) &enemies[i] );
    if(numMov==2)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, movCuadrado, (void *) &enemies[i] );
    if(numMov==3)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, movCirc, (void *) &enemies[i] );
    if(numMov==4)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, movTriang, (void *) &enemies[i] );
    if(numMov==5)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, curveBezier, (void *) &enemies[i] );
}

// MODIFICACIONES DESPUES DE PERDIDA DE ARCHIVOS OBJECTS.C Y OBJECTS.H

void  * disparaEnemigo( void * arg){
    Enemy *e =(Enemy *) arg;

    e->bullet.y = e->y+e->h;
    e->bullet.x = e->x+e->w/2;
    e->bullet.live = true;
    while(e->bullet.y < 480 and e->bullet.live){
        usleep(50000);
        e->bullet.y += e->bullet.speed;
    }
    e->bullet.live = false;
    enemyIsShooting = false;
    pthread_exit(NULL);
}

void ShooterEnemy(Enemy *enemies, int size){
    enemyIsShooting = true;
    bool disponibles=true;
    int cant = 0;
    for (int i =0;i<size;i++){
        if (enemies[i].live) cant++;
    }
    if (cant==0) disponibles = false;

    if (disponibles){
        int rnd = rand()%size;
        while (!enemies[rnd].live || enemies[rnd].bullet.live){
            rnd = rand()%size;
        }
        pthread_t h1;
        pthread_create (&h1, NULL, disparaEnemigo, (void *) &enemies[rnd] );
    }
}

void CollideBulletSpaceship(Enemy enemies[],int eSize,SpaceShip &ship){
    Enemy *e;
    for(int j =0; j < eSize; j++){
        e = &enemies[j];
        if(e->bullet.live){
            if( bbcollision(e->bullet.x , e->bullet.y, e->bullet.r, e->bullet.r,
                            (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h)){
                e->bullet.live = false;
                colision=true;
                vidas--;
            }
            return;
        }else colision=false;
    }
}

void UpdateBulletEnemy(Enemy enemies[],int size){
	for(int i = 0; i < size; i++){
		if(enemies[i].bullet.live){
			enemies[i].bullet.y += enemies[i].bullet.speed;
			if(enemies[i].bullet.y > 480)
				enemies[i].bullet.live = false;
		}
	}
}

bool checkAnimationStatus(Enemy enemies1[],Enemy enemies2[],Enemy enemies3[],Enemy enemies4[],Enemy jefe1[]){
    for(int i=0;i<10;i++){
        if (enemies1[i].x < enemies1[i].x_fin ||
            enemies1[i].y < enemies1[i].y_fin) return true;
    }
    for(int i=0;i<8;i++){
        if (enemies2[i].x < enemies2[i].x_fin ||
            enemies2[i].y < enemies2[i].y_fin) return true;
    }
    for(int i=0;i<6;i++){
        if (enemies3[i].x < enemies3[i].x_fin ||
            enemies3[i].y < enemies3[i].y_fin) return true;
    }
    for(int i=0;i<4;i++){
        if (enemies4[i].x < enemies4[i].x_fin ||
            enemies4[i].y < enemies4[i].y_fin) return true;
    }
    for(int i=0;i<2;i++){
        if (jefe1[i].x < jefe1[i].x_fin ||
            jefe1[i].y < jefe1[i].y_fin) return true;
    }
    return false;
}

void DrawEnemyBullets(Enemy enemies[],int size){
	for( int i = 0; i < size; i++){
		if(enemies[i].bullet.live)
			al_draw_filled_circle(enemies[i].bullet.x, enemies[i].bullet.y, enemies[i].bullet.r , al_map_rgb(255, 255, 255));
	}
=======


void CollideBullet(Bullet bullet[], int bSize, Enemy enemies[], int eSize){
	for(int i = 0; i < bSize; i++){
		if(bullet[i].live){
			for(int j =0; j < eSize; j++){
				if(enemies[j].live){
					if( bbcollision(bullet[i].x , bullet[i].y, bullet[i].r, bullet[i].r,
                                    enemies[j].x, enemies[j].y, enemies[j].w, enemies[j].h)){
						bullet[i].live = false;
						enemies[j].live = false;
						colision=true; // solo para probar
						score += 7;
						return;
					}else colision=false;
				}
			}
		}
	}
}
//============================================================================================
/*
Prueba para la curva de bezier
*/

Point2D PointOnCubicBezier( Point2D* cp, float t )
{
    float   ax, bx, cx;
    float   ay, by, cy;
    float   tSquared, tCubed;
    Point2D result;

    /* cálculo de los coeficientes polinomiales*/

    cx = 3.0 * (cp[1].x - cp[0].x);
    bx = 3.0 * (cp[2].x - cp[1].x) - cx;
    ax = cp[3].x - cp[0].x - cx - bx;

    cy = 3.0 * (cp[1].y - cp[0].y);
    by = 3.0 * (cp[2].y - cp[1].y) - cy;
    ay = cp[3].y - cp[0].y - cy - by;

     /*calculate the curve point at parameter value t*/

    tSquared = t * t;
    tCubed = tSquared * t;

    result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp[0].x;
    result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + cp[0].y;

    return result;
}

/*
 ComputeBezier fills an array of Point2D structs with the curve
 points generated from the control points cp. Caller must
 allocate sufficient memory for the result, which is
 <sizeof(Point2D) numberOfPoints>
*/

void  curveBezierAux( Point2D* cp, int numberOfPoints, Point2D *curve, Enemy *e ) {
    float   dt;
    int	  i;

    dt = 1.0 / ( numberOfPoints - 1 );

    for( i = 0; i < numberOfPoints; i++){
        curve[i] = PointOnCubicBezier( cp, i*dt );
        e->x=(int)curve[i].x; e->y=(int)curve[i].y ;
            usleep(50000);
        }
}

void  * curveBezier( void * arg){
    Enemy *e =(Enemy *) arg;
    Point2D cp[4];
    cp[0].x=e->x;
    cp[0].y=e->y;
    cp[1].x=0;
    cp[1].y=0;
    cp[3].x=100;
    cp[3].y=0;
    cp[4].x=150;
    cp[4].y=10;
    Point2D *curve= (Point2D*)malloc(sizeof(Point2D) *15);
    curveBezierAux(cp,15, curve,e);
    pthread_exit(NULL);

>>>>>>> c05fe6ae2f39075b34aca66d3fadab340e19b84f
}

void movEnemies(Enemy *enemies, int size,int numMov){
    srand(time(NULL));
    pthread_t h[size];
    if(numMov==1)
    for( int i = 0 ; i < size ; i++)
        pthread_create (&h[i], NULL, animacionEntrada, (void *) &enemies[i] );
    if(numMov==2)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, movCuadrado, (void *) &enemies[i] );
    if(numMov==3)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, movCirc, (void *) &enemies[i] );
    if(numMov==4)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, movTriang, (void *) &enemies[i] );
    if(numMov==5)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, curveBezier, (void *) &enemies[i] );
}
