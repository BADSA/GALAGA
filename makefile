main: main.c
	gcc -std=c99 main.c objects.c -o galaga main.h objects.h -I/usr/include/allegro5 -L/usr/lib -lallegro -lm -lallegro_image -lallegro_ttf -lallegro_font
