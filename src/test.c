
//Meine Tools:
#include <LSD/LSD.h>//<- "Logging" System
#include "HCS.h"    //<- Entity Component System

void cursor_event()
{
    HCS_Cursor_button_get()->last_down = HCS_Cursor_button_get()->down;
    HCS_Cursor_button_get()->down = SDL_GetMouseState(&HCS_Cursor_position_get()->x,&HCS_Cursor_position_get()->y) & SDL_BUTTON_LMASK;
}

void init_event(int nothing)
{
    HCS_Entity_clear();
    HCS_Script_load("test.hcscript");
}

int main(int argc, char* argv[])
{
    HCS_Event_add("cursor",cursor_event);
    HCS_Collider_callback_list(HCS_Collider_STD_callback,"HCS_Collider_STD_callback");
    HCS_Clickable_callback_list(init_event,"init_event");

    HCS_Entity e = HCS_Entity_create("Init_Button");
    HCS_Body_add(e,-500,500,0,200);
    HCS_Sprite_add(e,"Start",1,4,true);
    HCS_Clickable_add(e,HCS_Click_toggle,HCS_Trig_released,"init_event",0);

    HCS_CONTINUE();
}