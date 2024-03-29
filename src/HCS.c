//Meine Tools:
#include <LSD/LSD.h>        //<- "Logging" System
#include "HCS.h"            //<- Entity Component System

#ifdef main
#undef main
#endif

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


struct HCS_runData* runData;

void HCS_Error(char* title, char* desc)
{
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS_ERROR: %s\n%s\n",title,desc);
    #endif
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, desc, runData->window);
}

#define HCS_Error(X,Y)  HCS_Error(X,Y); exit(3)

void HCS_Update(double delta)
{
    while(SDL_PollEvent(&runData->event))
    {
        if (runData->event.type == SDL_QUIT)
            runData->HCS_running = false;
        else if (runData->event.type == SDL_WINDOWEVENT)
        {
            if (runData->event.window.event == SDL_WINDOWEVENT_RESIZED) {

                SDL_GetWindowSize(runData->window, &runData->WIN_SIZE.w,&runData->WIN_SIZE.h);
                runData->STRETCH_WIDTH = (double)runData->WIN_SIZE.w / (double)runData->WIN_SIZE.h;
                runData->STRETCH_HEIGHT = (double)runData->WIN_SIZE.h / (double)runData->WIN_SIZE.w;
                runData->DRAW_OFFSET = (runData->WIN_SIZE.w - runData->WIN_SIZE.h / 9 * 16) / 2;
            }
        }
        #ifdef HCS_DEBUG
        else if (runData->event.type == SDL_KEYUP)
        {
            switch(runData->event.key.keysym.sym)
            {
                case HCS_INPUT_UP:
                    runData->HCS_Input_Pad.y = 0;
                    break;
                case HCS_INPUT_DOWN:
                    runData->HCS_Input_Pad.y = 0;
                    break;
                case HCS_INPUT_LEFT:
                    runData->HCS_Input_Pad.x = 0;
                    break;
                case HCS_INPUT_RIGHT:
                    runData->HCS_Input_Pad.x = 0;
                    break;
                case HCS_INPUT_A:
                    runData->HCS_Input_A.down = false;
                    break;
                case HCS_INPUT_B:
                    runData->HCS_Input_B.down = false;
                    break;

                default:
                    break;
            }
        }
        #endif
        else if (runData->event.type == SDL_KEYDOWN)
        {
            #ifdef HCS_DEBUG
            switch(runData->event.key.keysym.sym)
            {
                case HCS_INPUT_UP:
                    runData->HCS_Input_Pad.y = -50;
                    break;
                case HCS_INPUT_DOWN:
                    runData->HCS_Input_Pad.y = 50;
                    break;
                case HCS_INPUT_LEFT:
                    runData->HCS_Input_Pad.x = -50;
                    break;
                case HCS_INPUT_RIGHT:
                    runData->HCS_Input_Pad.x = 50;
                    break;
                case HCS_INPUT_A:
                    runData->HCS_Input_A.down = true;
                    break;
                case HCS_INPUT_B:
                    runData->HCS_Input_B.down = true;
                    break;
                case SDLK_r:
                    if (SDL_GetModState() & KMOD_CTRL)
                    {
                        HCS_Reload();
                    }
                    break;

                default:
                    break;
            }
            #endif
            if( runData->event.key.keysym.sym == SDLK_BACKSPACE && runData->HCS_Text_input_size > -1 )
            {
                runData->HCS_Text_input[runData->HCS_Text_input_size] = 0;
                runData->HCS_Text_input_size--;
            }
            else if( runData->event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
            {
                SDL_SetClipboardText(runData->HCS_Text_input);
            }
            else if( runData->event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
            {
                strcat(runData->HCS_Text_input,SDL_GetClipboardText());
                runData->HCS_Text_input_size += strlen(SDL_GetClipboardText());
            }
        }
        else if (runData->event.type == SDL_TEXTINPUT)
        {
            if(!(SDL_GetModState() & KMOD_CTRL && (runData->event.text.text[0] == 'c'|| runData->event.text.text[0] == 'C'|| runData->event.text.text[0] == 'v'|| runData->event.text.text[0] == 'V')))
            {
                strcat(runData->HCS_Text_input,runData->event.text.text);
                runData->HCS_Text_input_size += strlen(runData->event.text.text);
            }
        }
    }
    SDL_RenderPresent(runData->renderer);
    SDL_RenderClear(runData->renderer);
}


/*
 HCS-Funktionen für das Laden / Entladen und Initialisieren / Deinitialisieren
 */

void HCS_Init(char* argv[])
{
    LSD_Sys_path_change(argv,6,NULL);

    runData = malloc(sizeof(struct HCS_runData));
    struct HCS_runData zero = {0};
    *runData = zero;

    runData->DRAW_OFFSET = 0;
    runData->STRETCH_HEIGHT = 1;
    runData->STRETCH_WIDTH = 1;
    runData->WORLD_TO_SCREEN_X = 1000;
    runData->WORLD_TO_SCREEN_Y = 1000;
    runData->std.r = 125;
    runData->std.g = 125;
    runData->std.b = 125;
    runData->color.r = 255;
    runData->color.g = 255;
    runData->color.b = 255;
    runData->fullscreen = false;

    
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltERROR,"SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        #endif
        exit(1);
    }
    
    SDL_GetDisplayBounds(0,&runData->WIN_SIZE);
    
    runData->WIN_SIZE.h *= 0.75;
    runData->WIN_SIZE.w = runData->WIN_SIZE.h  / 9 * 16;
    
    runData->DRAW_OFFSET = (runData->WIN_SIZE.w - runData->WIN_SIZE.h / 9 * 16) / 2;

    runData->window = SDL_CreateWindow("HCS-Projekt",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,runData->WIN_SIZE.w ,runData->WIN_SIZE.h,
        SDL_WINDOW_METAL 
        #ifdef HCS_DEBUG
        | SDL_WINDOW_RESIZABLE 
        #endif
        );
    runData->renderer = SDL_CreateRenderer(runData->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(runData->renderer,runData->std.r,runData->std.g,runData->std.b,runData->std.a);
    SDL_RenderClear(runData->renderer);
    SDL_SetRenderDrawBlendMode(runData->renderer, SDL_BLENDMODE_BLEND);
    
    runData->STRETCH_WIDTH = (double)runData->WIN_SIZE.w / (double)runData->WIN_SIZE.h;
    runData->STRETCH_HEIGHT = (double)runData->WIN_SIZE.h / (double)runData->WIN_SIZE.w;
    SDL_SetWindowSize(runData->window, runData->WIN_SIZE.w,runData->WIN_SIZE.h);
    SDL_SetWindowPosition(runData->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


void HCS_Deinit()
{
    int i;
    for (i = 0; i < runData->HCS_Managed_Asset_used; i++)
        free(runData->HCS_Managed_Assets[i].path);
    SDL_DestroyWindow(runData->window);
    SDL_DestroyRenderer(runData->renderer);
    SDL_Quit();
    free(runData);
}


/*
 Entity-Such Funktionen
 */

bool HCS_Entity_has_component(HCS_Entity ent, HCS_Component comp)
{
    return (-1 != runData->HCS_Entities[ent].comp_ids[comp]);
}

int HCS_Entity_get_component_id(HCS_Entity ent, HCS_Component comp)
{
    return runData->HCS_Entities[ent].comp_ids[comp];
}

HCS_Entity HCS_Entity_get_entity_id(int comp_id, HCS_Component comp)
{
    int j;
    for (j = 0; j < runData->HCS_Entity_used; j++)
    {
        int i = runData->HCS_Entity_list[j];
        if (runData->HCS_Entities[i].comp_ids[comp] == comp_id)
            return i;
    }
    HCS_Error("Entity-Suche","Entity konnte nicht nach Komponenten gefunden werden!");
}

HCS_Entity HCS_Entity_get_by_name(char* n)
{
    int i,j;
    for (j = 0; j < runData->HCS_Entity_used; j++)
    {
        i = runData->HCS_Entity_list[j];
        if (0 == strcmp(HCS_Entity_tag_get(i),n))
            return i;
    }

    char desc[2056];
    sprintf(desc,"Konnte Entity nicht nach Namen finden!:\n \"%s\"", n);
    HCS_Error("Entity-Suche",desc);
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
    // HCS_Name_remove(e);
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
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Event %s erfolgreich hinzugefügt!",n);
    #endif
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
            #ifdef HCS_DEBUG
            LSD_Log(LSD_ltCUSTOM,"HCS: Event %s erfolgreich entfernt!",n);
            #endif
            return;
        }
    }
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: %s",n);
    LSD_Log(LSD_ltERROR,"Event konnte nicht entfernt werden, Name wurde nicht gefunden!");
    #endif
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

HCS_Entity_data* HCS_Entity_data_get(HCS_Entity e)
{
    return &runData->HCS_Entities[e];
}

HCS_Entity HCS_Entity_create(char* n)
{
    HCS_Entity ent = LSD_Math_get_id_from_array(runData->HCS_Entity_list, &runData->HCS_Entity_used, HCS_MAX_ENTITIES);
    int i;
    for (i = 0; i < HCS_NUM_COMPONENTS; i++)
        runData->HCS_Entities[ent].comp_ids[i] = -1;
    runData->HCS_Entities[ent].tag = malloc(strlen(n) + 1);
    strcpy(runData->HCS_Entities[ent].tag,n);
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s erfolgreicht erstellt!",ent,runData->HCS_Entities[ent].tag);
    #endif
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
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s erfolgreicht entfernt!",ent,HCS_Entity_tag_get(ent));
    free(HCS_Entity_tag_get(ent));
    #endif
}

void HCS_Drawable_translate_rect(SDL_Rect* r)
{
    
    r->y = LSD_Math_map(r->y,0,runData->WORLD_TO_SCREEN_Y,0,runData->WIN_SIZE.h);
    r->h = LSD_Math_map(r->h,0,runData->WORLD_TO_SCREEN_Y,0,runData->WIN_SIZE.h);
    r->x = LSD_Math_map(r->x,0,runData->WORLD_TO_SCREEN_X * runData->STRETCH_WIDTH,0,runData->WIN_SIZE.w);
    r->w = LSD_Math_map(r->w,0,runData->WORLD_TO_SCREEN_X * runData->STRETCH_WIDTH,0,runData->WIN_SIZE.w);
    
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

/* !!!NUR FÜR UI-ELEMENTE!!! */
LSD_Vec2i HCS_Screen_size_get()
{
    LSD_Vec2i size = {runData->WORLD_TO_SCREEN_X,runData->WORLD_TO_SCREEN_Y};
    return size;
}
void HCS_Void_func()
{
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltERROR,"Du solltest das hier niemals sehen können!");
    #endif
    return;
}

bool HCS_Entity_exist(char* n)
{
    int i,j;
    for (j = 0; j < runData->HCS_Entity_used; j++)
    {
        i = runData->HCS_Entity_list[j];
        if (0 == strcmp(n,HCS_Entity_tag_get(i)))
        {
            return true;
        }
    }
    return false;
}


HCS_Sprite* HCS_Asset(char* path)
{
    int j,line,collum;
    FILE* file;
    char lines[48][69 * 3];
    
    for (j = 0; j < runData->HCS_Managed_Asset_used; j++)
    {
        if (0 == strcmp(path,runData->HCS_Managed_Assets[j].path))
            return &runData->HCS_Managed_Assets[j].spr;
    }

    if (runData->HCS_Managed_Asset_used >= HCS_MAX_ASSETS)
    {
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltERROR,"Keine freien Assets mehr!\nHatte keinen Platz mehr für: %s",path);
        #endif
    }
    
    HCS_Sprite* spr = &runData->HCS_Managed_Assets[runData->HCS_Managed_Asset_used].spr;
    runData->HCS_Managed_Assets[runData->HCS_Managed_Asset_used].path = malloc(strlen(path) + 1);
    strcpy(runData->HCS_Managed_Assets[runData->HCS_Managed_Asset_used].path,path);
    
    file = fopen(path,"r");
    for (line = 0; line < 48; line++)
         fgets(lines[line],69 * 3,file);
    fclose(file);
    for (line = 0; line < 16; line++)
    {
        sscanf(lines[line +  0],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",
            &spr->raw.RED[line][0],&spr->raw.RED[line][1],&spr->raw.RED[line][2],&spr->raw.RED[line][3],&spr->raw.RED[line][4],&spr->raw.RED[line][5],&spr->raw.RED[line][6],&spr->raw.RED[line][7],
            &spr->raw.RED[line][8],&spr->raw.RED[line][9],&spr->raw.RED[line][10],&spr->raw.RED[line][11],&spr->raw.RED[line][12],&spr->raw.RED[line][13],&spr->raw.RED[line][14],
            &spr->raw.RED[line][15]);
        sscanf(lines[line + 16],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",
            &spr->raw.GRN[line][0],&spr->raw.GRN[line][1],&spr->raw.GRN[line][2],&spr->raw.GRN[line][3],&spr->raw.GRN[line][4],&spr->raw.GRN[line][5],&spr->raw.GRN[line][6],&spr->raw.GRN[line][7],
            &spr->raw.GRN[line][8],&spr->raw.GRN[line][9],&spr->raw.GRN[line][10],&spr->raw.GRN[line][11],&spr->raw.GRN[line][12],&spr->raw.GRN[line][13],&spr->raw.GRN[line][14],
            &spr->raw.GRN[line][15]);
        sscanf(lines[line + 32],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",
            &spr->raw.BLU[line][0],&spr->raw.BLU[line][1],&spr->raw.BLU[line][2],&spr->raw.BLU[line][3],&spr->raw.BLU[line][4],&spr->raw.BLU[line][5],&spr->raw.BLU[line][6],&spr->raw.BLU[line][7],
            &spr->raw.BLU[line][8],&spr->raw.BLU[line][9],&spr->raw.BLU[line][10],&spr->raw.BLU[line][11],&spr->raw.BLU[line][12],&spr->raw.BLU[line][13],&spr->raw.BLU[line][14],
            &spr->raw.BLU[line][15]);
    }
    SDL_Surface* temp = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);
    SDL_Rect r;
    r.w = 1;
    r.h = 1;
    for (line = 15; line > -1; line--)
        for (collum = 0; collum < 16; collum++)
        {
            r.x = 15 - line;
            r.y = collum;
            SDL_FillRect(temp,&r,SDL_MapRGB(temp->format,spr->raw.RED[collum][line],spr->raw.GRN[collum][line],spr->raw.BLU[collum][line]));
        }
    
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
    
    spr->tex = SDL_CreateTextureFromSurface(runData->renderer,temp);
    SDL_FreeSurface(temp);
    
    return spr;
}

LSD_Vec2f* HCS_Camera_get()
{
    return &runData->HCS_Gfx_Camera;
}
LSD_Vec2i* HCS_Cursor_position_get()
{
    return &runData->HCS_Input_Cursor_position;
}
HCS_Button* HCS_Cursor_button_get()
{
    return &runData->HCS_Input_Cursor_button;
}
HCS_Button* HCS_Button_A_get()
{
    return &runData->HCS_Input_A;
}
HCS_Button* HCS_Button_B_get()
{
    return &runData->HCS_Input_B;
}
bool HCS_running_get()
{
    return runData->HCS_running;
}
SDL_Renderer* HCS_Gfx_renderer_get()
{
    return runData->renderer;
}
SDL_Window* HCS_Gfx_window_get()
{
    return runData->window;
}
LSD_Vec2d HCS_Gfx_stretch_get()
{
    return LSD_Vec_new_double(runData->STRETCH_WIDTH,runData->STRETCH_HEIGHT);
}
char* HCS_Text_input_get()
{
    return runData->HCS_Text_input;
}
int* HCS_Text_input_length_get()
{
    return &runData->HCS_Text_input_size;
}
void HCS_Stop()
{
    runData->HCS_running = false;
}




void Controller_Server_POST(LSD_WebServer* server)
{
    char* data_line = strstr(server->read_buffer,"Content-Type: ") + 14;
    data_line[(int)(strstr(data_line, "\n") - data_line)] = 0;
    sscanf(data_line, "%d %d %d %d",&runData->HCS_Input_A.down, &runData->HCS_Input_B.down, &runData->HCS_Input_Pad.x, &runData->HCS_Input_Pad.y);
}


LSD_Thread_function(Controller_Server)
{
    LSD_Thread_init();

    LSD_WebServer* server = LSD_WebServer_open("server",1234,LSD_WebServer_STD_GET,Controller_Server_POST);
    while(runData->HCS_running)
        LSD_WebServer_serve_once(server);
    LSD_WebServer_close(server);

    LSD_Thread_finish();
}


LSD_Thread_function(Collider_thread)
{
    LSD_Thread_init();
    while(runData->HCS_running)
    {
        HCS_Collider_system(LSD_Delta_none);
    }
    LSD_Thread_finish();
}

LSD_Thread_function(Move_Wrapper)
{
    LSD_Thread_init();
    LSD_Delta_add("Movement_Wrapper");
    double delta;
    while(runData->HCS_running)
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

void HCS_Script_dump(char* filename)
{
    char current_add[1024];
    int num_components = 1;
    char* file_to_be_written = malloc(num_components * 1024);
    int i,j;
    for (j = 0; j < runData->HCS_Entity_used; j++)
    {
        i = runData->HCS_Entity_list[j];

        sprintf(current_add,"{%s}\n",HCS_Entity_tag_get(i));

        strcat(file_to_be_written,current_add);

        if (HCS_Entity_has_component(i,HCS_cState))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cInput))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);   
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cBody))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cBody))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cMovement))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cClickable))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cSprite))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cCollider))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cGravity))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
        if (HCS_Entity_has_component(i,HCS_cJump))
        {
            num_components++;
            file_to_be_written = realloc(file_to_be_written,num_components * 1024);
            strcat(file_to_be_written,current_add);
        }
    }

    free(file_to_be_written);
}

void HCS_Script_load(char* filename)
{
    // #define LOAD_GEN_DEBUG

    #define ENT_BEGIN       '{'
    #define ENT_END         '}'    
    #define COMP_BEGIN      '['
    #define COMP_END        ']'
    #define COMP_ARGS       ':'
    #define COMP_NOARGS     ';'
    #define ARG_DELIMITER   "|"

    #define current *entity_data
    #define advance() entity_data++


    HCS_Entity current_entity;
    char current_entity_name[10000];
    char current_component_name[100];
    char current_component_args[10000];

    LSD_File* file = LSD_File_open(filename);

    char* entity_data = file->data;

    while (*entity_data)
    {
        if (current == ENT_BEGIN)
        {
            advance();
            int ent_point = 0;
            for (ent_point = 0; ent_point < 10000; ent_point++)
                current_entity_name[ent_point] = 0;
            ent_point = 0;
            while (current != ENT_END)
            {
                current_entity_name[ent_point] = current;
                ent_point++;
                advance();
            }
            current_entity = HCS_Entity_create(current_entity_name);
        }
        else if (current == COMP_BEGIN)
        {
            advance();
            int ent_point;
            for (ent_point = 0; ent_point < 100; ent_point++)
                current_component_name[ent_point] = 0;
            ent_point = 0;
            while (current != COMP_END)
            {
                current_component_name[ent_point] = current;
                ent_point++;
                advance();
            }
        }
        else if (current == COMP_NOARGS)
        {
            if (LSD_Sys_strcmp(current_component_name,"Input"))
                HCS_Input_add(current_entity);
            else if (LSD_Sys_strcmp(current_component_name,"State"))
                HCS_State_add(current_entity);
            advance();
        }
        else if (current == COMP_ARGS)
        {
            advance();
            while (current == ' ' || current == '\t')
                advance();
            int ent_point;
            for (ent_point = 0; ent_point < 10000; ent_point++)
                current_component_args[ent_point] = 0;
            ent_point = 0;
            while (current != COMP_NOARGS)
            {
                current_component_args[ent_point] = current;
                ent_point++;
                advance();
            }


            if (LSD_Sys_strcmp(current_component_name,"Body"))
            {
                const char* body_format = "%f|%f|%d|%d|";
                float x,y;
                int w,h;
                sscanf(current_component_args,body_format,&x,&y,&w,&h);
                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Body:%f, %f, %d, %d",current_entity_name, x,y,w,h);
                #endif
                HCS_Body_add(current_entity,x,y,w,h);
            }
            else if (LSD_Sys_strcmp(current_component_name,"Movement"))
            {
                const char* movement_format = "%d|%d";
                int sx,sy;
                sscanf(current_component_args,movement_format,&sx,&sy);
                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Movement: %d, %d",current_entity_name, sx,sy);
                #endif
                HCS_Movement_add(current_entity,sx,sy);
            }
            else if (LSD_Sys_strcmp(current_component_name,"Sprite"))
            {
                char path[100];
                int type,layer;
                bool use_text;
                char* token = strtok(current_component_args,ARG_DELIMITER);
                strcpy(path,token);

                token = strtok(NULL,ARG_DELIMITER);
                
                sscanf(token,"%d",&layer);

                token = strtok(NULL,ARG_DELIMITER);
                
                sscanf(token,"%d",&type);
                
                token = strtok(NULL,ARG_DELIMITER);
                if (LSD_Sys_strcmp(token,"false"))
                {
                    use_text = false;
                }
                else if (LSD_Sys_strcmp(token,"true"))
                {
                    use_text = true;
                }

                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Sprite: %s, %d, %d",current_entity_name, path, type, use_text);
                #endif
                HCS_Sprite_add(current_entity,path,layer,type,use_text);
            }
            else if (LSD_Sys_strcmp(current_component_name,"Collider"))
            {
                LSD_Vec2f pos_mod;
                LSD_Vec2i size_mod;
                char* token = strtok(current_component_args,ARG_DELIMITER);
                sscanf(token,"Vec(%f,%f)",&pos_mod.x,&pos_mod.y);
                token = strtok(NULL,ARG_DELIMITER);
                sscanf(token,"Vec(%d,%d)",&size_mod.x,&size_mod.y);
                token = strtok(NULL,ARG_DELIMITER);


                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Collider: Vec(%f,%f), Vec(%d,%d), %s",current_entity_name, pos_mod.x,pos_mod.y,size_mod.x,size_mod.y,token);
                #endif
                HCS_Collider_add(current_entity,pos_mod,size_mod,token);
            }
            else if (LSD_Sys_strcmp(current_component_name,"Gravity"))
            {
                const char* gravity_format = "%f|%f|";
                float n,m;
                sscanf(current_component_args,gravity_format,&n,&m);
                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Gravity: %f, %f",current_entity_name, n,m);
                #endif
                HCS_Gravity_add(current_entity,n,m);
            }
            else if (LSD_Sys_strcmp(current_component_name,"Jump"))
            {
                float n,l;
                bool m;

                char* token = strtok(current_component_args,ARG_DELIMITER);

                sscanf(token,"%f",&n);

                token = strtok(NULL,ARG_DELIMITER);

                if (LSD_Sys_strcmp(token,"false"))
                {
                    m = false;
                }
                else if (LSD_Sys_strcmp(token,"true"))
                {
                    m = true;
                }

                token = strtok(NULL,ARG_DELIMITER);

                sscanf(token,"%f",&l);

                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Jump: %f, %d, %f",current_entity_name, n,m,l);
                #endif
                HCS_Jump_add(current_entity,n,m,l);
            }
            else if (LSD_Sys_strcmp(current_component_name,"Clickable"))
            {
                char clicktype_text[100];
                char trigtype_text[100];
                char callback_name[1000];
                int func_data;
                HCS_Clicktype clicktype;
                HCS_Triggertype trigtype;


                char* token = strtok(current_component_args,ARG_DELIMITER);
                strcpy(clicktype_text,token);
                token = strtok(NULL,ARG_DELIMITER);
                strcpy(trigtype_text,token);
                token = strtok(NULL,ARG_DELIMITER);
                strcpy(callback_name,token);
                token = strtok(NULL,ARG_DELIMITER);
                sscanf(token,"%d",&func_data);


                if (LSD_Sys_strcmp(clicktype_text,"HCS_Click_on"))
                {
                    clicktype = HCS_Click_on;
                }
                else if (LSD_Sys_strcmp(clicktype_text,"HCS_Click_off"))
                {
                    clicktype = HCS_Click_off;
                }
                else if (LSD_Sys_strcmp(clicktype_text,"HCS_Click_toggle"))
                {
                    clicktype = HCS_Click_toggle;
                }

                if (LSD_Sys_strcmp(trigtype_text,"HCS_Trig_released"))
                {
                    trigtype = HCS_Trig_released;
                } 
                else if (LSD_Sys_strcmp(trigtype_text,"HCS_Trig_down"))
                {
                    trigtype = HCS_Trig_down;
                } 

                #ifdef LOAD_GEN_DEBUG
                LSD_Log(LSD_ltMESSAGE,"%s: Clickable: %s, %s, %s, %d",current_entity_name,clicktype_text,trigtype_text,callback_name,token);
                #endif
                HCS_Clickable_add(current_entity,clicktype,trigtype,callback_name,func_data);

            }
            advance();

        }
        else
            advance();
    }
    LSD_File_close(file);
}


//Main-Loop mit Game-Loop:
int main(int argc, char* argv[])
{
    //Library-Initialisierung
    HCS_Init(argv);
    #ifdef HCS_DEBUG
    LSD_Log_level_set(LSD_llALL);
    #else
    LSD_Log_level_set(LSD_llNONE);
    #endif

    runData->HCS_running = HCS_Main(argc,argv);
    #ifdef HCS_MULTI_THREAD
        #ifdef HCS_SINGLE_THREAD
            #error HCS_MULTI_THREAD und HCS_SINGLE_THREAD definiert!
        #endif
    #endif

    #ifdef HCS_MULTI_THREAD
    LSD_Thread_add("Movement",Move_Wrapper);
    LSD_Thread_add("Collider",Collider_thread);
    #endif
    #ifndef HCS_DEBUG
    LSD_Thread_add("Controller",Controller_Server);
    #endif
    //Game-Loop
    LSD_Delta_add("Main_delta");
    double delta;
    while(runData->HCS_running || LSD_Thread_used > 0)
    {
        LSD_Thread_system();
        if (runData->HCS_running)
        {
            LSD_Delta_tick("Main_delta");
            delta = LSD_Delta_get("Main_delta")->delta;
            #ifdef HCS_SINGLE_THREAD
            HCS_Jump_system(delta);
            HCS_Gravity_system(delta);
            HCS_Movement_system(delta);
            HCS_Collider_system(LSD_Delta_none);
            #endif
            HCS_Sprite_system(LSD_Delta_none);
            HCS_Clickable_system(LSD_Delta_none);
            HCS_Input_system(LSD_Delta_none);
            HCS_Event_run();
        }
        HCS_Update(LSD_Delta_none);
    }
    LSD_Delta_remove("Main_delta");
    HCS_Entity_clear();
    //Library-Deinitialisierung
    HCS_Deinit();
    exit(0);
}

void HCS_Reload()
{
    HCS_Entity_clear();
    runData->HCS_running = HCS_Main(0,NULL);
}
