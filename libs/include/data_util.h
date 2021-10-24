
#pragma once

void remove_element_from_array(int arr[], int* max_arr, int* index_to_be_removed)
{
    int i;
    for (i = *index_to_be_removed + 1; i < *max_arr + 1; i++)
    {
        arr[i - 1] = arr[i];
    }
    (*max_arr)--;
    *index_to_be_removed = -1;
}

int get_unused_id_from_blacklist(int black_list[], int* black_fill, int max_id)
{
    int i;
    for (i = 0; i < max_id; i++)
    {
        bool newo = true;
        int j;
        for (j = 0; j < (*black_fill); j++)
            if (i == black_list[j])
                newo = false;
        if (newo)
        {
            black_list[*black_fill] = i;
            (*black_fill)++;
            return i;
        }
    }
    printf("Keine freien IDs!\n");
    exit(1);
}

float map_number_in_range_to_new_range(float num, float min1, float max1, float min2, float max2)
{
    return (num - min1) * (max2 - min2) / (max1 - min1) + min2;
}
