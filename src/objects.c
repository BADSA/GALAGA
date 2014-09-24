#include "main.h"
#include <stdio.h>
#include <time.h>
#include <cmath>
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
    // CREAR LAS NAVES ENEMIGAS===================
    //Enemy *enemies;
    //int x_inicial = 90;
    int x_final=90+xFinal;
    //int y_inicial = 220;
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

void *moverEnemigo (void *arguments){
    Enemy *e =(Enemy *) arguments;
    int direc=false;
    if (e->x < e->x_fin){
        direc= true;
    }

    //printf(" %d , %d , random %d \n", e->y , e->y_fin,r);
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
void *trayecCirc (void *arguments){
    sleep(2);
    Enemy *e =(Enemy *) arguments;
    //printf(" %d , %d , random %d \n", e->y , e->y_fin,r);
    //double x,y;
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

void *trayecCuadrado (void *arguments){
    sleep(2);
    Enemy *e =(Enemy *) arguments;
    //printf(" %d , %d , random %d \n", e->y , e->y_fin,r);
    while (e->live){
        //printf("Esta vivo\n");
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

void movCuadrado(Enemy *enemies, int size){

     printf(" Solo una vez \n");
     srand(time(NULL));
    pthread_t h[size];
    for( int i = 0 ; i < size/2 ; i++){
        pthread_create (&h[i], NULL, trayecCuadrado, (void *) &enemies[i] );
    }
    for( int i = size/2 ; i < size; i++){
        pthread_create (&h[i], NULL, trayecCuadrado, (void *) &enemies[i] );
    }
}

void movCirc(Enemy *enemies, int size){

     printf(" Solo una vez \n");
     srand(time(NULL));
    pthread_t h[size];
    for( int i = 0 ; i < size/2 ; i++){
        pthread_create (&h[i], NULL, trayecCirc, (void *) &enemies[i] );
    }
    for( int i = size/2 ; i < size; i++){
        pthread_create (&h[i], NULL, trayecCirc, (void *) &enemies[i] );
    }
}


void AnimacionEntrada(Enemy *enemies, int size){
     printf(" Solo una vez \n");
     srand(time(NULL));
    pthread_t h[size];
    for( int i = 0 ; i < size ; i+=2){
        //printf("creacion del hilo %d \n", contador );
        pthread_create (&h[i], NULL, moverEnemigo, (void *) &enemies[i] );
    }
    for( int i = 1 ; i < size; i+=2){
        //printf("creacion del hilo %d \n", contador );
        pthread_create (&h[i], NULL, moverEnemigo, (void *) &enemies[i] );
        //pthread_create (&h[i++], NULL, moverEnemigo, (void *) enemies );
    }
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
