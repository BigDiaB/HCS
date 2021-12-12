
//Meine Tools:
#include <LSD/LSD.h>//<- "Logging" System
#include "HCS.h"	//<- Entity Component System

extern struct HCS_runData* runData;
extern HCS_Gfx_Color color;
extern HCS_Gfx_Color std;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern HCS_Gfx_Rectangle WIN_SIZE;
extern bool HCS_Gfx_Mouse_clicked;
extern bool HCS_Gfx_Mouse_last_clicked;
extern bool HCS_Input_A_Ldown;
extern bool HCS_Input_A_down;
extern bool HCS_Input_A_pressed;
extern bool HCS_Input_A_released;
extern bool HCS_Input_B_Ldown;
extern bool HCS_Input_B_down;
extern bool HCS_Input_B_pressed;
extern bool HCS_Input_B_released;
extern LSD_Vec2i HCS_Input_Pad;
extern bool HCS_Input_Menu;
extern LSD_Vec2f HCS_Gfx_Camera;
extern LSD_Vec2i HCS_Gfx_Mouse_pos;
extern double WORLD_TO_SCREEN_X;
extern double WORLD_TO_SCREEN_Y;
extern double DRAW_OFFSET;
extern double STRETCH_WIDTH;
extern double STRETCH_HEIGHT;
extern bool fullscreen;
extern bool running;
extern char HCS_TextInput[2056];
extern int HCS_TextSize;

void cursor_event()
{
    HCS_Gfx_Mouse_last_clicked = HCS_Gfx_Mouse_clicked;
    HCS_Gfx_Mouse_clicked = SDL_GetMouseState(&HCS_Gfx_Mouse_pos.x,&HCS_Gfx_Mouse_pos.y) & SDL_BUTTON_LMASK;
}


#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H

#define NUM_COLOURS 16

struct HCS_editorData
{
    HCS_Sprite* Canvas[64];
    HCS_Sprite_raw colours[NUM_COLOURS];
    HCS_Gfx_Texture colour_tex[NUM_COLOURS];
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
    running = false;
}

void clear_canvas(int nothing)
{
    int i,j;
    HCS_Gfx_Surface temp = SDL_CreateRGBSurface(0,8,8,32,0,0,0,0);
    HCS_Sprite_raw spr = {0};
    for (i = 0; i < 64; i++)
        editorData->Canvas[i]->raw = spr;
    for (j = 0; j < 64; j++)
    {
        SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,0,0,0));
        SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
        editorData->Canvas[j]->tex = SDL_CreateTextureFromSurface(renderer,temp);
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
    
    HCS_Gfx_Surface temp = SDL_CreateRGBSurface(0,8,8,32,0,0,0,0);
    SDL_FillRect(temp,NULL,SDL_MapRGB(temp->format,this->raw.RED[0][0],this->raw.GRN[0][0],this->raw.BLU[0][0]));
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
    this->tex = SDL_CreateTextureFromSurface(renderer,temp);
    SDL_FreeSurface(temp);
}

void text_box_event()
{
    static char test[2056];
    if (!HCS_Entity_exist("Safe_Name"))
    {
        HCS_TextInput[0] = 0;
        HCS_Entity e = HCS_Entity_create("Safe_Name");
        HCS_Body_add(e,HCS_Screen_size_get().x * STRETCH_WIDTH / 4,200,100,HCS_Screen_size_get().x * STRETCH_WIDTH / 24);
        HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
        HCS_Sprite_use_text(HCS_Entity_get_by_name("Safe_Name"),HCS_TextInput,strlen(HCS_TextInput));
        strcpy(test,HCS_TextInput);
    }
    if (0 != strcmp(test,HCS_TextInput))
    {
        HCS_Sprite_use_text(HCS_Entity_get_by_name("Safe_Name"),HCS_TextInput,strlen(HCS_TextInput));
        strcpy(test,HCS_TextInput);
        if (strlen(HCS_TextInput) > 12)
        {
            HCS_TextInput[12] = 0;
            HCS_TextSize = 11;
        }
    }
}

void cancel_safe(int what);

void on_safe_click(int nothing)
{
    if (editorData->save_for_real == false)
    {
        HCS_Entity e = HCS_Entity_create("Safe_Background");
        HCS_Body_add(e,HCS_Screen_size_get().x * STRETCH_WIDTH / 4 - 20,100,HCS_Screen_size_get().x * STRETCH_WIDTH / 2 + 20,700);
        HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);


        e = HCS_Entity_create("Done_Button");
        HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH) / 4, 650,100,HCS_Screen_size_get().x * STRETCH_WIDTH / 24);
        HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
        HCS_Sprite_use_text(e,"Done",4);
        HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_off,HCS_Trig_released);
        HCS_Clickable_add_func(e,on_safe_click,0);
        HCS_Event_add("Safe_Name",text_box_event);
        
        e = HCS_Entity_create("Cancel_Button");
        HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH) / 2, 650,100,HCS_Screen_size_get().x * STRETCH_WIDTH / 24);
        HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
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
        char* img_data;
        char* format_string = "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu";
        img_data = malloc(strlen(format_string));
#define CANVAS editorData->Canvas
        sprintf(img_data,format_string,
                CANVAS[0]->raw.RED[0][0],CANVAS[1]->raw.RED[0][0],CANVAS[2]->raw.RED[0][0],CANVAS[3]->raw.RED[0][0],CANVAS[4]->raw.RED[0][0],CANVAS[5]->raw.RED[0][0],CANVAS[6]->raw.RED[0][0],CANVAS[7]->raw.RED[0][0],
                CANVAS[8]->raw.RED[0][0],CANVAS[9]->raw.RED[0][0],CANVAS[10]->raw.RED[0][0],CANVAS[11]->raw.RED[0][0],CANVAS[12]->raw.RED[0][0],CANVAS[13]->raw.RED[0][0],CANVAS[14]->raw.RED[0][0],CANVAS[15]->raw.RED[0][0],
                CANVAS[16]->raw.RED[0][0],CANVAS[17]->raw.RED[0][0],CANVAS[18]->raw.RED[0][0],CANVAS[19]->raw.RED[0][0],CANVAS[20]->raw.RED[0][0],CANVAS[21]->raw.RED[0][0],CANVAS[22]->raw.RED[0][0],CANVAS[23]->raw.RED[0][0],
                CANVAS[24]->raw.RED[0][0],CANVAS[25]->raw.RED[0][0],CANVAS[26]->raw.RED[0][0],CANVAS[27]->raw.RED[0][0],CANVAS[28]->raw.RED[0][0],CANVAS[29]->raw.RED[0][0],CANVAS[30]->raw.RED[0][0],CANVAS[31]->raw.RED[0][0],
                CANVAS[32]->raw.RED[0][0],CANVAS[33]->raw.RED[0][0],CANVAS[34]->raw.RED[0][0],CANVAS[35]->raw.RED[0][0],CANVAS[36]->raw.RED[0][0],CANVAS[37]->raw.RED[0][0],CANVAS[38]->raw.RED[0][0],CANVAS[39]->raw.RED[0][0],
                CANVAS[40]->raw.RED[0][0],CANVAS[41]->raw.RED[0][0],CANVAS[42]->raw.RED[0][0],CANVAS[43]->raw.RED[0][0],CANVAS[44]->raw.RED[0][0],CANVAS[45]->raw.RED[0][0],CANVAS[46]->raw.RED[0][0],CANVAS[47]->raw.RED[0][0],
                CANVAS[48]->raw.RED[0][0],CANVAS[49]->raw.RED[0][0],CANVAS[50]->raw.RED[0][0],CANVAS[51]->raw.RED[0][0],CANVAS[52]->raw.RED[0][0],CANVAS[53]->raw.RED[0][0],CANVAS[54]->raw.RED[0][0],CANVAS[55]->raw.RED[0][0],
                CANVAS[56]->raw.RED[0][0],CANVAS[57]->raw.RED[0][0],CANVAS[58]->raw.RED[0][0],CANVAS[59]->raw.RED[0][0],CANVAS[60]->raw.RED[0][0],CANVAS[61]->raw.RED[0][0],CANVAS[62]->raw.RED[0][0],CANVAS[63]->raw.RED[0][0],
                
                CANVAS[0]->raw.GRN[0][0],CANVAS[1]->raw.GRN[0][0],CANVAS[2]->raw.GRN[0][0],CANVAS[3]->raw.GRN[0][0],CANVAS[4]->raw.GRN[0][0],CANVAS[5]->raw.GRN[0][0],CANVAS[6]->raw.GRN[0][0],CANVAS[7]->raw.GRN[0][0],
                CANVAS[8]->raw.GRN[0][0],CANVAS[9]->raw.GRN[0][0],CANVAS[10]->raw.GRN[0][0],CANVAS[11]->raw.GRN[0][0],CANVAS[12]->raw.GRN[0][0],CANVAS[13]->raw.GRN[0][0],CANVAS[14]->raw.GRN[0][0],CANVAS[15]->raw.GRN[0][0],
                CANVAS[16]->raw.GRN[0][0],CANVAS[17]->raw.GRN[0][0],CANVAS[18]->raw.GRN[0][0],CANVAS[19]->raw.GRN[0][0],CANVAS[20]->raw.GRN[0][0],CANVAS[21]->raw.GRN[0][0],CANVAS[22]->raw.GRN[0][0],CANVAS[23]->raw.GRN[0][0],
                CANVAS[24]->raw.GRN[0][0],CANVAS[25]->raw.GRN[0][0],CANVAS[26]->raw.GRN[0][0],CANVAS[27]->raw.GRN[0][0],CANVAS[28]->raw.GRN[0][0],CANVAS[29]->raw.GRN[0][0],CANVAS[30]->raw.GRN[0][0],CANVAS[31]->raw.GRN[0][0],
                CANVAS[32]->raw.GRN[0][0],CANVAS[33]->raw.GRN[0][0],CANVAS[34]->raw.GRN[0][0],CANVAS[35]->raw.GRN[0][0],CANVAS[36]->raw.GRN[0][0],CANVAS[37]->raw.GRN[0][0],CANVAS[38]->raw.GRN[0][0],CANVAS[39]->raw.GRN[0][0],
                CANVAS[40]->raw.GRN[0][0],CANVAS[41]->raw.GRN[0][0],CANVAS[42]->raw.GRN[0][0],CANVAS[43]->raw.GRN[0][0],CANVAS[44]->raw.GRN[0][0],CANVAS[45]->raw.GRN[0][0],CANVAS[46]->raw.GRN[0][0],CANVAS[47]->raw.GRN[0][0],
                CANVAS[48]->raw.GRN[0][0],CANVAS[49]->raw.GRN[0][0],CANVAS[50]->raw.GRN[0][0],CANVAS[51]->raw.GRN[0][0],CANVAS[52]->raw.GRN[0][0],CANVAS[53]->raw.GRN[0][0],CANVAS[54]->raw.GRN[0][0],CANVAS[55]->raw.GRN[0][0],
                CANVAS[56]->raw.GRN[0][0],CANVAS[57]->raw.GRN[0][0],CANVAS[58]->raw.GRN[0][0],CANVAS[59]->raw.GRN[0][0],CANVAS[60]->raw.GRN[0][0],CANVAS[61]->raw.GRN[0][0],CANVAS[62]->raw.GRN[0][0],CANVAS[63]->raw.GRN[0][0],
                
                CANVAS[0]->raw.BLU[0][0],CANVAS[1]->raw.BLU[0][0],CANVAS[2]->raw.BLU[0][0],CANVAS[3]->raw.BLU[0][0],CANVAS[4]->raw.BLU[0][0],CANVAS[5]->raw.BLU[0][0],CANVAS[06]->raw.BLU[0][0],CANVAS[7]->raw.BLU[0][0],
                CANVAS[8]->raw.BLU[0][0],CANVAS[9]->raw.BLU[0][0],CANVAS[10]->raw.BLU[0][0],CANVAS[11]->raw.BLU[0][0],CANVAS[12]->raw.BLU[0][0],CANVAS[13]->raw.BLU[0][0],CANVAS[14]->raw.BLU[0][0],CANVAS[15]->raw.BLU[0][0],
                CANVAS[16]->raw.BLU[0][0],CANVAS[17]->raw.BLU[0][0],CANVAS[18]->raw.BLU[0][0],CANVAS[19]->raw.BLU[0][0],CANVAS[20]->raw.BLU[0][0],CANVAS[21]->raw.BLU[0][0],CANVAS[22]->raw.BLU[0][0],CANVAS[23]->raw.BLU[0][0],
                CANVAS[24]->raw.BLU[0][0],CANVAS[25]->raw.BLU[0][0],CANVAS[26]->raw.BLU[0][0],CANVAS[27]->raw.BLU[0][0],CANVAS[28]->raw.BLU[0][0],CANVAS[29]->raw.BLU[0][0],CANVAS[30]->raw.BLU[0][0],CANVAS[31]->raw.BLU[0][0],
                CANVAS[32]->raw.BLU[0][0],CANVAS[33]->raw.BLU[0][0],CANVAS[34]->raw.BLU[0][0],CANVAS[35]->raw.BLU[0][0],CANVAS[36]->raw.BLU[0][0],CANVAS[37]->raw.BLU[0][0],CANVAS[38]->raw.BLU[0][0],CANVAS[39]->raw.BLU[0][0],
                CANVAS[40]->raw.BLU[0][0],CANVAS[41]->raw.BLU[0][0],CANVAS[42]->raw.BLU[0][0],CANVAS[43]->raw.BLU[0][0],CANVAS[44]->raw.BLU[0][0],CANVAS[45]->raw.BLU[0][0],CANVAS[46]->raw.BLU[0][0],CANVAS[47]->raw.BLU[0][0],
                CANVAS[48]->raw.BLU[0][0],CANVAS[49]->raw.BLU[0][0],CANVAS[50]->raw.BLU[0][0],CANVAS[51]->raw.BLU[0][0],CANVAS[52]->raw.BLU[0][0],CANVAS[53]->raw.BLU[0][0],CANVAS[54]->raw.BLU[0][0],CANVAS[55]->raw.BLU[0][0],
                CANVAS[56]->raw.BLU[0][0],CANVAS[57]->raw.BLU[0][0],CANVAS[58]->raw.BLU[0][0],CANVAS[59]->raw.BLU[0][0],CANVAS[60]->raw.BLU[0][0],CANVAS[61]->raw.BLU[0][0],CANVAS[62]->raw.BLU[0][0],CANVAS[63]->raw.BLU[0][0]
                );
        
        strcpy(editorData->save_file_name,"touch assets/");
        strcat(editorData->save_file_name,HCS_TextInput);
        strcat(editorData->save_file_name,".hgx");
        system(editorData->save_file_name);
        
        strcpy(editorData->save_file_name,"assets/");
        strcat(editorData->save_file_name,HCS_TextInput);
        strcat(editorData->save_file_name,".hgx");
        LSD_File* save_file = LSD_File_open(editorData->save_file_name);
        
        LSD_File_write(save_file,img_data);
        
        LSD_File_close(save_file);
        free(img_data);
        LSD_Log(LSD_ltMESSAGE,"Canvas erfolgreich als Datei gespeichert!: %s",HCS_TextInput);
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
    
    for (k = 0; k < NUM_COLOURS; k++)
    {
        for (j = 0; j < 8; j++)
            for (i = 0; i < 8; i++)
            {
                editorData->colours[k].RED[j][i] = editorData->RED[k];
                editorData->colours[k].GRN[j][i] = editorData->GRN[k];
                editorData->colours[k].BLU[j][i] = editorData->BLU[k];
            }
    }
    
    HCS_Gfx_Rectangle r;
    HCS_Gfx_Surface temp = SDL_CreateRGBSurface(0,8,8,32,0,0,0,0);
    r.w = 1;
    r.h = 1;
    for (k = 0; k < NUM_COLOURS; k++)
    {
        for (i = 0; i < 8; i++)
            for (j = 0; j < 8; j++)
            {
                r.x = j;
                r.y = i;
                SDL_FillRect(temp,&r,SDL_MapRGB(temp->format,editorData->colours[k].RED[j][i],editorData->colours[k].GRN[j][i],editorData->colours[k].BLU[j][i]));
            }
        
        SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
        
        editorData->colour_tex[k] = SDL_CreateTextureFromSurface(renderer,temp);
    }
    
    SDL_FreeSurface(temp);
    
    int row = 0, collum = 0, index = 0;
    index = 0;
    char name[20] = {0};
    char name_num[4];
    index = 0;
    for (collum = 0; collum < 8; collum++)
        for (row = 0; row < 8; row++)
        {
            strcpy(name,"canvas");
            sprintf(name_num, "%d", index);
            strcat(name,name_num);
            HCS_Entity e = HCS_Entity_create(name);
            HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH) / 2 - 100 * (-4 + row), 100 + (collum) * 100, 95, 95);
            HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
            HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
            HCS_Clickable_add_func(e,on_canvas_click,e);
            editorData->Canvas[index] = HCS_Sprite_get(e);
            index++;
        }

    row = 0;
    collum = 0;
    index = 0;
    char name2[20] = {0};
    char name_num2[4];
    index = 0;
    for (collum = 0; collum < 4; collum++)
        for (row = 0; row < 4; row++)
        {
            strcpy(name2,"colour");
            sprintf(name_num2, "%d", index);
            strcat(name2,name_num2);
            HCS_Entity e = HCS_Entity_create(name2);
            HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
            HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
            HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
            HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
            HCS_Clickable_add_func(e,on_menu_click,index);
            index++;
        }
    
    
    
    HCS_Entity e = HCS_Entity_create("Save_Button");
    HCS_Body_add(e,10,100 + 0 * 100,100,75);
    HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
    HCS_Sprite_use_text(e,"Safe",4);
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    HCS_Clickable_add_func(e,on_safe_click,index);
    
    e = HCS_Entity_create("Clear_Button");
    HCS_Body_add(e,10,100 + 1 * 100,100,75);
    HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
    HCS_Sprite_use_text(e,"Clear",5);
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    HCS_Clickable_add_func(e,clear_canvas,index);
    
    e = HCS_Entity_create("Quit_Button");
    HCS_Body_add(e,10,100 + 2 * 100,100,75);
    HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
        HCS_Sprite_use_text(e,"Quit",4);
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    HCS_Clickable_add_func(e,sprite_editor_deinit,index);
    
    HCS_Event_add("cursor_event",cursor_event);
    
    editorData->save_for_real = false;
    
    HCS_CONTINUE();
}

#endif
