
#pragma once

int HCS_Collider_add(HCS_Entity e)
{
    if (!HCS_Entity_has_component(e,HCS_cBody))
    {
        LSD_Log(LSD_ltERROR, "Entity hat nicht die vorausgesetzten Komponente für Collider");
    }
    runData->HCS_Entities[e][HCS_cCollider] = get_unused_id_from_blacklist(runData->HCS_Collider_list, &runData->HCS_Collider_used, HCS_MAX_COLLIDERS);
    runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].active = true;
    if (HCS_Entity_has_component(e,HCS_cMovement))
        runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].type = HCS_Col_Dynamic;
    else
        runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)].type = HCS_Col_Static;
    return HCS_Entity_get_component_id(e,HCS_cCollider);
    
}


HCS_Collider* HCS_Collider_get(HCS_Entity e)
{
    return &runData->HCS_Colliders[HCS_Entity_get_component_id(e,HCS_cCollider)];
}

void HCS_Collider_remove(HCS_Entity e)
{
    remove_element_from_array(runData->HCS_Collider_list, &runData->HCS_Collider_used, &runData->HCS_Entities[e][HCS_cCollider]);
}

bool AABB(vec2f pos1, vec2f pos2, vec2i size1, vec2i size2)
{
    return (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y);
}

void HCS_Collider_system()
{
    int r;
    for (r = 0; r < runData->HCS_Collider_used; r++)
    {
        int i = runData->HCS_Collider_list[r];
        if (runData->HCS_Colliders[i].type == HCS_Col_Dynamic)
        {
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
                            HCS_Body* two = (HCS_Body_get((HCS_Entity_get_entity_id(j, HCS_cCollider))));
                            if (AABB(
                                     one->pos,
                                     two->pos,
                                     one->size,
                                     two->size))
                            {
                                
                                if (runData->HCS_Colliders[j].type == HCS_Col_Static)
                                {
                                    vec2f s1 = (*HCS_Body_get(HCS_Entity_get_entity_id(i, HCS_cCollider))).pos;
                                    vec2f s2 = (*HCS_Body_get(HCS_Entity_get_entity_id(j, HCS_cCollider))).pos;
                                    vec2i w1 = (*HCS_Body_get(HCS_Entity_get_entity_id(i, HCS_cCollider))).size;
                                    vec2i w2 = (*HCS_Body_get(HCS_Entity_get_entity_id(j, HCS_cCollider))).size;
                                    HCS_Movement* move = HCS_Movement_get(HCS_Entity_get_entity_id(i, HCS_cCollider));
                                    vec2f overlap;
                                    overlap.x = (one->pos.x + one->size.x > two->pos.x + two->size.x ? two->pos.x + two->size.x - one->pos.x : one->pos.x + one->size.x - two->pos.x);
                                    overlap.y = (one->pos.y + one->size.y > two->pos.y + two->size.y ? two->pos.y + two->size.y - one->pos.y : one->pos.y + one->size.y - two->pos.y);
                                    
                                    if (fabsf(overlap.y) < fabsf(overlap.x))
                                    {
#define MOE -1 //Wiggle-Room
//#define CRF 0 //Bounce
                                        // TOP:
                                        if ((s1.x + w1.x > s2.x || s1.x < s2.x + w2.x) && (s1.y + w1.y > s2.y && s1.y + w1.y < s2.y + w2.x/2) && move->vel.y > 0)
                                        {
                                            one->pos.y = two->pos.y - one->size.y - MOE;
                                            //move->vel.y *= -CRF;
                                            move->vel.y = 0;
                                            runData->HCS_Colliders[i].on_ground = true;
                                        }
                                        // BOTTOM:
                                        else if ((s1.x + w1.x > s2.x + 5 && s1.x < s2.x + w2.x - 5) && (s1.y < s2.y + w2.y && s1.y > s2.y + w2.y/2))
                                        {
                                            one->pos.y = two->pos.y + two->size.y + MOE;
                                            //move->vel.y *= -CRF;
                                            move->vel.y = 0;
                                        }
                                    }
                                    else
                                    {
                                        // RIGHT:
                                        if ((s1.y + w1.y > s2.y || s1.y < s2.y + w2.y) && (s1.x + w1.x > s2.x && s1.x + w1.y < s2.x + w2.x/2))
                                        {
                                            one->pos.x = two->pos.x - one->size.x - MOE;
                                            //move->vel.x *= -CRF;
                                            move->vel.x = 0;
                                        }
                                        // LEFT:
                                        else if ((s1.y + w1.y > s2.y || s1.y < s2.y + w2.y) && (s1.x < s2.x + w2.x && s1.x > s2.x + w2.x/2))
                                        {
                                            one->pos.x = two->pos.x + two->size.x + MOE;
                                            //move->vel.x *= -CRF;
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

