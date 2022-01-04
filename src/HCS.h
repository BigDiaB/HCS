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
 -Collisions checken mit wenig overhead!                                        FERTIG!           -> In collision.h
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
 -Managed Asset für Sprites... Ughh...                                          FERTIG!           -> HCS_Asset()- Funktion und HCS_Managed_Asset struct
 -Sprite-Data Cachen (Esentially Managed-Assets)!                               FERTIG!           -> HCS_Asset()- Funktion und HCS_Managed_Asset struct
 -Font als "System-Sprites" speichern um SDL(2)_ttf los zu werden!              FERTIG!           -> HCS_Sprite_use_text() und die Buchstaben-Abbilder in Assets/Font
 -"exit(X)" hinter allen LSD_Log(LSD_ltERROR,...) hinzufügen die es brauchen!   FERTIG!           -> Halt da wo es nötig ist
 -Clickables blockieren, wenn ein body darüber liegt                            SEMI FERTIG!      -> Mit HCS_Clickable_active_toggle() kann jetzt die funltionalität getoggled werden
 -Namen der Sprite-Editor-Funktionen ändern (evtl. mit SPREDIT-Präfix)          FERTIG!           -> Halt überall in test.c
 -In Drawable nur sachen drawen, die auch auf dem Bildschirm sind!              FERTIG!           -> Im HCS_Sprite_system() mit AABB()s um zu gucken, ob die Sprites mit dem Bildschirm überlappen!
 -AABB() mit nach LSD bringen!                                                  FERTIG!           -> LSD_Math_AABB() als #define damit man alle Datentypen benutzen kann (aka int, float, double, etc)
 -Alle Komponenten sollten mit einer Funktion komplett initialisiert werden können! FERTIG!       -> Z.B. in HCS.h
 -Alle Helper-Funcs müssen in einem Array sein! (Colliders,Clickables)          FERTIG!           -> HCS_Collider(bzw. Clickable)_callbacks in runData
 -Names zu Tags umbenennen und sie mit dem HCS_Entity_data-Struct mergen!       FERTIG!           -> Entity-Data-Struct
 -Entities aus Dateien laden (evtl mit eigener Script-Sprache (.hcscript))!     FERTIG!           -> HCS_Script_load()
 -Kamera: 16/9 Viewport immer zentriert in der Mitte (Non-UI)                   FERTIG!           -> In sprite.h werden jetzt alle Nicht-UI-sprites entsprechend verschoben!
 
 -Draw-Primitives (Rechtecke mit Farbe!) als Komponent (HCS_Drawable ?)!
 -Datenaustausch über Sockets (Kein WebServer!) in LSD einbauen!
 -Clipping für Bodys (HCS_Body_align_left oder sowas)!
 -Horst-Script in LSD einbauen (LSD_Script ?)!
 -Collider- und Clickable-Callbacks mit Horst-Script-Funktionen ersetzen!
 -Entities in Scripts Dumpen!
 -Animationen für Drawables (Timer + Quad und States oder sowas kp...)!
 -Cycle-Speed-Cap für Threads!
 -Irgendwie Sound hinkriegen (Möglichst ohne SDL_Mixer (Siehe GitHub Star!)!)!
 
 Very Nice To Haves™:
 
 */

#ifndef HCS_H
#define HCS_H

#define HCS_DEBUG  

#include <SDL2/SDL.h>

#define HCS_CONTINUE()  return true
#define HCS_STOP()  return false
#define main HCS_Main

#define HCS_INPUT_UP                                SDLK_w
#define HCS_INPUT_DOWN                              SDLK_s
#define HCS_INPUT_LEFT                              SDLK_a
#define HCS_INPUT_RIGHT                             SDLK_d
#define HCS_INPUT_A                                 SDLK_c
#define HCS_INPUT_B                                 SDLK_SPACE

#define HCS_MAX_ENTITIES 1000

#define HCS_MAX_ASSETS 300
#define HCS_MAX_SPRITES 300
#define HCS_MAX_CLICKABLES 300
#define HCS_MAX_COLLIDERS 300

#define HCS_MAX_TEXTINPUT 300

#define HCS_MAX_BODIES 500
#define HCS_MAX_MOVEMENTS 500
#define HCS_MAX_GRAVITIES 500
#define HCS_MAX_STATES 500
#define HCS_MAX_JUMPS 500
#define HCS_MAX_INPUTS 500

#define HCS_MAX_EVENTS 200

typedef enum {
    HCS_cState, HCS_cBody, HCS_cMovement,HCS_cClickable, HCS_cSprite, HCS_cCollider, HCS_cGravity, HCS_cJump, HCS_cInput, HCS_NUM_COMPONENTS
} HCS_Component;

typedef enum {
    HCS_Click_on, HCS_Click_off, HCS_Click_toggle
} HCS_Clicktype;

typedef enum {
    HCS_Trig_released, HCS_Trig_down
} HCS_Triggertype;

typedef enum {
    HCS_Col_Static, HCS_Col_Dynamic
} HCS_Collisiontype;

typedef int HCS_Entity;

typedef struct
{
    char* tag;
    int comp_ids[HCS_NUM_COMPONENTS];
} HCS_Entity_data;

typedef struct
{
    unsigned char RED[16][16];
    unsigned char GRN[16][16];
    unsigned char BLU[16][16];
} HCS_Sprite_raw;

typedef struct
{
    int layer;
    SDL_Rect body;
    SDL_Texture* tex;
    HCS_Sprite_raw raw;
    bool use_text;
    char* path;
} HCS_Sprite;

typedef struct {
    char* name;
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

typedef struct {
    char* name;
    void (*func)(int);
} HCS_Clickable_helper_func;

typedef struct{
    HCS_Clicktype type;
    HCS_Triggertype trigger;
    bool active;
    bool down;
    bool old_down;
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

typedef struct {
    char* name;
    void (*func)(HCS_Entity, HCS_Entity);
} HCS_Collider_helper_func;

typedef struct{
    HCS_Collisiontype type;
    HCS_Body collider;
    HCS_Body offset;
    bool on_ground;
    bool last_on_ground;
    bool active;
    void (*func)(HCS_Entity,HCS_Entity);
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

typedef struct {
    bool down;
    bool last_down;
    bool pressed;
    bool released;
} HCS_Button;

LSD_Vec2i HCS_Screen_size_get();
HCS_Sprite* HCS_Asset(char* path);
void HCS_Void_func();
void HCS_Init(char* argv[]);
void HCS_Deinit();
void HCS_Stop();
void HCS_Update(double delta);
void HCS_Gfx_Fullscreen_toggle();
int HCS_Main(int argc, char* argv[]);
void HCS_Script_load(char* filename);
void HCS_Error(char* title, char* desc);

LSD_Vec2f* HCS_Camera_get();
LSD_Vec2i* HCS_Cursor_position_get();
HCS_Button* HCS_Cursor_button_get();
HCS_Button* HCS_Button_A_get();
HCS_Button* HCS_Button_B_get();
bool HCS_running_get();

SDL_Renderer* HCS_Gfx_renderer_get();
SDL_Window* HCS_Gfx_window_get();
LSD_Vec2d HCS_Gfx_stretch_get();

char* HCS_Text_input_get();
int* HCS_Text_input_length_get();

void sprite_new(HCS_Sprite* spr, char* filename);

HCS_Entity HCS_Entity_create(char* n);
void HCS_Entity_remove(HCS_Entity ent);
void HCS_Entity_kill(HCS_Entity e);
void HCS_Entity_clear();
HCS_Entity_data* HCS_Entity_data_get(HCS_Entity e);
#define HCS_Entity_tag_get(e) HCS_Entity_data_get(e)->tag

bool HCS_Entity_has_component(HCS_Entity ent, HCS_Component comp);
int HCS_Entity_get_component_id(HCS_Entity ent, HCS_Component comp);
HCS_Entity HCS_Entity_get_entity_id(int comp_list_number, HCS_Component component);
HCS_Entity HCS_Entity_get_by_name(char* n);
bool HCS_Entity_exist(char* n);

void HCS_Event_add(char* n,void (*sys));
void HCS_Event_remove(char* n);
void HCS_Event_run();

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

void HCS_Drawable_translate_rect(SDL_Rect* r);
int HCS_Sprite_add(HCS_Entity e, char* n, int t, bool use_text);
HCS_Sprite* HCS_Sprite_get(HCS_Entity e);
void HCS_Sprite_remove(HCS_Entity e);
void HCS_Sprite_system();

void HCS_Clickable_callback_list(void (*func)(int),char* n);
int HCS_Clickable_add(HCS_Entity e, HCS_Clicktype type, HCS_Triggertype t,char* n,int func_data);
HCS_Clickable* HCS_Clickable_get(HCS_Entity e);
void HCS_Clickable_remove(HCS_Entity e);
void HCS_Clickable_system();

void HCS_Collider_callback_list(void (*func)(HCS_Entity,HCS_Entity),char* n);
void HCS_Collider_STD_callback(HCS_Entity this, HCS_Entity other);
int HCS_Collider_add(HCS_Entity e, LSD_Vec2f pos_mod, LSD_Vec2i size_mod, char* n);
HCS_Collider* HCS_Collider_get(HCS_Entity e);
void HCS_Collider_remove(HCS_Entity e);
void HCS_Collider_system();


struct HCS_runData {

    int HCS_Entity_ids_used;
    int HCS_Entity_ids[HCS_MAX_ENTITIES];

    SDL_Color color;
    SDL_Color std;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Rect WIN_SIZE;

    double WORLD_TO_SCREEN_X;
    double WORLD_TO_SCREEN_Y;

    double DRAW_OFFSET;
    double STRETCH_WIDTH;
    double STRETCH_HEIGHT;

    bool fullscreen;

    bool HCS_running;
    bool HCS_Input_disabled;

    char HCS_Text_input[HCS_MAX_TEXTINPUT];
    int HCS_Text_input_size;

    HCS_Button HCS_Input_A;
    HCS_Button HCS_Input_B;
    LSD_Vec2i HCS_Input_Pad;

    HCS_Button HCS_Input_Cursor_button;
    LSD_Vec2i HCS_Input_Cursor_position;

    LSD_Vec2f HCS_Gfx_Camera;

    HCS_Entity_data HCS_Entities[HCS_MAX_ENTITIES];
    int HCS_Entity_list[HCS_MAX_ENTITIES];
    int HCS_Entity_used;

    HCS_Managed_Asset HCS_Managed_Assets[HCS_MAX_ASSETS];
    int HCS_Managed_Asset_used;
    
    HCS_Event HCS_Events[HCS_MAX_EVENTS];
    int HCS_Event_list[HCS_MAX_EVENTS];
    int HCS_Event_used;
    
    HCS_Body HCS_Bodies[HCS_MAX_BODIES];
    int HCS_Body_list[HCS_MAX_BODIES];
    int HCS_Body_used;
    
    HCS_Sprite HCS_Sprites[HCS_MAX_SPRITES];
    int HCS_Sprite_list[HCS_MAX_SPRITES];
    int HCS_Sprite_used;
    
    HCS_Clickable HCS_Clickables[HCS_MAX_CLICKABLES];
    int HCS_Clickable_list[HCS_MAX_CLICKABLES];
    int HCS_Clickable_used;
    HCS_Clickable_helper_func HCS_Clickable_callbacks[HCS_MAX_CLICKABLES];
    int HCS_Clickable_callback_used;
    
    HCS_Collider HCS_Colliders[HCS_MAX_COLLIDERS];
    int HCS_Collider_list[HCS_MAX_COLLIDERS];
    int HCS_Collider_used;
    HCS_Collider_helper_func HCS_Collider_callbacks[HCS_MAX_COLLIDERS];
    int HCS_Collider_callback_used;
    
    HCS_Gravity HCS_Gravities[HCS_MAX_GRAVITIES];
    int HCS_Gravity_list[HCS_MAX_GRAVITIES];
    int HCS_Gravity_used;
    
    HCS_Movement HCS_Movements[HCS_MAX_MOVEMENTS];
    int HCS_Movement_list[HCS_MAX_MOVEMENTS];
    int HCS_Movement_used;
    
    HCS_State HCS_States[HCS_MAX_STATES];
    int HCS_State_list[HCS_MAX_STATES];
    int HCS_State_used;
    
    HCS_Jump HCS_Jumps[HCS_MAX_JUMPS];
    int HCS_Jump_list[HCS_MAX_JUMPS];
    int HCS_Jump_used;
    
    HCS_Input HCS_Inputs[HCS_MAX_INPUTS];
    int HCS_Input_list[HCS_MAX_INPUTS];
    int HCS_Input_used;
};

#endif
