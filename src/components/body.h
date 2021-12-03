
#pragma once

int HCS_Body_add(HCS_Entity e, float x, float y, int w, int h)
{
    runData->HCS_Entities[e][HCS_cBody] = LSD_Math_get_id_from_array(runData->HCS_Body_list, &runData->HCS_Body_used, HCS_MAX_BODIES);
    runData->HCS_Bodies[HCS_Entity_get_component_id(e,HCS_cBody)].pos.x = x;
    runData->HCS_Bodies[HCS_Entity_get_component_id(e,HCS_cBody)].pos.y = y;
    runData->HCS_Bodies[HCS_Entity_get_component_id(e,HCS_cBody)].size.x = w;
    runData->HCS_Bodies[HCS_Entity_get_component_id(e,HCS_cBody)].size.y = h;
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Body hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    return HCS_Entity_get_component_id(e,HCS_cBody);
}

HCS_Body* HCS_Body_get(HCS_Entity e)
{
    if (!HCS_Entity_has_component(e,HCS_cBody))
        LSD_Log(LSD_ltERROR,"Entity hat keinen Body!");
    return &runData->HCS_Bodies[HCS_Entity_get_component_id(e,HCS_cBody)];
}

void HCS_Body_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Body_list,&runData->HCS_Body_used,&runData->HCS_Entities[e][HCS_cBody]);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Body entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}
