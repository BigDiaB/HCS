
#pragma once

int HCS_Input_add(HCS_Entity e)
{
    runData->HCS_Entities[e][HCS_cInput] = get_unused_id_from_blacklist(runData->HCS_Input_list, &runData->HCS_Input_used, HCS_MAX_INPUTS);
    runData->HCS_Inputs[HCS_Entity_get_component_id(e,HCS_cInput)].active = true;
    
    return HCS_Entity_get_component_id(e,HCS_cInput);
}

HCS_Input* HCS_Input_get(HCS_Entity e)
{
    return &runData->HCS_Inputs[HCS_Entity_get_component_id(e,HCS_cInput)];
}

void HCS_Input_remove(HCS_Entity e)
{
    remove_element_from_array(runData->HCS_Input_list, &runData->HCS_Input_used, &runData->HCS_Entities[e][HCS_cInput]);
}

void HCS_Input_system()
{
    int j;
    for (j = 0; j < runData->HCS_Input_used; j++)
    {
        int i = runData->HCS_Input_list[j];
        
        if (runData->HCS_Inputs[i].active)
        {
            HCS_State* d = HCS_State_get(HCS_Entity_get_entity_id(i,HCS_cInput));
            d->up = isDown(INPUT_UP);
            d->down = isDown(INPUT_DOWN);
            d->left = isDown(INPUT_LEFT);
            d->right = isDown(INPUT_RIGHT);
            d->jump = isDown(INPUT_JUMP);
            d->action1 = isDown(INPUT_ACTION_1);
            d->action2 = isDown(INPUT_ACTION_2);
            
            if (HCS_Entity_has_component(HCS_Entity_get_entity_id(i,HCS_cInput),HCS_cCollider))
                d->on_ground = HCS_Collider_get(HCS_Entity_get_entity_id(i,HCS_cInput))->on_ground;
            else
                d->on_ground = false;                
        }
    }
}
