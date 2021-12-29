
#pragma once

void HCS_Clickable_callback_list(void (*func)(int),char* n)
{
    if (runData->HCS_Clickable_callback_used >= HCS_MAX_CLICKABLES)
    {
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltERROR,"Du kannst keine Clickable-Callbacks mehr hinzufügen!");
        #endif
    }
    runData->HCS_Clickable_callbacks[runData->HCS_Clickable_callback_used].func = func;
    runData->HCS_Clickable_callbacks[runData->HCS_Clickable_callback_used].name = malloc(strlen(n) + 1);
    strcpy(runData->HCS_Clickable_callbacks[runData->HCS_Clickable_callback_used].name,n);
    runData->HCS_Clickable_callback_used++;
}

int HCS_Clickable_add(HCS_Entity e, HCS_Clicktype type, HCS_Triggertype t,char* n,int func_data)
{
    if (!HCS_Entity_has_component(e,HCS_cBody) || !HCS_Entity_has_component(e,HCS_cSprite))
    {
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltERROR,"Entity fehlen vorausgesetzte Komponente für Clickable!");
        #endif
    }
    runData->HCS_Entities[e].comp_ids[HCS_cClickable] = LSD_Math_get_id_from_array(runData->HCS_Clickable_list, &runData->HCS_Clickable_used, HCS_MAX_CLICKABLES);
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].old_down = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].down = false;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].type = type;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].trigger = t;
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].active = true;

    int i;
    bool found = false;
    for (i = 0; i < runData->HCS_Clickable_callback_used; i++)
        if (0 == strcmp(n, runData->HCS_Clickable_callbacks[i].name))
        {
            runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].func = runData->HCS_Clickable_callbacks[i].func;
            found = true;
        }
    if (!found)
    {
        for (i = 0; i < runData->HCS_Clickable_callback_used; i++)
            LSD_Log(LSD_ltWARNING,"%s",runData->HCS_Clickable_callbacks[i].name);
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltERROR,"Clickable-Callback konnte nicht gefunden werden: %s",n);
        #endif
    }
    runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)].func_data = func_data;
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Clickable hinzugefügt!",e,HCS_Entity_tag_get(e));
    #endif
    return HCS_Entity_get_component_id(e,HCS_cClickable);
}

HCS_Clickable* HCS_Clickable_get(HCS_Entity e)
{
    return &runData->HCS_Clickables[HCS_Entity_get_component_id(e,HCS_cClickable)];
}


void HCS_Clickable_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Clickable_list,&runData->HCS_Clickable_used,&runData->HCS_Entities[e].comp_ids[HCS_cClickable]);
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Clickable entfernt!",e,HCS_Entity_tag_get(e));
    #endif
}

void HCS_Clickable_system(double delta)
{
    bool hot;
    int j;
    for (j = 0; j < runData->HCS_Clickable_used; j++)
    {
        int i = runData->HCS_Clickable_list[j];
        if (runData->HCS_Clickables[i].active)
        {
            HCS_Body bod = *HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable));
            LSD_Vec2i temp_size = {0,0};
            LSD_Vec2i temp_bod_size = { HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->size.x,HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->size.y};
            LSD_Vec2f temp_pos;
            if (HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->type > HCS_Drawable_Drawtype_UI)
            {
               temp_pos.x = bod.pos.x * runData->STRETCH_WIDTH ;
               temp_pos.y = bod.pos.y ;
               temp_bod_size.x *= runData->STRETCH_WIDTH;
            }
            else
            {
                temp_pos.x = (bod.pos.x - runData->HCS_Gfx_Camera.x) ;
                temp_pos.y = (bod.pos.y - runData->HCS_Gfx_Camera.y) ;
            }
            temp_pos.y = LSD_Math_map(temp_pos.y,0,runData->WORLD_TO_SCREEN_Y,0,runData->WIN_SIZE.h);
            temp_bod_size.y = LSD_Math_map(temp_bod_size.y,0,runData->WORLD_TO_SCREEN_Y,0,runData->WIN_SIZE.h);
            temp_pos.x = LSD_Math_map(temp_pos.x,0,runData->WORLD_TO_SCREEN_X * runData->STRETCH_WIDTH,0,runData->WIN_SIZE.w);
            temp_bod_size.x = LSD_Math_map(temp_bod_size.x,0,runData->WORLD_TO_SCREEN_X * runData->STRETCH_WIDTH,0,runData->WIN_SIZE.w);
            hot = false;
            if (LSD_Math_AABB(runData->HCS_Input_Cursor_position,temp_pos,temp_size,temp_bod_size))
            {
                runData->HCS_Clickables[i].old_down = runData->HCS_Clickables[i].down;
                runData->HCS_Clickables[i].down = runData->HCS_Input_Cursor_button.down;
                hot = true;
                switch(runData->HCS_Clickables[i].trigger)
                {
                    case HCS_Trig_released:
                        if (runData->HCS_Clickables[i].old_down && !runData->HCS_Input_Cursor_button.down)
                            runData->HCS_Clickables[i].func(runData->HCS_Clickables[i].func_data);
                        break;

                    case HCS_Trig_down:
                        if (!runData->HCS_Clickables[i].old_down && runData->HCS_Input_Cursor_button.down)
                            runData->HCS_Clickables[i].func(runData->HCS_Clickables[i].func_data);
                        break;


                    default:
                        break;
                }
                
            }
            else
                runData->HCS_Clickables[i].down = false;
            if (runData->HCS_Clickables[i].down && hot)
                SDL_SetTextureColorMod(HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,75, 75, 75);
            else if (hot && !runData->HCS_Clickables[i].down && !runData->HCS_Clickables[i].old_down)
                SDL_SetTextureColorMod(HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,150, 150, 150);
            else if (!hot)
                SDL_SetTextureColorMod(HCS_Sprite_get(HCS_Entity_get_entity_id(i,HCS_cClickable))->tex,255, 255, 255);
        }
    }
}
