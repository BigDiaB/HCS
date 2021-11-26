
#pragma once

int HCS_Input_add(HCS_Entity e)
{
    runData->HCS_Entities[e][HCS_cInput] = LSD_Math_get_id_from_array(runData->HCS_Input_list, &runData->HCS_Input_used, HCS_MAX_INPUTS);
    runData->HCS_Inputs[HCS_Entity_get_component_id(e,HCS_cInput)].active = true;
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht Input hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    return HCS_Entity_get_component_id(e,HCS_cInput);
}

HCS_Input* HCS_Input_get(HCS_Entity e)
{
    return &runData->HCS_Inputs[HCS_Entity_get_component_id(e,HCS_cInput)];
}

void HCS_Input_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Input_list, &runData->HCS_Input_used, &runData->HCS_Entities[e][HCS_cInput]);
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht Input entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}

void HCS_Input_system(double delta)
{
    int j;
    for (j = 0; j < runData->HCS_Input_used; j++)
    {
        int i = runData->HCS_Input_list[j];
        
        if (runData->HCS_Inputs[i].active)
        {
            HCS_State* d = HCS_State_get(HCS_Entity_get_entity_id(i,HCS_cInput));
            d->up = isDown("up");
            d->down = isDown("down");
            d->left = isDown("left");
            d->right = isDown("right");
            d->A = isDown("A");
            d->B = isDown("B");

            if (HCS_Entity_has_component(HCS_Entity_get_entity_id(i,HCS_cInput),HCS_cCollider))
                d->on_ground = HCS_Collider_get(HCS_Entity_get_entity_id(i,HCS_cInput))->on_ground;
            else
                d->on_ground = false;                
        }
    }
}
