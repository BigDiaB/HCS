
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font * font;

bool RGX_Input_up;
bool RGX_Input_down;
bool RGX_Input_left;
bool RGX_Input_right;
bool RGX_Input_A;
bool RGX_Input_B;
bool RGX_Mouse_clicked;

bool RGX_Input_last_up;
bool RGX_Input_last_down;
bool RGX_Input_last_left;
bool RGX_Input_last_right;
bool RGX_Input_last_A;
bool RGX_Input_last_B;
bool RGX_Mouse_last_clicked;

vec2f RGX_Camera;
vec2i RGX_Mouse_pos;

typedef SDL_Texture* RGX_Texture;
typedef SDL_Surface* RGX_Surface;
typedef SDL_Rect     RGX_Rectangle;
typedef SDL_Color    RGX_Color;


#define RGX_Text_to_surface(X)                  TTF_RenderText_Solid(font,X,color)
#define RGX_Text_to_texture(X)                  SDL_CreateTextureFromSurface(renderer,TTF_RenderText_Solid(font,X,color))

#define RGX_Texture_color_mod(X,R,G,B)          SDL_SetTextureColorMod(X,R,G,B)
#define RGX_Texture_alpha_mod(X,A)              SDL_SetTextureAlphaMod(X,Y)

#define RGX_Texture_draw(X,Y,Z)                 SDL_RenderCopy(renderer,X,Y,Z)
#define RGX_Rectangle_fill(X)                   SDL_RenderFillRect(renderer,&X)
#define RGX_Rectangle_draw(X)                   SDL_RenderDrawRect(renderer,&X)
#define RGX_DrawColor_set(R,G,B,A)              SDL_SetRenderDrawColor(renderer,R,G,B,A)

#define RGX_Image_load(X)                       IMG_Load(X)
#define RGX_Surface_to_texture(X)               SDL_CreateTextureFromSurface(renderer,X)
#define RGX_Texture_destroy(X)                  SDL_DestroyTexture(X)

#define INPUT_UP                                SDLK_w
#define INPUT_DOWN                              SDLK_s
#define INPUT_LEFT                              SDLK_a
#define INPUT_RIGHT                             SDLK_d
#define INPUT_A                                 SDLK_c
#define INPUT_B                                 SDLK_SPACE

double WORLD_TO_SCREEN_X = 1000;
double WORLD_TO_SCREEN_Y = 1000;

/* !!!NUR FÜR UI-ELEMENTE!!! */
vec2i get_screen_size()
{
    vec2i size = {WORLD_TO_SCREEN_X,WORLD_TO_SCREEN_Y};
    return size;
}

double DRAW_OFFSET = 0;
double STRETCH_WIDTH = 1;
double STRETCH_HEIGHT = 1;

bool fullscreen = false;
bool running;

RGX_Color color = {255,255,255,255};
RGX_Color std = {125,125,125,255};
RGX_Rectangle WIN_SIZE;
RGX_Rectangle OLD_WIN_SIZE;

void RGX_Fullscreen_toggle();
void RGX_Draw_black_bars();

bool isPressed(char* key);
bool isReleased(char* key);
bool isDown(char* key);

bool RGX_Init();
bool RGX_Update();
bool RGX_Deinit();

