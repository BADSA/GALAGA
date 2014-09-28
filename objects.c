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

void InitTbeam(Tbeam &tbeam){
	tbeam.x = WIDTH / 2;
	tbeam.y = 420;
	tbeam.speed = 5;
	tbeam.image = al_load_bitmap("img/tBeam.png");
    tbeam.w = al_get_bitmap_width(tbeam.image);
    tbeam.h = al_get_bitmap_height(tbeam.image);
    tbeam.live = false;
}

void InitEnemies(Enemy enemies[],int speed, ALLEGRO_BITMAP *img,ALLEGRO_BITMAP *img2,bool isBoss, int size = 10,int xFinal = 0 , int yFinal =  0,int xInicial=0){
    int x_final=90+xFinal;
    for (int i =0;i<size;i++){
        enemies[i].ID=i;
        enemies[i].image = img;
        enemies[i].atacando = false;
        enemies[i].x_fin = x_final;
        enemies[i].x =xInicial;
        enemies[i].live = true;
        enemies[i].y_fin = yFinal;
        enemies[i].y = -5;
        enemies[i].w = al_get_bitmap_width(enemies[i].image);
        enemies[i].h = al_get_bitmap_height(enemies[i].image);
        enemies[i].opcional = 0;
        enemies[i].speed = speed;
        if (isBoss){
            enemies[i].cantVidas = 2;
            enemies[i].image2 = img2;
            enemies[i].isBoss = true;
        }else{
            enemies[i].bullet.r = 2;
            enemies[i].bullet.speed = speed + 3;
            enemies[i].bullet.live=false;

            enemies[i].isBoss = false;
        }
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
    while(e->y>e->y_fin){
        usleep(50000);
        e->y -= e->speed;
    }
    e->y -=5;
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
    e->y = e->y_fin;
    e->x = e->x_fin;
    if (e->opcional!=-1)
        pthread_exit(NULL);
}

void *movTriang (void *arguments){
    sleep(3);
    Enemy *e =(Enemy *) arguments;
    while (e->live && status == CHALLENGING_STAGE){
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
    animacionEntrada((void*)e);
    pthread_exit(NULL);
}
void *movCirc (void *arguments){
    sleep(3);
    Enemy *e =(Enemy *) arguments;
    while (e->live && status == CHALLENGING_STAGE){
        int ran = rand();
        for (double t = 0; t < 2*(3.14159265); t += 0.01) {
            usleep(10000+ran%10000);
            e->x =(int)( 75*cos(t) + e->x_fin);
            e->y =(int) (75*sin(t) + e->y_fin);
        }
    }
    animacionEntrada((void*)e);
    pthread_exit(NULL);
}

void *movCuadrado (void *arguments){
    sleep(3);
    Enemy *e =(Enemy *) arguments;
    while (e->live && status == CHALLENGING_STAGE){
        int ran = rand();
        if(e->ID<6){
            for (int i=e->x_fin; i<e->x_fin+150; i+=e->speed){e->x=i;usleep(50000+ran%50000);}; //e->x=i;
            for (int i=e->y_fin; i<e->y_fin+80; i+=e->speed){e->y=i;usleep(50000+ran%50000);}; //e->x=i;
            for (int i=e->x; i>e->x_fin; i-=e->speed){e->x=i;usleep(50000+ran%50000);}; //e->x=i;
            for (int i=e->y; i>e->y_fin; i-=e->speed){e->y=i;usleep(50000+ran%50000);}; //e->x=i;
        }else{
            for (int i=e->x_fin; i>e->x_fin-150; i-=e->speed){e->x=i;usleep(50000+ran%50000);}; //e->x=i;
            for (int i=e->y_fin; i<e->y_fin+80; i+=e->speed){e->y=i;usleep(50000+ran%50000);}; //e->x=i;
            for (int i=e->x; i<e->x_fin; i+=e->speed){e->x=i;usleep(50000+ran%50000);}; //e->x=i;
            for (int i=e->y; i>e->y_fin; i-=e->speed){e->y=i;usleep(50000+ran%50000);}; //e->x=i;
        }

    }
    animacionEntrada((void*)e);
    pthread_exit(NULL);
}



void DrawBullet(Bullet bullet[], int size){
	for( int i = 0; i < size; i++){
		if(bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y, bullet[i].r , al_map_rgb(0, 0, 255));
	}
}

void FireBullet(Bullet bullet[], int size, SpaceShip &ship){
	for( int i = 0; i < size; i++){
		if(!bullet[i].live){
			bullet[i].x = ship.x;
			bullet[i].y = ship.y-10;
			bullet[i].live = true;
			al_play_sample(boing, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
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
						al_play_sample(boom, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
						if (enemies[j].isBoss){
                            enemies[j].cantVidas--;
                            if (enemies[j].cantVidas==0){
                                enemies[j].live = false;
                                muertos++;
                            }else{
                                enemies[j].image = enemies[j].image2;
                            }
                            if (status==JUEGO) score += 17;
                            else if (status == CHALLENGING_STAGE) scoreChaSta +=100;
						}else{
                            enemies[j].live = false;
                            //colision=true; // solo para probar
                            muertos++;
                            if (status==JUEGO) score += 7;
                            else if (status == CHALLENGING_STAGE) scoreChaSta +=100;
						}
						return;
					}//else colision=false;
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

void  curveBezierAux( Point2D* cp, int numberOfPoints, Point2D *curve, Enemy *e , bool disparar = false) {
    float   dt;
    int	  i;

    dt = 1.0 / ( numberOfPoints - 1 );

    for( i = 0; i < numberOfPoints; i++){
        if (i==25){
            pthread_t h1;
            pthread_create (&h1, NULL, disparaEnemigo, (void *) &e );
        }
        curve[i] = PointOnCubicBezier( cp, i*dt );
        e->x=(int)curve[i].x; e->y=(int)curve[i].y ;
            usleep(50000);
        }
    /*
    for( i = numberOfPoints-1; i>0; i--){
        curve[i] = PointOnCubicBezier( cp, i*dt );
        e->x=(int)curve[i].x; e->y=(int)curve[i].y ;
            usleep(50000);
    }*/
}


void  * curveBezier( void * arg){
    bool KamiDispa = false;
    Enemy *e = (Enemy *) arg;
    if (e->opcional>15000){
        e->opcional -= 5000;
        KamiDispa = true;
    }
    e->atacando = true;
    int x =e->x_fin;
    int y=e->y_fin;
    int navePos = e->opcional%10000;
    int difer1=150, difer2=50, difer3=0;
    Point2D curve[50]; //= (Point2D*)malloc(sizeof(Point2D) *50);
    Point2D cp[4];
    Point2D temp[4];
    if (e->opcional/10000)
        difer3=100;
    if(e->x_fin>=(WIDTH)/2){
        difer1*=-1;
        difer2*=-1;
    }
    cp[0].x=x;
    cp[0].y=y;
    cp[1].x=x+difer1;
    cp[1].y=y-50;
    cp[2].x=cp[1].x;
    cp[2].y=cp[1].y+200;
    cp[3].x=navePos;
    cp[3].y=420 + difer3 + 20;
    temp[0].x=cp[3].x;
    temp[0].y=cp[3].y;
    temp[1].x=cp[3].x+difer2;//reflejar es a la derecha o izq
    temp[1].y=cp[2].y-50;//asi queda
    temp[2].x=temp[1].x+(4*difer2);// -300;
    temp[2].y=temp[1].y;
    temp[3].x=cp[3].x+(6*difer2);
    temp[3].y=cp[3].y+200;

    if (KamiDispa){
        curveBezierAux(cp,50, curve,e,true);
        //lockKamiDispa = false;
    }
    else curveBezierAux(cp,50, curve,e);

    //if (KamiDispa)
     //   lockKamiDispa = false;

    if(difer3==0){
        curveBezierAux(temp,50, curve,e);
        lockColocho = false;
    }else{
        lockKamikase = false;
    }
    e->x=e->x_fin;
    e->y=-20;
    e->atacando=false;
    animacionEntrada((void *)e);
    pthread_exit(NULL);

}

void KamikaseEnemy(Enemy enemies[],int size,SpaceShip *s){

    bool disponibles=true;
    int cant = 0;
    for (int i =0;i<size;i++){
        if (enemies[i].live) cant++;
    }
    if (cant==0) disponibles = false;

    if (disponibles){
        int nrnd = rand()%size;
        while (!enemies[nrnd].live || enemies[nrnd].atacando ){ //
            nrnd = rand()%size;
        }
        enemies[nrnd].opcional=s->x+10000;
        lockKamikase = true;
        pthread_t h;
        pthread_create (&h, NULL, curveBezier, (void *) &enemies[nrnd]);
    }
}

void ColochoEnemy(Enemy enemies[],int size, SpaceShip *s){


    bool disponibles=true;
    int cant = 0;
    for (int i =0;i<size;i++){
        if (enemies[i].live) cant++;
    }
    if (cant==0) disponibles = false;

    if (disponibles){
        int nrnd = rand()%size;
        while (!enemies[nrnd].live || enemies[nrnd].atacando ){ //
            nrnd = rand()%size;
        }
        enemies[nrnd].opcional=s->x;
        lockColocho = true;
        pthread_t h;
        pthread_create (&h, NULL, curveBezier, (void *) &enemies[nrnd]);
    }
}

void KamiDispaEnemy(Enemy enemies[],int size,SpaceShip *s){
    bool disponibles=true;
    int cant = 0;
    for (int i =0;i<size;i++){
        if (enemies[i].live) cant++;
    }
    if (cant==0) disponibles = false;

    if (disponibles){
        int nrnd = rand()%size;
        while (!enemies[nrnd].live || enemies[nrnd].atacando ){ //
            nrnd = rand()%size;
        }
        enemies[nrnd].opcional=s->x+15000;
        lockKamiDispa = true;
        pthread_t h;
        pthread_create (&h, NULL, curveBezier, (void *) &enemies[nrnd]);
    }
}



void * movJefe(void *arg){
    Enemy *e = (Enemy *) arg;
    e->atacando = true;
    int x = e->opcional -15;
    int direc=0;
    int dif=0;
    if (e->x>x){
        direc=(-1*e->speed);
        dif=e->x-x;
    }else{
        direc=e->speed;
        dif=x-e->x;
    }
    while(e->x!=x && dif>=0){
        e->x+=direc;
        e->y+=e->speed;
        usleep(30000);
        dif-=e->speed;
    }
    while(e->y<360){
        e->y+=e->speed;
        usleep(30000);
    }
    tBeam = true;
    sleep(1);
    e->opcional=-1;
    animacionEntrada((void*)e);
    tBeam=false;
    if (capturado) notInPos = true;
    capturado=false;
    lockJefe = false;
    e->atacando = false;
    pthread_exit(NULL);
}


void BossEnemy(Enemy enemies[],int size,SpaceShip *s){

    bool disponibles=true;
    int cant = 0;
    for (int i =0;i<size;i++){
        if (enemies[i].live) cant++;
    }
    if (cant==0) disponibles = false;

    if (disponibles){
        int nrnd = rand()%size;
        while (!enemies[nrnd].live || enemies[nrnd].atacando ){ //
            nrnd = rand()%size;
        }
        enemies[nrnd].opcional=s->x;
        lockJefe = true;
        pthread_t h;
        pthread_create (&h, NULL, movJefe, (void *) &enemies[nrnd]);
    }
}

void movEnemies(Enemy *enemies, int size,int numMov, SpaceShip *s = NULL){
    srand(time(NULL));
    pthread_t h[size];
    int nrnd;

    if(numMov==1)
        for( int i = 0 ; i < size ; i++)
            pthread_create (&h[i], NULL, animacionEntrada, (void *) &enemies[i] );
    else if(numMov==2)
        for( int i = 0 ; i < size ; i++){
            nrnd = rand()%3;
            if (nrnd==0) pthread_create (&h[i], NULL, movTriang, (void *) &enemies[i] );
            else if (nrnd==1) pthread_create (&h[i], NULL, movCirc, (void *) &enemies[i] );
            else if (nrnd==2) pthread_create (&h[i], NULL, movCuadrado, (void *) &enemies[i] );
        }
    /*
    else if (numMov==5){
        lockColocho = true;
        bool disponibles=true;
        int cant = 0;
        for (int i =0;i<size;i++){
            if (enemies[i].live) cant++;
        }
        if (cant==0) disponibles = false;

        if (disponibles){
            nrnd = rand()%size;
            while (!enemies[nrnd].live || enemies[nrnd].bullet.live){
                nrnd = rand()%size;
            }
            enemies[nrnd].opcional=s->x;
            pthread_create (&h[nrnd], NULL, curveBezier, (void *) &enemies[nrnd]);
        }
    }
    */
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

void * dibujarExplosion(void * arg){
    SpaceShip *e =(SpaceShip *) arg;
    e->image=al_load_bitmap("img/explode.png");
    usleep(500000);
    e->image=al_load_bitmap("img/zero.PNG");
    e->x=WIDTH/2;
    explosion = false;
    pthread_exit(NULL);
}

void CollideBulletSpaceship(Enemy enemies[],int eSize,SpaceShip &ship){
    Enemy *e;
    for(int j =0; j < eSize; j++){
        e = &enemies[j];
        if(e->bullet.live){
            if( bbcollision(e->bullet.x , e->bullet.y, e->bullet.r, e->bullet.r,
                            (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h)){
                e->bullet.live = false;
                explosion = true;
                pthread_t h1;
                pthread_create (&h1, NULL, dibujarExplosion, (void *) &ship );
                vidas--;
                al_play_sample(ow, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            }
            //return;
        }
    }
}

bool isSpaceshipCollide(Enemy e1[],Enemy e2[],Enemy e3[],Enemy e4[],Enemy j1[],SpaceShip &ship){
    Enemy *e;
    for(int j =0; j < 10; j++){
        e = &e1[j];
        if(e->live)
            if( bbcollision(e->x , e->y, e->w, e->h,
                (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h))
                    return true;
    }
    for(int j =0; j < 8; j++){
        e = &e2[j];
        if(e->live)
            if( bbcollision(e->x , e->y, e->w, e->h,
                (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h))
                    return true;
    }
    for(int j =0; j < 6; j++){
        e = &e3[j];
        if(e->live)
            if( bbcollision(e->x , e->y, e->w, e->h,
                (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h))
                    return true;
    }
    for(int j =0; j < 4; j++){
        e = &e4[j];
        if(e->live)
            if( bbcollision(e->x , e->y, e->w, e->h,
                (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h))
                    return true;
    }
    for(int j =0; j < 2; j++){
        e = &j1[j];
        if(e->live)
            if( bbcollision(e->x , e->y, e->w, e->h,
                (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h))
                    return true;
    }
    return false;
}


void CollideEnemiesSpaceship(Enemy enemies[],int eSize,SpaceShip &ship){
    Enemy *e;
    for(int j =0; j < eSize; j++){
        e = &enemies[j];
        if(e->live){
            if (bbcollision(e->x , e->y, e->w, e->h,
                            (ship.x - ship.w/2) + 5, ship.y - ship.h/2, ship.w -5, ship.h)){
                explosion = true;
                pthread_t h1;
                pthread_create (&h1, NULL, dibujarExplosion, (void *) &ship );
                al_play_sample(ow, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                vidas--;
                //break;
            }
        }
    }
    //colision = false;
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
        if (enemies1[i].x != enemies1[i].x_fin ||
            enemies1[i].y != enemies1[i].y_fin) return true;
    }
    for(int i=0;i<8;i++){
        if (enemies2[i].x != enemies2[i].x_fin ||
            enemies2[i].y != enemies2[i].y_fin) return true;
    }
    for(int i=0;i<6;i++){
        if (enemies3[i].x != enemies3[i].x_fin ||
            enemies3[i].y != enemies3[i].y_fin) return true;
    }
    for(int i=0;i<4;i++){
        if (enemies4[i].x != enemies4[i].x_fin ||
            enemies4[i].y != enemies4[i].y_fin) return true;
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
			al_draw_filled_circle(enemies[i].bullet.x, enemies[i].bullet.y, enemies[i].bullet.r , al_map_rgb(255, 0, 0));
	}
}

void limpiarBullets(Enemy *enemies,int size){
    for( int i = 0; i < size; i++){
		enemies[i].bullet.live = false;
	}
}


void * temporizador(void * arg){
    while(segundosChaSta>0){
        segundosChaSta--;
        sleep(1);
    }
    pthread_exit(NULL);
}

void iniciarTempo(){
    pthread_t h1;
    pthread_create (&h1, NULL, temporizador, NULL );
}

bool allDied(Enemy *enemies1,int se1,Enemy *enemies2,int se2,Enemy *enemies3,int se3,
             Enemy *enemies4,int se4, Enemy *jefe = NULL, int sj =0){
    for(int i =0;i<se1;i++)
        if (enemies1[i].live) return false;
    for(int i =0;i<se2;i++)
        if (enemies2[i].live) return false;
    for(int i =0;i<se3;i++)
        if (enemies3[i].live) return false;
    for(int i =0;i<se4;i++)
        if (enemies4[i].live) return false;
    if (jefe!= NULL){
        for(int i =0;i<sj;i++)
            if (jefe[i].live) return false;
    }
    return true;
}


void DrawTbeam(Enemy e[],SpaceShip *ship,Tbeam tbeam,int size){
    for (int i=0; i< size; i++){
        if (tBeam){
            if (e[i].live)
                if(e[i].atacando){
                    tbeam.x=e[i].x-15;
                    tbeam.y=e[i].y+30;
                    al_draw_bitmap(tbeam.image,tbeam.x,tbeam.y, 0);
                    if (bbcollision(tbeam.x+30,tbeam.y,5,tbeam.h,ship->x-ship->w/2,ship->y+ship->h/2,ship->w,ship->h)){
                        capturado=true;
                    }
                    if (capturado){
                        ship->x=tbeam.x+32;
                        ship->y=tbeam.y+50;
                    }
                }
        }
    }
}
