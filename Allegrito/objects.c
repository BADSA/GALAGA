#include "main.h"



void MoveShipLeft(SpaceShip &ship)
{
	ship.x -= ship.speed;
	if(ship.x < 0)
		ship.x = 0;
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
            }
        }else {
            while (e->x > e->x_fin){
                usleep(5000+ran%10000);
                e->x -= e->speed;
            }
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

    }
    pthread_exit(NULL);
}



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

    /* c�lculo de los coeficientes polinomiales*/

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
