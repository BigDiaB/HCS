
#pragma once

#define HCS_Drawable_Drawtype_UI 8

void HCS_Drawable_translate_rect(HCS_Gfx_Rectangle* r)
{
    
    r->y = map_number_in_range_to_new_range(r->y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
    r->h = map_number_in_range_to_new_range(r->h,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
    r->x = map_number_in_range_to_new_range(r->x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
    r->w = map_number_in_range_to_new_range(r->w,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
}

int HCS_Drawable_add(HCS_Entity e, char* n)
//int HCS_Drawable_add(HCS_Entity e, char* n, float x, float y, bool text, HCS_Drawtype t)
{
    runData->HCS_Entities[e][HCS_cDrawable] = get_unused_id_from_blacklist(runData->HCS_Drawable_list, &runData->HCS_Drawable_used, HCS_MAX_DRAWABLES);
    
//    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].path = n;
//    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].pos.x = x;
//    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].pos.y = y;
//    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].draw = true;
//    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].type = t;
    
    int i = HCS_Entity_get_component_id(e,HCS_cDrawable);
    
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht ein Drawable hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].tex = HCS_Asset_manager(n, HCS_AAdd);
    return HCS_Entity_get_component_id(e,HCS_cDrawable);
}


HCS_Drawable* HCS_Drawable_get(HCS_Entity e)
{
    if (!HCS_Entity_has_component(e, HCS_cDrawable))
    {
        printf("HCS_Entity %d hat keinen drawablen!\n",e);
        exit(1);
    }
    return &runData->HCS_Drawables[HCS_Entity_get_component_id(e, HCS_cDrawable)];
}
void HCS_Drawable_remove(HCS_Entity e)
{
    if (runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].managed)
        HCS_Asset_manager(runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].path, HCS_ARemove);
    else
    HCS_Gfx_Texture_destroy(runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].tex);
    remove_element_from_array(runData->HCS_Drawable_list,&runData->HCS_Drawable_used,&runData->HCS_Entities[e][HCS_cDrawable]);
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht ein Drawable entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}


// bool dAABB(vec2f pos1, vec2f pos2, vec2i size1, vec2i size2)
// {
//    return (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y);
// }

void HCS_Drawable_system()
{
    HCS_Drawable* depth_buffer[num_draw_types][HCS_MAX_DRAWABLES];
    int used_buffer[num_draw_types];
    int j;
    for (j = 0; j < num_draw_types; j++)
        used_buffer[j] = 0;
    for (j = 0; j < runData->HCS_Drawable_used; j++)
    {
        int i = runData->HCS_Drawable_list[j];
        
        if (runData->HCS_Drawables[i].draw)
        {
            // if (dAABB(runData->HCS_Drawables[i].pos,vec_new_float(0,0),runData->HCS_Drawables[i].size, vec_new_int(get_screen_size().x - runData->camera.x,get_screen_size().y - runData->camera.y)))
            // {
                depth_buffer[runData->HCS_Drawables[i].type][used_buffer[runData->HCS_Drawables[i].type]] = &runData->HCS_Drawables[i];
                used_buffer[runData->HCS_Drawables[i].type]++;
            // }
        }
        
        if (HCS_Entity_has_component(HCS_Entity_get_entity_id(i,HCS_cDrawable),HCS_cBody))
        {
            runData->HCS_Drawables[i].pos = (*HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cDrawable))).pos;
            runData->HCS_Drawables[i].size = (*HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cDrawable))).size;
        }
        
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
                r.x = (float)((depth_buffer[t][i]->pos.x) ) * STRETCH_WIDTH;
                r.y = (float)((depth_buffer[t][i]->pos.y) );
                r.w = (*depth_buffer[t][i]).size.x  * STRETCH_WIDTH;
                r.h = (*depth_buffer[t][i]).size.y ;
                HCS_Drawable_translate_rect(&r);
                if (depth_buffer[t][i]->draw_rect)
                {
                    HCS_Gfx_DrawColor_set(depth_buffer[t][i]->color.r,depth_buffer[t][i]->color.g,depth_buffer[t][i]->color.b,depth_buffer[t][i]->color.a);
                    if (depth_buffer[t][i]->fill_rect)
                        HCS_Gfx_Rectangle_fill(r);
                    HCS_Gfx_Rectangle_draw(r);
                    HCS_Gfx_DrawColor_set(std.r,std.g,std.b,std.a);
                }
                if (!(*depth_buffer[t][i]).use_quad)
                    HCS_Gfx_Texture_draw(depth_buffer[t][i]->tex, NULL, &r);
                else
                {
                    HCS_Gfx_Rectangle q;
                    q.x = (*depth_buffer[t][i]).quad_pos.x;
                    q.y = (*depth_buffer[t][i]).quad_pos.y;
                    q.w = (*depth_buffer[t][i]).quad_size.x;
                    q.h = ((*depth_buffer[t][i])).quad_size.y;
                    HCS_Gfx_Texture_draw((*depth_buffer[t][i]).tex, &q, &r);
                }
            }
        }
        else
        {
            int i;
            for (i = 0; i < used_buffer[t]; i++)
            {
                HCS_Gfx_Rectangle r;
                r.x = (float)((depth_buffer[t][i]->pos.x - HCS_Gfx_Camera.x) );
                r.y = (float)((depth_buffer[t][i]->pos.y - HCS_Gfx_Camera.y) );
                r.w = (*depth_buffer[t][i]).size.x ;
                r.h = (*depth_buffer[t][i]).size.y ;
                HCS_Drawable_translate_rect(&r);
                if (depth_buffer[t][i]->draw_rect)
                {
                    HCS_Gfx_DrawColor_set(depth_buffer[t][i]->color.r,depth_buffer[t][i]->color.g,depth_buffer[t][i]->color.b,depth_buffer[t][i]->color.a);
                    if (depth_buffer[t][i]->fill_rect)
                        HCS_Gfx_Rectangle_fill(r);
                    HCS_Gfx_Rectangle_draw(r);
                    HCS_Gfx_DrawColor_set(std.r,std.g,std.b,std.a);
                }
                if (!(*depth_buffer[t][i]).use_quad)
                    HCS_Gfx_Texture_draw(depth_buffer[t][i]->tex, NULL, &r);
                else
                {
                    HCS_Gfx_Rectangle q;
                    q.x = (*depth_buffer[t][i]).quad_pos.x;
                    q.y = (*depth_buffer[t][i]).quad_pos.y;
                    q.w = (*depth_buffer[t][i]).quad_size.x;
                    q.h = ((*depth_buffer[t][i])).quad_size.y;
                    HCS_Gfx_Texture_draw((*depth_buffer[t][i]).tex, &q, &r);
                }
            }
        }
    }
}
