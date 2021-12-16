/*
 TODO:
 -Endlich die verdammten Error-Messages einfügen!!                              DONE!             -> Siehe source
 -Fullscreen- Windowed- Schalter oder sowas implementieren                      DONE!             -> toggle_fullscreen()
 -Text-rendering mit SDL2_ttf                                                   DONE!             -> TEXT_TO_TEXTURE-Macro u. TEXT_TO_SURFACE-Macro
 -Interactable-Component (Tasten-Kombinationen, Mouseclicks, etc...)            NOT REALLY DONE!  -> Clickable-Component
 -Button-Komponent, aufbauend auf "drawable" und "interactable"                 SEMI DONE!        -> Clickable-Component
 -"Event-Queue": Function-Pointer Liste für "Event-Listeners"                   DONE!             -> check_events() und Source in ECS.h
 -"System-Liste": Function-Pointer Liste für "System-Functions"                 DONE!             -> run_systems() und Source in ECS.h
 -Layering für "drawable"                                                       DONE!             -> draw_type in Source in drawable.h
 -Das "SDL2-Zeug" vom Rest trennen, evtl. eigene Header-File                    DONE!             -> Source in SDL.h
 -Events doch mit Namen linken...                                               DONE!             -> War überrschend easy! Siehe Source in ECS.h
 -Clickables sind gerade broken! Maybe cAABB()                                  DONE!             -> WINDOW_SCALE vergessen...
 -rem_event() scheint auch broken zu sein...                                    GEFIXT!           -> event_list[e] und so...
 -Systeme und Events                                                            FERTIG!           -> Source in HCS.h
 -Die alten Komponenten rüber-porten:                                           FERTIG!           -> Source in HCS.h
 -Screen-Koordinaten irgenwie mappen                                            FERTIG!           -> data_util.h: Map-Funktion + Anwendung davon in Clickable.h und Drawable.h
 -Drawable_reset im Drawable-Struct speichern!                                  FERTIG!           -> Reset-Flag im Drawable-Struct
 -"Black-Bars" für Links und Rechts implementieren!                             FERTIG!           -> #define BLACK_BARS
 -Die Keys in Input "mappable" machen                                           FERTIG!           -> mit #defines oben in main.c
 -Die tick() -Funktion mit c-lib func machen für cross-compatability!           FERTIG!           -> Platform-Indepente tick()-Funktion
 -WINDOW_SCALE entfernen!                                                       FERTIG!           -> Was erwartest du jetzt eigentlich? Es ist nicht mehr da!
 -SDL2 Funktionen mehr abstrackt machen mit Wrappern und defines!               FERTIG!           -> LIB_PLATFORM ist jetzt überall
 -mouseDown mit isDown usw. mergen!                                             FERTIG!           -> Siehe in isDown, isPressed und isReleased
 -"Black-Bars" sind broken...                                                   GEFIXT!           -> if(fullscreen) hat gefehlt
 -Asset-Manager, der die Paths überprüft und so                                 FERTIG!           -> HCS_Asset_Manager() + HCS_Managed_assets in HCS.h
 -LSD_Log überarbeiten mit Format-String wie in printf()!                       FERTIG!           -> In LSD.h, greift aber immernoch auf LSD_Log_old() zurück!
 -LSD_Log mit LSD_Log_old mergen!                                               FERTIG!           -> In LSD.h, greift jetzt nicht mehr auf LSD_Log_old zurück!
 -Jump-Timer hinzufügen!                                                        FERTIG!           -> In jump.h
 -Collisions checken mit zu wenig overhead!                                     FERTIG!           -> In collision.h
 -Input überarbeiten, sodass nur noch UP,DOWN,LEFT,RIGHT,A,B,C,D existieren!    FERTIG!           -> in HCS.h
 -GFX fertig stellen, d.h. alles was mit Grafik und SDL2 zu tun hat wrappen!    FERTIG!           -> in HCS.h
 -Platform.h effektiv mit GFX ersetzen!                                         FERTIG!           -> HCS.h
 -Anständige Messages für LSD_Log() in den HCS Funktionen!                      FERTIG!           -> in HCS.h + Komponenten
 -Seperate Deltas für die einzelnen Threads!                                    FERTIG!           -> LSD_Delta in LSD
 -Seperate threads für System-Gruppen!                                          FERTIG!           -> HCS_System is no more!
 -Optional extra Collider-rect für Collider anstelle von Body                   FERTIG!           -> Collider-Offset in HCS_Collider-Struct
 -Clickables wieder hinzufügen!                                                 FERTIG!           -> HCS_Clickable und Clickable.h
 -Bessere veränderbare Collider!                                                FERTIG!           -> in HCS_Collider_add()
 -Sprite-Layering... again ... Ughhhhhhhhhhhhhhhh!                              FERTIG!           -> CMD-C CMD-V aus Drawable-Ruinen
 -Handy per QR-Code oder ID-Nummer verbinden und als Controller benutzen        YESS! YESSSSS!!!  -> Es ist vollbracht! In web-server-controller.h!
 -"Fake Cursor" aka Pointer, der mit Dpad oder Stick gesteuert wird!            FERTIG!           -> In Controller-Server!
 -Wrapper um den Webserver schreiben (LSD_Server?)                              FERTIG!           -> LSD_WebServer
 -Coole Dateiendung überlegen!                                                  FERTIG!           -> .hgx
 -Sprite Editor + Exporteur damit wir SDL(2)_image los werden!                  YESSSSS!          -> Es ist fertig!
 
 -Sprite-Data Cachen (Esentially Managed-Assets)!
 -Animationen für Drawables (Timer + Quad und States oder sowas kp...)
 -In Drawable nur sachen drawen, die auch auf dem Bildschirm sind!
 -Managed Asset für Sprites... Ughh...
 -Cap für Threads
 -Font als "System-Sprites" speichern um SDL(2)_ttf los zu werden!
 -"exit(X)" hinter allen LSD_Log(LSD_ltERROR,...) hinzufügen die es brauchen!
 -Irgendwie Sound hinkriegen (Möglichst mit SDL_Mixer!)!
 -Funktionen so benennen: HCS_EntityCreate("Ich");
 
 Very Nice To Haves™:
 
 */

#ifndef HCS_H
#define HCS_H

#include <SDL2/SDL.h>

#define HCS_CONTINUE()  return true
#define HCS_STOP()  return false
#define main HCS_Main

#define AABB(pos1,pos2,size1,size2) (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y)

#define HCS_Gfx_Texture_color_mod(X,R,G,B)          SDL_SetTextureColorMod(X,R,G,B)
#define HCS_Gfx_Texture_alpha_mod(X,A)              SDL_SetTextureAlphaMod(X,Y)

#define HCS_Gfx_Texture_draw(X,Y,Z)                 SDL_RenderCopy(renderer,X,Y,&Z)
#define HCS_Gfx_Rectangle_fill(X)                   SDL_RenderFillRect(renderer,&X)
#define HCS_Gfx_Rectangle_draw(X)                   SDL_RenderDrawRect(renderer,&X)
#define HCS_Gfx_DrawColor_set(R,G,B,A)              SDL_SetRenderDrawColor(renderer,R,G,B,A)

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

#define HCS_MAX_NAMES 2000
#define HCS_MAX_ENTITIES 2000

#define HCS_MAX_BODIES 2000
#define HCS_MAX_SPRITES 2000
#define HCS_MAX_CLICKABLES 2000
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
    unsigned char RED[16][16];
    unsigned char GRN[16][16];
    unsigned char BLU[16][16];
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

typedef struct {
    HCS_Sprite spr;
    char* path;
} HCS_Managed_Asset;

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
HCS_Sprite* HCS_Asset(char* path);
void HCS_Void_func();
void HCS_Init(char* argv[]);
void HCS_Deinit();
void HCS_Update(double delta);
void HCS_Gfx_Fullscreen_toggle();
int HCS_Main(int argc, char* argv[]);

void sprite_new(HCS_Sprite* spr, char* filename);

HCS_Entity HCS_Entity_create(char* n);
void HCS_Entity_remove(HCS_Entity ent);
void HCS_Entity_kill(HCS_Entity e);
void HCS_Entity_clear();

bool HCS_Entity_has_component(HCS_Entity ent, HCS_Component comp);
int HCS_Entity_get_component_id(HCS_Entity ent, HCS_Component comp);
HCS_Entity HCS_Entity_get_entity_id(int comp_list_number, HCS_Component component);
HCS_Entity HCS_Entity_get_by_name(char* n);
bool HCS_Entity_exist(char* n);

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
int HCS_Sprite_add(HCS_Entity e, char* n, HCS_Drawtype t);
void HCS_Sprite_use_text(HCS_Entity e, char* n, int length);
HCS_Sprite* HCS_Sprite_get(HCS_Entity e);
void HCS_Sprite_remove(HCS_Entity e);
void HCS_Sprite_system();

int HCS_Clickable_add(HCS_Entity e, bool* action, HCS_Clicktype type, HCS_Triggertype t);
void HCS_Clickable_add_func(HCS_Entity e,void(*func)(int),int func_data);
HCS_Clickable* HCS_Clickable_get(HCS_Entity e);
void HCS_Clickable_remove(HCS_Entity e);

int HCS_Collider_add(HCS_Entity e, LSD_Vec2f pos_mod, LSD_Vec2i size_mod);
HCS_Collider* HCS_Collider_get(HCS_Entity e);
void HCS_Collider_remove(HCS_Entity e);
void HCS_Collider_system();

struct HCS_runData {

    HCS_Managed_Asset HCS_Managed_Assets[HCS_MAX_SPRITES];
    int HCS_Managed_Asset_used;
    
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
