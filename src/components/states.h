
#pragma once

int HCS_State_add(HCS_Entity e)
{
    runData->HCS_Entities[e][HCS_cState] = LSD_Math_get_id_from_array(runData->HCS_State_list, &runData->HCS_State_used, HCS_MAX_STATES);
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].up = false;
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].down = false;
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].left = false;
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].right = false;
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].A = false;
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].B = false;
    runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)].on_ground = false;
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht States hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    return HCS_Entity_get_component_id(e,HCS_cState);
}

HCS_State* HCS_State_get(HCS_Entity e)
{
    return &runData->HCS_States[HCS_Entity_get_component_id(e,HCS_cState)];
}

void HCS_State_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_State_list,&runData->HCS_State_used,&runData->HCS_Entities[e][HCS_cState]);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht States entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}
