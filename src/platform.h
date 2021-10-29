
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <soundio/soundio.h>
#include <SDL2/SDL_mixer.h>

LIB_PLATFORM_COLOR color = {255,255,255,255};
LIB_PLATFORM_COLOR std = {125,125,125,255};
LIB_PLATFORM_RECTANGLE WIN_SIZE;
LIB_PLATFORM_RECTANGLE OLD_WIN_SIZE;

struct all_keys
{
    int A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
    ,NUM_0,NUM_1,NUM_2,NUM_3,NUM_4,NUM_5,NUM_6,NUM_7,NUM_8,NUM_9
    ,SHIFT,STRG,ALT,ESCAPE,ENTER,SPACE
    ,M_LEFT,M_RIGHT;
};
static struct all_keys k;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font * font;

bool isDown(int key)
{
    if (key == -1000)
        return mouse_left;
    else if (key == -2000)
        return mouse_right;
    return keys[key];
}
bool isPressed(int key)
{
    if (key == -1000)
        return mouse_left && !last_mouse_left;
    else if (key == -2000)
        return mouse_right && !last_mouse_right;
    return keys[key] && !old_keys[key];
}
bool isReleased(int key)
{
    if (key == -1000)
        return !mouse_left && last_mouse_left;
    else if (key == -2000)
        return !mouse_right && last_mouse_right;
    return !keys[key] && old_keys[key];
}

void update_keys()
{
    int i;
    for (i = 0; i < 322;i++)
        old_keys[i] = keys[i];
    
    SDL_Event event;
    while( SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym < 322)
                    keys[event.key.keysym.sym] = true;
                break;
                
            case SDL_KEYUP:
                if (event.key.keysym.sym < 322)
                    keys[event.key.keysym.sym] = false;
                break;
        }
    }
    
    SDL_PumpEvents();
    
    unsigned int buttons;
    
    buttons = SDL_GetMouseState(&mouse_pos.x,&mouse_pos.y);
    
    last_mouse_right = mouse_right;
    last_mouse_left = mouse_left;
    
    if (buttons & SDL_BUTTON_LMASK)
        mouse_left = true;
    else
        mouse_left = false;
    
    if (buttons & SDL_BUTTON_RMASK)
        mouse_right = true;
    else
        mouse_right = false;
}
void init_keys(struct all_keys* k)
{
    
    (*k).A = SDLK_a;
    (*k).B = SDLK_b;
    (*k).C = SDLK_c;
    (*k).D = SDLK_d;
    (*k).E = SDLK_e;
    (*k).F = SDLK_f;
    (*k).G = SDLK_g;
    (*k).H = SDLK_h;
    (*k).I = SDLK_i;
    (*k).J = SDLK_j;
    (*k).K = SDLK_k;
    (*k).L = SDLK_l;
    (*k).M = SDLK_m;
    (*k).N = SDLK_n;
    (*k).O = SDLK_o;
    (*k).P = SDLK_p;
    (*k).Q = SDLK_q;
    (*k).R = SDLK_r;
    (*k).S = SDLK_s;
    (*k).T = SDLK_t;
    (*k).U = SDLK_u;
    (*k).V = SDLK_v;
    (*k).W = SDLK_w;
    (*k).X = SDLK_x;
    (*k).Y = SDLK_y;
    (*k).Z = SDLK_z;
    (*k).NUM_0 = SDLK_0;
    (*k).NUM_1 = SDLK_1;
    (*k).NUM_2 = SDLK_2;
    (*k).NUM_3 = SDLK_3;
    (*k).NUM_4 = SDLK_4;
    (*k).NUM_5 = SDLK_5;
    (*k).NUM_6 = SDLK_6;
    (*k).NUM_7 = SDLK_7;
    (*k).NUM_8 = SDLK_8;
    (*k).NUM_9 = SDLK_9;
    (*k).SHIFT = SDLK_LSHIFT;
    (*k).STRG = SDLK_LCTRL;
    (*k).ALT = SDLK_LALT;
    (*k).ESCAPE = SDLK_ESCAPE;
    (*k).ENTER = SDLK_RETURN;
    (*k).SPACE = SDLK_SPACE;
    (*k).M_LEFT = -1000;
    (*k).M_RIGHT = -2000;
}

void change_res(int* hr, int* vr)
{
    WIN_SIZE.w = *hr;
    WIN_SIZE.h = *vr;
    STRETCH_WIDTH = (double)WIN_SIZE.w / (double)WIN_SIZE.h;
    STRETCH_HEIGHT = (double)WIN_SIZE.h / (double)WIN_SIZE.w;
    SDL_SetWindowSize(window, WIN_SIZE.w  / 2,WIN_SIZE.h  / 2);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void toggle_fullscreen()
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer,std.r,std.g,std.b,std.a);
    
    fullscreen = !fullscreen;
    
    SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : fullscreen);
    SDL_GetWindowSize(window,&WIN_SIZE.w,&WIN_SIZE.h);
    
    if (!fullscreen)
    {
        WIN_SIZE.w = OLD_WIN_SIZE.w;
        WIN_SIZE.h = OLD_WIN_SIZE.h;
    }
    else
        DRAW_OFFSET = (WIN_SIZE.w - OLD_WIN_SIZE.w * ((double)WIN_SIZE.h / (double)OLD_WIN_SIZE.h)) / 2;
    
    change_res(&WIN_SIZE.w,&WIN_SIZE.h);
    SDL_SetWindowSize(window, WIN_SIZE.w ,WIN_SIZE.h );
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    SDL_Delay(500);
    frozen = true;
}

void init_SDL()
{
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
    window = SDL_CreateWindow("HCS-Projekt",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIN_SIZE.w ,WIN_SIZE.h ,SDL_WINDOW_METAL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer,std.r,std.g,std.b,std.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    WIN_SIZE.w = START_WIDTH;
    WIN_SIZE.h = START_HEIGHT;
    OLD_WIN_SIZE.w = WIN_SIZE.w;
    OLD_WIN_SIZE.h = WIN_SIZE.h;
    
    change_res(&WIN_SIZE.w,&WIN_SIZE.h);
    
    SDL_SetWindowSize(window, WIN_SIZE.w ,WIN_SIZE.h );
    
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    IMG_Init(IMG_INIT_JPG);
    TTF_Init();
    font = TTF_OpenFont("assets/font.ttf", 25);
    if (font == NULL)
    {
        printf("TTF konnte die Font-Datei nicht öffnen!\n");
        exit(1);
    }
    running = true;
}

void quit_SDL()
{
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
}
