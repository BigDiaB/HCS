#pragma once 

#define HCS_Drawable_Drawtype_UI 8

void sprite_new(HCS_Sprite* spr, char* filename)
{
    FILE* file;
    file = fopen(filename,"r");
    char lines[24][34];
    int line,collum;
    for (line = 0; line < 24; line++)
         fgets(lines[line],33,file);

    for (line = 0; line < 8; line++)
    {
        sscanf(lines[line +  0],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",&spr->raw.RED[line][0],&spr->raw.RED[line][1],&spr->raw.RED[line][2],&spr->raw.RED[line][3],&spr->raw.RED[line][4],&spr->raw.RED[line][5],&spr->raw.RED[line][6],&spr->raw.RED[line][7]);
        sscanf(lines[line +  8],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",&spr->raw.GRN[line][0],&spr->raw.GRN[line][1],&spr->raw.GRN[line][2],&spr->raw.GRN[line][3],&spr->raw.GRN[line][4],&spr->raw.GRN[line][5],&spr->raw.GRN[line][6],&spr->raw.GRN[line][7]);
        sscanf(lines[line + 16],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",&spr->raw.BLU[line][0],&spr->raw.BLU[line][1],&spr->raw.BLU[line][2],&spr->raw.BLU[line][3],&spr->raw.BLU[line][4],&spr->raw.BLU[line][5],&spr->raw.BLU[line][6],&spr->raw.BLU[line][7]);
    }
    HCS_Gfx_Surface temp = SDL_CreateRGBSurface(0,8,8,32,0,0,0,0);
    HCS_Gfx_Rectangle r;
    r.w = 1;
    r.h = 1;
    for (line = 7; line > -1; line--)
        for (collum = 0; collum < 8; collum++)
        {
            r.x = 7 - line;
            r.y = collum;
            SDL_FillRect(temp,&r,SDL_MapRGB(temp->format,spr->raw.RED[collum][line],spr->raw.GRN[collum][line],spr->raw.BLU[collum][line]));
        }
    
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,254,0,0));
    
    spr->tex = SDL_CreateTextureFromSurface(renderer,temp);
    SDL_FreeSurface(temp);
}

int HCS_Sprite_add(HCS_Entity e, char* n, HCS_Drawtype t)
{
    int index = LSD_Math_get_id_from_array(runData->HCS_Sprite_list,&runData->HCS_Sprite_used, HCS_MAX_SPRITES);
    runData->HCS_Entities[e][HCS_cSprite] = index;
    
    sprite_new(&runData->HCS_Sprites[index],n);

    runData->HCS_Sprites[index].type = t;

    

    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Sprite hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    return index;
}

HCS_Sprite* HCS_Sprite_get(HCS_Entity e)
{
    return &runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)];
}

void HCS_Sprite_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Sprite_list, &runData->HCS_Sprite_used, &runData->HCS_Entities[e][HCS_cSprite]);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Sprite entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}

void HCS_Sprite_system(double delta)
{
    int depth_buffer[HCS_NUM_DRAWTYPES][HCS_MAX_SPRITES] = {};
    int used_buffer[HCS_NUM_DRAWTYPES] = {0};

    int i,t;
    for (t = 0; t < runData->HCS_Sprite_used; t++)
    {
        int i = runData->HCS_Sprite_list[t];
        depth_buffer[runData->HCS_Sprites[i].type][used_buffer[runData->HCS_Sprites[i].type]] = HCS_Entity_get_entity_id(i,HCS_cSprite);
        used_buffer[runData->HCS_Sprites[i].type]++;
    }

    for (t = 0; t < HCS_Drawable_Drawtype_UI; t++)
    {
        for (i = 0; i < used_buffer[t]; i++)
        {
             HCS_Gfx_Rectangle r;
             HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
             r.x = b->pos.x - HCS_Gfx_Camera.x;
             r.y = b->pos.y - HCS_Gfx_Camera.y;
             r.w = b->size.x;
             r.h = b->size.y;
             HCS_Drawable_translate_rect(&r);
             HCS_Gfx_Texture_draw(runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,r);
        }
    }
    for (t = HCS_Drawable_Drawtype_UI; t < HCS_NUM_DRAWTYPES; t++)
    {
        for (i = 0; i < used_buffer[t]; i++)
        {
            HCS_Gfx_Rectangle r;
            HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
            r.x = b->pos.x * STRETCH_WIDTH;
            r.y = b->pos.y;
            r.w = b->size.x  * STRETCH_WIDTH;
            r.h = b->size.y;
            HCS_Drawable_translate_rect(&r);
            HCS_Gfx_Texture_draw(runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,r);
        }
    }
}

void HCS_Sprite_use_text(HCS_Entity e, char* n)
{
    int len = strlen(n);
    HCS_Body_get(e)->size.x = HCS_Body_get(e)->size.y * len;
    char path[200];
    char temp[len];
    int i;
    for (i = 0; i < len; i++)
        temp[i] = toupper(n[i]);
    FILE* file;
    HCS_Sprite* spr;
    HCS_Gfx_Rectangle re;
    re.w = 1;
    re.h = 1;
    HCS_Gfx_Rectangle r;
    r.w = 8;
    r.h = 8;
    r.y = 0;
    
    SDL_DestroyTexture(HCS_Sprite_get(e)->tex);
    HCS_Sprite_get(e)->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, len * 9, 8);
    SDL_SetTextureBlendMode( HCS_Sprite_get(e)->tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer,HCS_Sprite_get(e)->tex);
//    SDL_RenderClear(renderer);
    for (i = 0; i < len; i++)
    {
        if (temp[i] != ' ')
        {
            strcpy(path,"assets/Font/");
            char character[2];
            character[0] = temp[i];
            character[1] = 0;
            strcat(path,character);
            strcat(path,".hgx");
            file = fopen(path,"r");
            char lines[24][34];
            int line,collum;
            for (line = 0; line < 24; line++)
                 fgets(lines[line],33,file);
            r.x = i * 9;
            sprite_new(spr,path);
            SDL_RenderCopy(renderer,spr->tex,NULL,&r);
        }
    }
    SDL_SetRenderTarget(renderer,NULL);
}
