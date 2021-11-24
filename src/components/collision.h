
#pragma once

int HCS_Collider_add(HCS_Entity e, vec2i size_mod)
{
    if (!HCS_Entity_has_component(e,HCS_cBody))
    {
        LSD_Log(LSD_ltERROR, "Entity hat nicht die vorausgesetzten Komponente für Collider");
    }
    runData->HCS_Entities[e][HCS_cCollider] = get_unused_id_from_blacklist(runData->HCS_Collider_list, &runData->HCS_Collider_used, HCS_MAX_COLLIDERS);
    
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].active = true;
    
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].offset = size_mod;
    
    
    if (HCS_Entity_has_component(e,HCS_cMovement))
        runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].type = HCS_Col_Dynamic;
    else
        runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].type = HCS_Col_Static;
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht ein Collider hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    return HCS_Entity_get_component_id(e,HCS_cCollider);
    
}


HCS_Collider* HCS_Collider_get(HCS_Entity e)
{
    return &runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)];
}

void HCS_Collider_remove(HCS_Entity e)
{
    remove_element_from_array(runData->HCS_Collider_list, &runData->HCS_Collider_used, &runData->HCS_Entities[e][HCS_cCollider]);
    LSD_Log(LSD_ltMESSAGE,"Entity %d mit dem Namen %s wurde erfolgreicht ein Collider entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
}


void HCS_Collider_system(double delta)
{
    int r;
    for (r = 0; r < runData->HCS_Collider_used; r++)
    {
        int i = runData->HCS_Collider_list[r];
        if (runData->HCS_Colliders[i].type == HCS_Col_Dynamic)
        {
            runData->HCS_Colliders[i].last_on_ground = runData->HCS_Colliders[i].on_ground;
            runData->HCS_Colliders[i].on_ground = false;
            int s;
            for (s = 0; s < runData->HCS_Collider_used; s++)
            {
                int j = runData->HCS_Collider_list[s];
                if (i != j)
                {
                    if (runData->HCS_Colliders[i].active && runData->HCS_Colliders[j].active)
                    {
                        {
                            HCS_Body* one = (HCS_Body_get((HCS_Entity_get_entity_id(i, HCS_cCollider))));
                            HCS_Body* t_two = (HCS_Body_get((HCS_Entity_get_entity_id(j, HCS_cCollider))));
                            HCS_Body* two = &runData->HCS_Colliders[j].collider;
                            HCS_Body* test = &runData->HCS_Colliders[i].collider;
                            *test = *one;
                            
                            *two = *t_two;
                            
                            if (runData->HCS_Colliders[j].offset.x != 0)
                                two->size.x -= t_two->size.x / runData->HCS_Colliders[j].offset.x;
                            if (runData->HCS_Colliders[j].offset.y != 0)
                                two->size.y -= t_two->size.x / runData->HCS_Colliders[j].offset.y;

                            if (runData->HCS_Colliders[i].offset.x != 0)
                                test->size.x -= one->size.x / runData->HCS_Colliders[i].offset.x;
                            if (runData->HCS_Colliders[i].offset.y != 0)
                                test->size.y -= one->size.x / runData->HCS_Colliders[i].offset.y;
                            
                            if (AABB(
                                     test->pos,
                                     two->pos,
                                     test->size,
                                     two->size))
                            {
                                
                                if (runData->HCS_Colliders[j].type == HCS_Col_Static)
                                {
                                    HCS_Movement* move = HCS_Movement_get(HCS_Entity_get_entity_id(i, HCS_cCollider));
                                    
                                    #define player_bottom test->pos.y + test->size.y
                                    #define tiles_bottom two->pos.y + two->size.y
                                    #define player_right test->pos.x + test->size.x
                                    #define tiles_right two->pos.x + two->size.x
                                    
                                    #define b_collision tiles_bottom - test->pos.y
                                    #define t_collision player_bottom - two->pos.y
                                    #define l_collision player_right - two->pos.x
                                    #define r_collision tiles_right - test->pos.x
                                    
                                    if (fabsf(test->pos.y  > two->pos.y ? two->pos.y + two->size.y - test->pos.y : test->pos.y + test->size.y - two->pos.y) < fabsf(test->pos.x > two->pos.x ? two->pos.x + two->size.x - test->pos.x : test->pos.x + test->size.x - two->pos.x))
                                    {
                                        
                                        if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision )
                                        {
                                            //Top collision
                                            one->pos.y = two->pos.y - test->size.y;
                                            move->vel.y = 0;
                                            runData->HCS_Colliders[i].on_ground = true;
                                        }
                                        if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)
                                        {
                                            //bottom collision
                                            one->pos.y = two->pos.y + two->size.y;
                                            move->vel.y = 0;
                                        }
                                        
                                    }
                                    else
                                    {
                                        
                                        if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
                                        {
                                            //Left collision
                                            one->pos.x  = two->pos.x - test->size.x;
                                            move->vel.x = 0;
                                        }
                                        if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision )
                                        {
                                            //Right collision
                                            one->pos.x = two->pos.x + two->size.x;
                                            move->vel.x = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

