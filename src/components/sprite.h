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
        sscanf(lines[line +  0],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",&spr->RED[line][0],&spr->RED[line][1],&spr->RED[line][2],&spr->RED[line][3],&spr->RED[line][4],&spr->RED[line][5],&spr->RED[line][6],&spr->RED[line][7]);
        sscanf(lines[line +  8],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",&spr->GRN[line][0],&spr->GRN[line][1],&spr->GRN[line][2],&spr->GRN[line][3],&spr->GRN[line][4],&spr->GRN[line][5],&spr->GRN[line][6],&spr->GRN[line][7]);
        sscanf(lines[line + 16],"%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",&spr->BLU[line][0],&spr->BLU[line][1],&spr->BLU[line][2],&spr->BLU[line][3],&spr->BLU[line][4],&spr->BLU[line][5],&spr->BLU[line][6],&spr->BLU[line][7]);
    }
    HCS_Gfx_Surface temp = SDL_CreateRGBSurface(0,8,8,32,0,0,0,0);
    HCS_Gfx_Rectangle r;
    r.w = 1;
    r.h = 1;
    for (line = 0; line < 8; line++)
        for (collum = 0; collum < 8; collum++)
        {
            r.x = collum;
            r.y = line;
            SDL_FillRect(temp,&r,SDL_MapRGB(temp->format,spr->RED[collum][line],spr->GRN[collum][line],spr->BLU[collum][line]));
        }
    
    SDL_SetColorKey(temp,SDL_TRUE,SDL_MapRGB(temp->format,255,0,0));
    
    spr->tex = SDL_CreateTextureFromSurface(renderer,temp);
    SDL_FreeSurface(temp);
}

int HCS_Sprite_add(HCS_Entity e, char* n, HCS_Drawtype t)
{
    int index = LSD_Math_get_id_from_array(runData->HCS_Sprite_list,&runData->HCS_Sprite_used, HCS_MAX_SPRITES);
    runData->HCS_Entities[e][HCS_cSprite] = index;
    HCS_Sprite* spr =&runData->HCS_Sprites[index];
    
    sprite_new(spr,n);

    spr->type = t;

    

    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht ein Sprite hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    return index;
}

HCS_Sprite* HCS_Sprite_get(HCS_Entity e)
{
    return &runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)];
}

void HCS_Sprite_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Sprite_list, &runData->HCS_Sprite_used, &runData->HCS_Entities[e][HCS_cSprite]);
}

void HCS_Sprite_system(double delta)
{
    // HCS_Gfx_Rectangle r;
    // int i,j;
    // for (j = 0; j < runData->HCS_Sprite_used; j++)
    // {
    //     i = runData->HCS_Sprite_list[j];
    //     HCS_Body* bod = HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cSprite));
    //     r.x = bod->pos.x - HCS_Gfx_Camera.x;
    //     r.y = bod->pos.y - HCS_Gfx_Camera.y;
    //     r.w = bod->size.x;
    //     r.h = bod->size.y;
        
    //     HCS_Drawable_translate_rect(&r);
        
    //     SDL_RenderCopy(renderer,runData->HCS_Sprites[i].tex,NULL,&r);
    // }

    int depth_buffer[num_draw_types][HCS_MAX_SPRITES];
    int used_buffer[num_draw_types];




    int j;
    for (j = 0; j < num_draw_types; j++)
        used_buffer[j] = 0;
    for (j = 0; j < runData->HCS_Sprite_used; j++)
    {
        int i = runData->HCS_Sprite_list[j];
        depth_buffer[runData->HCS_Sprites[i].type][used_buffer[runData->HCS_Sprites[i].type]] = HCS_Entity_get_entity_id(i,HCS_cSprite);
        used_buffer[runData->HCS_Sprites[i].type]++;
    }

    int t;
    for (t = 0; t < num_draw_types; t++)
    {
        if (t > HCS_Drawable_Drawtype_UI)
        {
            int i;
            for (i = 0; i < used_buffer[t]; i++)
            {
                HCS_Gfx_Rectangle r;
                HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
                r.x = b->pos.x * STRETCH_WIDTH;
                r.y = b->pos.y;
                r.w = b->size.x  * STRETCH_WIDTH;
                r.h = b->size.y;
                HCS_Drawable_translate_rect(&r);
                SDL_RenderCopy(renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
            }
        }
        else
        {
            int i;
            for (i = 0; i < used_buffer[t]; i++)
            {
                HCS_Gfx_Rectangle r;
                HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
                r.x = b->pos.x - HCS_Gfx_Camera.x;
                r.y = b->pos.y - HCS_Gfx_Camera.y;
                r.w = b->size.x;
                r.h = b->size.y;
                HCS_Drawable_translate_rect(&r);
                SDL_RenderCopy(renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
            }
        }
    }
}
