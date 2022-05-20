#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

SDL_Texture *loadImage(SDL_Renderer* rendu,const char *name);
SDL_Texture *loadText(SDL_Renderer* rendu,const char *text, SDL_Color coul, TTF_Font* font);