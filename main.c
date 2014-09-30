#include "main.h"
#include "objects.c"

bool keys[] = {false, false, false,false, false, false};
enum KEYS{LEFT, RIGHT, UP, DOWN, SPACE, ENTER};

// GLOBAL VARIABLES=========================

bool done = false, render = false, explosion = false;

bool lockKamikase = false, lockColocho = false, lockJefe = false;
bool lockKamiDispa = false;
bool enemyIsShooting = false;

bool colision = false, initChaSta = false, notPlayed = true,difChaSta = true;
bool tBeam = false,capturado = false;


bool notInPos = false;
bool inGameOverFirstTime = true;
int score = 0;
int vidas = 3;
int status = MENU;
int NUM_BULLETS = 5;
int NUM_ENEMIES = 10;
int muertos = 0,nivel = 1, speed = 2 , segundosChaSta = 26 ;
int scoreChaSta = 0;
int velocidadAtaque = 50000;


bool animacionTerminada = false;

ALLEGRO_SAMPLE *bgSound;
ALLEGRO_SAMPLE *yellSound;
ALLEGRO_SAMPLE *shotSound;
ALLEGRO_SAMPLE *boomSound;
ALLEGRO_SAMPLE *gameoverSound;
ALLEGRO_SAMPLE *captureSound;

ALLEGRO_SAMPLE_INSTANCE *songInstance = NULL;

ALLEGRO_BITMAP *imagen_jugador;
ALLEGRO_BITMAP *imagen_explosion;

void limpiarTeclas(){
    keys[LEFT] = false;
    keys[RIGHT]= false;
    keys[UP]= false;
    keys[DOWN]= false;
    keys[LEFT]= false;
    keys[SPACE]= false;
    keys[ENTER]= false;
}

bool isChaSta(int nivel){
    return nivel%4==0;
}

void liberarLocks(){
    enemyIsShooting = false;
    lockKamikase = false;
    lockColocho = false;
    lockKamiDispa = false;
    lockJefe = false;
}

void initConfig(){
    score = 0;
    vidas = 3;
    muertos = 0;
    nivel = 1;
    speed = 2;
    segundosChaSta = 26 ;
    scoreChaSta = 0;
    velocidadAtaque = 50000;
}

int bbcollision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h){
    if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
        (b1_y > b2_y + b2_h - 1) || // is b1 under b2?
        (b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
        (b2_y > b1_y + b1_h - 1))   // is b2 under b1?
    {
        return 0; // no collision
    }
    return 1; // collision
}

void limpiarBalasDePantalla(Enemy *e2,Enemy *e4, Bullet b[]){
    limpiarBullets(e2,8);
    limpiarBullets(e4,4);

	for(int i = 0; i < NUM_BULLETS; i++){
		b[i].live = false;
	}
}



int main(){

	//allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *bg = NULL;
	ALLEGRO_TIMER *timer;


	//program init
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	//==============================================
	//ADDON INSTALL
	//==============================================
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

    // FONT DEL PROGRAMA.
	ALLEGRO_FONT *font = al_load_ttf_font("fonts/Sarpanch-SemiBold.ttf",30,0 );
	ALLEGRO_FONT *font_copy = al_load_ttf_font("fonts/Sarpanch-SemiBold.ttf",20,0 );
	// 50 30 20 15 10
	ALLEGRO_FONT *font50 = al_load_ttf_font("fonts/pirulen.ttf",50,0 );
	ALLEGRO_FONT *font30 = al_load_ttf_font("fonts/pirulen.ttf",30,0 );
	ALLEGRO_FONT *font20 = al_load_ttf_font("fonts/pirulen.ttf",20,0 );
	ALLEGRO_FONT *font15 = al_load_ttf_font("fonts/pirulen.ttf",15,0 );
	ALLEGRO_FONT *font10 = al_load_ttf_font("fonts/pirulen.ttf",10,0 );

	ALLEGRO_BITMAP *ENM1 = al_load_bitmap("img/En01.PNG");
	ALLEGRO_BITMAP *ENM2 = al_load_bitmap("img/En02.PNG");
	ALLEGRO_BITMAP *ENM3 = al_load_bitmap("img/En03.PNG");
	ALLEGRO_BITMAP *ENM4 = al_load_bitmap("img/En04.PNG");
	ALLEGRO_BITMAP *JEFE = al_load_bitmap("img/jefe1.PNG");
	ALLEGRO_BITMAP *JEFE2 = al_load_bitmap("img/jefe2.PNG");


    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(3);

    bgSound = al_load_sample( "sounds/main.wav" );
    yellSound=al_load_sample( "sounds/ow.wav" );
    shotSound=al_load_sample( "sounds/fire.wav" );
    boomSound=al_load_sample( "sounds/boom.wav" );
    captureSound=al_load_sample( "sounds/capture.wav" );
    gameoverSound=al_load_sample( "sounds/Game_Over.wav" );


    songInstance = al_create_sample_instance(bgSound);

	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());


    // VARIABLES DEL JUEGO ========================
    bg = al_load_bitmap("img/sp1.jpg");
    imagen_jugador = al_load_bitmap("img/zero.PNG");
    imagen_explosion = al_load_bitmap("img/explode.png");

    SpaceShip nave_jugador;
    Enemy enemies1[10];
    Enemy enemies2[10];
    Enemy enemies3[10];
    Enemy enemies4[10];
    Enemy jefe1[2];
    Bullet bullets[NUM_BULLETS];
    Tbeam tbeam;


    char vidas_char[2];
    char nivel_char[2];
    char var[10];
    ALLEGRO_COLOR cJugar = GRAY_SELECTED;
    ALLEGRO_COLOR cAcerca = GRAY;
    ALLEGRO_COLOR cSalir = GRAY;
    short seleccion = JUGAR;
    bool keyPressed = false;
    bool animacion = true;
    bool animacion2 = true;
    int iniciarMovs = true;


    // INICIALIZAR OBJETOS====================

    InitShip(nave_jugador);
    InitBullet(bullets, NUM_BULLETS);
    InitTbeam(tbeam);
    InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
    InitEnemies(enemies2,speed,ENM2,NULL,false,10,50,180,WIDTH/2+100);
    InitEnemies(enemies3,speed,ENM3,NULL,false,10,100,140,WIDTH);
    InitEnemies(enemies4,speed,ENM4,NULL,false,10,150,100,(WIDTH/2)+100);
    InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);

	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

    int ataque;

    ALLEGRO_EVENT ev;
	while(!done){


		al_wait_for_event(event_queue, &ev);

        al_play_sample_instance(songInstance);

		if (status == MENU){
            //==============================================
            //INPUT
            //==============================================
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[UP] = true;
                    keys[DOWN] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = true;
                    keys[UP] = false;
                    break;
                case ALLEGRO_KEY_ENTER:
                    keys[ENTER] = true;
                    break;
                }
                keyPressed = true;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_ENTER:
                    keys[ENTER] = false;
                    break;
                }
            }

            //==============================================
            //GAME UPDATE
            //==============================================
            else if(ev.type == ALLEGRO_EVENT_TIMER){
                render = true;
                if (keyPressed){
                    if (keys[ENTER]){
                        if (seleccion == JUGAR){
                            initConfig();
                            status = JUEGO;
                        }else if (seleccion == ACERCA){
                            status = DESCRIPCION;
                        }else if (seleccion == SALIR){
                            done = true;
                        }
                        keyPressed = false;
                    }else if(keys[UP]){
                        if (seleccion == ACERCA){
                            cJugar = GRAY_SELECTED;
                            cAcerca = GRAY;
                            cSalir = GRAY;
                            seleccion = JUGAR;
                        }else if (seleccion == SALIR){
                            cJugar = GRAY;
                            cAcerca = GRAY_SELECTED;
                            cSalir = GRAY;
                            seleccion = ACERCA;
                        }
                        keyPressed = false;
                    }else if(keys[DOWN]){
                        if (seleccion == JUGAR){
                            cJugar = GRAY;
                            cAcerca = GRAY_SELECTED;
                            cSalir = GRAY;
                            seleccion = ACERCA;
                        }else if (seleccion == ACERCA){
                            cJugar = GRAY;
                            cAcerca = GRAY;
                            cSalir = GRAY_SELECTED;
                            seleccion = SALIR;
                        }
                        keyPressed = false;
                    }
                }
            }


            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;
                al_draw_text(font50, GREEN, WIDTH/2 -140,HEIGHT/2 - 130, 0, "GALAGA");
                al_draw_text(font30, cJugar, WIDTH/2 -70,HEIGHT/2 - 60, 0, "Jugar");
                al_draw_text(font30, cAcerca, WIDTH/2 -90,HEIGHT/2 -20, 0, "Acerca");
                al_draw_text(font30, cSalir, WIDTH/2 -65,HEIGHT/2 + 20, 0, "Salir");
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }

        }else if (status == DESCRIPCION){

            //==============================================
            //INPUT
            //==============================================
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:
                    status = MENU;
                    break;
                case ALLEGRO_KEY_BACKSPACE:
                    status = MENU;
                    break;
                }
                keys[ENTER] = false;
            }

            render = true;

            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;

                al_draw_text(font30, GREEN, WIDTH/2 -130,HEIGHT/2 - 200, 0, "Descripcion: ");
                al_draw_text(font20, GRAY2, 80 ,70, 0, "Dispara a las naves enemigas");
                al_draw_text(font20, GRAY2, 50 ,90, 0, "antes de que lancen sus ataques");
                al_draw_text(font20, GRAY2, 210 ,110, 0, "y te derriben!");

                al_draw_text(font30, GREEN, WIDTH/2 -160,140, 0, "Instrucciones: ");
                al_draw_text(font20, GRAY2, 40,170, 0, "Flechas del cursor para moverte");
                al_draw_text(font20, GRAY2, 30,190, 0, "y barra espaciadora para disparar.");

                al_draw_text(font30, GREEN, 110,250, 0, "DESARROLLADORES:");
                al_draw_text(font30, GRAY_SELECTED, 85,280, 0, "Daniel Solis Mendez");
                al_draw_text(font30, GRAY_SELECTED, 60,310, 0, "Melvin Elizondo Perez");

                al_draw_text(font_copy, GRAY, 70,440, 0, "BADSA Corp | Todos los derechos reservados 2014");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
















		}else if (status == JUEGO){

           if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:

                    done = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    if (!explosion)
                        keys[LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if (!explosion)
                        keys[RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = true;
                    if (!animacion2 && !capturado) FireBullet(bullets, NUM_BULLETS, nave_jugador);
                    break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = false;
                    break;
                }
            }
            //==============================================
            //GAME UPDATE
            //==============================================
            else if(ev.type == ALLEGRO_EVENT_TIMER){
                render = true;
                if(keys[LEFT])
                        MoveShipLeft(nave_jugador);
                else if(keys[RIGHT])
                        MoveShipRight(nave_jugador);

                UpdateBullet(bullets, NUM_BULLETS);

                if (!animacion2 ){
                    ataque = rand()%1000;
                    if (ataque>=0 and ataque<50){
                        if (!lockJefe)
                           BossEnemy(jefe1,2,&nave_jugador);
                    }else if (ataque>=50 and ataque<400){
                        if (!enemyIsShooting)
                           ShooterEnemy(enemies2, 8);
                    }else if (ataque>=400 and ataque<600){
                        if (!lockKamikase)
                            KamikaseEnemy(enemies1,10,&nave_jugador);
                    }else if (ataque>=600 and ataque<800){
                        if (!lockColocho)
                            ColochoEnemy(enemies3,6,&nave_jugador);
                    }else if (ataque>=800 and ataque<1000){
                        if (!lockKamiDispa)
                            KamiDispaEnemy(enemies4,4,&nave_jugador);
                    }

                    CollideBullet(bullets,NUM_BULLETS,enemies1,NUM_ENEMIES);
                    CollideBullet(bullets,NUM_BULLETS,enemies2,8);
                    CollideBullet(bullets,NUM_BULLETS,enemies3,6);
                    CollideBullet(bullets,NUM_BULLETS,enemies4,4);
                    CollideBullet(bullets,NUM_BULLETS,jefe1,2);
                }


                if (!animacion2 && !colision && !capturado){
                    CollideBulletSpaceship(enemies2,8,nave_jugador);
                    CollideBulletSpaceship(enemies4,4,nave_jugador);

                    CollideEnemiesSpaceship(enemies1,10,nave_jugador);
                    CollideEnemiesSpaceship(enemies2,8,nave_jugador);
                    CollideEnemiesSpaceship(enemies3,6,nave_jugador);
                    CollideEnemiesSpaceship(enemies4,4,nave_jugador);
                    CollideEnemiesSpaceship(jefe1,2,nave_jugador);
                }
                colision = isSpaceshipCollide(enemies1,enemies2,enemies3,enemies4,jefe1,nave_jugador);
            }

            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;

                // INFORMACION DEL JUEGO Y BG
                if (vidas==0){lockKamiDispa= false; status = GAME_OVER ; limpiarTeclas();}

                if (animacion){
                    movEnemies(enemies1,10,1);
                    movEnemies(enemies2, 8,1);
                    movEnemies(enemies3,6,1);
                    movEnemies(enemies4,4,1);
                    movEnemies(jefe1,2,1);
                    animacion = false;
                }

                // Objetos de la Ventana
                al_draw_bitmap(bg, 0, 0, 0);
                al_draw_bitmap(imagen_jugador, 5,440,0);

                al_draw_text(font, al_map_rgb(255,255,255), 40,430, 0, "X");

                sprintf(vidas_char,"%d",vidas);
                al_draw_text(font, al_map_rgb(255,255,255), 60,430, 0, vidas_char);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 + 180, 0,ALLEGRO_ALIGN_CENTRE, "Score");
                char vartext[10];
                sprintf(vartext,"%d",score);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 + 230, 0, 0, vartext);

                al_draw_text(font15, al_map_rgb(255,255,255), 550, 450,ALLEGRO_ALIGN_CENTRE, "Nivel");
                sprintf(nivel_char,"%d",nivel);
                al_draw_text(font20, al_map_rgb(255,255,255), 600,445, 0, nivel_char);

                if (tBeam){
                    DrawTbeam(jefe1,&nave_jugador, tbeam,2);
                }

                if (notInPos){
                    vidas--;
                    nave_jugador.x = WIDTH/2;
                    nave_jugador.y = 420;
                    notInPos = false;
                }

                al_draw_bitmap(nave_jugador.image, nave_jugador.x - nave_jugador.w / 2, nave_jugador.y - nave_jugador.h / 2, 0);

                // Dibujar Balas
                DrawBullet(bullets, NUM_BULLETS);

                DrawEnemies(enemies1,10);
                DrawEnemies(enemies2,8);
                DrawEnemies(enemies3,6);
                DrawEnemies(enemies4,4);
                DrawEnemies(jefe1,2);

                if(!animacion){
                    if (animacion2) animacion2 = checkAnimationStatus(enemies1, enemies2, enemies3, enemies4, jefe1);
                    if (!animacion2){
                        DrawEnemyBullets(enemies2,8);
                        DrawEnemyBullets(enemies4,4);
                    }
                }

                if (allDied(enemies1,10,enemies2,8,enemies3,6,enemies4,4,jefe1,2)){
                    limpiarBalasDePantalla(enemies2,enemies4,bullets);
                    if (!isChaSta(nivel+1)){
                        nivel++;
                        muertos=0;
                        speed++;speed++;
                        tBeam = false;
                        velocidadAtaque -= 5000;
                        //limpiarBalasDePantalla(enemies2,enemies4,bullets);
                        InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
                        InitEnemies(enemies2,speed,ENM2,NULL,false,8,50,180,WIDTH/2+100);
                        InitEnemies(enemies3,speed,ENM3,NULL,false,6,100,140,WIDTH);
                        InitEnemies(enemies4,speed,ENM4,NULL,false,4,150,100,(WIDTH/2)+100);
                        InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);
                        animacion = true;
                        animacion2 = true;
                        // Liberar locks
                        liberarLocks();
                    }else{
                        //notPlayed = false;
                        initChaSta = false;
                        animacion = true;
                        animacion2= true;
                        scoreChaSta = 0;
                        status = CHALLENGING_STAGE;
                    }

                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));

            }



















		}else if (status == CHALLENGING_STAGE){

		    if (!initChaSta){
                iniciarMovs = true;
                InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
                InitEnemies(enemies2,speed,ENM2,NULL,false,10,0,180,WIDTH/2+100);
                InitEnemies(enemies3,speed,ENM3,NULL,false,10,0,140,WIDTH);
                InitEnemies(enemies4,speed,ENM4,NULL,false,10,0,100,(WIDTH/2)+100);
                initChaSta = true;
                iniciarTempo();
		    }

            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = true;
                     FireBullet(bullets, NUM_BULLETS, nave_jugador); //if (!animacion2)
                    break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            {
                switch(ev.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    keys[LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    keys[RIGHT] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    keys[SPACE] = false;
                    break;
                }
            }

            else if(ev.type == ALLEGRO_EVENT_TIMER){

                render = true;
                if(keys[LEFT])
                    MoveShipLeft(nave_jugador);
                else if(keys[RIGHT])
                    MoveShipRight(nave_jugador);

                UpdateBullet(bullets, NUM_BULLETS);

                CollideBullet(bullets,NUM_BULLETS,enemies1,10);
                CollideBullet(bullets,NUM_BULLETS,enemies2,10);
                CollideBullet(bullets,NUM_BULLETS,enemies3,10);
                CollideBullet(bullets,NUM_BULLETS,enemies4,10);
            }


            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;
                if (animacion){
                    movEnemies(enemies1,10,1);
                    movEnemies(enemies2, 10,1);
                    movEnemies(enemies3,10,1);
                    movEnemies(enemies4,10,1);
                    animacion=false;
                }


                // Objetos de la Ventana
                al_draw_bitmap(bg, 0, 0, 0);
                al_draw_bitmap(imagen_jugador, 5,440,0);
                al_draw_text(font, al_map_rgb(255,255,255), 40,430, 0, "X");
                sprintf(vidas_char,"%d",vidas);
                al_draw_text(font, al_map_rgb(255,255,255), 60,430, 0, vidas_char);

                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 + 180, 0,ALLEGRO_ALIGN_CENTRE, "Score");
                char vartext[10];
                sprintf(vartext,"%d",score);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 +230, 0, 0, vartext);

                al_draw_text(font10, al_map_rgb(255,255,255), WIDTH/2 + 175, 40,ALLEGRO_ALIGN_CENTRE, "Score");
                char vartext2[10];
                sprintf(vartext2,"%d",scoreChaSta);
                al_draw_text(font10, al_map_rgb(255,255,255), WIDTH/2 +205, 40, 0, vartext2);


                sprintf(var,"%d",segundosChaSta);
                al_draw_text(font20, al_map_rgb(255,255,255), WIDTH/2,10, 0, var);

                al_draw_bitmap(nave_jugador.image, nave_jugador.x - nave_jugador.w / 2, nave_jugador.y - nave_jugador.h / 2, 0);

                //al_draw_rectangle(x+5, y ,x+nave_jugador.w-5,y+nave_jugador.h,GRAY,1);

                // Dibujar Balas
                DrawBullet(bullets, NUM_BULLETS);


                al_draw_text(font15, al_map_rgb(255,255,255), 500, 450,ALLEGRO_ALIGN_CENTRE, "Challenging Stage");

                DrawEnemies(enemies1,10);
                DrawEnemies(enemies2,10);
                DrawEnemies(enemies3,10);
                DrawEnemies(enemies4,10);

                if(!animacion){
                    if (animacion2){
                        animacion2 = checkAnimationStatus(enemies1, enemies2, enemies3, enemies4, jefe1);
                    }
                }

                if (iniciarMovs){
                    movEnemies(enemies1,10,2);
                    movEnemies(enemies2,10,2);
                    movEnemies(enemies3,10,2);
                    movEnemies(enemies4,10,2);
                    iniciarMovs = false;
                }


                if (segundosChaSta==0 || allDied(enemies1,10,enemies2,10, enemies3,10, enemies4,10)){
                    if (segundosChaSta==0){
                        score += scoreChaSta;
                    }else{
                        score += 10000;
                    }
                    segundosChaSta = 26;
                    InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
                    InitEnemies(enemies2,speed,ENM2,NULL,false,8,50,180,WIDTH/2+100);
                    InitEnemies(enemies3,speed,ENM3,NULL,false,6,100,140,WIDTH);
                    InitEnemies(enemies4,speed,ENM4,NULL,false,4,150,100,(WIDTH/2)+100);
                    InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);

                    animacion = true;
                    animacion2 = true;

                    liberarLocks();
                    nivel++;
                    status = JUEGO;

                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }















		}else if(status == GAME_OVER ){
		    if (inGameOverFirstTime){
                al_play_sample(gameoverSound, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                inGameOverFirstTime=false;
            }
            //==============================================
            //INPUT
            //==============================================
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                    break;
                case ALLEGRO_KEY_UP:
                    keys[UP] = true;
                    keys[DOWN] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    keys[DOWN] = true;
                    keys[UP] = false;
                    break;
                case ALLEGRO_KEY_ENTER:
                    keys[ENTER] = true;
                    break;
                }
            }

            //==============================================
            //GAME UPDATE
            //==============================================
            else if(ev.type == ALLEGRO_EVENT_TIMER){
                render = true;
                if (keys[ENTER]){
                    if (seleccion == JUGAR){
                        limpiarBalasDePantalla(enemies2,enemies4,bullets);
                        liberarLocks();
                        initConfig();
                        inGameOverFirstTime = true;
                        tBeam = false;
                        InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
                        InitEnemies(enemies2,speed,ENM2,NULL,false,8,50,180,WIDTH/2+100);
                        InitEnemies(enemies3,speed,ENM3,NULL,false,6,100,140,WIDTH);
                        InitEnemies(enemies4,speed,ENM4,NULL,false,4,150,100,(WIDTH/2)+100);
                        InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);

                        animacion = true;
                        animacion2 = true;
                        status = JUEGO;
                    }else if (seleccion == SALIR){
                        done=true;
                    }
                }else if(keys[UP]){
                    if (seleccion == SALIR){
                        cJugar = GRAY_SELECTED;
                        cSalir = GRAY;
                        seleccion = JUGAR;
                    }
                }else if(keys[DOWN]){
                    if (seleccion == JUGAR){
                        cJugar = GRAY;
                        cSalir = GRAY_SELECTED;
                        seleccion = SALIR;
                    }
                }
            }

            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;

                al_draw_text(font50, GREEN, WIDTH/2-200 ,HEIGHT/2-50 , 0, "GAME OVER");
                al_draw_text(font15, cJugar, WIDTH/2-50 ,HEIGHT/2+10, 0, "Reintentar");
                al_draw_text(font15, cSalir, WIDTH/2-20 ,HEIGHT/2+40 , 0, "Salir");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }

        }

        //al_flip_display();
        //al_clear_to_color(al_map_rgb(0,0,0));

    }


    al_destroy_font(font);
    al_destroy_font(font50);
    al_destroy_font(font30);
    al_destroy_font(font20);
    al_destroy_font(font15);
    al_destroy_font(font10);
    al_destroy_font(font_copy);

    for (int i=0;i<10;i++){
        al_destroy_bitmap(enemies1[i].image);
        al_destroy_bitmap(enemies2[i].image);
        al_destroy_bitmap(enemies3[i].image);
        al_destroy_bitmap(enemies4[i].image);
        al_destroy_bitmap(jefe1[i].image);
    }
    al_destroy_sample(bgSound);
    al_destroy_sample(shotSound);
    al_destroy_sample(yellSound);

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object

	return 0;
}
