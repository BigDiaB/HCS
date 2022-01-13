
//Meine Tools:
#include <LSD/LSD.h>//<- "Logging" System
#include "HCS.h"    //<- Entity Component System


#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H

void cursor_event()
{
    HCS_Cursor_button_get()->last_down = HCS_Cursor_button_get()->down;
    HCS_Cursor_button_get()->down = SDL_GetMouseState(&HCS_Cursor_position_get()->x,&HCS_Cursor_position_get()->y) & SDL_BUTTON_LMASK;
}
struct HCS_SpriteEditorData
{
    HCS_Sprite* Canvas[16 * 16];
    HCS_Sprite_raw colours[16];
    SDL_Texture* colour_tex[16];
    unsigned char RED[16];
    unsigned char GRN[16];
    unsigned char BLU[16];
    bool colour_selected[16];
    bool save_for_real;
    char save_file_name[2200];
    
};

static struct HCS_SpriteEditorData* editorData;

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
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltMESSAGE,"Canvas erfolgreich zurückgesetzt!");
    #endif
}

void on_menu_click(int who)
{
    if (editorData->save_for_real)
        return;
    int i;
    for (i = 0; i < 16; i++)
        editorData->colour_selected[i] = false;
    editorData->colour_selected[who] = true;
}

void on_canvas_click(int name_index)
{
    if (editorData->save_for_real)
        return;

    char name[100];
    char name_num[10];
    strcpy(name,"canvas");
    sprintf(name_num, "%d", name_index);
    strcat(name,name_num);

    HCS_Sprite* this = HCS_Sprite_get(HCS_Entity_get_by_name(name));
    int i,j,index = -1;
    for (i = 0; i < 16; i++)
        if (editorData->colour_selected[i])
            index = i;
    if (index == -1)
    {
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltWARNING,"Du musst erst eine Farbe auswählen du Idiot!");
        #endif
        return;
    }
    
    this->raw = editorData->colours[index];
    SDL_Surface* temp = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);
    SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,editorData->colours[index].RED[0][0],editorData->colours[index].GRN[0][0],editorData->colours[index].BLU[0][0]));
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
    this->tex = SDL_CreateTextureFromSurface(HCS_Gfx_renderer_get(),temp);
    SDL_FreeSurface(temp);
}

void text_box_event()
{
    static int last_len = 0;
    if (!HCS_Entity_exist("Safe_Name"))
    {
        memset(HCS_Text_input_get(),0,*HCS_Text_input_length_get());
        *HCS_Text_input_length_get() = 0;
        HCS_Entity e = HCS_Entity_create("Safe_Name");
        HCS_Body_add(e,-595,200,100,100);
        HCS_Sprite_add(e,HCS_Text_input_get(),5,4, true);
    }
    if (*HCS_Text_input_length_get() != last_len)
    {
        last_len = *HCS_Text_input_length_get();
        if (*HCS_Text_input_length_get() > 11)
        {
            HCS_Text_input_get()[12] = 0;
            *HCS_Text_input_length_get() = 11;
        }
        else
        {
            HCS_Sprite_remove(HCS_Entity_get_by_name("Safe_Name"));
            HCS_Sprite_add(HCS_Entity_get_by_name("Safe_Name"),HCS_Text_input_get(),5,4, true);
        }
    }
}

void cancel_safe(int what);

void on_safe_click(int nothing)
{
    if (editorData->save_for_real == false)
    {
        HCS_Entity e = HCS_Entity_create("Safe_Background");
        HCS_Body_add(e,-600,100,1200,700);
        HCS_Sprite_add(e,"assets/default.hgx",4,4,false);


        e = HCS_Entity_create("Done_Button");
        HCS_Body_add(e,150,650,100,100);
        HCS_Sprite_add(e,"Done",5,4,true);
        HCS_Clickable_add(e,HCS_Click_off,HCS_Trig_released,"on_safe_click",0);

        HCS_Event_add("Safe_Name",text_box_event);
        
        e = HCS_Entity_create("Cancel_Button");
        HCS_Body_add(e,-550,650,100,100);
        HCS_Sprite_add(e,"Cancel",5,4,true);
        HCS_Clickable_add(e,HCS_Click_off,HCS_Trig_released,"cancel_safe",69420);
        
        editorData->save_for_real = true;
        
        return;
    }
    editorData->save_for_real = false;
    if (HCS_Entity_exist("Cancel_Button"))
        HCS_Entity_kill(HCS_Entity_get_by_name("Cancel_Button"));
    if (HCS_Entity_exist("Safe_Name"))
        HCS_Entity_kill(HCS_Entity_get_by_name("Safe_Name"));
    if (HCS_Entity_exist("Safe_Background"))
        HCS_Entity_kill(HCS_Entity_get_by_name("Safe_Background"));
    if (HCS_Entity_exist("Done_Button"))
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
            sprintf(temp,imng_data_format_string,CANVAS[i]->raw.RED[0][0],CANVAS[i+1]->raw.RED[0][0],CANVAS[i+2]->raw.RED[0][0],CANVAS[i+3]->raw.RED[0][0],CANVAS[i+4]->raw.RED[0][0],
                    CANVAS[i+5]->raw.RED[0][0],CANVAS[i+6]->raw.RED[0][0],CANVAS[i+7]->raw.RED[0][0],CANVAS[i+8]->raw.RED[0][0],CANVAS[i+9]->raw.RED[0][0],CANVAS[i+10]->raw.RED[0][0],
                    CANVAS[i+11]->raw.RED[0][0],CANVAS[i+12]->raw.RED[0][0],CANVAS[i+13]->raw.RED[0][0],CANVAS[i+14]->raw.RED[0][0],CANVAS[i+15]->raw.RED[0][0]);
            strcat(img_data,temp);
        }

        img_data_offset = strlen(img_data);

        for (i = 0; i < 256; i+=16)
        {
            char temp[200000];
            sprintf(temp,imng_data_format_string,CANVAS[i]->raw.GRN[0][0],CANVAS[i+1]->raw.GRN[0][0],CANVAS[i+2]->raw.GRN[0][0],CANVAS[i+3]->raw.GRN[0][0],CANVAS[i+4]->raw.GRN[0][0],
                    CANVAS[i+5]->raw.GRN[0][0],CANVAS[i+6]->raw.GRN[0][0],CANVAS[i+7]->raw.GRN[0][0],CANVAS[i+8]->raw.GRN[0][0],CANVAS[i+9]->raw.GRN[0][0],CANVAS[i+10]->raw.GRN[0][0],
                    CANVAS[i+11]->raw.GRN[0][0],CANVAS[i+12]->raw.GRN[0][0],CANVAS[i+13]->raw.GRN[0][0],CANVAS[i+14]->raw.GRN[0][0],CANVAS[i+15]->raw.GRN[0][0]);
            strcat(img_data,temp);
        }

        img_data_offset = strlen(img_data);

        for (i = 0; i < 256; i+=16)
        {
            char temp[200000];
            sprintf(temp,imng_data_format_string,CANVAS[i]->raw.BLU[0][0],CANVAS[i+1]->raw.BLU[0][0],CANVAS[i+2]->raw.BLU[0][0],CANVAS[i+3]->raw.BLU[0][0],CANVAS[i+4]->raw.BLU[0][0],
                    CANVAS[i+5]->raw.BLU[0][0],CANVAS[i+6]->raw.BLU[0][0],CANVAS[i+7]->raw.BLU[0][0],CANVAS[i+8]->raw.BLU[0][0],CANVAS[i+9]->raw.BLU[0][0],CANVAS[i+10]->raw.BLU[0][0],
                    CANVAS[i+11]->raw.BLU[0][0],CANVAS[i+12]->raw.BLU[0][0],CANVAS[i+13]->raw.BLU[0][0],CANVAS[i+14]->raw.BLU[0][0],CANVAS[i+15]->raw.BLU[0][0]);
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
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltMESSAGE,"Canvas erfolgreich als Datei gespeichert!: %s",HCS_Text_input_get());
        #endif
#undef CANVAS
    }
}

void cancel_safe(int what)
{
    editorData->save_for_real = true;
    on_safe_click(what);
}

struct HCS_SpriteEditorData HCS_SpriteEditorData = {0};
static struct HCS_SpriteEditorData* editorData = &HCS_SpriteEditorData;
int main(int argc, char* argv[])
{
    HCS_Collider_callback_list(HCS_Collider_STD_callback,"HCS_Collider_STD_callback");
    HCS_Clickable_callback_list(sprite_editor_deinit,"sprite_editor_deinit");
    HCS_Clickable_callback_list(clear_canvas,"clear_canvas");
    HCS_Clickable_callback_list(on_menu_click,"on_menu_click");
    HCS_Clickable_callback_list(on_canvas_click,"on_canvas_click");
    HCS_Clickable_callback_list(cancel_safe,"cancel_safe");
    HCS_Clickable_callback_list(on_safe_click,"on_safe_click");


    editorData->RED[0] = 0;
    editorData->GRN[0] = 0;
    editorData->BLU[0] = 0;
    
    editorData->RED[1] = 255;
    editorData->GRN[1] = 255;
    editorData->BLU[1] = 255;
    
    editorData->RED[2] = 255;
    editorData->GRN[2] = 0;
    editorData->BLU[2] = 0;
    
    editorData->RED[3] = 255;
    editorData->GRN[3] = 255;
    editorData->BLU[3] = 0;
    
    editorData->RED[4] = 255;
    editorData->GRN[4] = 0;
    editorData->BLU[4] = 255;
    
    editorData->RED[5] = 0;
    editorData->GRN[5] = 255;
    editorData->BLU[5] = 0;
    
    editorData->RED[6] = 0;
    editorData->GRN[6] = 255;
    editorData->BLU[6] = 255;
    
    editorData->RED[7] = 0;
    editorData->GRN[7] = 0;
    editorData->BLU[7] = 255;
    
    editorData->RED[8] = 20;
    editorData->GRN[8] = 20;
    editorData->BLU[8] = 20;
    
    editorData->RED[9] = 100;
    editorData->GRN[9] = 20;
    editorData->BLU[9] = 20;
    
    editorData->RED[10] = 20;
    editorData->GRN[10] = 100;
    editorData->BLU[10] = 20;
    
    editorData->RED[11] = 20;
    editorData->GRN[11] = 20;
    editorData->BLU[11] = 100;
    
    editorData->RED[12] = 100;
    editorData->GRN[12] = 100;
    editorData->BLU[12] = 20;
    
    editorData->RED[13] = 100;
    editorData->GRN[13] = 20;
    editorData->BLU[13] = 100;
    
    editorData->RED[14] = 20;
    editorData->GRN[14] = 100;
    editorData->BLU[14] = 100;
    
    editorData->RED[15] = 254;
    editorData->GRN[15] = 0;
    editorData->BLU[15] = 0;
    int i,j,k;
    
    for (k = 0; k < 16; k++)
    {
        for (j = 0; j < 8; j++)
            for (i = 0; i < 8; i++)
            {
                editorData->colours[k].RED[j][i] = editorData->RED[k];
                editorData->colours[k].GRN[j][i] = editorData->GRN[k];
                editorData->colours[k].BLU[j][i] = editorData->BLU[k];
            }
    }

    HCS_Entity e = HCS_Entity_create("Draw_Background");
    HCS_Body_add(e,-50 * 8 + 80, 80, 100 * 8 + 40, 100 * 8 + 40);
    HCS_Sprite_add(e,"assets/default.hgx",3,4,false);

    int row = 0, collum = 0, index = 0;
    index = 0;
    char name[30] = {0};
    char name_num[10];
    index = 0;
    for (collum = 0; collum < 16; collum++)
        for (row = 0; row < 16; row++)
        {
            strcpy(name,"canvas");
            sprintf(name_num, "%d", index);
            strcat(name,name_num);
            HCS_Entity e = HCS_Entity_create(name);
            HCS_Body_add(e,- 50 * (-9 + row), 100 + (collum) * 50, 45, 45);
            HCS_Sprite_add(e,"assets/default.hgx",4,4,false);
            HCS_Clickable_add(e,HCS_Click_toggle,HCS_Trig_down,"on_canvas_click",index);
            editorData->Canvas[index] = HCS_Sprite_get(e);
            index++;
        }

    row = 0;
    collum = 0;
    index = 0;
    for (collum = 0; collum < 4; collum++)
        for (row = 0; row < 4; row++)
        {
            strcpy(name,"colour");
            sprintf(name_num, "%d", index);
            strcat(name,name_num);
            HCS_Entity e = HCS_Entity_create(name);
            HCS_Body_add(e,(-60 - 60 * row), 100 + (collum) * 60, 50, 50);
            HCS_Sprite_primitive_add(e, 4, 3, editorData->colours[collum * 4 + row].RED[0][0],editorData->colours[collum * 4 + row].GRN[0][0],editorData->colours[collum * 4 + row].BLU[0][0]);
            HCS_Clickable_add(e,HCS_Click_toggle,HCS_Trig_released,"on_menu_click",index);
            index++;
        }
    
    
    
    e = HCS_Entity_create("Save_Button");
    HCS_Body_add(e,0,100,100,75);
    HCS_Sprite_add(e,"Safe",3,2,true);
    HCS_Clickable_add(e,HCS_Click_toggle,HCS_Trig_released,"on_safe_click",index);
    
    e = HCS_Entity_create("Clear_Button");
    HCS_Body_add(e,0,200,100,75);
    HCS_Sprite_add(e,"Clear",3,2,true);
    HCS_Clickable_add(e,HCS_Click_toggle,HCS_Trig_released,"clear_canvas",index);
    
    e = HCS_Entity_create("Quit_Button");
    HCS_Body_add(e,0,300,100,75);
    HCS_Sprite_add(e,"Quit",3,2,true);
    HCS_Clickable_add(e,HCS_Click_toggle,HCS_Trig_released,"sprite_editor_deinit",index);
    
    HCS_Event_add("cursor_event",cursor_event);
    
    clear_canvas(0);
    
    // HCS_Entity_clear();

    // HCS_Script_load("test.hcscript");

    HCS_CONTINUE();
}

#endif