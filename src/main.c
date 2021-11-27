
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h> //<- Für den Controller-Server
#include <netinet/in.h> //<- Für den Controller-Server

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

 -Animationen für Drawables (Timer + Quad und States oder sowas kp...)
 -"Fake Cursor" aka Pointer, der mit Dpad oder Stick gesteuert wird
 -In Drawable nur sachen drawen, die auch auf dem Bildschirm sind!
 
 -Tile-World-Map-Loader?
 -"Spatial-Hashing" für Terrain-Collisions!
 -Managed Asset für Sprites... Ughh...

 -Cap für Threads
 
 
 -Sprite Editor + Exporteur damit wir SDL(2)_image los werden!
 -Font als "System-Sprites" speichern um SDL(2)_ttf los zu werden!
 -Coole Dateiendung überlegen!
 
 -"exit(X)" hinter allen LSD_Log(LSD_ltERROR,...) hinzufügen!
 
 -Sound überarbeiten!                                                           FÜRS ERSTE AUF EIS GELEGT!
 -Das runData-Struct serialisieren und wieder deserialisieren!                  FÜRS ERSTE AUF EIS GELEGT! (Evtl. später mit Data Desk arbeiten!)
 -"Black-Bars" für Oben und Unten implementieren!                               NICHT NOTWENDIG!  -> Windowed-Mode hat immer die gleiche Ratio wie Fullscreen!
 -Irgendwie Sound hinkriegen (Möglichst mit SDL_Mixer!)!                        NICHT WIRKLICH... -> Bereits entfernt, lol
 
 Very Nice To Haves™:
 -Auf SDL(1) umsteigen, für einfaches Porten zu Wii, 3ds, Vita, etc.            NOPE, NO PAIN NO GAIN, lol
 -Handy per QR-Code oder ID-Nummer verbinden und als Controller benutzen
 */
bool thread_sync = false;
bool game_started = false;

void game_start_event()
{
    if (game_started)
    {
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
        HCS_Body_add(e,10,800,2200,500);
        HCS_Sprite_add(e,"box.txt",HCS_Draw_Background0);
        HCS_Collider_add(e,LSD_Vec_new_float(0,0),LSD_Vec_new_int(0,0));

        HCS_Event_remove("game_start");
    }
    
}

void init_event()
{
    HCS_Entity e = HCS_Entity_create("Start_Button");
    HCS_Body_add(e,HCS_Screen_size_get().x / 2 + 500, 300, 600, 400);
    HCS_Sprite_add(e,"box.txt",HCS_Draw_Menu0);
    HCS_Clickable_add(e,&game_started,HCS_Click_on);

    e = HCS_Entity_create("Quit_Button");
    HCS_Body_add(e,HCS_Screen_size_get().x / 2 - 300, 300, 600, 400);
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


LSD_Thread_function(Controller_Server)
{
    LSD_Thread_init();
    int server_fd, new_socket;
    const char* template = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: %lu\n\n%s";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LSD_Log(LSD_ltERROR, "Socket wurde nicht erstellt!");
        exit(1);
    }

    struct sockaddr_in address;
    #define PORT 1234
    #define NUM_CONNECTIONS 1

    memset((char*)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        LSD_Log(LSD_ltERROR,"Bind ist fehlgeschlagen!");
        exit(1);
    }

    if (listen(server_fd, NUM_CONNECTIONS) < 0)
    {
        LSD_Log(LSD_ltERROR,"Listen ist fehlgeschlagen!");
        exit(1);
    }



    int addrlen = sizeof(address);

    while(running)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
        {
            LSD_Log(LSD_ltERROR, "Accept ist fehlgeschlagen!");
            exit(1);
        }


        char buffer[30000] = {0};

        int valread = read(new_socket, buffer, 1024);
        // system("clear");
        // LSD_Log(LSD_ltMESSAGE, "%s", buffer);
        if (valread < 0)
        {
            LSD_Log(LSD_ltWARNING, "Read konnte keine Bytes lesen!");
        }

        char* message = malloc(1);

        if(strstr(buffer, "GET") != NULL)
        {
            buffer[(int)(strstr(buffer, "\n") - buffer) - 10] = 0;

            char path[1024] = "server";
            strcat(path,buffer + 4);

            LSD_File* file = LSD_File_open(path);

            message = realloc(message,strlen(file->data) + strlen(template) + 100);

            sprintf(message,template,strlen(file->data),file->data);

            LSD_File_close(file);
        }
        else if(strstr(buffer, "POST") != NULL)
        {
            // buffer[(int)(strstr(buffer, "\n") - buffer)] = 0;
            // LSD_Log(LSD_ltMESSAGE,"%s", buffer);
            char* right_line = strstr(buffer,"Content-Type: ") + 14;
            right_line[(int)(strstr(right_line, "\n") - right_line)] = 0;
            // LSD_Log(LSD_ltMESSAGE,"%s", right_line);

           

            sscanf(right_line, "%d %d %d %d",&HCS_Input_A_down, &HCS_Input_B_down, &HCS_Input_Pad.x, &HCS_Input_Pad.y);
        }



        write(new_socket, message, strlen(message));

        free(message);

        close(new_socket);

        while(!thread_sync) {}
        thread_sync = false;

    }
    close(server_fd);

    LSD_Delta_remove("Controller_Server");
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
    HCS_Event_add("init",init_event);
    //Game-Loop
    while(running || LSD_Thread_used > 0)
    {
        LSD_Thread_system();
        if (running)
        {
            // HCS_Input_A_Ldown = HCS_Input_A_down;
            // HCS_Input_B_Ldown = HCS_Input_B_down;

            // HCS_Input_A_pressed = HCS_Input_A_down && !HCS_Input_A_Ldown;
            // HCS_Input_B_pressed = HCS_Input_B_down && !HCS_Input_B_Ldown;

            // HCS_Input_A_released = !HCS_Input_A_down && HCS_Input_A_Ldown;
            // HCS_Input_B_released = !HCS_Input_B_down && HCS_Input_B_Ldown;

            HCS_Sprite_system(LSD_Delta_none);
            HCS_Clickable_system(LSD_Delta_none);
            HCS_Update(LSD_Delta_none);
            HCS_Event_run();
            thread_sync = true;
        }
    }
    HCS_Entity_clear();
    //Library-Deinitialisierung
    HCS_Deinit();
    exit(0);
}
