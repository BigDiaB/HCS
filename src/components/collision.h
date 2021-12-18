
#pragma once

int HCS_Collider_add(HCS_Entity e, LSD_Vec2f pos_mod, LSD_Vec2i size_mod, void (*func)(HCS_Entity, HCS_Entity))
{
    if (!HCS_Entity_has_component(e,HCS_cBody))
    {
        LSD_Log(LSD_ltERROR, "Entity hat nicht die vorausgesetzten Komponente für Collider");
    }
    runData->HCS_Entities[e][HCS_cCollider] = LSD_Math_get_id_from_array(runData->HCS_Collider_list, &runData->HCS_Collider_used, HCS_MAX_COLLIDERS);
    
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].active = true;
    
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].offset.size = size_mod;
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].offset.pos = pos_mod;
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].func = func;
    
    
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

void HCS_Collider_STD_callback(HCS_Entity this, HCS_Entity other)
{
    int i = HCS_Entity_get_component_id(this,HCS_cCollider), j = HCS_Entity_get_component_id(other,HCS_cCollider);

    HCS_Body* this_body = HCS_Body_get(HCS_Entity_get_entity_id(i, HCS_cCollider));
    HCS_Body* other_collider = &runData->HCS_Colliders[j].collider;
    HCS_Body* this_collider = &runData->HCS_Colliders[i].collider;

    LSD_Vec2i pos_diff = {0,0};
    LSD_Vec_sub(pos_diff,this_collider->pos,HCS_Body_get(HCS_Entity_get_entity_id(i, HCS_cCollider))->pos);

    if (runData->HCS_Colliders[j].type == HCS_Col_Static)
    {
        HCS_Movement* move = HCS_Movement_get(HCS_Entity_get_entity_id(i, HCS_cCollider));
        
        #define this_bottom this_collider->pos.y + this_collider->size.y
        #define other_bottom other_collider->pos.y + other_collider->size.y
        #define this_right this_collider->pos.x + this_collider->size.x
        #define other_right other_collider->pos.x + other_collider->size.x
        
        #define b_collision other_bottom - this_collider->pos.y
        #define t_collision this_bottom - other_collider->pos.y
        #define l_collision this_right - other_collider->pos.x
        #define r_collision other_right - this_collider->pos.x

        
        if (fabsf(this_collider->pos.y  > other_collider->pos.y ? other_collider->pos.y + other_collider->size.y - this_collider->pos.y : this_collider->pos.y + this_collider->size.y - other_collider->pos.y) < fabsf(this_collider->pos.x > other_collider->pos.x ? other_collider->pos.x + other_collider->size.x - this_collider->pos.x : this_collider->pos.x + this_collider->size.x - other_collider->pos.x))
        {
            
            if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision )
            {
                this_body->pos.y = other_collider->pos.y - this_collider->size.y - pos_diff.y;
                move->vel.y = 0;
                runData->HCS_Colliders[i].on_ground = true;
            }
            if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)
            {
                this_body->pos.y = other_collider->pos.y + other_collider->size.y - pos_diff.y;
                move->vel.y = 0;
            }
            
        }
        else
        {
            
            if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
            {
                this_body->pos.x  = other_collider->pos.x - this_collider->size.x - pos_diff.x;
                move->vel.x = 0;
            }
            if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision )
            {
                this_body->pos.x = other_collider->pos.x + other_collider->size.x - pos_diff.x;
                move->vel.x = 0;
            }
        }
    }
}


void HCS_Collider_system(double delta)
{
    int r,i,j,s;
    for (r = 0; r < runData->HCS_Collider_used; r++)
    {
        i = runData->HCS_Collider_list[r];
        if (runData->HCS_Colliders[i].type == HCS_Col_Dynamic)
        {
            runData->HCS_Colliders[i].last_on_ground = runData->HCS_Colliders[i].on_ground;
            runData->HCS_Colliders[i].on_ground = false;
            for (s = 0; s < runData->HCS_Collider_used; s++)
            {
                j = runData->HCS_Collider_list[s];
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
                                test->size.x -= 3;
                            }
                            if (runData->HCS_Colliders[i].offset.pos.y != 0)
                            {
                                test->pos.y += runData->HCS_Colliders[i].offset.pos.y * (one->size.y / 8);
                                test->size.y -= 3;
                            }

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
                            }
                            
                            
                            if (AABB(test->pos,two->pos,test->size,two->size))
                                runData->HCS_Colliders[i].func(HCS_Entity_get_entity_id(i,HCS_cCollider),HCS_Entity_get_entity_id(j,HCS_cCollider));
                        }
                    }
                }
            }
        }
    }
}

