
#include <stdio.h>
#include <stdlib.h>

//Meine Tools:
#include <LSD/LSD.h>        //<- "Logging" System
#include "HCS.h"            //<- Entity Component System

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

 -Animationen für Drawables (Timer + Quad und States oder sowas kp...)
 -"Fake Cursor" aka Pointer, der mit Dpad oder Stick gesteuert wird
 -In Drawable nur sachen drawen, die auch auf dem Bildschirm sind!
 
 -Tile-World-Map-Loader?
 -"Spatial-Hashing" für Terrain-Collisions!
 -Managed Asset für Sprites... Ughh...

 -Cap für Threads

 -Wrapper um den Webserver schreiben (LSD_Server?)
 
 
 -Sprite Editor + Exporteur damit wir SDL(2)_image los werden!
 -Font als "System-Sprites" speichern um SDL(2)_ttf los zu werden!
 -Coole Dateiendung überlegen!
 
 -"exit(X)" hinter allen LSD_Log(LSD_ltERROR,...) hinzufügen!
 
 -Sound überarbeiten!                                                           FÜRS ERSTE AUF EIS GELEGT!
 -Das runData-Struct serialisieren und wieder deserialisieren!                  FÜRS ERSTE AUF EIS GELEGT! (Evtl. später mit Data Desk arbeiten!)
 -"Black-Bars" für Oben und Unten implementieren!                               NICHT NOTWENDIG!  -> Windowed-Mode hat immer die gleiche Ratio wie Fullscreen!
 -Irgendwie Sound hinkriegen (Möglichst mit SDL_Mixer!)!                        NICHT WIRKLICH... -> Bereits entfernt, lol
 -Auf SDL(1) umsteigen, für einfaches Porten zu Wii, 3ds, Vita, etc.            NOPE, NO PAIN NO GAIN, lol
 
 Very Nice To Haves™:

 */

bool game_started = false;
HCS_Gfx_Texture QR_TEX;
HCS_Gfx_Rectangle QR_BODY = {100,100,400,400};
void game_start_event()
{

    HCS_Gfx_Texture_draw(QR_TEX, NULL, QR_BODY);


    if (game_started)
    {
        SDL_DestroyTexture(QR_TEX);
        HCS_Entity_kill(HCS_Entity_get_by_name("Start_Button"));
        HCS_Entity_kill(HCS_Entity_get_by_name("Quit_Button"));
        
        HCS_Entity e = HCS_Entity_create("Player");
        HCS_State_add(e);
        HCS_Body_add(e,500,100,500,500);
        HCS_Movement_add(e,4000,4000);
        HCS_Input_add(e);
        HCS_Sprite_add(e,"gfx.txt",HCS_Draw_Sprite);
        HCS_Collider_add(e,LSD_Vec_new_float(2,0),LSD_Vec_new_int(6,0));
        HCS_Jump_add(e,6000,true,0);
        HCS_Gravity_add(e,0,4000);
        
        e = HCS_Entity_create("Box");
        HCS_Body_add(e,1400,550,500,500);
        HCS_Sprite_add(e,"box.txt",HCS_Draw_Background1);
        HCS_Collider_add(e,LSD_Vec_new_float(0,0),LSD_Vec_new_int(0,0));
        HCS_Clickable_add(e,&running,HCS_Click_off);
        
        e = HCS_Entity_create("Box2");
        HCS_Body_add(e,10,800,2200,200);
        HCS_Sprite_add(e,"box.txt",HCS_Draw_Background0);
        HCS_Collider_add(e,LSD_Vec_new_float(0,0),LSD_Vec_new_int(0,0));

        HCS_Event_remove("game_start");
    }
    
}

void init_event()
{
    QR_TEX = SDL_CreateTextureFromSurface(renderer,SDL_LoadBMP("server/Controller-Server.bmp"));
    HCS_Entity e = HCS_Entity_create("Start_Button");
    HCS_Body_add(e,HCS_Screen_size_get().x / 2 + 500, 300, 600, 400);
    HCS_Sprite_add(e,"box.txt",HCS_Draw_Menu0);
    HCS_Clickable_add(e,&game_started,HCS_Click_on);

    e = HCS_Entity_create("Quit_Button");
    HCS_Body_add(e,HCS_Screen_size_get().x / 2 - 300, 300, 600, 400);
    HCS_Sprite_add(e,"box.txt",HCS_Draw_Menu0);
    HCS_Sprite_remove(e);
    HCS_Sprite_add(e,"box.txt",HCS_Draw_Menu0);
    HCS_Clickable_add(e,&running,HCS_Click_off);

    HCS_Event_add("game_start",game_start_event);

    HCS_Event_remove("init");
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

HCS_Sprite CURSOR_SPRITE;
HCS_Gfx_Rectangle CURSOR_BODY;
bool CURSOR_INIT = false;

void HCS_Cursor_event()
{
    if (!CURSOR_INIT)
    {
        CURSOR_BODY.w = 75;
        CURSOR_BODY.h = 75;
        sprite_new(&CURSOR_SPRITE,"cursor.txt"); 
        CURSOR_INIT = true;   
        LSD_Delta_add("HCS_Cursor");
    }
    LSD_Delta_tick("HCS_Cursor");
    double delta = LSD_Delta_get("HCS_Cursor")->delta;
    if (HCS_Input_Menu)
    {
        HCS_Input_A_pressed = HCS_Input_A_down && !HCS_Input_A_Ldown;
        HCS_Input_B_pressed = HCS_Input_B_down && !HCS_Input_B_Ldown;
        HCS_Input_A_released = !HCS_Input_A_down && HCS_Input_A_Ldown;
        HCS_Input_B_released = !HCS_Input_B_down && HCS_Input_B_Ldown;

        HCS_Gfx_Mouse_last_clicked = HCS_Gfx_Mouse_clicked;
        HCS_Gfx_Mouse_clicked = HCS_Input_B_down && HCS_Input_Menu;

        LSD_Vec2d temp;
        LSD_Vec_mul(temp, HCS_Input_Pad, LSD_Vec_new_double(10,10));
        LSD_Vec_mul(temp, temp, LSD_Vec_new_double(delta,delta));
        LSD_Vec_add(HCS_Gfx_Mouse_pos,HCS_Gfx_Mouse_pos,temp);

        if (HCS_Gfx_Mouse_pos.x + 75 / 8 * 7 > WIN_SIZE.w)
            HCS_Gfx_Mouse_pos.x = WIN_SIZE.w - 75 / 8 * 7;

        if (HCS_Gfx_Mouse_pos.x < 0)
            HCS_Gfx_Mouse_pos.x = 0;

        if (HCS_Gfx_Mouse_pos.y + 75 / 8 * 7 > WIN_SIZE.h)
            HCS_Gfx_Mouse_pos.y = WIN_SIZE.h - 75 / 8 * 7;

        if (HCS_Gfx_Mouse_pos.y < 0)
            HCS_Gfx_Mouse_pos.y = 0;
    }
    CURSOR_BODY.x = HCS_Gfx_Mouse_pos.x;
    CURSOR_BODY.y = HCS_Gfx_Mouse_pos.y;
    SDL_RenderCopy(renderer,CURSOR_SPRITE.tex,NULL,&CURSOR_BODY);
}


void Controller_Server_POST(LSD_WebServer* server)
{
    char* right_line = strstr(server->read_buffer,"Content-Type: ") + 14;
    right_line[(int)(strstr(right_line, "\n") - right_line)] = 0;
    sscanf(right_line, "%d %d %d %d",&HCS_Input_A_down, &HCS_Input_B_down, &HCS_Input_Pad.x, &HCS_Input_Pad.y);
}


LSD_Thread_function(Controller_Server)
{
    LSD_Thread_init();

    LSD_WebServer* server = LSD_WebServer_open("server",LSD_WebServer_STD_GET,Controller_Server_POST);
    LSD_WebServer_serve_while(server,&running);
    LSD_WebServer_close(server);

    LSD_Thread_finish();
}

//Main-Loop mit Game-Loop:
int main(int argc, char* argv[])
{
    
    
    //Library-Initialisierung
    HCS_Init(argv);

    

    LSD_Log_level_set(LSD_llALL);
    LSD_Thread_add("Miscellaneous",Misc_Wrapper);
    LSD_Thread_add("Movement",Move_Wrapper);
    LSD_Thread_add("Controller",Controller_Server);
    HCS_Event_add("Cursor",HCS_Cursor_event);
    HCS_Event_add("init",init_event);

    LSD_Delta_add("Main_Delta");
    double main_delta;
    //Game-Loop
    while(running || LSD_Thread_used > 0)
    {
        LSD_Delta_tick("Main_Delta");
        main_delta = LSD_Delta_get("Main_Delta")->delta;

        if (HCS_Input_A_pressed)
        {
            HCS_Input_Menu = !HCS_Input_Menu;
        }

        LSD_Thread_system();
        if (running)
        {
            HCS_Sprite_system(LSD_Delta_none);
            HCS_Clickable_system(LSD_Delta_none);
            HCS_Event_run();
            
            HCS_Update(main_delta);
        }
    }
    LSD_Delta_remove("Main_Delta");

    HCS_Entity_clear();
    //Library-Deinitialisierung
    HCS_Deinit();
    exit(0);
}
