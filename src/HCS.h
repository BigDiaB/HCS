#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define HCS_Gfx_Text_to_surface(X)                  TTF_RenderText_Solid(font,X,color)
#define HCS_Gfx_Text_to_texture(X)                  SDL_CreateTextureFromSurface(renderer,TTF_RenderText_Solid(font,X,color))

#define HCS_Gfx_Texture_color_mod(X,R,G,B)          SDL_SetTextureColorMod(X,R,G,B)
#define HCS_Gfx_Texture_alpha_mod(X,A)              SDL_SetTextureAlphaMod(X,Y)

#define HCS_Gfx_Texture_draw(X,Y,Z)                 SDL_RenderCopy(renderer,X,Y,Z)
#define HCS_Gfx_Rectangle_fill(X)                   SDL_RenderFillRect(renderer,&X)
#define HCS_Gfx_Rectangle_draw(X)                   SDL_RenderDrawRect(renderer,&X)
#define HCS_Gfx_DrawColor_set(R,G,B,A)              SDL_SetRenderDrawColor(renderer,R,G,B,A)

#define HCS_Gfx_Image_load(X)                       IMG_Load(X)
#define HCS_Gfx_Surface_to_texture(X)               SDL_CreateTextureFromSurface(renderer,X)
#define HCS_Gfx_Texture_destroy(X)                  SDL_DestroyTexture(X)

#define HCS_INPUT_UP                                SDLK_w
#define HCS_INPUT_DOWN                              SDLK_s
#define HCS_INPUT_LEFT                              SDLK_a
#define HCS_INPUT_RIGHT                             SDLK_d
#define HCS_INPUT_A                                 SDLK_c
#define HCS_INPUT_B                                 SDLK_SPACE

typedef SDL_Texture* HCS_Gfx_Texture;
typedef SDL_Surface* HCS_Gfx_Surface;
typedef SDL_Rect     HCS_Gfx_Rectangle;
typedef SDL_Color    HCS_Gfx_Color;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font * font;

bool HCS_Gfx_Input_up;
bool HCS_Gfx_Input_down;
bool HCS_Gfx_Input_left;
bool HCS_Gfx_Input_right;
bool HCS_Gfx_Input_A;
bool HCS_Gfx_Input_B;
bool HCS_Gfx_Mouse_clicked;

bool HCS_Gfx_Input_last_up;
bool HCS_Gfx_Input_last_down;
bool HCS_Gfx_Input_last_left;
bool HCS_Gfx_Input_last_right;
bool HCS_Gfx_Input_last_A;
bool HCS_Gfx_Input_last_B;
bool HCS_Gfx_Mouse_last_clicked;

vec2f HCS_Gfx_Camera;
vec2i HCS_Gfx_Mouse_pos;

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

HCS_Gfx_Color color = {255,255,255,255};
HCS_Gfx_Color std = {125,125,125,255};
HCS_Gfx_Rectangle WIN_SIZE;
HCS_Gfx_Rectangle OLD_WIN_SIZE;

#define HCS_MAX_NAMES 256
#define HCS_MAX_ENTITIES 256

#define HCS_MAX_BODIES 200
#define HCS_MAX_DRAWABLES 200
#define HCS_MAX_CLICKABLES 200
#define HCS_MAX_COLLIDERS 200
#define HCS_MAX_MOVEMENTS 200
#define HCS_MAX_GRAVITIES 200
#define HCS_MAX_STATES 200
#define HCS_MAX_JUMPS 200
#define HCS_MAX_INPUTS 200

#define HCS_MAX_EVENTS 20
#define HCS_MAX_SYSTEMS 10

typedef int HCS_Entity;

void HCS_Void_func()
{
    LSD_Log(LSD_ltERROR,"Du solltest das hier niemals sehen können!");
    return;
}

typedef enum {
    HCS_cName, HCS_cState, HCS_cBody, HCS_cDrawable, HCS_cClickable, HCS_cMovement, HCS_cCollider, HCS_cGravity, HCS_cJump, HCS_cInput, HCS_NUM_COMPONENTS
} HCS_Component;

typedef enum {
    HCS_Click_toggle, HCS_Click_on, HCS_Click_off
} HCS_Clicktype;

typedef enum {
    HCS_Col_Static, HCS_Col_Dynamic
} HCS_Collisiontype;

typedef enum {
    HCS_Draw_Background0, HCS_Draw_Background1, HCS_Draw_Background2, HCS_Draw_Sprite, HCS_Draw_Decal, HCS_Draw_Effect, HCS_Draw_Debug, HCS_Draw_Menu0, HCS_Draw_Menu1, HCS_Draw_Menu2, HCS_Draw_DebugUI, num_draw_types
} HCS_Drawtype;

typedef enum {
    HCS_AAdd, HCS_ARemove
} HCS_Managed_assettype;

typedef enum {
    HCS_Sound_Music, HCS_Sound_Effect
} HCS_Soundtype;

typedef struct {
    char* name;
} HCS_Name;

typedef struct {
    vec2f pos;
    vec2i size;
} HCS_Body;

typedef struct {
    SDL_Texture* tex;
    SDL_Color color;
    vec2i size;
    vec2f pos;
    vec2i quad_pos;
    vec2i quad_size;
    HCS_Drawtype type;
    char* path;
    bool managed;
    bool use_path_as_image_text;
    bool draw;
    bool use_quad;
    bool draw_rect;
    bool fill_rect;
} HCS_Drawable;

typedef struct {
    int used;
    char* path;
    HCS_Gfx_Texture tex;
} HCS_Managed_asset;

typedef struct{
    bool active;
    vec2d force;
} HCS_Gravity;

typedef struct{
    bool active;
} HCS_Input;

typedef struct{
    double jump_ground_timer;
    double jump_time;
    double jump_timer;
    double strength;
    bool can_jump;
    bool active;
    bool needs_ground;
} HCS_Jump;

typedef struct {
    vec2f vel;
    vec2f speed;
} HCS_Movement;

typedef struct{
    bool* action;
    bool old_down;
    bool down;
    bool hot;
    HCS_Clicktype type;
} HCS_Clickable;

typedef struct{
    HCS_Collisiontype type;
    bool on_ground;
    bool last_on_ground;
    bool active;
} HCS_Collider;

typedef struct{
    bool on_ground;
    bool up;
    bool down;
    bool left;
    bool right;
    bool A;
    bool B;
} HCS_State;

typedef struct {
    char* name;
    void (*event)(void);
} HCS_Event;

typedef struct {
    char* name;
    void (*system)(void);
} HCS_System;




HCS_Gfx_Texture HCS_Asset_manager(char* path, HCS_Managed_assettype action);

void HCS_Init();
void HCS_Deinit();
void HCS_Stop();

HCS_Entity HCS_Entity_create(char* n);
void HCS_Entity_remove(HCS_Entity ent);
void HCS_Entity_kill(HCS_Entity e);

bool HCS_Entity_has_component(HCS_Entity ent, HCS_Component comp);
int HCS_Entity_get_component_id(HCS_Entity ent, HCS_Component comp);
HCS_Entity HCS_Entity_get_entity_id(int comp_list_number, HCS_Component component);
HCS_Entity HCS_Entity_get_by_name(char* n);

void HCS_Event_init();
void HCS_Event_add(char* n,void (*sys));
void HCS_Event_remove(char* n);
void HCS_Event_run();

void HCS_System_add(char* n,void (*sys));
void HCS_System_remove(char* n);
void HCS_System_run();

void HCS_Name_add(HCS_Entity ent, char* n);
HCS_Name* HCS_Name_get(HCS_Entity ent);
void HCS_Name_remove(HCS_Entity ent);

int HCS_State_add(HCS_Entity e);
HCS_State* HCS_State_get(HCS_Entity e);
void HCS_State_remove(HCS_Entity e);

int HCS_Input_add(HCS_Entity e);
HCS_Input* HCS_Input_get(HCS_Entity e);
void HCS_Input_remove(HCS_Entity e);
void HCS_Input_system();

int HCS_Jump_add(HCS_Entity e, double n, bool m, double l);
HCS_Jump* HCS_Jump_get(HCS_Entity e);
void HCS_Jump_remove(HCS_Entity e);
void HCS_Jump_system();

int HCS_Gravity_add(HCS_Entity e, double n, double m);
HCS_Gravity* HCS_Gravity_get(HCS_Entity e);
void HCS_Gravity_remove(HCS_Entity e);
void HCS_Gravity_system();

int HCS_Body_add(HCS_Entity e, float x, float y, int w, int h);
HCS_Body* HCS_Body_get(HCS_Entity e);
void HCS_Body_remove(HCS_Entity e);

int HCS_Clickable_add(HCS_Entity e, bool* action, HCS_Clicktype type);
HCS_Clickable* HCS_Clickable_get(HCS_Entity e);
void HCS_Clickable_remove(HCS_Entity e);
void HCS_Clickable_system();

void HCS_Drawable_translate_rect(HCS_Gfx_Rectangle* r);
int HCS_Drawable_add(HCS_Entity e, char* n, float x, float y, bool text, HCS_Drawtype t);
void HCS_Drawable_add_quad(HCS_Entity e, int quad_x, int quad_y, int quad_w, int quad_h);
void HCS_Drawable_add_rect(HCS_Entity e, int r, int g, int b, int a, bool fill);
void HCS_Drawable_reset_unmanaged_with_text(HCS_Entity e, char* text);
void HCS_Drawable_reset_unmanaged(HCS_Entity e, HCS_Gfx_Surface surf);
HCS_Drawable* HCS_Drawable_get(HCS_Entity e);
void HCS_Drawable_remove(HCS_Entity e);
void HCS_Drawable_system();

int HCS_Movement_add(HCS_Entity e, float sx, float sy);
HCS_Movement* HCS_Movement_get(HCS_Entity e);
void HCS_Movement_remove(HCS_Entity e);
void HCS_Movement_system();

int HCS_Collider_add(HCS_Entity e);
HCS_Collider* HCS_Collider_get(HCS_Entity e);
void HCS_Collider_remove(HCS_Entity e);
void HCS_Collider_system();

void HCS_Gfx_Fullscreen_toggle();
void HCS_Gfx_Draw_black_bars();

bool isPressed(char* key);
bool isReleased(char* key);
bool isDown(char* key);

bool HCS_Gfx_Init();
bool HCS_Gfx_Update();
bool HCS_Gfx_Deinit();


struct HCS_Data {

    HCS_Event HCS_Events[HCS_MAX_EVENTS];
    HCS_Entity HCS_Event_list[HCS_MAX_EVENTS];
    int HCS_Event_used;
    
    HCS_System HCS_Systems[HCS_MAX_SYSTEMS];
    HCS_Entity HCS_System_list[HCS_MAX_SYSTEMS];
    int HCS_System_used;




    HCS_Entity HCS_Entities[HCS_MAX_ENTITIES][HCS_NUM_COMPONENTS];
    HCS_Entity HCS_Entity_list[HCS_MAX_ENTITIES];
    int HCS_Entity_used;
    
    HCS_Name HCS_Names[HCS_MAX_NAMES];
    HCS_Entity HCS_Name_list[HCS_MAX_NAMES];
    int HCS_Name_used;
    
    HCS_Body HCS_Bodies[HCS_MAX_BODIES];
    HCS_Entity HCS_Body_list[HCS_MAX_BODIES];
    int HCS_Body_used;
    
    HCS_Drawable HCS_Drawables[HCS_MAX_DRAWABLES];
    HCS_Entity HCS_Drawable_list[HCS_MAX_DRAWABLES];
    int HCS_Drawable_used;

    HCS_Managed_asset HCS_Managed_assets[HCS_MAX_DRAWABLES];
    HCS_Entity HCS_Managed_asset_list[HCS_MAX_DRAWABLES];
    int HCS_Managed_asset_used;
    
    HCS_Clickable HCS_Clickables[HCS_MAX_CLICKABLES];
    HCS_Entity HCS_Clickable_list[HCS_MAX_CLICKABLES];
    int HCS_Clickable_used;
    
    HCS_Collider HCS_Colliders[HCS_MAX_COLLIDERS];
    HCS_Entity HCS_Collider_list[HCS_MAX_COLLIDERS];
    int HCS_Collider_used;
    
    HCS_Gravity HCS_Gravities[HCS_MAX_GRAVITIES];
    HCS_Entity HCS_Gravity_list[HCS_MAX_GRAVITIES];
    int HCS_Gravity_used;
    
    HCS_Movement HCS_Movements[HCS_MAX_MOVEMENTS];
    HCS_Entity HCS_Movement_list[HCS_MAX_MOVEMENTS];
    int HCS_Movement_used;
    
    HCS_State HCS_States[HCS_MAX_STATES];
    HCS_Entity HCS_State_list[HCS_MAX_STATES];
    int HCS_State_used;
    
    HCS_Jump HCS_Jumps[HCS_MAX_JUMPS];
    HCS_Entity HCS_Jump_list[HCS_MAX_JUMPS];
    int HCS_Jump_used;
    
    HCS_Input HCS_Inputs[HCS_MAX_INPUTS];
    HCS_Entity HCS_Input_list[HCS_MAX_INPUTS];
    int HCS_Input_used;
};

struct HCS_Data* runData;

void HCS_Gfx_Fullscreen_toggle()
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
    
    STRETCH_WIDTH = (double)WIN_SIZE.w / (double)WIN_SIZE.h;
    STRETCH_HEIGHT = (double)WIN_SIZE.h / (double)WIN_SIZE.w;
    SDL_SetWindowSize(window, WIN_SIZE.w  / 2,WIN_SIZE.h  / 2);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowSize(window, WIN_SIZE.w ,WIN_SIZE.h );
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    SDL_Delay(500);
    frozen = true;
}
#ifdef BLACK_BARS
void HCS_Gfx_Draw_black_bars()
{
    if (!fullscreen)
        return;
    HCS_Gfx_Rectangle r;
    HCS_Gfx_Rectangle r2;
    
    r.x = 0;
    r.y = 0;
    r.w = DRAW_OFFSET;
    r.h = WIN_SIZE.h;
    
    r2.x = WIN_SIZE.w - DRAW_OFFSET;
    r2.y = 0;
    r2.w = DRAW_OFFSET;
    r2.h = WIN_SIZE.h;
    
    LIB_PLATFORM_SET_DRAW_COLOR(0,0,0,255);
    
    LIB_PLATFORM_FILL_RECT(&r);
    LIB_PLATFORM_FILL_RECT(&r2);
    
    LIB_PLATFORM_DRAW_RECT(&r);
    LIB_PLATFORM_DRAW_RECT(&r2);
    
    LIB_PLATFORM_SET_DRAW_COLOR(std.r,std.g,std.b,std.a);
}
#endif

bool isPressed(char* key)
{
    if (0 == strcmp("up",key))
        return HCS_Gfx_Input_up && !HCS_Gfx_Input_last_up;
    if (0 == strcmp("down",key))
        return HCS_Gfx_Input_down && !HCS_Gfx_Input_last_down;
    if (0 == strcmp("left",key))
        return HCS_Gfx_Input_left && !HCS_Gfx_Input_last_left;
    if (0 == strcmp("right",key))
        return HCS_Gfx_Input_right && !HCS_Gfx_Input_last_right;
    if (0 == strcmp("A",key))
        return HCS_Gfx_Input_A && !HCS_Gfx_Input_last_A;
    if (0 == strcmp("B",key))
        return HCS_Gfx_Input_B && !HCS_Gfx_Input_last_B;
    if (0 == strcmp("mouse",key))
        return HCS_Gfx_Mouse_clicked && !HCS_Gfx_Mouse_last_clicked;
    LSD_Log(LSD_ltERROR,"Key: %s stimmt mit keinem der Vorgegebenen überein!",key);
    return false;
}
bool isReleased(char* key)
{
    if (0 == strcmp("up",key))
        return !HCS_Gfx_Input_up && HCS_Gfx_Input_last_up;
    if (0 == strcmp("down",key))
        return !HCS_Gfx_Input_down && HCS_Gfx_Input_last_down;
    if (0 == strcmp("left",key))
        return !HCS_Gfx_Input_left && HCS_Gfx_Input_last_left;
    if (0 == strcmp("right",key))
        return !HCS_Gfx_Input_right && HCS_Gfx_Input_last_right;
    if (0 == strcmp("A",key))
        return !HCS_Gfx_Input_A && HCS_Gfx_Input_last_A;
    if (0 == strcmp("B",key))
        return !HCS_Gfx_Input_B && HCS_Gfx_Input_last_B;
    if (0 == strcmp("mouse",key))
        return !HCS_Gfx_Mouse_clicked && HCS_Gfx_Mouse_last_clicked;
    LSD_Log(LSD_ltERROR,"Key: %s stimmt mit keinem der Vorgegebenen überein!",key);
    return false;
}
bool isDown(char* key)
{
    if (0 == strcmp("up",key))
        return HCS_Gfx_Input_up;
    if (0 == strcmp("down",key))
        return HCS_Gfx_Input_down;
    if (0 == strcmp("left",key))
        return HCS_Gfx_Input_left;
    if (0 == strcmp("right",key))
        return HCS_Gfx_Input_right;
    if (0 == strcmp("A",key))
        return HCS_Gfx_Input_A;
    if (0 == strcmp("B",key))
        return HCS_Gfx_Input_B;
    if (0 == strcmp("mouse",key))
        return HCS_Gfx_Mouse_clicked;
    LSD_Log(LSD_ltERROR,"Key: %s stimmt mit keinem der Vorgegebenen überein!",key);
    return false;
}

bool HCS_Gfx_update()
{
    SDL_Event event;
    while( SDL_PollEvent(&event))
        switch(event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case HCS_INPUT_UP:
                HCS_Gfx_Input_up = true;
                break;
                case HCS_INPUT_DOWN:
                HCS_Gfx_Input_down = true;
                break;
                case HCS_INPUT_LEFT:
                HCS_Gfx_Input_left = true;
                break;
                case HCS_INPUT_RIGHT:
                HCS_Gfx_Input_right = true;
                break;
                case HCS_INPUT_A:
                HCS_Gfx_Input_A = true;
                break;
                case HCS_INPUT_B:
                HCS_Gfx_Input_B = true;
                break;
            }
            break;
            case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
                case HCS_INPUT_UP:
                HCS_Gfx_Input_up = false;
                break;
                case HCS_INPUT_DOWN:
                HCS_Gfx_Input_down = false;
                break;
                case HCS_INPUT_LEFT:
                HCS_Gfx_Input_left = false;
                break;
                case HCS_INPUT_RIGHT:
                HCS_Gfx_Input_right = false;
                break;
                case HCS_INPUT_A:
                HCS_Gfx_Input_A = false;
                break;
                case HCS_INPUT_B:
                HCS_Gfx_Input_B = false;
                break;
            }
            break;
        }
    
    HCS_Gfx_Mouse_last_clicked = HCS_Gfx_Mouse_clicked;
    
    if (SDL_GetMouseState(&HCS_Gfx_Mouse_pos.x,&HCS_Gfx_Mouse_pos.y) & SDL_BUTTON_LMASK)
        HCS_Gfx_Mouse_clicked = true;
    else
        HCS_Gfx_Mouse_clicked = false;

#ifdef BLACK_BARS
    HCS_Gfx_Draw_black_bars();
#endif
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    return true;
}


HCS_Gfx_Texture HCS_Asset_manager(char* path, HCS_Managed_assettype action)
{
    switch (action)
    {
        case HCS_AAdd:
        {
            int j;
            for (j = 0; j < runData->HCS_Managed_asset_used; j++)
            {
                int i = runData->HCS_Managed_asset_list[j];
                if (0 == strcmp(runData->HCS_Managed_assets[i].path,path))
                {
                    runData->HCS_Managed_assets[i].used++;
                    return runData->HCS_Managed_assets[i].tex;
                }
            } 
            {
                    int id = get_unused_id_from_blacklist(runData->HCS_Managed_asset_list, &runData->HCS_Managed_asset_used, HCS_MAX_DRAWABLES);
                    runData->HCS_Managed_asset_list[runData->HCS_Managed_asset_used] = id;
                    runData->HCS_Managed_assets[id].tex = HCS_Gfx_Surface_to_texture(HCS_Gfx_Image_load(path));
                    runData->HCS_Managed_assets[id].path = path;
                    runData->HCS_Managed_assets[id].used = 1;
                    return runData->HCS_Managed_assets[id].tex;
                }
            break;
        }
        case HCS_ARemove:
        {
        int j;
        for (j = 0; j < runData->HCS_Managed_asset_used; j++)
        {
            int i = runData->HCS_Managed_asset_list[j];
            if (0 == strcmp(runData->HCS_Managed_assets[i].path,path))
            {
                runData->HCS_Managed_assets[i].used--;
                if (runData->HCS_Managed_assets[i].used <= 0)
                {
                    HCS_Gfx_Texture_destroy(runData->HCS_Managed_assets[i].tex);
                    remove_element_from_array(runData->HCS_Managed_asset_list,&runData->HCS_Managed_asset_used,&j);
                }
                return NULL;
            }
        }
        LSD_Log(LSD_ltERROR,"Asset konnte nicht entfernt werden, weil es nicht nach Namen gefunden wurde!: %s",path);
        break;
        }

        default:
        break;
    }
    return NULL;
}


/*
 HCS-Funktionen für das Laden / Entladen und Initialisieren / Deinitialisieren
 */

void HCS_Init(char* argv[])
{
    prepare_path(argv);
    gettimeofday(&begin, 0);
    
    runData = malloc(sizeof(struct HCS_Data));
    struct HCS_Data zero = {0};
    *runData = zero;
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    SDL_GetDisplayBounds(0,&WIN_SIZE);
    
    WIN_SIZE.w *= 0.75;
    WIN_SIZE.h *= 0.75;
    
    window = SDL_CreateWindow("HCS-Projekt",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIN_SIZE.w ,WIN_SIZE.h ,SDL_WINDOW_METAL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer,std.r,std.g,std.b,std.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    STRETCH_WIDTH = (double)WIN_SIZE.w / (double)WIN_SIZE.h;
    STRETCH_HEIGHT = (double)WIN_SIZE.h / (double)WIN_SIZE.w;
    SDL_SetWindowSize(window, WIN_SIZE.w,WIN_SIZE.h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    IMG_Init(IMG_INIT_JPG);
    TTF_Init();
    font = TTF_OpenFont("assets/font.ttf", 25);
    if (font == NULL)
    {
        char buf[1024];
        printf("TTF konnte die Font-Datei nicht öffnen!:\n%s\n",getcwd(buf,1024));
        exit(1);
    }
    running = true;
    
    HCS_System_add("SYS_Delta_timer",tick);
    HCS_System_add("SYS_Graphics_Update",HCS_Gfx_update);
    HCS_Event_init();

}


void HCS_Deinit()
{
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    free(runData);
}


void HCS_Stop()
{
    running = false;
}


/*
 Entity-Such Funktionen
 */

bool HCS_Entity_has_component(HCS_Entity ent, HCS_Component comp)
{
    return (-1 != runData->HCS_Entities[ent][comp]);
}

int HCS_Entity_get_component_id(HCS_Entity ent, HCS_Component comp)
{
    return runData->HCS_Entities[ent][comp];
}

HCS_Entity HCS_Entity_get_entity_id(int comp_list_number, HCS_Component component)
{
    int j;
    for (j = 0; j < runData->HCS_Entity_used; j++)
    {
        int i = runData->HCS_Entity_list[j];
        if (runData->HCS_Entities[i][component] == comp_list_number)
            return i;
    }
    LSD_Log(LSD_ltERROR,"Entity konnte nicht nach Komponenten gefunden werden!");
    return 0; //Das hier wird niemals vorkommen, weil LSD bei Errors automatisch exitet!
}

HCS_Entity HCS_Entity_get_by_name(char* n)
{
    int j;
    for (j = 0; j < runData->HCS_Name_used; j++)
    {
        int i = runData->HCS_Name_list[j];
        if (0 == strcmp(runData->HCS_Names[i].name,n))
            return HCS_Entity_get_entity_id(i,HCS_cName);
    }
    LSD_Log(LSD_ltMESSAGE,n);
    LSD_Log(LSD_ltERROR,"Konnte Entity nicht nach Namen finden!");
    return 0; //Das hier wird niemals vorkommen, weil LSD bei Errors automatisch exitet!
}

void HCS_Entity_kill(HCS_Entity e)
{
    if (HCS_Entity_has_component(e,HCS_cInput))
        HCS_Input_remove(e);
    if (HCS_Entity_has_component(e,HCS_cGravity))
        HCS_Gravity_remove(e);
    if (HCS_Entity_has_component(e,HCS_cJump))
        HCS_Jump_remove(e);
    if (HCS_Entity_has_component(e,HCS_cCollider))
        HCS_Collider_remove(e);
    if (HCS_Entity_has_component(e,HCS_cMovement))
        HCS_Movement_remove(e);
    if (HCS_Entity_has_component(e,HCS_cClickable))
        HCS_Clickable_remove(e);
    if (HCS_Entity_has_component(e,HCS_cDrawable))
        HCS_Drawable_remove(e);
    if (HCS_Entity_has_component(e,HCS_cBody))
        HCS_Body_remove(e);
    if (HCS_Entity_has_component(e,HCS_cState))
        HCS_State_remove(e);
    HCS_Name_remove(e);
    HCS_Entity_remove(e);
}

void HCS_Entity_clear()
{
    while(runData->HCS_Entity_used)
        HCS_Entity_kill(runData->HCS_Entity_list[0]);
}
/*
 Events und Systeme:
    Gekoppelt mit Namen, für leichtes Hinzufügen und Entfernen
 */

void HCS_Event_add(char* n,void (*sys))
{
    int id = get_unused_id_from_blacklist(runData->HCS_Event_list, &runData->HCS_Event_used, HCS_MAX_EVENTS);
    runData->HCS_Event_list[runData->HCS_Event_used] = id;
    runData->HCS_Events[id].event = sys;
    runData->HCS_Events[id].name = n;
    LSD_Log(LSD_ltMESSAGE,"Event %s erfolgreich hinzugefügt!",n);
}

void HCS_Event_remove(char* n)
{
    int j;
    for (j = 0; j < runData->HCS_Event_used; j++)
    {
        int i = runData->HCS_Event_list[j];
        if (0 == strcmp(runData->HCS_Events[i].name,n))
        {
            runData->HCS_Events[i].event = HCS_Void_func;
            remove_element_from_array(runData->HCS_Event_list,&runData->HCS_Event_used,&j);
            LSD_Log(LSD_ltMESSAGE,"Event %s erfolgreich entfernt!",n);
            return;
        }
    }
    LSD_Log(LSD_ltMESSAGE,n);
    LSD_Log(LSD_ltERROR,"Event konnte nicht entfernt werden, Name wurde nicht gefunden");
}

void HCS_Event_run()
{
    int j;
    for (j = 0; j < runData->HCS_Event_used; j++)
    {
        int i = runData->HCS_Event_list[j];
        runData->HCS_Events[i].event();
    }
}

void HCS_System_add(char* n,void (*sys))
{
    int id = get_unused_id_from_blacklist(runData->HCS_System_list, &runData->HCS_System_used, HCS_MAX_SYSTEMS);
    runData->HCS_System_list[runData->HCS_System_used] = id;
    runData->HCS_Systems[id].system = sys;
    runData->HCS_Systems[id].name = n;
    LSD_Log(LSD_ltMESSAGE,"System %s erfolgreich hinzugefügt!",n);
}

void HCS_System_remove(char* n)
{
    int j;
    for (j = 0; j < runData->HCS_System_used; j++)
    {
        int i = runData->HCS_System_list[j];
        if (strcmp(runData->HCS_Systems[i].name,n))
        {
            runData->HCS_Systems[i].system = HCS_Void_func;
            remove_element_from_array(runData->HCS_System_list,&runData->HCS_System_used,&j);
            LSD_Log(LSD_ltMESSAGE,"System %s erfolgreich entfernt!",n);
            return;
        }
    }
    LSD_Log(LSD_ltMESSAGE,n);
    LSD_Log(LSD_ltERROR,"System konnte nicht entfernt werden, Name wurde nicht gefunden");
}

void HCS_System_run()
{
    int j;
    for (j = 0; j < runData->HCS_System_used; j++)
    {
        int i = runData->HCS_System_list[j];
        runData->HCS_Systems[i].system();
    }
}

/*
 Namen-Funktionen, für das Hinzufügen, Suchen und Entfernen von Namen
 */

void HCS_Name_add(HCS_Entity ent, char* n)
{
    int index = get_unused_id_from_blacklist(runData->HCS_Name_list,&runData->HCS_Name_used, HCS_MAX_NAMES);
    runData->HCS_Entities[ent][HCS_cName] = index;
    runData->HCS_Names[index].name = n;
}

HCS_Name* HCS_Name_get(HCS_Entity ent)
{
    return &runData->HCS_Names[HCS_Entity_get_component_id(ent,HCS_cName)];
}

void HCS_Name_remove(HCS_Entity ent)
{
    remove_element_from_array(runData->HCS_Name_list, &runData->HCS_Name_used, &runData->HCS_Entities[ent][HCS_cName]);
}

HCS_Entity HCS_Entity_create(char* n)
{
    HCS_Entity ent = get_unused_id_from_blacklist(runData->HCS_Entity_list, &runData->HCS_Entity_used, HCS_MAX_ENTITIES);
    int i;
    for (i = 0; i < HCS_NUM_COMPONENTS; i++)
        runData->HCS_Entities[ent][i] = -1;
    HCS_Name_add(ent,n);
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s erfolgreicht erstellt!",ent,n);
    return ent;
}

void HCS_Entity_remove(HCS_Entity ent)
{
    int index;
    int i;
    for (i = 0; i < runData->HCS_Entity_used; i++)
        if (runData->HCS_Entity_list[i] == ent)
            index = i;
    remove_element_from_array(runData->HCS_Entity_list, &runData->HCS_Entity_used, &index);
    LSD_Log(LSD_ltMESSAGE,"Entity %d erfolgreicht entfernt!",ent);
}

#include "components/drawable.h"
#include "components/clickable.h"

#include "components/input.h"

#include "components/states.h"
#include "components/body.h"

#include "components/movement.h"
#include "components/collision.h"
#include "components/jump.h"
#include "components/gravity.h"
