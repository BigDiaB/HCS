#pragma once 

#define HCS_NUM_DRAWTYPES 7
#define HCS_NUM_DRAWLAYERS 10

void sprite_new(HCS_Sprite* spr, char* filename)
{
    HCS_Sprite* test = HCS_Asset(filename);
    if (test != NULL)
        *spr = *test;
}

int HCS_Sprite_add(HCS_Entity e, char* n,int l,int t, bool use_text)
{
    runData->HCS_Entities[e].comp_ids[HCS_cSprite] = LSD_Math_get_id_from_array(runData->HCS_Sprite_list,&runData->HCS_Sprite_used, HCS_MAX_SPRITES);

    if (!use_text)
        runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)] = *HCS_Asset(n);
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

    runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)].type = t;
    runData->HCS_Sprites[HCS_Entity_get_component_id(e,HCS_cSprite)].layer = l;

    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Sprite hinzugefügt!",e,HCS_Entity_tag_get(e));
    #endif
    
    return HCS_Entity_get_component_id(e,HCS_cSprite);
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
    int i,j,t,c;
    for (c = 0; c < HCS_NUM_DRAWLAYERS; c++)
        for (t = 1; t < HCS_NUM_DRAWTYPES; t++)
            for (j = 0; j < runData->HCS_Sprite_used; j++)
            {
                i = runData->HCS_Sprite_list[j];
                SDL_Rect r;
                HCS_Body* b = HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cSprite));
                if (t == runData->HCS_Sprites[i].type && c == runData->HCS_Sprites[i].layer)
                {
                    switch(t)
                    {
                        case 1:
                            r.x = b->pos.x;
                            r.y = b->pos.y;
                            r.w = b->size.x;
                            r.h = b->size.y;
                            HCS_Drawable_translate_rect(&r);
                            r.x += runData->DRAW_OFFSET;
                            break;
                        case 2:
                            r.x = b->pos.x - runData->HCS_Gfx_Camera.x;
                            r.y = b->pos.y - runData->HCS_Gfx_Camera.y;
                            r.w = b->size.x;
                            r.h = b->size.y;
                            HCS_Drawable_translate_rect(&r);
                            r.x += runData->DRAW_OFFSET;
                        break;
                        case 3:
                            r.x = b->pos.x;
                            r.y = b->pos.y;
                            r.w = b->size.x;
                            r.h = b->size.y;
                            r.x += (HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x);
                            HCS_Drawable_translate_rect(&r);
                            break;
                        case 4:
                            r.x = b->pos.x;
                            r.y = b->pos.y;
                            r.w = b->size.x;
                            r.h = b->size.y;
                            r.x += (HCS_Screen_size_get().x * HCS_Gfx_stretch_get().x) / 2;
                            HCS_Drawable_translate_rect(&r);
                            break;
                        case 5:
                            r.x = b->pos.x;
                            r.y = b->pos.y;
                            r.w = b->size.x;
                            r.h = b->size.y;
                            r.y += (HCS_Screen_size_get().y * HCS_Gfx_stretch_get().y);
                            HCS_Drawable_translate_rect(&r);
                            break;
                        case 6:
                            r.x = b->pos.x * runData->STRETCH_WIDTH;
                            r.y = b->pos.y;
                            r.w = b->size.x  * runData->STRETCH_WIDTH;
                            r.h = b->size.y;
                            HCS_Drawable_translate_rect(&r);
                            break;
                        default:
                            break;
                }
                if (LSD_Math_AABB(LSD_Vec_new_float(r.x,r.y),LSD_Vec_new_float(0,0),LSD_Vec_new_int(r.w,r.h),LSD_Vec_new_int(runData->WIN_SIZE.w,runData->WIN_SIZE.h)))
                    SDL_RenderCopy(runData->renderer,runData->HCS_Sprites[i].tex,NULL,&r);
            }
        }   
}