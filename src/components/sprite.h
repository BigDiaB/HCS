#pragma once 
int HCS_Sprite_add(HCS_Entity e, char* n)
{
    int index = get_unused_id_from_blacklist(runData->HCS_Sprite_list,&runData->HCS_Sprite_used, HCS_MAX_SPRITES);
    runData->HCS_Entities[e][HCS_cSprite] = index;
    HCS_Sprite* spr =&runData->HCS_Sprites[index];
    
    FILE* file;
    file = fopen(n,"r");
    char lines[24][34];
    int line,collum;
    for (line = 0; line < 24; line++)
         fgets(lines[line],33,file);
//    while(fgets(lines[line],33,file) != NULL)
//        line++;

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
    
    return index;
}

HCS_Sprite* HCS_Sprite_get(HCS_Entity e)
{
    return &runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)];
}

void HCS_Sprite_remove(HCS_Entity e)
{
    remove_element_from_array(runData->HCS_Sprite_list, &runData->HCS_Sprite_used, &runData->HCS_Entities[e][HCS_cSprite]);
}

void HCS_Sprite_system(double delta)
{
    HCS_Gfx_Rectangle r;
    int i,j;
    for (j = 0; j < runData->HCS_Sprite_used; j++)
    {
        i = runData->HCS_Sprite_list[j];
        HCS_Body* bod = HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cSprite));
        r.x = bod->pos.x;
        r.y = bod->pos.y;
        r.w = bod->size.x;
        r.h = bod->size.y;
        
        SDL_RenderCopy(renderer,runData->HCS_Sprites[i].tex,NULL,&r);
    }
}
