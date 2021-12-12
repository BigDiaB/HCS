//Meine Tools:
#include <LSD/LSD.h>        //<- "Logging" System
#include "HCS.h"            //<- Entity Component System

struct HCS_runData* runData;
HCS_Gfx_Color color = {255,255,255,255};
HCS_Gfx_Color std = {125,125,125,255};

SDL_Window* window;
SDL_Renderer* renderer;
HCS_Gfx_Rectangle WIN_SIZE;

bool HCS_Gfx_Mouse_clicked;
bool HCS_Gfx_Mouse_last_clicked;

bool HCS_Input_A_Ldown;
bool HCS_Input_A_down;
bool HCS_Input_A_pressed;
bool HCS_Input_A_released;

bool HCS_Input_B_Ldown;
bool HCS_Input_B_down;
bool HCS_Input_B_pressed;
bool HCS_Input_B_released;

LSD_Vec2i HCS_Input_Pad;

bool HCS_Input_Menu = true;

LSD_Vec2f HCS_Gfx_Camera = {0,0};
LSD_Vec2i HCS_Gfx_Mouse_pos;

double WORLD_TO_SCREEN_X = 1000;
double WORLD_TO_SCREEN_Y = 1000;

double DRAW_OFFSET = 0;
double STRETCH_WIDTH = 1;
double STRETCH_HEIGHT = 1;

bool fullscreen = false;
bool running;

//HCS_Gfx_Texture HCS_Asset_manager(char* path, HCS_Managed_assettype action)
//{
//    switch (action)
//    {
//        case HCS_AAdd:
//        {
//            int j;
//            for (j = 0; j < runData->HCS_Managed_asset_used; j++)
//            {
//                int i = runData->HCS_Managed_asset_list[j];
//                if (0 == strcmp(runData->HCS_Managed_assets[i].path,path))
//                {
//                    runData->HCS_Managed_assets[i].used++;
//                    return runData->HCS_Managed_assets[i].tex;
//                }
//            }
//            {
//                    int id = LSD_Math_get_id_from_array(runData->HCS_Managed_asset_list, &runData->HCS_Managed_asset_used, HCS_MAX_SPRITES);
//                    runData->HCS_Managed_asset_list[runData->HCS_Managed_asset_used] = id;
////                    runData->HCS_Managed_assets[id].tex = HCS_Gfx_Surface_to_texture(HCS_Gfx_Image_load(path));
//                    runData->HCS_Managed_assets[id].path = path;
//                    runData->HCS_Managed_assets[id].used = 1;
//                    return runData->HCS_Managed_assets[id].tex;
//                }
//            break;
//        }
//        case HCS_ARemove:
//        {
//        int j;
//        for (j = 0; j < runData->HCS_Managed_asset_used; j++)
//        {
//            int i = runData->HCS_Managed_asset_list[j];
//            if (0 == strcmp(runData->HCS_Managed_assets[i].path,path))
//            {
//                runData->HCS_Managed_assets[i].used--;
//                if (runData->HCS_Managed_assets[i].used <= 0)
//                {
////                    HCS_Gfx_Texture_destroy(runData->HCS_Managed_assets[i].tex);
//                    LSD_Math_remove_object_from_array(runData->HCS_Managed_asset_list,&runData->HCS_Managed_asset_used,&j);
//                }
//                return NULL;
//            }
//        }
//        LSD_Log(LSD_ltERROR,"Asset konnte nicht entfernt werden, weil es nicht nach Namen gefunden wurde!: %s",path);
//        break;
//        }
//
//        default:
//        break;
//    }
//    return NULL;
//}

void HCS_Update(double delta)
{
    SDL_Event event;
    while( SDL_PollEvent(&event))
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(window, &WIN_SIZE.w,&WIN_SIZE.h);
                STRETCH_WIDTH = (double)WIN_SIZE.w / (double)WIN_SIZE.h;
                STRETCH_HEIGHT = (double)WIN_SIZE.h / (double)WIN_SIZE.w;
                HCS_Entity_clear();
                free(runData);
                runData = malloc(sizeof(struct HCS_runData));
                struct HCS_runData zero = {0};
                *runData = zero;
                running = HCS_Main(0,NULL);
            }
        }
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}


/*
 HCS-Funktionen für das Laden / Entladen und Initialisieren / Deinitialisieren
 */

void HCS_Init(char* argv[])
{
    LSD_File_path_prepare(argv,6);
    
    runData = malloc(sizeof(struct HCS_runData));
    struct HCS_runData zero = {0};
    *runData = zero;
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    SDL_GetDisplayBounds(0,&WIN_SIZE);
    
    WIN_SIZE.w *= 0.75;
    WIN_SIZE.h *= 0.75;
    
    window = SDL_CreateWindow("HCS-Projekt",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIN_SIZE.w ,WIN_SIZE.h ,SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer,std.r,std.g,std.b,std.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    STRETCH_WIDTH = (double)WIN_SIZE.w / (double)WIN_SIZE.h;
    STRETCH_HEIGHT = (double)WIN_SIZE.h / (double)WIN_SIZE.w;
    SDL_SetWindowSize(window, WIN_SIZE.w,WIN_SIZE.h);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


void HCS_Deinit()
{
    SDL_Quit();
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
    int i,j;
    for (j = 0; j < runData->HCS_Name_used; j++)
    {
        i = runData->HCS_Name_list[j];
        if (0 == strcmp(runData->HCS_Names[i].name,n))
            return HCS_Entity_get_entity_id(i,HCS_cName);
    }
    LSD_Log(LSD_ltERROR,"Konnte Entity nicht nach Namen finden!: %s", n);
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
    if (HCS_Entity_has_component(e,HCS_cSprite))
        HCS_Sprite_remove(e);
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
    int id = LSD_Math_get_id_from_array(runData->HCS_Event_list, &runData->HCS_Event_used, HCS_MAX_EVENTS);
    runData->HCS_Event_list[runData->HCS_Event_used] = id;
    runData->HCS_Events[id].event = sys;
    runData->HCS_Events[id].name = n;
    LSD_Log(LSD_ltCUSTOM,"HCS: Event %s erfolgreich hinzugefügt!",n);
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
            LSD_Math_remove_object_from_array(runData->HCS_Event_list,&runData->HCS_Event_used,&j);
            LSD_Log(LSD_ltCUSTOM,"HCS: Event %s erfolgreich entfernt!",n);
            return;
        }
    }
    LSD_Log(LSD_ltCUSTOM,"HCS: %s",n);
    LSD_Log(LSD_ltERROR,"Event konnte nicht entfernt werden, Name wurde nicht gefunden!");
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

/*
 Namen-Funktionen, für das Hinzufügen, Suchen und Entfernen von Namen
 */

void HCS_Name_add(HCS_Entity ent, char* n)
{
    int index = LSD_Math_get_id_from_array(runData->HCS_Name_list,&runData->HCS_Name_used, HCS_MAX_NAMES);
    runData->HCS_Entities[ent][HCS_cName] = index;
    strcpy(runData->HCS_Names[index].name,n);
}

HCS_Name* HCS_Name_get(HCS_Entity ent)
{
    return &runData->HCS_Names[HCS_Entity_get_component_id(ent,HCS_cName)];
}

void HCS_Name_remove(HCS_Entity ent)
{
    LSD_Math_remove_object_from_array(runData->HCS_Name_list, &runData->HCS_Name_used, &runData->HCS_Entities[ent][HCS_cName]);
}

HCS_Entity HCS_Entity_create(char* n)
{
    HCS_Entity ent = LSD_Math_get_id_from_array(runData->HCS_Entity_list, &runData->HCS_Entity_used, HCS_MAX_ENTITIES);
    int i;
    for (i = 0; i < HCS_NUM_COMPONENTS; i++)
        runData->HCS_Entities[ent][i] = -1;
    HCS_Name_add(ent,n);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s erfolgreicht erstellt!",ent,n);
    return ent;
}

void HCS_Entity_remove(HCS_Entity ent)
{
    int index;
    int i;
    for (i = 0; i < runData->HCS_Entity_used; i++)
        if (runData->HCS_Entity_list[i] == ent)
            index = i;
    LSD_Math_remove_object_from_array(runData->HCS_Entity_list, &runData->HCS_Entity_used, &index);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d erfolgreicht entfernt!",ent);
}

void HCS_Drawable_translate_rect(HCS_Gfx_Rectangle* r)
{
    
    r->y = LSD_Math_map(r->y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
    r->h = LSD_Math_map(r->h,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
    r->x = LSD_Math_map(r->x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
    r->w = LSD_Math_map(r->w,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
    
//    r->y = LSD_Math_map(r->y,0,WORLD_TO_SCREEN_Y * STRETCH_HEIGHT,0,WIN_SIZE.h);
//    r->h = LSD_Math_map(r->h,0,WORLD_TO_SCREEN_Y * STRETCH_HEIGHT,0,WIN_SIZE.h);
//    r->x = LSD_Math_map(r->x,0,WORLD_TO_SCREEN_X,0,WIN_SIZE.w);
//    r->w = LSD_Math_map(r->w,0,WORLD_TO_SCREEN_X,0,WIN_SIZE.w);
}

#include "components/input.h"

#include "components/states.h"
#include "components/body.h"

#include "components/movement.h"
#include "components/collision.h"
#include "components/jump.h"
#include "components/gravity.h"

#include "components/sprite.h"
//#include "components/drawable.h"
#include "components/clickable.h"

void Controller_Server_POST(LSD_WebServer* server)
{
    char* data_line = strstr(server->read_buffer,"Content-Type: ") + 14;
    data_line[(int)(strstr(data_line, "\n") - data_line)] = 0;
    sscanf(data_line, "%d %d %d %d",&HCS_Input_A_down, &HCS_Input_B_down, &HCS_Input_Pad.x, &HCS_Input_Pad.y);
}


LSD_Thread_function(Controller_Server)
{
    LSD_Thread_init();

    LSD_WebServer* server = LSD_WebServer_open("server",LSD_WebServer_STD_GET,Controller_Server_POST);
    while(running)
        LSD_WebServer_serve_once(server);
    LSD_WebServer_close(server);

    LSD_Thread_finish();
}


LSD_Thread_function(Misc_Wrapper)
{
    LSD_Thread_init();
    while(running)
    {
        HCS_Collider_system(LSD_Delta_none);
        HCS_Input_system(LSD_Delta_none);
    }
    LSD_Thread_finish();
}

LSD_Thread_function(Move_Wrapper)
{
    LSD_Thread_init();
    LSD_Delta_add("Movement_Wrapper");
    double delta;
    while(running)
    {
        LSD_Delta_tick("Movement_Wrapper");
        delta = LSD_Delta_get("Movement_Wrapper")->delta;
        HCS_Jump_system(delta);
        HCS_Gravity_system(delta);
        HCS_Movement_system(delta);
    }
    LSD_Delta_remove("Movement_Wrapper");
    LSD_Thread_finish();
}

/* !!!NUR FÜR UI-ELEMENTE!!! */
LSD_Vec2i HCS_Screen_size_get()
{
    LSD_Vec2i size = {WORLD_TO_SCREEN_X,WORLD_TO_SCREEN_Y};
    return size;
}
void HCS_Void_func()
{
    LSD_Log(LSD_ltERROR,"Du solltest das hier niemals sehen können!");
    return;
}

bool HCS_Entity_exist(char* n)
{
    int i,j;
    for (j = 0; j < runData->HCS_Name_used; j++)
    {
        i = runData->HCS_Name_list[j];
        if (0 == strcmp(n,runData->HCS_Names[i].name))
        {
            return true;
        }
    }
    return false;
}


//Main-Loop mit Game-Loop:
int main(int argc, char* argv[])
{
    //Library-Initialisierung
    HCS_Init(argv);
    LSD_Log_level_set(LSD_llALL);
    running = HCS_Main(argc,argv);
//    LSD_Thread_add("Miscellaneous",Misc_Wrapper);
//    LSD_Thread_add("Movement",Move_Wrapper);
//    LSD_Thread_add("Controller",Controller_Server);
    //Game-Loop
    while(running || LSD_Thread_used > 0)
    {
        LSD_Thread_system();
        if (running)
        {
            HCS_Sprite_system(LSD_Delta_none);
            HCS_Clickable_system(LSD_Delta_none);
            HCS_Event_run();
        }
        HCS_Update(LSD_Delta_none);
    }
    HCS_Entity_clear();
    //Library-Deinitialisierung
    HCS_Deinit();
    exit(0);
}
