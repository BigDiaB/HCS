//Meine Tools:
#include <LSD/LSD.h>        //<- "Logging" System
#include "HCS.h"            //<- Entity Component System

#ifdef main
#undef main
#endif

struct HCS_runData* runData;

void HCS_Update(double delta)
{
    while(SDL_PollEvent(&runData->event))
    {
        if (runData->event.type == SDL_QUIT)
            runData->HCS_running = false;
        else if (runData->event.type == SDL_WINDOWEVENT)
        {
            if (runData->event.window.event == SDL_WINDOWEVENT_RESIZED) {
                HCS_Entity_clear();

                SDL_Window* win = runData->window;
                SDL_Renderer* ren = runData->renderer;
                free(runData);
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
                runData->renderer = ren;
                runData->window = win;
                runData->fullscreen = false;
                SDL_GetWindowSize(runData->window, &runData->WIN_SIZE.w,&runData->WIN_SIZE.h);
                runData->STRETCH_WIDTH = (double)runData->WIN_SIZE.w / (double)runData->WIN_SIZE.h;
                runData->STRETCH_HEIGHT = (double)runData->WIN_SIZE.h / (double)runData->WIN_SIZE.w;
                runData->HCS_running = HCS_Main(0,NULL);
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
    LSD_File_path_prepare(argv,6,NULL);

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
    
    runData->window = SDL_CreateWindow("HCS-Projekt",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,runData->WIN_SIZE.w ,runData->WIN_SIZE.h ,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltERROR,"Entity konnte nicht nach Komponenten gefunden werden!");
    #endif
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
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltERROR,"Konnte Entity nicht nach Namen finden!: %s", n);
    #endif
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

/*
 Namen-Funktionen, für das Hinzufügen, Suchen und Entfernen von Namen
 */

void HCS_Name_add(HCS_Entity ent, char* n)
{
    int index = LSD_Math_get_id_from_array(runData->HCS_Name_list,&runData->HCS_Name_used, HCS_MAX_NAMES);
    runData->HCS_Entities[ent][HCS_cName] = index;
    runData->HCS_Names[index].name = malloc(strlen(n) + 1);
    strcpy(runData->HCS_Names[index].name,n);
}

HCS_Name* HCS_Name_get(HCS_Entity ent)
{
    return &runData->HCS_Names[HCS_Entity_get_component_id(ent,HCS_cName)];
}

void HCS_Name_remove(HCS_Entity ent)
{
    free(runData->HCS_Names[HCS_Entity_get_component_id(ent,HCS_cName)].name);
    LSD_Math_remove_object_from_array(runData->HCS_Name_list, &runData->HCS_Name_used, &runData->HCS_Entities[ent][HCS_cName]);
}

HCS_Entity HCS_Entity_create(char* n)
{
    HCS_Entity ent = LSD_Math_get_id_from_array(runData->HCS_Entity_list, &runData->HCS_Entity_used, HCS_MAX_ENTITIES);
    int i;
    for (i = 0; i < HCS_NUM_COMPONENTS; i++)
        runData->HCS_Entities[ent][i] = -1;
    HCS_Name_add(ent,n);
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s erfolgreicht erstellt!",ent,n);
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
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d erfolgreicht entfernt!",ent);
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

// #include "components/input.h"

// #include "components/states.h"
// #include "components/body.h"

// #include "components/movement.h"
// #include "components/collision.h"
// #include "components/jump.h"
// #include "components/gravity.h"

// #include "components/sprite.h"
// //#include "components/drawable.h"
// #include "components/clickable.h"

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


LSD_Thread_function(Misc_Wrapper)
{
    LSD_Thread_init();
    while(runData->HCS_running)
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
    #ifdef HCS_DEBUG
    LSD_Log_level_set(LSD_llALL);
    #else
    LSD_Log_level_set(LSD_llNONE);
    #endif
    runData->HCS_running = HCS_Main(argc,argv);
    LSD_Thread_add("Miscellaneous",Misc_Wrapper);
    LSD_Thread_add("Movement",Move_Wrapper);
    #ifndef HCS_DEBUG
    LSD_Thread_add("Controller",Controller_Server);
    #endif
    //Game-Loop
    while(runData->HCS_running || LSD_Thread_used > 0)
    {
        LSD_Thread_system();
        if (runData->HCS_running)
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