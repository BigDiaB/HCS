
#pragma once

int HCS_Gravity_add(HCS_Entity e, double n, double m)
{
    if (!HCS_Entity_has_component(e,HCS_cMovement))
    {
        #ifdef HCS_DEBUG
        LSD_Log(LSD_ltERROR,"Entity fehlen vorausgesetzte Komponente für Gravity!");
        #endif
    }
    runData->HCS_Entities[e][HCS_cGravity] = LSD_Math_get_id_from_array(runData->HCS_Gravity_list, &runData->HCS_Gravity_used, HCS_MAX_GRAVITIES);
    runData->HCS_Gravities[HCS_Entity_get_component_id(e,HCS_cGravity)].force.x = n;
    runData->HCS_Gravities[HCS_Entity_get_component_id(e,HCS_cGravity)].force.y = m;
    runData->HCS_Gravities[HCS_Entity_get_component_id(e,HCS_cGravity)].active = true;
    
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht Gravity hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    #endif
    
    return HCS_Entity_get_component_id(e,HCS_cGravity);
}

HCS_Gravity* HCS_Gravity_get(HCS_Entity e)
{
    return &runData->HCS_Gravities[HCS_Entity_get_component_id(e,HCS_cGravity)];
}

void HCS_Gravity_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Gravity_list,&runData->HCS_Gravity_used,&runData->HCS_Entities[e][HCS_cGravity]);
    #ifdef HCS_DEBUG
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht Gravity entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    #endif
}

void HCS_Gravity_system(double delta)
{
    int j;
    for (j = 0; j < runData->HCS_Gravity_used; j++)
    {
        int i = runData->HCS_Gravity_list[j];
        if (runData->HCS_Gravities[i].active)
        {
            if (!HCS_Collider_get(HCS_Entity_get_entity_id(i,HCS_cGravity))->on_ground)
            {
                if (HCS_Movement_get(HCS_Entity_get_entity_id(i,HCS_cGravity))->vel.y > 0)

                    HCS_Movement_get(HCS_Entity_get_entity_id(i,HCS_cGravity))->vel.y += (runData->HCS_Gravities[i].force.y  * 10) * delta;
                else
                    HCS_Movement_get(HCS_Entity_get_entity_id(i,HCS_cGravity))->vel.y += ((runData->HCS_Gravities[i].force.y  * 10) / 3) * delta;
            }
            HCS_Movement_get(HCS_Entity_get_entity_id(i,HCS_cGravity))->vel.x += (runData->HCS_Gravities[i].force.x  * 10) * delta;
        }
    }
}
