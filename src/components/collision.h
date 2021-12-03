
#pragma once

int HCS_Collider_add(HCS_Entity e, LSD_Vec2f pos_mod, LSD_Vec2i size_mod)
{
    if (!HCS_Entity_has_component(e,HCS_cBody))
    {
        LSD_Log(LSD_ltERROR, "Entity hat nicht die vorausgesetzten Komponente für Collider");
    }
    runData->HCS_Entities[e][HCS_cCollider] = LSD_Math_get_id_from_array(runData->HCS_Collider_list, &runData->HCS_Collider_used, HCS_MAX_COLLIDERS);
    
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].active = true;
    
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].offset.size = size_mod;
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].offset.pos = pos_mod;
    
    
    if (HCS_Entity_has_component(e,HCS_cMovement))
        runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].type = HCS_Col_Dynamic;
    else
        runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].type = HCS_Col_Static;
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Collider hinzugefügt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
    return HCS_Entity_get_component_id(e,HCS_cCollider);
    
}


HCS_Collider* HCS_Collider_get(HCS_Entity e)
{
    return &runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)];
}

void HCS_Collider_remove(HCS_Entity e)
{
    LSD_Math_remove_object_from_array(runData->HCS_Collider_list, &runData->HCS_Collider_used, &runData->HCS_Entities[e][HCS_cCollider]);
    LSD_Log(LSD_ltCUSTOM,"HCS: Entity %d mit dem Namen %s wurde erfolgreicht ein Collider entfernt!",e,HCS_Name_get(HCS_Entity_get_component_id(e,HCS_cName))->name);
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
                            
                            if (runData->HCS_Colliders[i].offset.size.x != 0)
                                test->size.x -= (double)(one->size.x / 8) * (double)runData->HCS_Colliders[i].offset.size.x;
                            if (runData->HCS_Colliders[i].offset.size.y != 0)
                                test->size.y -= (double)(one->size.y / 8) * (double)runData->HCS_Colliders[i].offset.size.y;
                            if (runData->HCS_Colliders[i].offset.pos.x != 0)
                            {
                                test->pos.x += runData->HCS_Colliders[i].offset.pos.x * (one->size.x / 8);
                                // test->pos.x += 1;
                                test->size.x -= 3;
                            }
                            // else
                                // test->pos.x -= 1;
                            if (runData->HCS_Colliders[i].offset.pos.y != 0)
                            {
                                test->pos.y += runData->HCS_Colliders[i].offset.pos.y * (one->size.y / 8);
                                // test->pos.y += 1;
                                test->size.y -= 3;
                            }
                            // else
                                // test->pos.y -= 1;

                            if (runData->HCS_Colliders[i].offset.size.x != 0)
                                two->size.x -= (double)(t_two->size.x / 8) * (double)runData->HCS_Colliders[j].offset.size.x;
                            if (runData->HCS_Colliders[i].offset.size.y != 0)
                                two->size.y -= (double)(t_two->size.y / 8) * (double)runData->HCS_Colliders[j].offset.size.y;
                            if (runData->HCS_Colliders[j].offset.pos.x != 0)
                            {
                                two->pos.x += runData->HCS_Colliders[j].offset.pos.x * (t_two->size.x / 8);
                                two->pos.x += 1;
                                two->size.x -= 3;
                            }
                            else
                                two->pos.x -= 1;
                            if (runData->HCS_Colliders[j].offset.pos.y != 0)
                            {
                                two->pos.y += runData->HCS_Colliders[j].offset.pos.y * (t_two->size.y / 8);
                                two->pos.y += 1;
                                // two->size.y -= 3;
                            }
                            // else
                                // two->pos.y -= 1;
                            
                            

                            LSD_Vec2i pos_diff = {0,0};
                            LSD_Vec_sub(pos_diff,test->pos,one->pos);
                            
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
                                            one->pos.y = two->pos.y - test->size.y - pos_diff.y;
                                            move->vel.y = 0;
                                            runData->HCS_Colliders[i].on_ground = true;
                                        }
                                        if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)
                                        {
                                            //bottom collision
                                            one->pos.y = two->pos.y + two->size.y - pos_diff.y;
                                            move->vel.y = 0;
                                        }
                                        
                                    }
                                    else
                                    {
                                        
                                        if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
                                        {
                                            //Left collision
                                            one->pos.x  = two->pos.x - test->size.x - pos_diff.x;
                                            move->vel.x = 0;
                                        }
                                        if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision )
                                        {
                                            //Right collision
                                            one->pos.x = two->pos.x + two->size.x - pos_diff.x;
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

