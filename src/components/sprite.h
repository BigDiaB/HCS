#pragma once 

#define HCS_Drawable_Drawtype_UI 9
#define HCS_NUM_DRAWTYPES 10

void sprite_new(HCS_Sprite* spr, char* filename)
{
    HCS_Sprite* test = HCS_Asset(filename);
    if (test != NULL)
    {
        *spr = *test;
        return;
    }
    
}

int HCS_Sprite_add(HCS_Entity e, char* n, int t, bool use_text)
{
    int index = LSD_Math_get_id_from_array(runData->HCS_Sprite_list,&runData->HCS_Sprite_used, HCS_MAX_SPRITES);
    runData->HCS_Entities[e].comp_ids[HCS_cSprite] = index;
    
    runData->HCS_Sprites[index].layer = t;

    if (!use_text)
        runData->HCS_Sprites[index] = *HCS_Asset(n);
    else
    {
    int len = strlen(n);
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


    

    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Sprite hinzugefügt!",e,HCS_Entity_tag_get(e));
    #endif
    
    return index;
}

HCS_Sprite* HCS_Sprite_get(HCS_Entity e)
{
    return &runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)];
}

void HCS_Sprite_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Sprite_list, &runData->HCS_Sprite_used, &runData->HCS_Entities[e].comp_ids[HCS_cSprite]);
    #ifdef HCS_DEBUG    
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Sprite entfernt!",e,HCS_Entity_tag_get(e));
    #endif
}

void HCS_Sprite_system(double delta)
{
    int depth_buffer[HCS_NUM_DRAWTYPES][HCS_MAX_SPRITES] = {};
    int used_buffer[HCS_NUM_DRAWTYPES] = {0};

    int i,t;
    for (t = 0; t < runData->HCS_Sprite_used; t++)
    {
        int i = runData->HCS_Sprite_list[t];
        depth_buffer[runData->HCS_Sprites[i].layer][used_buffer[runData->HCS_Sprites[i].layer]] = i;
        used_buffer[runData->HCS_Sprites[i].layer]++;
    }

    for (t = 0; t < 3; t++)
    {
        for (i = 0; i < used_buffer[t]; i++)
        {
             SDL_Rect r;
             HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[t][i],HCS_cSprite));
             r.x = b->pos.x;
             r.y = b->pos.y;
             r.w = b->size.x;
             r.h = b->size.y;
             HCS_Drawable_translate_rect(&r);
             if (LSD_Math_AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
                SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
        }
    }

    for (t = 3; t < HCS_Drawable_Drawtype_UI - 1; t++)
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
             r.x += (runData->WIN_SIZE.w - runData->WIN_SIZE.h / 9 * 16) / 2;
             if (LSD_Math_AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
                SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
        }
    }

    for (i = 0; i < used_buffer[HCS_Drawable_Drawtype_UI]; i++)
    {
        SDL_Rect r;
        HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(depth_buffer[HCS_Drawable_Drawtype_UI][i],HCS_cSprite));
        r.x = b->pos.x - runData->HCS_Gfx_Camera.x;
        r.y = b->pos.y - runData->HCS_Gfx_Camera.y;
        r.w = b->size.x;
        r.h = b->size.y;
        HCS_Drawable_translate_rect(&r);
        if (LSD_Math_AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
            SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
    }


    for (t = HCS_Drawable_Drawtype_UI + 1; t < HCS_NUM_DRAWTYPES; t++)
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
            if (LSD_Math_AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
                SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[depth_buffer[t][i]].tex,NULL,&r);
        }
    }
}