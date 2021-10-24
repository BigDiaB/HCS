
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

#define vec_add(Z,X,Y) Z.x = X.x + Y.x; Z.y = X.y + Y.y;
#define vec_sub(Z,X,Y) Z.x = X.x - Y.x; Z.y = X.y - Y.y;
#define vec_mul(Z,X,Y) Z.x = X.x * Y.x; Z.y = X.y * Y.y;
#define vec_div(Z,X,Y) Z.x = X.x / Y.x; Z.y = X.y / Y.y;
