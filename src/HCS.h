#ifndef HCS_H
#define HCS_H

#include <SDL2/SDL.h>

#define AABB(pos1,pos2,size1,size2) (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y)

#define HCS_Gfx_Texture_color_mod(X,R,G,B)          SDL_SetTextureColorMod(X,R,G,B)
#define HCS_Gfx_Texture_alpha_mod(X,A)              SDL_SetTextureAlphaMod(X,Y)

#define HCS_Gfx_Texture_draw(X,Y,Z)                 SDL_RenderCopy(renderer,X,Y,&Z)
#define HCS_Gfx_Rectangle_fill(X)                   SDL_RenderFillRect(renderer,&X)
#define HCS_Gfx_Rectangle_draw(X)                   SDL_RenderDrawRect(renderer,&X)
#define HCS_Gfx_DrawColor_set(R,G,B,A)              SDL_SetRenderDrawColor(renderer,R,G,B,A)

#define HCS_Gfx_Image_load(X)                       IMG_Load(X)
#define HCS_Gfx_Surface_to_texture(X)               SDL_CreateTextureFromSurface(renderer,X)
#define HCS_Gfx_Texture_destroy(X)                  SDL_DestroyTexture(X)

#define HCS_Gfx()                                   HCS_Sprite_system(LSD_Delta_none); HCS_Clickable_system(LSD_Delta_none)

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

#define HCS_MAX_NAMES 256
#define HCS_MAX_ENTITIES 256

#define HCS_MAX_BODIES 200
#define HCS_MAX_SPRITES 200
#define HCS_MAX_CLICKABLES 200
#define HCS_MAX_COLLIDERS 200
#define HCS_MAX_MOVEMENTS 200
#define HCS_MAX_GRAVITIES 200
#define HCS_MAX_STATES 200
#define HCS_MAX_JUMPS 200
#define HCS_MAX_INPUTS 200

#define HCS_MAX_EVENTS 20000

typedef int HCS_Entity;

typedef enum {
    HCS_cName, HCS_cState, HCS_cBody, HCS_cMovement,HCS_cClickable, HCS_cSprite, HCS_cCollider, HCS_cGravity, HCS_cJump, HCS_cInput, HCS_NUM_COMPONENTS
} HCS_Component;

typedef enum {
    HCS_Draw_Background0, HCS_Draw_Background1, HCS_Draw_Background2, HCS_Draw_Sprite, HCS_Draw_Decal, HCS_Draw_Effect, HCS_Draw_Debug, HCS_Draw_Menu0, HCS_Draw_Menu1, HCS_Draw_Menu2, HCS_Draw_DebugUI, HCS_NUM_DRAWTYPES
} HCS_Drawtype;

typedef enum {
    HCS_Click_on, HCS_Click_off, HCS_Click_toggle
} HCS_Clicktype;

typedef enum {
    HCS_Trig_released, HCS_Trig_down
} HCS_Triggertype;

typedef enum {
    HCS_Col_Static, HCS_Col_Dynamic
} HCS_Collisiontype;

typedef struct
{
    unsigned char RED[8][8];
    unsigned char GRN[8][8];
    unsigned char BLU[8][8];
} HCS_Sprite_raw;

typedef struct
{
    HCS_Drawtype type;
    HCS_Gfx_Rectangle body;
    HCS_Gfx_Texture tex;
    HCS_Sprite_raw raw;
    char* path;
} HCS_Sprite;

typedef struct {
    char name[100];
} HCS_Name;

typedef struct {
    LSD_Vec2f pos;
    LSD_Vec2i size;
} HCS_Body;

typedef struct{
    bool active;
    LSD_Vec2d force;
} HCS_Gravity;

typedef struct{
    bool active;
} HCS_Input;

typedef struct{
    HCS_Clicktype type;
    HCS_Triggertype trigger;
    bool down;
    bool old_down;
    bool* action;
    bool use_func;
    int func_data;
    void (*func)(int);
} HCS_Clickable;


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
    LSD_Vec2f vel;
    LSD_Vec2f speed;
} HCS_Movement;

typedef struct{
    HCS_Collisiontype type;
    HCS_Body collider;
    HCS_Body offset;
    bool on_ground;
    bool last_on_ground;
    bool active;
} HCS_Collider;

typedef struct {
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

LSD_Vec2i HCS_Screen_size_get();
void HCS_Void_func();
void HCS_Init(char* argv[]);
void HCS_Deinit();
void HCS_Stop();
void HCS_Update(double delta);
void HCS_Gfx_Fullscreen_toggle();
void HCS_Main(int argc, char* argv[]);

void sprite_new(HCS_Sprite* spr, char* filename);

HCS_Entity HCS_Entity_create(char* n);
void HCS_Entity_remove(HCS_Entity ent);
void HCS_Entity_kill(HCS_Entity e);
void HCS_Entity_clear();

bool HCS_Entity_has_component(HCS_Entity ent, HCS_Component comp);
int HCS_Entity_get_component_id(HCS_Entity ent, HCS_Component comp);
HCS_Entity HCS_Entity_get_entity_id(int comp_list_number, HCS_Component component);
HCS_Entity HCS_Entity_get_by_name(char* n);

void HCS_Event_add(char* n,void (*sys));
void HCS_Event_remove(char* n);
void HCS_Event_run();

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

int HCS_Movement_add(HCS_Entity e, float sx, float sy);
HCS_Movement* HCS_Movement_get(HCS_Entity e);
void HCS_Movement_remove(HCS_Entity e);
void HCS_Movement_system();

void HCS_Drawable_translate_rect(HCS_Gfx_Rectangle* r);
//int HCS_Drawable_add(HCS_Entity e, char* n, float x, float y, bool text, HCS_Drawtype t);
//int HCS_Drawable_add(HCS_Entity e, char* n);
//void HCS_Drawable_add_quad(HCS_Entity e, int quad_x, int quad_y, int quad_w, int quad_h);
//void HCS_Drawable_add_rect(HCS_Entity e, int r, int g, int b, int a, bool fill);
//void HCS_Drawable_reset_unmanaged_with_text(HCS_Entity e, char* text);
//void HCS_Drawable_reset_unmanaged(HCS_Entity e, HCS_Gfx_Surface surf);
//HCS_Drawable* HCS_Drawable_get(HCS_Entity e);
//void HCS_Drawable_remove(HCS_Entity e);
//void HCS_Drawable_system();

int HCS_Sprite_add(HCS_Entity e, char* n, HCS_Drawtype t);
HCS_Sprite* HCS_Sprite_get(HCS_Entity e);
void HCS_Sprite_remove(HCS_Entity e);
void HCS_Sprite_system();

int HCS_Clickable_add(HCS_Entity e, bool* action, HCS_Clicktype type, HCS_Triggertype t);
HCS_Clickable* HCS_Clickable_get(HCS_Entity e);
void HCS_Clickable_remove(HCS_Entity e);

int HCS_Collider_add(HCS_Entity e, LSD_Vec2f pos_mod, LSD_Vec2i size_mod);
HCS_Collider* HCS_Collider_get(HCS_Entity e);
void HCS_Collider_remove(HCS_Entity e);
void HCS_Collider_system();


struct HCS_runData {

    HCS_Event HCS_Events[HCS_MAX_EVENTS];
    HCS_Entity HCS_Event_list[HCS_MAX_EVENTS];
    int HCS_Event_used;

    HCS_Entity HCS_Entities[HCS_MAX_ENTITIES][HCS_NUM_COMPONENTS];
    HCS_Entity HCS_Entity_list[HCS_MAX_ENTITIES];
    int HCS_Entity_used;
    
    HCS_Name HCS_Names[HCS_MAX_NAMES];
    HCS_Entity HCS_Name_list[HCS_MAX_NAMES];
    int HCS_Name_used;
    
    HCS_Body HCS_Bodies[HCS_MAX_BODIES];
    HCS_Entity HCS_Body_list[HCS_MAX_BODIES];
    int HCS_Body_used;
    
    HCS_Sprite HCS_Sprites[HCS_MAX_SPRITES];
    HCS_Entity HCS_Sprite_list[HCS_MAX_SPRITES];
    int HCS_Sprite_used;
    
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



#endif
