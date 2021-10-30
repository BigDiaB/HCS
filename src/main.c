
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/time.h>


#include "data_util.h"
#include "vec.h"

#define START_WIDTH 1920
#define START_HEIGHT 1080
// #define BLACK_BARS

//Platform-"Wrapper"
#include "no_platform.h"    //<- Platformunabhängig
#include "platform.h"       //<- Platformabhängig
//Meine Tools:
#include "LSD.h"            //<- Logging System
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
 -Ein Debug-Fenster (PiP) mit Daten zu den Entities                             NICHT NOTWENDIG   -> F.U.
 -Output-Log beim Exit generieren                                               NICHT NOTWENDIG   -> F.U.  <- Wäre eventuell schon Cool...
 -Das "SDL2-Zeug" vom Rest trennen, evtl. eigene Header-File                    DONE!             -> Source in SDL.h
 -Szenen hinzufügen                                                             DILLIGAF?         -> Nein.
 -Systems und Events mit Namen Linken, nicht mit Enums!                         Will ich das?     -> Zu viel aufwand, bräuchte 2x Name- s, _list und _fill!
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
 -"Black-Bars" für Oben und Unten implementieren!                               NICHT NOTWENDIG!  -> Windowed-Mode wird immer slimmer als Fullscreen sein!
 -Irgendwie Sound hinkriegen (Möglichst mit SDL_Mixer!)!                        NICHT WIRKLICH... -> Es funktioniert zwar, es muss aber nochmal kräftig überarbeitet werden!
 -"Black-Bars" sind broken...                                                   GEFIXT!           -> if(fullscreen) hat gefehlt
 -Asset-Manager, der die Paths überprüft und so                                 FERTIG!           -> HCS_Asset_Manager() + HCS_Managed_assets in HCS.h
 -LSD_Log überarbeiten mit Format-String wie in printf()!                       FERTIG!           -> In LSD.h, greift aber immernoch auf LSD_Log_old() zurück!
 -LSD_Log mit LSD_Log_old mergen!                                               FERTIG!           -> In LSD.h, greift jetzt nicht mehr auf LSD_Log_old zurück!
 -Jump-Timer hinzufügen!                                                        FERTIG!           -> In jump.h
 -Animationen für Drawables (Timer + Quad und States oder sowas kp...)
 -Sound überarbeiten!
 -"Fake Cursor" aka Pointer, der mit Dpad oder Stick gesteuert wird
 -In Drawable nur sachen drawen, die auch auf dem Bildschirm sind!
 -Das runData-Struct serialisieren und wieder deserialisieren!                  FÜRS ERSTE AUF EIS GELEGT! (Evtl. später mit Data Desk arbeiten!)


Very Nice To Haves™:
-Handy per QR-Code verbinden und als Controller benutzen
 */

void fullscreen_event()
{
    if (isPressed(k.F))
    {
        toggle_fullscreen();
    }
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
    HCS_Body_add(e,get_screen_size().x / 2 - 300,get_screen_size().y / 2 - 150,250,150);
    HCS_Movement_add(e,4000,0);
    HCS_Collider_add(e);
    HCS_Gravity_add(e,0,1500);
    HCS_Jump_add(e,3800,true,0);
    HCS_Input_add(e);
    
    HCS_Drawable_add(e,"Beenden",300,300,true,HCS_Draw_Sprite);
    HCS_Clickable_add(e,&running,HCS_Click_off);
    HCS_Drawable_add_rect(e,100,100,100,125,true);
    
    e = HCS_Entity_create("Door");
    
    HCS_Body_add(e,800,0,500,600);
    HCS_Drawable_add(e,"assets/door.png",0,0,false,HCS_Draw_Background1);
    
    e = HCS_Entity_create("Platform");
    
    HCS_Body_add(e,800,500,525,100);
    HCS_Collider_add(e);
    HCS_Drawable_add(e,"assets/MC_Block.jpg",0,0,false,HCS_Draw_Decal);
    
    e = HCS_Entity_create("Ground");
    
    HCS_Body_add(e,0,get_screen_size().y - 200,get_screen_size().x,200);
    HCS_Collider_add(e);
    HCS_Drawable_add(e,"assets/sand.jpg",0,0,false,HCS_Draw_Background0);
    
    e = HCS_Entity_create("Tree");
    
    HCS_Body_add(e,200,50,300,800);
    HCS_Drawable_add(e,"assets/tree.png",0,0,false,HCS_Draw_Background1);

    
    HCS_Event_add("Camera",camera_event);
    HCS_Event_remove("Start");
}

bool started = false;

void menu_event()
{
    if (!started)
        return;
    HCS_Entity_kill(HCS_Entity_get_by_name("Start_Button"));
    HCS_Entity_kill(HCS_Entity_get_by_name("Quit_Button"));
    HCS_Event_add("Start",start_event);
    HCS_Event_remove("Menu");
}


int main(int argc, char* argv[])
{   
    LIB_PLATFORM_INIT();
    HCS_Init();
    
    LSD_Log_level_set(LSD_llALL);
    
    HCS_Event_add("Fullscreen",fullscreen_event);
    HCS_Event_add("Menu",menu_event);
    
    HCS_System_add("Input",HCS_Input_system);
    HCS_System_add("Clickable",HCS_Clickable_system);
    HCS_System_add("Drawable",HCS_Drawable_system);
    HCS_System_add("Collider",HCS_Collider_system);
    HCS_System_add("Movement",HCS_Movement_system);
    HCS_System_add("Jump",HCS_Jump_system);
    HCS_System_add("Gravity",HCS_Gravity_system);


    HCS_Entity e = HCS_Entity_create("Quit_Button");

    HCS_Body_add(e,get_screen_size().x / 2 - 300,get_screen_size().y / 2 - 300,600,300);
    HCS_Drawable_add(e,"Start",0,0,true,HCS_Draw_Menu1);
    HCS_Clickable_add(e,&started,HCS_Click_on);
    HCS_Drawable_add_rect(e,100,100,100,125,true);

    e = HCS_Entity_create("Start_Button");

    HCS_Body_add(e,get_screen_size().x / 2 - 300,get_screen_size().y / 2 + 100,600,300);
    HCS_Drawable_add(e,"Beenden",0,0,true,HCS_Draw_Menu1);
    HCS_Clickable_add(e,&running,HCS_Click_off);
    HCS_Drawable_add_rect(e,100,100,100,125,true);

    while(running)
    {
        HCS_System_run();
        HCS_Event_run();
        LIB_PLATFORM_UPDATE();
        tick();
    }
    
    HCS_Deinit();
//    Mix_Quit();
    LIB_PLATFORM_DEINIT();
    exit(2);
}
