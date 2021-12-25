
#pragma once

int HCS_Input_add(HCS_Entity e)
{
    runData->HCS_Entities[e][HCS_cInput] = LSD_Math_get_id_from_array(runData->HCS_Input_list, &runData->HCS_Input_used, HCS_MAX_INPUTS);
    runData->HCS_Inputs[HCS_Entity_get_component_id(e,HCS_cInput)].active = true;
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht Input hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    #endif
    
    return HCS_Entity_get_component_id(e,HCS_cInput);
}

HCS_Input* HCS_Input_get(HCS_Entity e)
{
    return &runData->HCS_Inputs[HCS_Entity_get_component_id(e,HCS_cInput)];
}

void HCS_Input_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Input_list, &runData->HCS_Input_used, &runData->HCS_Entities[e][HCS_cInput]);
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht Input entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    #endif
}

void HCS_Input_system(double delta)
{

    if (runData->HCS_Input_disabled)
        return;
    int j;
    for (j = 0; j < runData->HCS_Input_used; j++)
    {
        int i = runData->HCS_Input_list[j];
        
        if (runData->HCS_Inputs[i].active)
        {
            HCS_State* d = HCS_State_get(HCS_Entity_get_entity_id(i,HCS_cInput));
            d->up = (runData->HCS_Input_Pad.y < -40);
            d->down = (runData->HCS_Input_Pad.y > 40);
            d->left = (runData->HCS_Input_Pad.x < -40);
            d->right = (runData->HCS_Input_Pad.x > 40);
            d->A = (runData->HCS_Input_A.down);
            d->B = (runData->HCS_Input_B.down);

            if (HCS_Entity_has_component(HCS_Entity_get_entity_id(i,HCS_cInput),HCS_cCollider))
                d->on_ground = HCS_Collider_get(HCS_Entity_get_entity_id(i,HCS_cInput))->on_ground;
            else
                d->on_ground = false;                
        }
    }
}
