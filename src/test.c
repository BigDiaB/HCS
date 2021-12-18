
//Meine Tools:
#include <LSD/LSD.h>//<- "Logging" System
#include "HCS.h"	//<- Entity Component System

void cursor_event()
{
    HCS_Cursor_button_get()->last_down = HCS_Cursor_button_get()->down;
    HCS_Cursor_button_get()->down = SDL_GetMouseState(&HCS_Cursor_position_get()->x,&HCS_Cursor_position_get()->y) & SDL_BUTTON_LMASK;
}


#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H

#define NUM_COLOURS 16

struct HCS_editorData
{
    HCS_Sprite* Canvas[16 * 16];
    HCS_Sprite_raw colours[NUM_COLOURS];
    SDL_Texture* colour_tex[NUM_COLOURS];
    unsigned char RED[NUM_COLOURS];
    unsigned char GRN[NUM_COLOURS];
    unsigned char BLU[NUM_COLOURS];
    bool colour_selected[NUM_COLOURS];
    bool dummy_bool;
    bool save_for_real;
    char save_file_name[2200];
    
};

static struct HCS_editorData* editorData;

void sprite_editor_deinit(int nothing)
{
    HCS_Stop();
}

void clear_canvas(int nothing)
{
    int i,j;
    SDL_Surface* temp = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);
    HCS_Sprite_raw spr = {0};
    for (i = 0; i < 16 * 16; i++)
        editorData->Canvas[i]->raw = spr;
    for (j = 0; j < 16 * 16; j++)
    {
        SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,0,0,0));
        SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
        editorData->Canvas[j]->tex = SDL_CreateTextureFromSurface(HCS_Gfx_renderer_get(),temp);
    }
    SDL_FreeSurface(temp);
    LSD_Log(LSD_ltMESSAGE,"Canvas erfolgreich zurückgesetzt!");
}

void on_menu_click(int who)
{
    if (editorData->save_for_real)
        return;
    int i;
    for (i = 0; i < NUM_COLOURS; i++)
        editorData->colour_selected[i] = false;
    editorData->colour_selected[who] = true;
}

void on_canvas_click(int self)
{
    if (editorData->save_for_real)
        return;
    HCS_Sprite* this = HCS_Sprite_get(self);
    int i,j,index = -1;
    for (i = 0; i < NUM_COLOURS; i++)
        if (editorData->colour_selected[i])
            index = i;
    if (index == -1)
    {
        LSD_Log(LSD_ltWARNING,"Du musst erst eine Farbe auswählen du Idiot!");
        return;
    }
    
    for (j = 0; j < 8; j++)
        for (i = 0; i < 8; i++)
            this->raw = editorData->colours[index];
    
    SDL_Surface* temp = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);
    SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,this->raw.RED[0][0],this->raw.GRN[0][0],this->raw.BLU[0][0]));
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
    this->tex = SDL_CreateTextureFromSurface(HCS_Gfx_renderer_get(),temp);
    SDL_FreeSurface(temp);
}

void text_box_event()
{
    static char test[2056];
    if (!HCS_Entity_exist("Safe_Name"))
    {
        HCS_Text_input_get()[0] = 0;
        HCS_Entity e = HCS_Entity_create("Safe_Name");
        HCS_Body_add(e,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x / 4,200,100,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x / 24);
        HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
        HCS_Sprite_use_text(HCS_Entity_get_by_name("Safe_Name"),HCS_Text_input_get(),*HCS_Text_input_length_get());
        strcpy(test,HCS_Text_input_get());
    }
    if (0 != strcmp(test,HCS_Text_input_get()))
    {
        strcpy(test,HCS_Text_input_get());
        if (*HCS_Text_input_length_get() > 12)
        {
            HCS_Text_input_get()[12] = 0;
            *HCS_Text_input_length_get() = 11;
        }
        else
            HCS_Sprite_use_text(HCS_Entity_get_by_name("Safe_Name"),HCS_Text_input_get(),*HCS_Text_input_length_get());
    }
}

void cancel_safe(int what);

void on_safe_click(int nothing)
{
    if (editorData->save_for_real == false)
    {
        HCS_Entity e = HCS_Entity_create("Safe_Background");
        HCS_Body_add(e,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x / 4 - 20,100,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x / 2 + 20,700);
        HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);


        e = HCS_Entity_create("Done_Button");
        HCS_Body_add(e,(HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x) / 4, 650,100,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x / 24);
        HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
        HCS_Sprite_use_text(e,"Done",4);
        HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_off,HCS_Trig_released);
        HCS_Clickable_add_func(e,on_safe_click,0);
        HCS_Event_add("Safe_Name",text_box_event);
        
        e = HCS_Entity_create("Cancel_Button");
        HCS_Body_add(e,(HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x) / 2, 650,100,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x / 24);
        HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
        HCS_Sprite_use_text(e,"Cancel",6);
        HCS_Clickable_add(e,&editorData->save_for_real,HCS_Click_off,HCS_Trig_released);
        HCS_Clickable_add_func(e,cancel_safe,69420);
        
        editorData->save_for_real = true;
        
        return;
    }
    editorData->save_for_real = false;
    HCS_Entity_kill(HCS_Entity_get_by_name("Cancel_Button"));
    HCS_Entity_kill(HCS_Entity_get_by_name("Safe_Name"));
    HCS_Entity_kill(HCS_Entity_get_by_name("Safe_Background"));
    HCS_Entity_kill(HCS_Entity_get_by_name("Done_Button"));
    HCS_Event_remove("Safe_Name");
    
    if (nothing != 69420)
    {
        char* imng_data_format_string = "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n";
        char* img_data;

        img_data = malloc(strlen(imng_data_format_string) * 3 * 16);
#define CANVAS editorData->Canvas
        int i;
        int img_data_offset = 0;
        for (i = 0; i < 256; i+=16)
        {
            char temp[200000];
            sprintf(temp,imng_data_format_string,CANVAS[i]->raw.RED[0][0],CANVAS[i+1]->raw.RED[0][0],CANVAS[i+2]->raw.RED[0][0],CANVAS[i+3]->raw.RED[0][0],CANVAS[i+4]->raw.RED[0][0],CANVAS[i+5]->raw.RED[0][0],CANVAS[i+6]->raw.RED[0][0],CANVAS[i+7]->raw.RED[0][0],CANVAS[i+8]->raw.RED[0][0],CANVAS[i+9]->raw.RED[0][0],CANVAS[i+10]->raw.RED[0][0],CANVAS[i+11]->raw.RED[0][0],CANVAS[i+12]->raw.RED[0][0],CANVAS[i+13]->raw.RED[0][0],CANVAS[i+14]->raw.RED[0][0],CANVAS[i+15]->raw.RED[0][0]);
            strcat(img_data,temp);
        }

        img_data_offset = strlen(img_data);

        for (i = 0; i < 256; i+=16)
        {
            char temp[200000];
            sprintf(temp,imng_data_format_string,CANVAS[i]->raw.GRN[0][0],CANVAS[i+1]->raw.GRN[0][0],CANVAS[i+2]->raw.GRN[0][0],CANVAS[i+3]->raw.GRN[0][0],CANVAS[i+4]->raw.GRN[0][0],CANVAS[i+5]->raw.GRN[0][0],CANVAS[i+6]->raw.GRN[0][0],CANVAS[i+7]->raw.GRN[0][0],CANVAS[i+8]->raw.GRN[0][0],CANVAS[i+9]->raw.GRN[0][0],CANVAS[i+10]->raw.GRN[0][0],CANVAS[i+11]->raw.GRN[0][0],CANVAS[i+12]->raw.GRN[0][0],CANVAS[i+13]->raw.GRN[0][0],CANVAS[i+14]->raw.GRN[0][0],CANVAS[i+15]->raw.GRN[0][0]);
            strcat(img_data,temp);
        }

        img_data_offset = strlen(img_data);

        for (i = 0; i < 256; i+=16)
        {
            char temp[200000];
            sprintf(temp,imng_data_format_string,CANVAS[i]->raw.BLU[0][0],CANVAS[i+1]->raw.BLU[0][0],CANVAS[i+2]->raw.BLU[0][0],CANVAS[i+3]->raw.BLU[0][0],CANVAS[i+4]->raw.BLU[0][0],CANVAS[i+5]->raw.BLU[0][0],CANVAS[i+6]->raw.BLU[0][0],CANVAS[i+7]->raw.BLU[0][0],CANVAS[i+8]->raw.BLU[0][0],CANVAS[i+9]->raw.BLU[0][0],CANVAS[i+10]->raw.BLU[0][0],CANVAS[i+11]->raw.BLU[0][0],CANVAS[i+12]->raw.BLU[0][0],CANVAS[i+13]->raw.BLU[0][0],CANVAS[i+14]->raw.BLU[0][0],CANVAS[i+15]->raw.BLU[0][0]);
            strcat(img_data,temp);
        }
        
        strcpy(editorData->save_file_name,"touch assets/");
        strcat(editorData->save_file_name,HCS_Text_input_get());
        strcat(editorData->save_file_name,".hgx");
        system(editorData->save_file_name);
        
        strcpy(editorData->save_file_name,"assets/");
        strcat(editorData->save_file_name,HCS_Text_input_get());
        strcat(editorData->save_file_name,".hgx");
        LSD_File* save_file = LSD_File_open(editorData->save_file_name);
        
        LSD_File_write(save_file,img_data);
        
        LSD_File_close(save_file);
        free(img_data);
        LSD_Log(LSD_ltMESSAGE,"Canvas erfolgreich als Datei gespeichert!: %s",HCS_Text_input_get());
    }
}

void cancel_safe(int what)
{
    editorData->save_for_real = true;
    on_safe_click(what);
}

struct HCS_editorData HCS_editorData;
static struct HCS_editorData* editorData = &HCS_editorData;
int main(int argc, char* argv[])
{
    // editorData->RED[0] = 0;
    // editorData->GRN[0] = 0;
    // editorData->BLU[0] = 0;
    
    // editorData->RED[1] = 255;
    // editorData->GRN[1] = 255;
    // editorData->BLU[1] = 255;
    
    // editorData->RED[2] = 255;
    // editorData->GRN[2] = 0;
    // editorData->BLU[2] = 0;
    
    // editorData->RED[3] = 255;
    // editorData->GRN[3] = 255;
    // editorData->BLU[3] = 0;
    
    // editorData->RED[4] = 255;
    // editorData->GRN[4] = 0;
    // editorData->BLU[4] = 255;
    
    // editorData->RED[5] = 0;
    // editorData->GRN[5] = 255;
    // editorData->BLU[5] = 0;
    
    // editorData->RED[6] = 0;
    // editorData->GRN[6] = 255;
    // editorData->BLU[6] = 255;
    
    // editorData->RED[7] = 0;
    // editorData->GRN[7] = 0;
    // editorData->BLU[7] = 255;
    
    // editorData->RED[8] = 20;
    // editorData->GRN[8] = 20;
    // editorData->BLU[8] = 20;
    
    // editorData->RED[9] = 100;
    // editorData->GRN[9] = 20;
    // editorData->BLU[9] = 20;
    
    // editorData->RED[10] = 20;
    // editorData->GRN[10] = 100;
    // editorData->BLU[10] = 20;
    
    // editorData->RED[11] = 20;
    // editorData->GRN[11] = 20;
    // editorData->BLU[11] = 100;
    
    // editorData->RED[12] = 100;
    // editorData->GRN[12] = 100;
    // editorData->BLU[12] = 20;
    
    // editorData->RED[13] = 100;
    // editorData->GRN[13] = 20;
    // editorData->BLU[13] = 100;
    
    // editorData->RED[14] = 20;
    // editorData->GRN[14] = 100;
    // editorData->BLU[14] = 100;
    
    // editorData->RED[15] = 254;
    // editorData->GRN[15] = 0;
    // editorData->BLU[15] = 0;
    // int i,j,k;
    
    // for (k = 0; k < NUM_COLOURS; k++)
    // {
    //     for (j = 0; j < 8; j++)
    //         for (i = 0; i < 8; i++)
    //         {
    //             editorData->colours[k].RED[j][i] = editorData->RED[k];
    //             editorData->colours[k].GRN[j][i] = editorData->GRN[k];
    //             editorData->colours[k].BLU[j][i] = editorData->BLU[k];
    //         }
    // }
    
    // SDL_Rect r;
    // SDL_Surface* temp = SDL_CreateRGBSurface(0,8,8,32,0,0,0,0);
    // r.w = 1;
    // r.h = 1;
    // for (k = 0; k < NUM_COLOURS; k++)
    // {
    //     for (i = 0; i < 8; i++)
    //         for (j = 0; j < 8; j++)
    //         {
    //             r.x = j;
    //             r.y = i;
    //             SDL_FillRect(temp,&r,SDL_MapRGB(temp->format,editorData->colours[k].RED[j][i],editorData->colours[k].GRN[j][i],editorData->colours[k].BLU[j][i]));
    //         }
        
    //     SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
        
    //     editorData->colour_tex[k] = SDL_CreateTextureFromSurface(HCS_Gfx_renderer_get(),temp);
    // }
    
    // SDL_FreeSurface(temp);
    

    // HCS_Entity e = HCS_Entity_create("Draw_Background");
    // HCS_Body_add(e,(HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x) / 2 - 320, 80, 100 * 8 + 40, 100 * 8 + 40);
    // HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);

    // int row = 0, collum = 0, index = 0;
    // index = 0;
    // char name[20] = {0};
    // char name_num[4];
    // index = 0;
    // for (collum = 0; collum < 16; collum++)
    //     for (row = 0; row < 16; row++)
    //     {
    //         strcpy(name,"canvas");
    //         sprintf(name_num, "%d", index);
    //         strcat(name,name_num);
    //         HCS_Entity e = HCS_Entity_create(name);
    //         HCS_Body_add(e,(HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x) / 2 - 50 * (-9 + row), 100 + (collum) * 50, 45, 45);
    //         HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
    //         HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    //         HCS_Clickable_add_func(e,on_canvas_click,e);
    //         editorData->Canvas[index] = HCS_Sprite_get(e);
    //         index++;
    //     }

    // row = 0;
    // collum = 0;
    // index = 0;
    // char name2[20] = {0};
    // char name_num2[4];
    // index = 0;
    // for (collum = 0; collum < 4; collum++)
    //     for (row = 0; row < 4; row++)
    //     {
    //         strcpy(name2,"colour");
    //         sprintf(name_num2, "%d", index);
    //         strcat(name2,name_num2);
    //         HCS_Entity e = HCS_Entity_create(name2);
    //         HCS_Body_add(e,(HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    //         HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
    //         HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    //         HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    //         HCS_Clickable_add_func(e,on_menu_click,index);
    //         index++;
    //     }
    
    
    
    // e = HCS_Entity_create("Save_Button");
    // HCS_Body_add(e,10,100,100,75);
    // HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
    // HCS_Sprite_use_text(e,"Safe",4);
    // HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    // HCS_Clickable_add_func(e,on_safe_click,index);
    
    // e = HCS_Entity_create("Clear_Button");
    // HCS_Body_add(e,10,200,100,75);
    // HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
    // HCS_Sprite_use_text(e,"Clear",5);
    // HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    // HCS_Clickable_add_func(e,clear_canvas,index);
    
    // e = HCS_Entity_create("Quit_Button");
    // HCS_Body_add(e,10,300,100,75);
    // HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Decal);
    // HCS_Sprite_use_text(e,"Quit",4);
    // HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    // HCS_Clickable_add_func(e,sprite_editor_deinit,index);
    
    // HCS_Event_add("cursor_event",cursor_event);
    
    // editorData->save_for_real = false;
    // clear_canvas(0);

    HCS_Entity e = HCS_Entity_create("Ground");
    HCS_Body_add(e,0,HCS_Screen_size_get().y * HCS_Gfx_stretch_get().y,HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x,400);
    HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Sprite);
    HCS_Collider_add(e,LSD_Vec_new_float(0,0),LSD_Vec_new_int(0,0),HCS_Collider_STD_callback);
    e = HCS_Entity_create("Player");
    HCS_State_add(e);
    HCS_Body_add(e,0,0,400,400);
    HCS_Sprite_add(e,"assets/default.hgx",HCS_Draw_Sprite);
    HCS_Input_add(e);
    HCS_Movement_add(e,4000,4000);
    HCS_Collider_add(e,LSD_Vec_new_float(0,0),LSD_Vec_new_int(0,0),HCS_Collider_STD_callback);
    HCS_CONTINUE();
}

#endif
