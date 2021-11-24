
#pragma once

int HCS_Interactable_add(HCS_Entity e)
{
    int index = get_unused_id_from_blacklist(runData->HCS_Interactable_list,&runData->HCS_Interactable_used, HCS_MAX_INTERACTABLES);
    runData->HCS_Entities[e][HCS_cSprite] = index;
    
    return index;
}

HCS_Interactable* HCS_Interactable_get(HCS_Entity e)
{
    return &runData->HCS_Interactables[runData->HCS_Interactable_list[runData->HCS_Entities[e][HCS_cInteractable]]];
}

void HCS_Interactable_remove(HCS_Entity e)
{
    remove_element_from_array(runData->HCS_Interactable_list, &runData->HCS_Interactable_used, &runData->HCS_Entities[e][HCS_cInteractable]);
}
