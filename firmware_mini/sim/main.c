#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"


int leds[LED_HEIGHT][LED_WIDTH][3];
int interval;
tick_fun tick_fp;

void setLedXY(uint8_t x, uint8_t y, uint8_t red,uint8_t green, uint8_t blue) {
	assert(x < LED_WIDTH);
	assert(y < LED_HEIGHT);
	leds[y][x][0] = red;
	leds[y][x][1] = green;
	leds[y][x][2] = blue;
}

void registerAnimation(tick_fun tick, uint16_t t, uint16_t ignore)
{
	tick_fp = tick;

	assert(t > 0);
	// 122Hz / tick
	interval = 1000000 / 122 * t;
}


int main(int argc, char *argv[]) {
	srand(time(NULL));

	SDL_Surface* screen = SDL_SetVideoMode(LED_WIDTH * ZOOM + ZOOM / 15, LED_HEIGHT * ZOOM + ZOOM / 15,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	SDL_Rect rect = { 0, 0, LED_WIDTH*ZOOM+(ZOOM/15), LED_HEIGHT*ZOOM+(ZOOM/15) };
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0x20,0x20,0x20));


	int running = 1;
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						default: break;
					}
				default: break;
			}
		}

		running &= !tick_fp();

		int x, y;
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {

				SDL_Rect rect = { ZOOM*x+(ZOOM/15), ZOOM*(LED_HEIGHT - y - 1)+(ZOOM/15), ZOOM-(ZOOM/15), ZOOM-(ZOOM/15) };
				SDL_FillRect(
					screen, 
					&rect, 
					SDL_MapRGB(screen->format, leds[y][x][0],leds[y][x][1],leds[y][x][2])
				);

			}
		}
		SDL_Flip(screen);

		usleep(interval);
	}

	SDL_Quit();
	return 0;
}

