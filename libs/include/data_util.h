
#ifndef DATA_UTIL_H
#define DATA_UTIL_H

#include <unistd.h>
#include <math.h>

typedef struct {
    int x,y;
} vec2i;

typedef struct {
    float x,y;
} vec2f;

typedef struct {
    double x,y;
} vec2d;

vec2i vec_new_int(int x, int y)
{
    vec2i vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

vec2f vec_new_float(float x, float y)
{
    vec2f vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

vec2d vec_new_double(double x, double y)
{
    vec2d vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

#define vec_add(Z,X,Y) Z.x = X.x + Y.x; Z.y = X.y + Y.y;
#define vec_sub(Z,X,Y) Z.x = X.x - Y.x; Z.y = X.y - Y.y;
#define vec_mul(Z,X,Y) Z.x = X.x * Y.x; Z.y = X.y * Y.y;
#define vec_div(Z,X,Y) Z.x = X.x / Y.x; Z.y = X.y / Y.y;


typedef char bool;
const char true = 1;
const char false = 0;

#define function_ptr(X,Y,Z) X(*Y)Z
#define AABB(pos1,pos2,size1,size2) (pos1.x < pos2.x+size2.x && pos2.x < pos1.x+size1.x && pos1.y < pos2.y+size2.y && pos2.y < pos1.y+size1.y)

void remove_element_from_array(int arr[], int* max_arr, int* index_to_be_removed)
{
    int i;
    for (i = *index_to_be_removed; i < *max_arr - 1; i++)
    {
        arr[i] = arr[i + 1];
    }
    (*max_arr)--;
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

#define CHARS_TIL_ROOT_OF_PROJ 6
void prepare_path(char* argv[])
{
    char path_save[1024];
    char cwd[1024];
    char* p;
    if(!(p = strrchr(argv[0], '/')))
        getcwd(cwd, sizeof(cwd));
    else
    {
        *p = '\0';
        getcwd(path_save, sizeof(path_save));
        chdir(argv[0]);
        getcwd(cwd, sizeof(cwd));
        chdir(path_save);
    }
    cwd[strlen(cwd)- CHARS_TIL_ROOT_OF_PROJ] = '\0';
    chdir(cwd);
}

#endif
