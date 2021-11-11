
#pragma once

double delta = 0.0f;
bool frozen = true;

struct timeval begin, end;

void tick()
{
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    delta = seconds + microseconds*1e-6;
    gettimeofday(&begin, 0);
    if (frozen)
    {
        frozen = false;
        delta = 0.0f;
    }
}