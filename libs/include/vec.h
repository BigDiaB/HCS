
#pragma once

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
