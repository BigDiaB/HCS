
#pragma once

int HCS_Clickable_add(HCS_Entity e, bool* action, HCS_Clicktype type)
{
    if (!HCS_Entity_has_component(e,HCS_cBody) || !HCS_Entity_has_component(e,HCS_cDrawable))
        LSD_Log(LSD_ltERROR,"Entity fehlen vorausgesetzte Komponente für Clickable!");
    runData->HCS_Entities[e][HCS_cClickable] = get_unused_id_from_blacklist(runData->HCS_Clickable_list, &runData->HCS_Clickable_used, HCS_MAX_CLICKABLES);
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].action = action;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].old_down = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].down = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].type = type;
    return HCS_Entity_get_component_id(e,HCS_cClickable);
}

HCS_Clickable* HCS_Clickable_get(HCS_Entity e)
{
    return &runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)];
}


void HCS_Clickable_remove(HCS_Entity e)
{
    remove_element_from_array(runData->HCS_Clickable_list,&runData->HCS_Clickable_used,&runData->HCS_Entities[e][HCS_cClickable]);
}

bool cAABB(vec2i pos1, vec2f pos2, vec2i size1, vec2i size2)
{
    return (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y);
}

void HCS_Clickable_system()
{
    bool hot;
    int j;
    for (j = 0; j < runData->HCS_Clickable_used; j++)
    {
        int i = runData->HCS_Clickable_list[j];
        HCS_Body bod = *HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable));
        vec2i temp_size = {10,10};
        vec2i temp_bod_size = {HCS_Drawable_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->size.x ,HCS_Drawable_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->size.y };
        vec2f temp_pos;
        if (HCS_Drawable_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->type > HCS_Drawable_Drawtype_UI)
        {
            temp_pos.x = bod.pos.x * STRETCH_WIDTH ;
            temp_pos.y = bod.pos.y ;
            temp_bod_size.x *= STRETCH_WIDTH;
        }
        else
        {
            temp_pos.x = (bod.pos.x - runData->camera.x) ;
            temp_pos.y = (bod.pos.y - runData->camera.y) ;
        }
        temp_pos.y = map_number_in_range_to_new_range(temp_pos.y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
        temp_bod_size.y = map_number_in_range_to_new_range(temp_bod_size.y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
        temp_pos.x = map_number_in_range_to_new_range(temp_pos.x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
        temp_bod_size.x = map_number_in_range_to_new_range(temp_bod_size.x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
        hot = false;
        if (cAABB(mouse_pos,temp_pos,temp_size,temp_bod_size))
        {
            if (isDown(k.M_LEFT))
                runData->HCS_Clickables[i].down = true;
            hot = true;
            if (runData->HCS_Clickables[i].old_down && !isDown(k.M_LEFT))
            {
                switch (runData->HCS_Clickables[i].type) {
                    case HCS_Click_toggle:
                        *(runData->HCS_Clickables[i].action) = !*(runData->HCS_Clickables[i].action);
                        break;
                    case HCS_Click_on:
                        *(runData->HCS_Clickables[i].action) = true;
                        break;
                    case HCS_Click_off:
                        *(runData->HCS_Clickables[i].action) = false;
                    default:
                        break;
                }
            }
        }
        else
        {
            runData->HCS_Clickables[i].down = false;
            switch (runData->HCS_Clickables[i].type)
            {
                case HCS_Click_on:
                    *(runData->HCS_Clickables[i].action) = false;
                    break;
                case HCS_Click_off:
                    *(runData->HCS_Clickables[i].action) = true;
                    break;
                default:
                    break;
            }
        }
        if (runData->HCS_Clickables[i].down && hot)
            LIB_PLATFORM_COLOR_MOD(HCS_Drawable_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,75, 75, 75);
        else if (hot && !runData->HCS_Clickables[i].down)
            LIB_PLATFORM_COLOR_MOD(HCS_Drawable_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,150, 150, 150);
        else if (!hot && !runData->HCS_Clickables[i].down)
            LIB_PLATFORM_COLOR_MOD(HCS_Drawable_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,200, 200, 200);
        runData->HCS_Clickables[i].old_down = runData->HCS_Clickables[i].down;
        
    }
}
