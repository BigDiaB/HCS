#pragma once 

#define HCS_Drawable_Drawtype_UI 8

void sprite_new(HCS_Sprite* spr, char* filename)
{
    HCS_Sprite* test = HCS_Asset(filename);
    if (test != NULL)
    {
        *spr = *test;
        return;
    }
    
}

int HCS_Sprite_add(HCS_Entity e, char* n, HCS_Drawtype t)
{
    int index = LSD_Math_get_id_from_array(runData->HCS_Sprite_list,&runData->HCS_Sprite_used, HCS_MAX_SPRITES);
    runData->HCS_Entities[e][HCS_cSprite] = index;
    
    runData->HCS_Sprites[index] = *HCS_Asset(n);

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
             SDL_Rect r;
             HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
             r.x = b->pos.x - runData->HCS_Gfx_Camera.x;
             r.y = b->pos.y - runData->HCS_Gfx_Camera.y;
             r.w = b->size.x;
             r.h = b->size.y;
             HCS_Drawable_translate_rect(&r);
             if (AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
                SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
        }
    }
    for (t = HCS_Drawable_Drawtype_UI; t < HCS_NUM_DRAWTYPES; t++)
    {
        for (i = 0; i < used_buffer[t]; i++)
        {
            SDL_Rect r;
            HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
            r.x = b->pos.x * runData->STRETCH_WIDTH;
            r.y = b->pos.y;
            r.w = b->size.x  * runData->STRETCH_WIDTH;
            r.h = b->size.y;
            HCS_Drawable_translate_rect(&r);
            if (AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
                SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
        }
    }
}

void HCS_Sprite_use_text(HCS_Entity e, char* n, int length)
{
    int len = length;
    if (len < 1)
    {
        n = " ";
        len = 1;
    }
    HCS_Body_get(e)->size.x = HCS_Body_get(e)->size.y * len;
    
    char path[200];
    char temp[len];
    int i;
    for (i = 0; i < len; i++)
        temp[i] = toupper(n[i]);
    FILE* file;
    HCS_Sprite* spr;
    SDL_Rect re;
    re.w = 1;
    re.h = 1;
    SDL_Rect r;
    r.w = 16;
    r.h = 16;
    r.y = 0;
    
    SDL_DestroyTexture(HCS_Sprite_get(e)->tex);
    HCS_Sprite_get(e)->tex = SDL_CreateTexture(runData->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, len * 18, 16);
    SDL_SetTextureBlendMode( HCS_Sprite_get(e)->tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(runData->renderer,HCS_Sprite_get(e)->tex);
//    SDL_RenderClear(runData->renderer);
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
            r.x = i * 18;
            spr = HCS_Asset(path);
            SDL_RenderCopy(runData->renderer,spr->tex,NULL,&r);
        }
    }
    SDL_SetRenderTarget(runData->renderer,NULL);
}
