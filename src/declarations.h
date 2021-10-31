#pragma once

#define HCS_MAX_NAMES 256
#define HCS_MAX_ENTITIES 256

#define HCS_MAX_BODIES 200
#define HCS_MAX_DRAWABLES 200
#define HCS_MAX_SOUNDABLES 200
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
    char* path;
    LIB_PLATFORM_TEXTURE tex;
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
    bool jump;
    bool sprint;
    bool action1;
    bool action2;
} HCS_State;

typedef struct {
    char* name;
    void (*event)(void);
} HCS_Event;

typedef struct {
    char* name;
    void (*system)(void);
} HCS_System;

typedef struct {
    vec2f pos;
    vec2i size;
    bool active;
} HCS_Camera_trigger;

typedef struct
{
    float x, y;
    char* following;
    bool follow_ent;
    bool follow_trigger;
} HCS_Camera;



LIB_PLATFORM_TEXTURE HCS_Asset_manager(char* path, HCS_Managed_assettype action);

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

void HCS_Event_add(char* n,void (*sys));
void HCS_Event_remove(char* n);
void HCS_Event_run();

void HCS_System_add(char* n,void (*sys));
void HCS_System_remove(char* n);
void HCS_System_run();

void HCS_Camera_follow_ent(char* name);
void HCS_Camera_follow_trigger(char* num);
void HCS_Camera_system();

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

void HCS_Drawable_translate_rect(LIB_PLATFORM_RECTANGLE* r);
int HCS_Drawable_add(HCS_Entity e, char* n, float x, float y, bool text, HCS_Drawtype t);
void HCS_Drawable_add_quad(HCS_Entity e, int quad_x, int quad_y, int quad_w, int quad_h);
void HCS_Drawable_add_rect(HCS_Entity e, int r, int g, int b, int a, bool fill);
void HCS_Drawable_reset_unmanaged_with_text(HCS_Entity e, char* text);
void HCS_Drawable_reset_unmanaged(HCS_Entity e, LIB_PLATFORM_SURFACE surf);
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
bool AABB(vec2f pos1, vec2f pos2, vec2i size1, vec2i size2);
