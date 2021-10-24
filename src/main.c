
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

#include "data_util.h"
#include "vec.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
//#include <soundio/soundio.h>
#include <SDL2/SDL_mixer.h>

#define START_WIDTH 1920
#define START_HEIGHT 1080
#define BLACK_BARS

double WORLD_TO_SCREEN_X = 1000;
double WORLD_TO_SCREEN_Y = 1000;
double DRAW_OFFSET = 0;
double STRETCH_WIDTH = 1;
double STRETCH_HEIGHT = 1;

/* !!!NUR FÜR UI-ELEMENTE!!! */
vec2i get_screen_size()
{
    vec2i size = {WORLD_TO_SCREEN_X,WORLD_TO_SCREEN_Y};
    return size;
}

bool keys[322] = {0};
bool old_keys[322] = {0};
vec2i mouse_pos;
bool mouse_left;
bool mouse_right;
bool last_mouse_left;
bool last_mouse_right;
bool show_sys_cursor = true;

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

struct keyboard
{
    int A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
    ,NUM_0,NUM_1,NUM_2,NUM_3,NUM_4,NUM_5,NUM_6,NUM_7,NUM_8,NUM_9
    ,SHIFT,STRG,ALT,ESCAPE,ENTER,SPACE
    ,M_LEFT,M_RIGHT;
};

struct keyboard k;

#define INPUT_UP        k.W
#define INPUT_DOWN      k.S
#define INPUT_LEFT      k.A
#define INPUT_RIGHT     k.D
#define INPUT_JUMP      k.SPACE
#define INPUT_ACTION_1  k.O
#define INPUT_ACTION_2  k.P
vec2f camera = {0,0};
bool fullscreen = false;
bool running;


#define LIB_PLATFORM_TEXT_TO_SURFACE(X)         TTF_RenderText_Solid(font,X,color)
#define LIB_PLATFORM_TEXT_TO_TEXTURE(X)         SDL_CreateTextureFromSurface(renderer,TTF_RenderText_Solid(font,X,color))

#define LIB_PLATFORM_INIT()                     init_SDL(); init_keys()
#define LIB_PLATFORM_DEINIT()                   quit_SDL()
#define LIB_PLATFORM_UPDATE()                   update_keys(); SDL_RenderPresent(renderer); SDL_RenderClear(renderer)

#define LIB_PLATFORM_COLOR_MOD(X,R,G,B)         SDL_SetTextureColorMod(X,R,G,B)
#define LIB_PLATFORM_ALPHA_MOD(X,Y)             SDL_SetTextureAlphaMod(X,Y)
#define LIB_PLATFORM_DRAW_TEXTURE(X,Y,Z)        SDL_RenderCopy(renderer,X,Y,Z)
#define LIB_PLATFORM_FILL_RECT(X)               SDL_RenderFillRect(renderer,X)
#define LIB_PLATFORM_DRAW_RECT(X)               SDL_RenderDrawRect(renderer,X)
#define LIB_PLATFORM_SET_DRAW_COLOR(R,G,B,A)    SDL_SetRenderDrawColor(renderer,R,G,B,A)

#define LIB_PLATFORM_LOAD_IMG(X)                IMG_Load(X)
#define LIB_PLATFORM_SURFACE_TO_TEXTURE(X)      SDL_CreateTextureFromSurface(renderer,X)
#define LIB_PLATFORM_TEXTURE_DESTROY(X)         SDL_DestroyTexture(X)

#define LIB_PLATFORM_COLOR                      SDL_Color
#define LIB_PLATFORM_RECTANGLE                  SDL_Rect
#define LIB_PLATFORM_SURFACE                    SDL_Surface*
#define LIB_PLATFORM_TEXTURE                    SDL_Texture*

LIB_PLATFORM_COLOR color = {255,255,255,255};
LIB_PLATFORM_COLOR std = {125,125,125,255};
LIB_PLATFORM_RECTANGLE WIN_SIZE;
LIB_PLATFORM_RECTANGLE OLD_WIN_SIZE;

double delta = 0.0f;
bool frozen = false;

struct timeval begin, end;
void tick()
{
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    delta = seconds + microseconds*1e-6;
    gettimeofday(&begin, 0);
    if (frozen)
    {
        frozen = false;
        delta = 0.0f;
    }
}

// Diese Funktionen müssen von den jeweiligen Wrappern vervollständigt werden!
void update_keys();
void init_keys();
void change_res(int* vr, int* hr);
void toggle_fullscreen();
void draw_black_bars();

#include "SDL_Wrapper.h"

#include "LSD.h"
#include "HCS.h"

/*
 TODO:
 -Endlich die verdammten Error-Messages einfügen!!                      DONE!             -> Siehe source
 -Fullscreen- Windowed- Schalter oder sowas implementieren              DONE!             -> toggle_fullscreen()
 -Text-rendering mit SDL2_ttf                                           DONE!             -> TEXT_TO_TEXTURE-Macro u. TEXT_TO_SURFACE-Macro
 -Interactable-Component (Tasten-Kombinationen, Mouseclicks, etc...)    NOT REALLY DONE!  -> Clickable-Component
 -Button-Komponent, aufbauend auf "drawable" und "interactable"         SEMI DONE!        -> Clickable-Component
 -"Event-Queue": Function-Pointer Liste für "Event-Listeners"           DONE!             -> check_events() und Source in ECS.h
 -"System-Liste": Function-Pointer Liste für "System-Functions"         DONE!             -> run_systems() und Source in ECS.h
 -Layering für "drawable"                                               DONE!             -> draw_type in Source in drawable.h
 -Ein Debug-Fenster (PiP) mit Daten zu den Entities                     NICHT NOTWENDIG   -> F.U.
 -Output-Log beim Exit generieren                                       NICHT NOTWENDIG   -> F.U.  <- Wäre eventuell schon Cool...
 -Das "SDL2-Zeug" vom Rest trennen, evtl. eigene Header-File            DONE!             -> Source in SDL.h
 -Szenen hinzufügen                                                     DILLIGAF?         -> Nein.
 -Systems und Events mit Namen Linken, nicht mit Enums!                 Will ich das?     -> Zu viel aufwand, bräuchte 2x Name- s, _list und _fill!
 -Events doch mit Namen linken...                                       DONE!             -> War überrschend easy! Siehe Source in ECS.h
 -Clickables sind gerade broken! Maybe cAABB()                          DONE!             -> WINDOW_SCALE vergessen...
 -rem_event() scheint auch broken zu sein...                            GEFIXT!           -> event_list[e] und so...
 -Systeme und Events                                                    FERTIG!           -> Source in HCS.h
 -Die alten Komponenten rüber-porten:                                   FERTIG!           -> Source in HCS.h
 -Screen-Koordinaten irgenwie mappen                                    FERTIG!           -> data_util.h: Map-Funktion + Anwendung davon in Clickable.h und Drawable.h
 -Drawable_reset im Drawable-Struct speichern!                          FERTIG!           -> Reset-Flag im Drawable-Struct
 -"Black-Bars" für Links und Rechts implementieren!                     FERTIG!           -> #define BLACK_BARS
 -Die Keys in Input "mappable" machen                                   FERTIG!           -> mit #defines oben in main.c
 -Die tick() -Funktion mit c-lib func machen für cross-compatability!   FERTIG!           -> Platform-Indepente tick()-Funktion
 -WINDOW_SCALE entfernen!                                               FERTIG!           -> Was erwartest du jetzt eigentlich? Es ist nicht mehr da!
 -SDL2 Funktionen mehr abstrackt machen mit Wrappern und defines!       FERTIG!           -> LIB_PLATFORM ist jetzt überall
 -mouseDown mit isDown usw. mergen!                                     FERTIG!           -> Siehe in isDown, isPressed und isReleased
 -"Black-Bars" für Oben und Unten implementieren!                       NICHT NOTWENDIG!  -> Windowed-Mode wird immer slimmer als Fullscreen sein!
 -Irgendwie Sound hinkriegen (Möglichst mit SDL_Mixer!)!                NICHT WIRKLICH... -> Es funktioniert zwar, es muss aber nochmal kräftig überarbeitet werden!
 -Das runData-Struct serialisieren und wieder deserialisieren!          FÜRS ERSTE AUF EIS GELEGT! (Evtl. später mit Data Desk arbeiten!)
 */


void fullscreen_event()
{
    if (isPressed(k.F))
    {
        toggle_fullscreen();
    }
}

void SFX_event()
{
    HCS_State* d = HCS_State_get(HCS_Entity_get_by_name("Player"));
    if (d->jump && d->on_ground)
        HCS_Soundable_get(HCS_Entity_get_by_name("Player"))->should_be_playing = true;
}

void camera_event()
{
    camera.x -= (camera.x - (HCS_Body_get(HCS_Entity_get_by_name("Player"))->size.x / 2 + HCS_Body_get(HCS_Entity_get_by_name("Player"))->pos.x - (get_screen_size().x * STRETCH_WIDTH) / 2)) * 0.04f;
    camera.y -= (camera.y - (HCS_Body_get(HCS_Entity_get_by_name("Player"))->size.y / 2 + HCS_Body_get(HCS_Entity_get_by_name("Player"))->pos.y - get_screen_size().y / 2)) * 0.04f;
}

void start_event()
{
    HCS_Entity e = HCS_Entity_create("Player");
    
    HCS_State_add(e);
    HCS_Body_add(e,get_screen_size().x / 2 - 300,get_screen_size().y / 2 - 150,300,150);
    HCS_Movement_add(e,3000,0);
    HCS_Collider_add(e);
    HCS_Gravity_add(e,0,1500);
    HCS_Jump_add(e,3800,true,0);
    HCS_Input_add(e);
    
    HCS_Soundable_add(e,"assets/jmp.wav",HCS_Sound_Effect);
    HCS_Drawable_add(e,"Beenden?",300,300,true,HCS_Draw_Sprite);
    HCS_Clickable_add(e,&running,HCS_Click_off);
    HCS_Drawable_add_rect(e,100,100,100,125,true);
    
    e = HCS_Entity_create("Door");
    
    HCS_Body_add(e,800,0,500,600);
    HCS_Drawable_add(e,"assets/door.png",0,0,false,HCS_Draw_Background1);
    
    e = HCS_Entity_create("Platform");
    
    HCS_Body_add(e,800,500,525,100);
    HCS_Collider_add(e);
    HCS_Drawable_add(e,"assets/test.png",0,0,false,HCS_Draw_Decal);
    
    e = HCS_Entity_create("Ground");
    
    HCS_Body_add(e,0,get_screen_size().y - 200,get_screen_size().x,200);
    HCS_Collider_add(e);
    HCS_Drawable_add(e,"assets/sand.jpg",0,0,false,HCS_Draw_Background0);
    
    e = HCS_Entity_create("Tree");
    
    HCS_Body_add(e,200,50,300,800);
    HCS_Drawable_add(e,"assets/tree.png",0,0,false,HCS_Draw_Background1);
    
    HCS_Event_add("Camera",camera_event);
    HCS_Event_add("Jump",SFX_event);
    HCS_Event_remove("Start");
}

int main(int argc, char* argv[])
{
    gettimeofday(&begin, 0);
    
#define CHARS_TIL_ROOT_OF_PROJ 6
    char path_save[1024];
    char cwd[1024];
    char* p;
    if(!(p = strrchr(argv[0], '/')))
        getcwd(cwd, sizeof(cwd));
    else
    {
        *p = '\0';
        getcwd(path_save, sizeof(path_save));
        chdir(argv[0]);
        getcwd(cwd, sizeof(cwd));
        chdir(path_save);
    }
    cwd[strlen(cwd)- CHARS_TIL_ROOT_OF_PROJ] = '\0';
    chdir(cwd);
    
    LIB_PLATFORM_INIT();
    HCS_Init();
    
    LSD_Log_level_set(LSD_llALL);
    
    HCS_Event_add("Fullscreen",fullscreen_event);
    HCS_Event_add("Start",start_event);
    
    HCS_System_add("Clickable",HCS_Clickable_system);
    HCS_System_add("Drawable",HCS_Drawable_system);
    HCS_System_add("Soundable",HCS_Soundable_system);
    HCS_System_add("Collider",HCS_Collider_system);
    HCS_System_add("Movement",HCS_Movement_system);
    HCS_System_add("Gravity",HCS_Gravity_system);
    HCS_System_add("Jump",HCS_Jump_system);
    HCS_System_add("Input",HCS_Input_system);
    
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    Mix_Init(MIX_INIT_OGG);
    printf("%s\n",Mix_GetError());
    while(running)
    {
        HCS_System_run();
        HCS_Event_run();
        draw_black_bars();
        LIB_PLATFORM_UPDATE();
        tick();
    }
    
    HCS_Deinit();
    Mix_Quit();
    LIB_PLATFORM_DEINIT();
    exit(2);
}
