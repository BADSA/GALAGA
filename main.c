#include "main.h"
#include "objects.c"

bool keys[] = {false, false, false};
enum KEYS{LEFT, RIGHT, SPACE};

// GLOBAL VARIABLES=========================
int NUM_BULLETS = 5;
int NUM_ENEMIES = 10;
bool done = false;
bool render = false;
bool colision = false;
int score = 0;
int vidas = 3;

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

int main(void){
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
	ALLEGRO_FONT *font = al_load_ttf_font("Sarpanch-SemiBold.ttf",30,0 );

    // VARIABLES DEL JUEGO ========================
    bg = al_load_bitmap("img/sp1.jpg");

    SpaceShip nave_jugador;
    Enemy enemies[10];
    Bullet bullets[5];


    char vidas_char[2];


    // INICIALIZAR OBJETOS====================
    InitShip(nave_jugador);
    InitBullet(bullets, NUM_BULLETS);
    InitEnemies(enemies,NUM_ENEMIES);
    //DrawEnemies(enemies,NUM_ENEMIES);

	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

    //AnimacionEntrada(enemies,NUM_ENEMIES);
    printf("ya");
    int animacion=1;
	while(!done){

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

        // INFORMACION DEL JUEGO Y BG
        // DrawWindowStatus();
		al_draw_bitmap(bg, 0, 0, 0);
        al_draw_bitmap(nave_jugador.image, 5,440,0);
        al_draw_text(font, al_map_rgb(255,255,255), 40,430, 0, "X");
        sprintf(vidas_char,"%d",vidas);
        al_draw_text(font, al_map_rgb(255,255,255), 60,430, 0, vidas_char);
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 - 30, 0,ALLEGRO_ALIGN_CENTRE, "Score");
        char vartext[10];
        sprintf(vartext,"%d",score);
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2 + 40, 0, 0, vartext);


		//==============================================
		//INPUT
		//==============================================
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
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

            CollideBullet(bullets,NUM_BULLETS,enemies,NUM_ENEMIES);
		}

		//==============================================
		//RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			if (animacion==1){
                animacion=0;
                printf("si primero");
                AnimacionEntrada(enemies,NUM_ENEMIES);
			}

			render = false;

            // Dibujar nave
			al_draw_bitmap(nave_jugador.image, nave_jugador.x - nave_jugador.w / 2, nave_jugador.y - nave_jugador.h / 2, 0);
			// Dibujar Balas
			DrawBullet(bullets, NUM_BULLETS);

            // Dibuja los enemigos.
            DrawEnemies(enemies,NUM_ENEMIES);



            //if (colision)
                //al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "COLISION");

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}

	}


    for (int i =0;i<10;i++)

    al_destroy_bitmap(enemies[i].image);

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object

	return 0;
}
