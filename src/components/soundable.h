#pragma once

int HCS_Soundable_add(HCS_Entity e, char* path, HCS_Soundtype type)
{
    runData->HCS_Entities[e][HCS_cSoundable] = get_unused_id_from_blacklist(runData->HCS_Soundable_list, &runData->HCS_Soundable_used, HCS_MAX_SOUNDABLES);
    runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].should_be_playing = false;
    runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].type = type;
    runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].path = path;
    
    if (type == HCS_Sound_Music)
        runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].music = Mix_LoadMUS(path);
    else
        runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].SFX = Mix_LoadWAV(path);
    
    return HCS_Entity_get_component_id(e,HCS_cSoundable);
    
}

HCS_Soundable* HCS_Soundable_get(HCS_Entity e)
{
    return &runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)];
}

void HCS_Soundable_remove(HCS_Entity e)
{
    if (runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].type == HCS_Sound_Effect)
        Mix_FreeChunk(runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].SFX);
    else
        Mix_FreeMusic(runData->HCS_Soundables[HCS_Entity_get_component_id(e,HCS_cSoundable)].music);
    
    remove_element_from_array(runData->HCS_Soundable_list,&runData->HCS_Soundable_used,&runData->HCS_Entities[e][HCS_cSoundable]);
}

void HCS_Soundable_system()
{
    int j;
    for (j = 0; j < runData->HCS_Soundable_used; j++)
    {
        int i = runData->HCS_Soundable_list[j];
        if (runData->HCS_Soundables[i].should_be_playing)
        {
            if (runData->HCS_Soundables[i].type == HCS_Sound_Effect)
            {
                Mix_PlayChannel( -1, runData->HCS_Soundables[i].SFX,0);
                runData->HCS_Soundables[i].should_be_playing = false;
            }
            else
            {
                Mix_PlayMusic(runData->HCS_Soundables[i].music, 1);
                runData->HCS_Soundables[i].should_be_playing = false;
            }
        }
    }
}
