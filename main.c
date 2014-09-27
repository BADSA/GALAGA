#include "main.h"
#include "objects.c"

bool keys[] = {false, false, false,false, false, false};
enum KEYS{LEFT, RIGHT, UP, DOWN, SPACE, ENTER};

// GLOBAL VARIABLES=========================

bool done = false;
bool render = false;
bool colision = false;
int score = 0;
int vidas = 3;
short status = MENU;
int NUM_BULLETS = 5;
int NUM_ENEMIES = 10;
int muertos = 0,nivel = 1, speed = 2;

bool enemyIsShooting = false;
bool animacionTerminada = false;

void limpiarTeclas(){
    keys[LEFT] = false;
    keys[RIGHT]= false;
    keys[UP]= false;
    keys[DOWN]= false;
    keys[LEFT]= false;
    keys[SPACE]= false;
    keys[ENTER]= false;
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
	//ALLEGRO_FONT *font_menu = al_load_ttf_font("pirulen.ttf",50,0 );
	//ALLEGRO_FONT *font_menu2 = al_load_ttf_font("pirulen.ttf",30,0 );
	//ALLEGRO_FONT *font_descr = al_load_ttf_font("pirulen.ttf",20,0 );

    // VARIABLES DEL JUEGO ========================
    bg = al_load_bitmap("img/sp1.jpg");

    SpaceShip nave_jugador;
    Enemy enemies1[10];
    Enemy enemies2[8];
    Enemy enemies3[6];
    Enemy enemies4[4];
    Enemy jefe1[2];
    Bullet bullets[5];
    Explosion explosion;


    char vidas_char[2];
    char nivel_char[2];
    ALLEGRO_COLOR cJugar = GRAY_SELECTED;
    ALLEGRO_COLOR cAcerca = GRAY;
    ALLEGRO_COLOR cSalir = GRAY;
    short seleccion = JUGAR;
    bool keyPressed = false;
    bool animacion = true;
    bool animacion2 = true;
//    int ataque = 0;

    // INICIALIZAR OBJETOS====================
    InitShip(nave_jugador);
    InitBullet(bullets, NUM_BULLETS);
    InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
    InitEnemies(enemies2,speed,ENM2,NULL,false,8,50,180,WIDTH/2+100);
    InitEnemies(enemies3,speed,ENM3,NULL,false,6,100,140,WIDTH);
    InitEnemies(enemies4,speed,ENM4,NULL,false,4,150,100,(WIDTH/2)+100);
    InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);
    InitExplosion(explosion);

	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);


    //int animacion=1;
    int movimientos=1;
    int ataque;
	while(!done){


		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

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

                al_draw_text(font(50), GREEN, WIDTH/2 -140,HEIGHT/2 - 130, 0, "GALAGA");
                al_draw_text(font(30), cJugar, WIDTH/2 -70,HEIGHT/2 - 60, 0, "Jugar");
                al_draw_text(font(30), cAcerca, WIDTH/2 -90,HEIGHT/2 -20, 0, "Acerca");
                al_draw_text(font(30), cSalir, WIDTH/2 -65,HEIGHT/2 + 20, 0, "Salir");

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

                al_draw_text(font(30), GREEN, WIDTH/2 -130,HEIGHT/2 - 200, 0, "Descripcion: ");
                al_draw_text(font(20), GRAY2, 80 ,70, 0, "Dispara a las naves enemigas");
                al_draw_text(font(20), GRAY2, 50 ,90, 0, "antes de que lancen sus ataques");
                al_draw_text(font(20), GRAY2, 210 ,110, 0, "y te derriben!");

                al_draw_text(font(30), GREEN, WIDTH/2 -160,140, 0, "Instrucciones: ");
                al_draw_text(font(20), GRAY2, 40,170, 0, "Flechas del cursor para moverte");
                al_draw_text(font(20), GRAY2, 30,190, 0, "y barra espaciadora para disparar.");

                al_draw_text(font(30), GREEN, 110,250, 0, "DESARROLLADORES:");
                al_draw_text(font(30), GRAY_SELECTED, 85,280, 0, "Daniel Solis Mendez");
                al_draw_text(font(30), GRAY_SELECTED, 60,310, 0, "Melvin Elizondo Perez");

                al_draw_text(font_copy, GRAY, 70,440, 0, "BADSA Corp | Todos los derechos reservados 2014");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
		}else if (status == JUEGO){
            //==============================================
            //INPUT
            //==============================================
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
                    FireBullet(bullets, NUM_BULLETS, nave_jugador);
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

                if (!animacion2){
                    ataque = rand()%1000;
                    if (ataque>=0 and ataque<=5){
                        //BossAtack();
                    }else if (ataque>=6 and ataque<=100){
                        if (!enemyIsShooting){
                            printf("Escogiendo enemigo disparador\n");
                            ShooterEnemy(enemies2, 8);
                        }
                    }
                }

                //if (nivel ==2) printf("%d %d %d\n",animacion,animacion2,enemyIsShooting);

                CollideBullet(bullets,NUM_BULLETS,enemies1,NUM_ENEMIES);
                CollideBullet(bullets,NUM_BULLETS,enemies2,8);
                CollideBullet(bullets,NUM_BULLETS,enemies3,6);
                CollideBullet(bullets,NUM_BULLETS,enemies4,4);
                CollideBullet(bullets,NUM_BULLETS,jefe1,2);

                // ESTO DEBE SERVIR
                CollideBulletSpaceship(enemies2,8,nave_jugador);

            }

            //==============================================
            //RENDER
            //==============================================
            if(render && al_is_event_queue_empty(event_queue))
            {
                render = false;
                // INFORMACION DEL JUEGO Y BG
                if (vidas==0){status = GAME_OVER ; limpiarTeclas();}

                if (animacion){
                    movEnemies(enemies1,10,1);
                    movEnemies(enemies2, 8,1);
                    movEnemies(enemies3,6,1);
                    movEnemies(enemies4,4,1);
                    movEnemies(jefe1,2,1);
                    animacion = false;
                    //movimientos=1;
                }

                al_draw_bitmap(bg, 0, 0, 0);
                al_draw_bitmap(al_load_bitmap("img/zero.PNG"), 5,440,0);
                al_draw_text(font, al_map_rgb(255,255,255), 40,430, 0, "X");
                sprintf(vidas_char,"%d",vidas);
                al_draw_text(font, al_map_rgb(255,255,255), 60,430, 0, vidas_char);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 - 30, 0,ALLEGRO_ALIGN_CENTRE, "Score");
                al_draw_text(font, al_map_rgb(255,255,255), 50, 0,ALLEGRO_ALIGN_CENTRE, "Nivel");
                sprintf(nivel_char,"%d",nivel);
                al_draw_text(font, al_map_rgb(255,255,255), 100,0, 0, nivel_char);
                char vartext[10];
                sprintf(vartext,"%d",score);
                al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 + 40, 0, 0, vartext);

                // Dibujar nave
                al_draw_bitmap(nave_jugador.image, nave_jugador.x - nave_jugador.w / 2, nave_jugador.y - nave_jugador.h / 2, 0);
                int x = nave_jugador.x - nave_jugador.w/2;
                int y = nave_jugador.y - nave_jugador.h/2;
                //al_draw_rectangle(x+5, y ,x+nave_jugador.w-5,y+nave_jugador.h,GRAY,1);
                // Dibujar Balas
                DrawBullet(bullets, NUM_BULLETS);

                // Dibuja los enemigos.
                DrawEnemies(enemies1,10);
                DrawEnemies(enemies2,8);
                DrawEnemies(enemies3,6);
                DrawEnemies(enemies4,4);
                DrawEnemies(jefe1,2);

                if(!animacion){
                    if (animacion2) animacion2 = checkAnimationStatus(enemies1, enemies2, enemies3, enemies4, jefe1);
                    printf("%d\n",animacion2);
                    if (!animacion2){
                        DrawEnemyBullets(enemies2,8);
                    }
                }

                if (muertos==30){
                    nivel++;
                    muertos=0;
                    speed++;speed++;
                    InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
                    InitEnemies(enemies2,speed,ENM2,NULL,false,8,50,180,WIDTH/2+100);
                    InitEnemies(enemies3,speed,ENM3,NULL,false,6,100,140,WIDTH);
                    InitEnemies(enemies4,speed,ENM4,NULL,false,4,150,100,(WIDTH/2)+100);
                    InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);
                    animacion = true;
                    animacion2 = true;
                    enemyIsShooting = false;
                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));


                /*
                if (movimientos){
                    movimientos=0;
                  //  movCuadrado(enemies1,10);
                   movEnemies(jefe1,8,5);
                }
                */

            }

		}else if(status == GAME_OVER ){

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
                        InitEnemies(enemies1,speed,ENM1,NULL,false,10,0,220,WIDTH/2);
                        InitEnemies(enemies2,speed,ENM2,NULL,false,8,50,180,WIDTH/2+100);
                        InitEnemies(enemies3,speed,ENM3,NULL,false,6,100,140,WIDTH);
                        InitEnemies(enemies4,speed,ENM4,NULL,false,4,150,100,(WIDTH/2)+100);
                        InitEnemies(jefe1,speed,JEFE,JEFE2,true,2,200,60);

                        limpiarTeclas();
                        animacion = true;
                        animacion2 = true;
                        nivel = 1;
                        vidas = 3;
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

                al_draw_text(font(50), GREEN, WIDTH/2-200 ,HEIGHT/2-50 , 0, "GAME OVER");
                al_draw_text(font(15), cJugar, WIDTH/2-50 ,HEIGHT/2+10, 0, "Reintentar");
                al_draw_text(font(15), cSalir, WIDTH/2-20 ,HEIGHT/2+40 , 0, "Salir");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }

        }

        //al_flip_display();
        //al_clear_to_color(al_map_rgb(0,0,0));

    }

    for (int i =0;i<10;i++){
        al_destroy_bitmap(enemies1[i].image);
        al_destroy_bitmap(enemies2[i].image);
        al_destroy_bitmap(enemies3[i].image);
        al_destroy_bitmap(enemies4[i].image);
        al_destroy_bitmap(jefe1[i].image);
    }
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

