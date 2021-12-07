
#pragma once

int HCS_Clickable_add(HCS_Entity e, bool* action, HCS_Clicktype type, HCS_Triggertype t)
{
    if (!HCS_Entity_has_component(e,HCS_cBody) || !HCS_Entity_has_component(e,HCS_cSprite))
        LSD_Log(LSD_ltERROR,"Entity fehlen vorausgesetzte Komponente für Clickable!");
    runData->HCS_Entities[e][HCS_cClickable] = LSD_Math_get_id_from_array(runData->HCS_Clickable_list, &runData->HCS_Clickable_used, HCS_MAX_CLICKABLES);
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].action = action;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].old_down = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].down = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].use_func = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].type = type;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].trigger = t;
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Clickable hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    return HCS_Entity_get_component_id(e,HCS_cClickable);
}

void HCS_Clickable_add_func(HCS_Entity e,void(*func)(int),int func_data)
{
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].use_func = true;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].func = func;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].func_data = func_data;

}

HCS_Clickable* HCS_Clickable_get(HCS_Entity e)
{
    return &runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)];
}


void HCS_Clickable_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Clickable_list,&runData->HCS_Clickable_used,&runData->HCS_Entities[e][HCS_cClickable]);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Clickable entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}

void HCS_Clickable_system(double delta)
{
    bool hot;
    int j;
    for (j = 0; j < runData->HCS_Clickable_used; j++)
    {
        int i = runData->HCS_Clickable_list[j];
        HCS_Body bod = *HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable));
        LSD_Vec2i temp_size = {10,10};
        LSD_Vec2i temp_bod_size = { HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->size.x,HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->size.y};
        LSD_Vec2f temp_pos;
               if (HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->type > HCS_Drawable_Drawtype_UI)
               {
                   temp_pos.x = bod.pos.x * STRETCH_WIDTH ;
                   temp_pos.y = bod.pos.y ;
                   temp_bod_size.x *= STRETCH_WIDTH;
               }
               else
        {
            temp_pos.x = (bod.pos.x - HCS_Gfx_Camera.x) ;
            temp_pos.y = (bod.pos.y - HCS_Gfx_Camera.y) ;
        }
        temp_pos.y = LSD_Math_map(temp_pos.y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
        temp_bod_size.y = LSD_Math_map(temp_bod_size.y,0,WORLD_TO_SCREEN_Y,0,WIN_SIZE.h);
        temp_pos.x = LSD_Math_map(temp_pos.x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
        temp_bod_size.x = LSD_Math_map(temp_bod_size.x,0,WORLD_TO_SCREEN_X * STRETCH_WIDTH,0,WIN_SIZE.w);
        hot = false;
        if (AABB(HCS_Gfx_Mouse_pos,temp_pos,temp_size,temp_bod_size))
        {
            runData->HCS_Clickables[i].old_down = runData->HCS_Clickables[i].down;
            runData->HCS_Clickables[i].down = HCS_Gfx_Mouse_clicked;
            hot = true;
            switch(runData->HCS_Clickables[i].trigger)
            {
                case HCS_Trig_released:
                    if (runData->HCS_Clickables[i].old_down && !HCS_Gfx_Mouse_clicked)
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
                        if (runData->HCS_Clickables[i].use_func)
                            runData->HCS_Clickables[i].func(runData->HCS_Clickables[i].func_data);
                    }
                    break;

                case HCS_Trig_down:
                    if (!runData->HCS_Clickables[i].old_down && HCS_Gfx_Mouse_clicked)
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
                        if (runData->HCS_Clickables[i].use_func)
                            runData->HCS_Clickables[i].func(runData->HCS_Clickables[i].func_data);
                    }
                    break;


                default:
                    break;
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
            HCS_Gfx_Texture_color_mod(HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,75, 75, 75);
        else if (hot && !runData->HCS_Clickables[i].down)
            HCS_Gfx_Texture_color_mod(HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,150, 150, 150);
        else if (!hot && !runData->HCS_Clickables[i].down)
            HCS_Gfx_Texture_color_mod(HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,255, 255, 255);
        
    }
}
