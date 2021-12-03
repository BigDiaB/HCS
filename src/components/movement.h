
#pragma once

int HCS_Movement_add(HCS_Entity e, float sx, float sy)
{
    if (!HCS_Entity_has_component(e,HCS_cBody))
        LSD_Log(LSD_ltERROR, "Entity hat nicht die vorausgesetzten Komponente für Movement");
    
    runData->HCS_Entities[e][HCS_cMovement] = LSD_Math_get_id_from_array(runData->HCS_Movement_list, &runData->HCS_Movement_used, HCS_MAX_MOVEMENTS);
    runData->HCS_Movements[HCS_Entity_get_component_id(e,HCS_cMovement)].speed.x = sx;
    runData->HCS_Movements[HCS_Entity_get_component_id(e,HCS_cMovement)].speed.y = sy;
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht Movement hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    return HCS_Entity_get_component_id(e,HCS_cMovement);
}

HCS_Movement* HCS_Movement_get(HCS_Entity e)
{
    return &runData->HCS_Movements[HCS_Entity_get_component_id(e,HCS_cMovement)];
}

void HCS_Movement_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Movement_list,&runData->HCS_Movement_used,&runData->HCS_Entities[e][HCS_cMovement]);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht Movement entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}



void HCS_Movement_system(double delta)
{
    int j;
    for (j = 0; j < runData->HCS_Movement_used; j++)
    {
        int i = runData->HCS_Movement_list[j];
        
        
        if (HCS_Entity_has_component(HCS_Entity_get_entity_id(i,HCS_cMovement),HCS_cState))
        {
            HCS_Entity e = HCS_Entity_get_entity_id(i,HCS_cMovement);
            HCS_State* d = HCS_State_get(e);
            
            if (!HCS_Entity_has_component(e,HCS_cGravity))
            {
                if (d->up)
                    runData->HCS_Movements[i].vel.y -= runData->HCS_Movements[i].speed.y * delta;
                if (d->down)
                    runData->HCS_Movements[i].vel.y += runData->HCS_Movements[i].speed.y * delta;
            }
            if (d->left)
                runData->HCS_Movements[i].vel.x -= runData->HCS_Movements[i].speed.x * delta;
            if (d->right)
                runData->HCS_Movements[i].vel.x += runData->HCS_Movements[i].speed.x * delta;
        }
        
        
        (*HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cMovement))).pos.x += runData->HCS_Movements[i].vel.x * delta;
        
        runData->HCS_Movements[i].vel.x *= pow(0.002f,delta);
        
        (*HCS_Body_get(HCS_Entity_get_entity_id(i,HCS_cMovement))).pos.y += runData->HCS_Movements[i].vel.y * delta;
        
        
        runData->HCS_Movements[i].vel.y *= pow(0.002,delta);
    }
}
