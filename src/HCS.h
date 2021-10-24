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

typedef enum {
    HCS_cName,HCS_cDrawable, HCS_cSoundable, HCS_cBody, HCS_cClickable, HCS_cCollider, HCS_cGravity, HCS_cMovement, HCS_cState, HCS_cJump, HCS_cInput, HCS_NUM_COMPONENTS
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
    bool use_path_as_image_text;
    bool draw;
    bool use_quad;
    bool draw_rect;
    bool fill_rect;
} HCS_Drawable;

typedef struct {
    bool should_be_playing;
    Mix_Chunk* SFX;
    Mix_Music* music;
    char* path;
    HCS_Soundtype type;
} HCS_Soundable;

typedef struct{
    bool active;
    vec2d force;
} HCS_Gravity;

typedef struct{
    bool active;
} HCS_Input;

typedef struct{
    double jump_time;
    double jump_timer;
    double strength;
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

struct HCS_Data {
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
    
    HCS_Soundable HCS_Soundables[HCS_MAX_SOUNDABLES];
    HCS_Entity HCS_Soundable_list[HCS_MAX_SOUNDABLES];
    int HCS_Soundable_used;
    
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
    
    
    
    
    HCS_Event HCS_Events[HCS_MAX_EVENTS];
    HCS_Entity HCS_Event_list[HCS_MAX_EVENTS];
    int HCS_Event_used;
    
    HCS_System HCS_Systems[HCS_MAX_SYSTEMS];
    HCS_Entity HCS_System_list[HCS_MAX_SYSTEMS];
    int HCS_System_used;
};

struct HCS_Data* runData;




/*
 HCS-Funktionen für das Laden / Entladen und Initialisieren / Deinitialisieren
 */

void HCS_Init()
{
    runData = malloc(sizeof(struct HCS_Data));
    struct HCS_Data zero = {0};
    *runData = zero;
}

void HCS_Deinit()
{
    free(runData);
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
}

void HCS_Event_remove(char* n)
{
    int j;
    for (j = 0; j < runData->HCS_Event_used; j++)
    {
        int i = runData->HCS_Event_list[j];
        if (0 == strcmp(runData->HCS_Events[i].name,n))
        {
            remove_element_from_array(runData->HCS_Event_list,&runData->HCS_Event_used,&i);
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
}

void HCS_System_remove(char* n)
{
    int j;
    for (j = 0; j < runData->HCS_System_used; j++)
    {
        int i = runData->HCS_System_list[j];
        if (0 == strcmp(runData->HCS_Systems[i].name,n))
        {
            remove_element_from_array(runData->HCS_System_list,&runData->HCS_System_used,&i);
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

#include "components/states.h"
#include "components/body.h"
#include "components/drawable.h"
#include "components/soundable.h"
#include "components/clickable.h"
#include "components/movement.h"
#include "components/collision.h"
#include "components/jump.h"
#include "components/gravity.h"
#include "components/input.h"
