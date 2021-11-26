
#pragma once

int HCS_Jump_add(HCS_Entity e, double n, bool m, double l)
{
    if (!HCS_Entity_has_component(e,HCS_cCollider) || !HCS_Entity_has_component(e,HCS_cMovement) || !HCS_Entity_has_component(e,HCS_cState))
        LSD_Log(LSD_ltERROR, "Entity fehlen vorausgesetzte Komponente für Jump");
    
    runData->HCS_Entities[e][HCS_cJump] = LSD_Math_get_id_from_array(runData->HCS_Jump_list, &runData->HCS_Jump_used, HCS_MAX_JUMPS);
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].strength = n;
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].needs_ground = m;
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].jump_time = l;
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].jump_timer = l;
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].active = true;
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].can_jump = true;
    runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)].jump_ground_timer = 0.0f;
    
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht Jump hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    
    return HCS_Entity_get_component_id(e,HCS_cJump);
}

HCS_Jump* HCS_Jump_get(HCS_Entity e)
{
    return &runData->HCS_Jumps[HCS_Entity_get_component_id(e,HCS_cJump)];
}

void HCS_Jump_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Jump_list,&runData->HCS_Jump_used,&runData->HCS_Entities[e][HCS_cJump]);
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht Jump entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}

void HCS_Jump_system(double delta)
{
    int j;
    for (j = 0; j < runData->HCS_Jump_used; j++)
    {
        int i = runData->HCS_Jump_list[j];
        HCS_Movement* mov = HCS_Movement_get(HCS_Entity_get_entity_id(i,HCS_cJump));
        HCS_State d = *HCS_State_get(HCS_Entity_get_entity_id(i,HCS_cJump));
        runData->HCS_Jumps[i].jump_ground_timer -= delta;
        if (d.B)
        {
            if (runData->HCS_Jumps[i].needs_ground)
            {
                if (runData->HCS_Jumps[i].jump_ground_timer > 0.0f)
                {
                    mov->vel.y = -runData->HCS_Jumps[i].strength;
                    runData->HCS_Jumps[i].jump_ground_timer = 0.0f;
                }
            }
            else
                if (runData->HCS_Jumps[i].jump_timer >= runData->HCS_Jumps[i].jump_time)
                {
                    mov->vel.y = -runData->HCS_Jumps[i].strength;
                    runData->HCS_Jumps[i].jump_timer = 0;
                }
        }
        else if (!d.on_ground)
            if (mov->vel.y < 0)
                mov->vel.y *= 0.5f;
        if (d.on_ground)
            runData->HCS_Jumps[i].jump_ground_timer = 0.2f;
    }
}
