
#pragma once

#define HCS_Drawable_Drawtype_UI 8

void HCS_Drawable_translate_rect(LIB_PLATFORM_RECTANGLE* r)
{
    
    r->y = map_number_in_range_to_new_range(r->y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
    r->h = map_number_in_range_to_new_range(r->h,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
    r->x = map_number_in_range_to_new_range(r->x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
    r->w = map_number_in_range_to_new_range(r->w,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
}


int HCS_Drawable_add(HCS_Entity e, char* n, float x, float y, bool text, HCS_Drawtype t)
{
    runData->HCS_Entities[e][HCS_cDrawable] = get_unused_id_from_blacklist(runData->HCS_Drawable_list, &runData->HCS_Drawable_used, HCS_MAX_DRAWABLES);
    
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].path = n;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].pos.x = x;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].pos.y = y;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].draw = true;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].type = t;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_pos.x = 0;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_pos.y = 0;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_size.x = 0;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_size.y = 0;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].use_quad = false;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].draw_rect = false;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].fill_rect = false;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].managed = !text;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].use_path_as_image_text = text;
    
    int i = HCS_Entity_get_component_id(e,HCS_cDrawable);
    
    if (runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].use_path_as_image_text)
    {
        LIB_PLATFORM_SURFACE surf = LIB_PLATFORM_TEXT_TO_SURFACE(n);
        runData->HCS_Drawables[i].size.x = surf->w;
        runData->HCS_Drawables[i].size.y = surf->h;
        runData->HCS_Drawables[i].tex = LIB_PLATFORM_SURFACE_TO_TEXTURE(surf);
        return HCS_Entity_get_component_id(e,HCS_cDrawable);
    }
    
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].tex = HCS_Asset_manager(n, HCS_AAdd);
    return HCS_Entity_get_component_id(e,HCS_cDrawable);
}

void HCS_Drawable_add_quad(HCS_Entity e, int quad_x, int quad_y, int quad_w, int quad_h)
{
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_pos.x = quad_x;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_pos.y = quad_y;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_size.x = quad_w;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].quad_size.y = quad_h;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].use_quad = true;
}

void HCS_Drawable_add_rect(HCS_Entity e, int r, int g, int b, int a, bool fill)
{
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].color.r = r;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].color.g = g;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].color.b = b;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].color.a = a;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].draw_rect = true;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].fill_rect = fill;
}

void HCS_Drawable_reset_unmanaged_with_text(HCS_Entity e, char* text)
{
    LIB_PLATFORM_SURFACE surf = LIB_PLATFORM_TEXT_TO_SURFACE(text);
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].size.x = surf->w;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].size.y = surf->h;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].tex = LIB_PLATFORM_SURFACE_TO_TEXTURE(surf);
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].managed = false;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].use_path_as_image_text = true;
}

void HCS_Drawable_reset_unmanaged(HCS_Entity e, LIB_PLATFORM_SURFACE surf)
{
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].size.x = surf->w;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].size.y = surf->h;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].tex = LIB_PLATFORM_SURFACE_TO_TEXTURE(surf);
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].managed = false;
    runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].use_path_as_image_text = false;
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
    LIB_PLATFORM_TEXTURE_DESTROY(runData->HCS_Drawables[HCS_Entity_get_component_id(e,HCS_cDrawable)].tex);
    remove_element_from_array(runData->HCS_Drawable_list,&runData->HCS_Drawable_used,&runData->HCS_Entities[e][HCS_cDrawable]);
}


//bool dAABB(vec2f pos1, vec2f pos2, vec2i size1, vec2i size2)
//{
//    return (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y);
//}

void HCS_Drawable_system()
{
    HCS_Drawable* depth_buffer[num_draw_types][HCS_MAX_DRAWABLES];
    int used_buffer[num_draw_types];
    int g;
    for (g = 0; g < num_draw_types; g++)
        used_buffer[g] = 0;
    int j;
    for (j = 0; j < runData->HCS_Drawable_used; j++)
    {
        int i = runData->HCS_Drawable_list[j];
        
        if (runData->HCS_Drawables[i].draw)
        {
            //            vec2f screen_pos = {0 - camera.x,0 - camera.y};
            //            vec2i screen_size = {WIN_SIZE.w + camera.x,WIN_SIZE.h + camera.x};
            //            if (dAABB(runData->HCS_Drawables[i].pos, screen_pos,runData->HCS_Drawables[i].size, screen_size))
            {
                depth_buffer[runData->HCS_Drawables[i].type][used_buffer[runData->HCS_Drawables[i].type]] = &runData->HCS_Drawables[i];
                used_buffer[runData->HCS_Drawables[i].type]++;
            }
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
                LIB_PLATFORM_RECTANGLE r;
                r.x = (float)((depth_buffer[t][i]->pos.x) ) * STRETCH_WIDTH;
                r.y = (float)((depth_buffer[t][i]->pos.y) );
                r.w = (*depth_buffer[t][i]).size.x  * STRETCH_WIDTH;
                r.h = (*depth_buffer[t][i]).size.y ;
                HCS_Drawable_translate_rect(&r);
                if (depth_buffer[t][i]->draw_rect)
                {
                    LIB_PLATFORM_SET_DRAW_COLOR(depth_buffer[t][i]->color.r,depth_buffer[t][i]->color.g,depth_buffer[t][i]->color.b,depth_buffer[t][i]->color.a);
                    if (depth_buffer[t][i]->fill_rect)
                        LIB_PLATFORM_FILL_RECT(r);
                    LIB_PLATFORM_DRAW_RECT(r);
                    LIB_PLATFORM_SET_DRAW_COLOR(std.r,std.g,std.b,std.a);
                }
                if (!(*depth_buffer[t][i]).use_quad)
                    LIB_PLATFORM_DRAW_TEXTURE(depth_buffer[t][i]->tex, NULL, &r);
                else
                {
                    LIB_PLATFORM_RECTANGLE q;
                    q.x = (*depth_buffer[t][i]).quad_pos.x;
                    q.y = (*depth_buffer[t][i]).quad_pos.y;
                    q.w = (*depth_buffer[t][i]).quad_size.x;
                    q.h = ((*depth_buffer[t][i])).quad_size.y;
                    LIB_PLATFORM_DRAW_TEXTURE((*depth_buffer[t][i]).tex, &q, &r);
                }
            }
        }
        else
        {
            int i;
            for (i = 0; i < used_buffer[t]; i++)
            {
                LIB_PLATFORM_RECTANGLE r;
                r.x = (float)((depth_buffer[t][i]->pos.x - camera.x) );
                r.y = (float)((depth_buffer[t][i]->pos.y - camera.y) );
                r.w = (*depth_buffer[t][i]).size.x ;
                r.h = (*depth_buffer[t][i]).size.y ;
                HCS_Drawable_translate_rect(&r);
                if (depth_buffer[t][i]->draw_rect)
                {
                    LIB_PLATFORM_SET_DRAW_COLOR(depth_buffer[t][i]->color.r,depth_buffer[t][i]->color.g,depth_buffer[t][i]->color.b,depth_buffer[t][i]->color.a);
                    if (depth_buffer[t][i]->fill_rect)
                        LIB_PLATFORM_FILL_RECT(r);
                    LIB_PLATFORM_DRAW_RECT(r);
                    LIB_PLATFORM_SET_DRAW_COLOR(std.r,std.g,std.b,std.a);
                }
                if (!(*depth_buffer[t][i]).use_quad)
                    LIB_PLATFORM_DRAW_TEXTURE(depth_buffer[t][i]->tex, NULL, &r);
                else
                {
                    LIB_PLATFORM_RECTANGLE q;
                    q.x = (*depth_buffer[t][i]).quad_pos.x;
                    q.y = (*depth_buffer[t][i]).quad_pos.y;
                    q.w = (*depth_buffer[t][i]).quad_size.x;
                    q.h = ((*depth_buffer[t][i])).quad_size.y;
                    LIB_PLATFORM_DRAW_TEXTURE((*depth_buffer[t][i]).tex, &q, &r);
                }
            }
        }
    }
}
