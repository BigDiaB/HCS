
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

};

static struct HCS_editorData* editorData;

void sprite_editor_deinit(int nothing)
{
    free(editorData);
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
	int i;
	for (i = 0; i < NUM_COLOURS; i++)
		editorData->colour_selected[i] = false;
	editorData->colour_selected[who] = true;
}

void on_canvas_click(int self)
{
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

void on_safe_click(int nothing)
{
    char* img_data;
    char* format_string = "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu";
    img_data = malloc(strlen(format_string));
    sprintf(img_data,format_string,
        editorData->Canvas[0]->raw.RED[0][0],editorData->Canvas[1]->raw.RED[0][0],editorData->Canvas[2]->raw.RED[0][0],editorData->Canvas[3]->raw.RED[0][0],editorData->Canvas[4]->raw.RED[0][0],editorData->Canvas[5]->raw.RED[0][0],editorData->Canvas[6]->raw.RED[0][0],editorData->Canvas[7]->raw.RED[0][0],
        editorData->Canvas[8]->raw.RED[0][0],editorData->Canvas[9]->raw.RED[0][0],editorData->Canvas[10]->raw.RED[0][0],editorData->Canvas[11]->raw.RED[0][0],editorData->Canvas[12]->raw.RED[0][0],editorData->Canvas[13]->raw.RED[0][0],editorData->Canvas[14]->raw.RED[0][0],editorData->Canvas[15]->raw.RED[0][0],
        editorData->Canvas[16]->raw.RED[0][0],editorData->Canvas[17]->raw.RED[0][0],editorData->Canvas[18]->raw.RED[0][0],editorData->Canvas[19]->raw.RED[0][0],editorData->Canvas[20]->raw.RED[0][0],editorData->Canvas[21]->raw.RED[0][0],editorData->Canvas[22]->raw.RED[0][0],editorData->Canvas[23]->raw.RED[0][0],
        editorData->Canvas[24]->raw.RED[0][0],editorData->Canvas[25]->raw.RED[0][0],editorData->Canvas[26]->raw.RED[0][0],editorData->Canvas[27]->raw.RED[0][0],editorData->Canvas[28]->raw.RED[0][0],editorData->Canvas[29]->raw.RED[0][0],editorData->Canvas[30]->raw.RED[0][0],editorData->Canvas[31]->raw.RED[0][0],
        editorData->Canvas[32]->raw.RED[0][0],editorData->Canvas[33]->raw.RED[0][0],editorData->Canvas[34]->raw.RED[0][0],editorData->Canvas[35]->raw.RED[0][0],editorData->Canvas[36]->raw.RED[0][0],editorData->Canvas[37]->raw.RED[0][0],editorData->Canvas[38]->raw.RED[0][0],editorData->Canvas[39]->raw.RED[0][0],
        editorData->Canvas[40]->raw.RED[0][0],editorData->Canvas[41]->raw.RED[0][0],editorData->Canvas[42]->raw.RED[0][0],editorData->Canvas[43]->raw.RED[0][0],editorData->Canvas[44]->raw.RED[0][0],editorData->Canvas[45]->raw.RED[0][0],editorData->Canvas[46]->raw.RED[0][0],editorData->Canvas[47]->raw.RED[0][0],
        editorData->Canvas[48]->raw.RED[0][0],editorData->Canvas[59]->raw.RED[0][0],editorData->Canvas[50]->raw.RED[0][0],editorData->Canvas[51]->raw.RED[0][0],editorData->Canvas[52]->raw.RED[0][0],editorData->Canvas[53]->raw.RED[0][0],editorData->Canvas[54]->raw.RED[0][0],editorData->Canvas[55]->raw.RED[0][0],
        editorData->Canvas[56]->raw.RED[0][0],editorData->Canvas[57]->raw.RED[0][0],editorData->Canvas[58]->raw.RED[0][0],editorData->Canvas[59]->raw.RED[0][0],editorData->Canvas[60]->raw.RED[0][0],editorData->Canvas[61]->raw.RED[0][0],editorData->Canvas[62]->raw.RED[0][0],editorData->Canvas[63]->raw.RED[0][0],

        editorData->Canvas[0]->raw.GRN[0][0],editorData->Canvas[1]->raw.GRN[0][0],editorData->Canvas[2]->raw.GRN[0][0],editorData->Canvas[3]->raw.GRN[0][0],editorData->Canvas[4]->raw.GRN[0][0],editorData->Canvas[5]->raw.GRN[0][0],editorData->Canvas[6]->raw.GRN[0][0],editorData->Canvas[7]->raw.GRN[0][0],
        editorData->Canvas[8]->raw.GRN[0][0],editorData->Canvas[9]->raw.GRN[0][0],editorData->Canvas[10]->raw.GRN[0][0],editorData->Canvas[11]->raw.GRN[0][0],editorData->Canvas[12]->raw.GRN[0][0],editorData->Canvas[13]->raw.GRN[0][0],editorData->Canvas[14]->raw.GRN[0][0],editorData->Canvas[15]->raw.GRN[0][0],
        editorData->Canvas[16]->raw.GRN[0][0],editorData->Canvas[17]->raw.GRN[0][0],editorData->Canvas[18]->raw.GRN[0][0],editorData->Canvas[19]->raw.GRN[0][0],editorData->Canvas[20]->raw.GRN[0][0],editorData->Canvas[21]->raw.GRN[0][0],editorData->Canvas[22]->raw.GRN[0][0],editorData->Canvas[23]->raw.GRN[0][0],
        editorData->Canvas[24]->raw.GRN[0][0],editorData->Canvas[25]->raw.GRN[0][0],editorData->Canvas[26]->raw.GRN[0][0],editorData->Canvas[27]->raw.GRN[0][0],editorData->Canvas[28]->raw.GRN[0][0],editorData->Canvas[29]->raw.GRN[0][0],editorData->Canvas[30]->raw.GRN[0][0],editorData->Canvas[31]->raw.GRN[0][0],
        editorData->Canvas[32]->raw.GRN[0][0],editorData->Canvas[33]->raw.GRN[0][0],editorData->Canvas[34]->raw.GRN[0][0],editorData->Canvas[35]->raw.GRN[0][0],editorData->Canvas[36]->raw.GRN[0][0],editorData->Canvas[37]->raw.GRN[0][0],editorData->Canvas[38]->raw.GRN[0][0],editorData->Canvas[39]->raw.GRN[0][0],
        editorData->Canvas[40]->raw.GRN[0][0],editorData->Canvas[41]->raw.GRN[0][0],editorData->Canvas[42]->raw.GRN[0][0],editorData->Canvas[43]->raw.GRN[0][0],editorData->Canvas[44]->raw.GRN[0][0],editorData->Canvas[45]->raw.GRN[0][0],editorData->Canvas[46]->raw.GRN[0][0],editorData->Canvas[47]->raw.GRN[0][0],
        editorData->Canvas[48]->raw.GRN[0][0],editorData->Canvas[59]->raw.GRN[0][0],editorData->Canvas[50]->raw.GRN[0][0],editorData->Canvas[51]->raw.GRN[0][0],editorData->Canvas[52]->raw.GRN[0][0],editorData->Canvas[53]->raw.GRN[0][0],editorData->Canvas[54]->raw.GRN[0][0],editorData->Canvas[55]->raw.GRN[0][0],
        editorData->Canvas[56]->raw.GRN[0][0],editorData->Canvas[57]->raw.GRN[0][0],editorData->Canvas[58]->raw.GRN[0][0],editorData->Canvas[59]->raw.GRN[0][0],editorData->Canvas[60]->raw.GRN[0][0],editorData->Canvas[61]->raw.GRN[0][0],editorData->Canvas[62]->raw.GRN[0][0],editorData->Canvas[63]->raw.GRN[0][0],

        editorData->Canvas[0]->raw.BLU[0][0],editorData->Canvas[1]->raw.BLU[0][0],editorData->Canvas[2]->raw.BLU[0][0],editorData->Canvas[3]->raw.BLU[0][0],editorData->Canvas[4]->raw.BLU[0][0],editorData->Canvas[5]->raw.BLU[0][0],editorData->Canvas[06]->raw.BLU[0][0],editorData->Canvas[7]->raw.BLU[0][0],
        editorData->Canvas[8]->raw.BLU[0][0],editorData->Canvas[9]->raw.BLU[0][0],editorData->Canvas[10]->raw.BLU[0][0],editorData->Canvas[11]->raw.BLU[0][0],editorData->Canvas[12]->raw.BLU[0][0],editorData->Canvas[13]->raw.BLU[0][0],editorData->Canvas[14]->raw.BLU[0][0],editorData->Canvas[15]->raw.BLU[0][0],
        editorData->Canvas[16]->raw.BLU[0][0],editorData->Canvas[17]->raw.BLU[0][0],editorData->Canvas[18]->raw.BLU[0][0],editorData->Canvas[19]->raw.BLU[0][0],editorData->Canvas[20]->raw.BLU[0][0],editorData->Canvas[21]->raw.BLU[0][0],editorData->Canvas[22]->raw.BLU[0][0],editorData->Canvas[23]->raw.BLU[0][0],
        editorData->Canvas[24]->raw.BLU[0][0],editorData->Canvas[25]->raw.BLU[0][0],editorData->Canvas[26]->raw.BLU[0][0],editorData->Canvas[27]->raw.BLU[0][0],editorData->Canvas[28]->raw.BLU[0][0],editorData->Canvas[29]->raw.BLU[0][0],editorData->Canvas[30]->raw.BLU[0][0],editorData->Canvas[31]->raw.BLU[0][0],
        editorData->Canvas[32]->raw.BLU[0][0],editorData->Canvas[33]->raw.BLU[0][0],editorData->Canvas[34]->raw.BLU[0][0],editorData->Canvas[35]->raw.BLU[0][0],editorData->Canvas[36]->raw.BLU[0][0],editorData->Canvas[37]->raw.BLU[0][0],editorData->Canvas[38]->raw.BLU[0][0],editorData->Canvas[39]->raw.BLU[0][0],
        editorData->Canvas[40]->raw.BLU[0][0],editorData->Canvas[41]->raw.BLU[0][0],editorData->Canvas[42]->raw.BLU[0][0],editorData->Canvas[43]->raw.BLU[0][0],editorData->Canvas[44]->raw.BLU[0][0],editorData->Canvas[45]->raw.BLU[0][0],editorData->Canvas[46]->raw.BLU[0][0],editorData->Canvas[47]->raw.BLU[0][0],
        editorData->Canvas[48]->raw.BLU[0][0],editorData->Canvas[59]->raw.BLU[0][0],editorData->Canvas[50]->raw.BLU[0][0],editorData->Canvas[51]->raw.BLU[0][0],editorData->Canvas[52]->raw.BLU[0][0],editorData->Canvas[53]->raw.BLU[0][0],editorData->Canvas[54]->raw.BLU[0][0],editorData->Canvas[55]->raw.BLU[0][0],
        editorData->Canvas[56]->raw.BLU[0][0],editorData->Canvas[57]->raw.BLU[0][0],editorData->Canvas[58]->raw.BLU[0][0],editorData->Canvas[59]->raw.BLU[0][0],editorData->Canvas[60]->raw.BLU[0][0],editorData->Canvas[61]->raw.BLU[0][0],editorData->Canvas[62]->raw.BLU[0][0],editorData->Canvas[63]->raw.BLU[0][0]
    );

    system("touch assets/temp.hgx");
    LSD_File* save_file = LSD_File_open("assets/temp.hgx");


    LSD_File_write(save_file,img_data);




    LSD_File_close(save_file);
    free(img_data);
    LSD_Log(LSD_ltMESSAGE,"Canvas erfolgreich als Datei gespeichert!");
}


void init_event()
{
	editorData = malloc(sizeof(struct HCS_editorData));
    struct HCS_editorData zero = {0};
    *editorData = zero;

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

    int row = 0;
    int collum = 0;
    int index = 0;

    HCS_Entity e = HCS_Entity_create("Colour0");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour1");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour2");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour3");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row = 0;
    collum++;

    e = HCS_Entity_create("Colour4");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour5");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour6");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour7");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row = 0;
    collum++;

    e = HCS_Entity_create("Colour8");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour9");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour10");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour11");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row = 0;
    collum++;

    e = HCS_Entity_create("Colour12");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour13");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour14");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;
    row++;

    e = HCS_Entity_create("Colour15");
    HCS_Body_add(e,(HCS_Screen_size_get().x * STRETCH_WIDTH - 60 - 60 * row), 100 + (collum) * 60, 50, 50);
    HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
    HCS_Sprite_get(e)->tex = editorData->colour_tex[index];
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
    HCS_Clickable_add_func(e,on_menu_click,index);
    index++;





    row = 0;
    collum = 0;
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
            e = HCS_Entity_create(name);
            HCS_Body_add(e,(1000 - 80 * row), 100 + (collum) * 80, 75, 75);
            HCS_Sprite_add(e,"assets/black.hgx",HCS_Draw_Decal);
            HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_down);
            HCS_Clickable_add_func(e,on_canvas_click,e);
            editorData->Canvas[index] = HCS_Sprite_get(e);
            index++;
        }



    e = HCS_Entity_create("Save_Button");
    HCS_Body_add(e,0,0,100,100);
    HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    HCS_Clickable_add_func(e,on_safe_click,index);

    e = HCS_Entity_create("Clear_Button");
    HCS_Body_add(e,0,120,100,100);
    HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    HCS_Clickable_add_func(e,clear_canvas,index);

    e = HCS_Entity_create("Quit_Button");
    HCS_Body_add(e,0,240,100,100);
    HCS_Sprite_add(e,"assets/box.hgx",HCS_Draw_Decal);
    HCS_Clickable_add(e,&editorData->dummy_bool,HCS_Click_toggle,HCS_Trig_released);
    HCS_Clickable_add_func(e,sprite_editor_deinit,index);

    HCS_Event_remove("init");    
}

#endif