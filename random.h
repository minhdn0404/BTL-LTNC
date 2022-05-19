

#pragma once
#include <cmath>

int random (int min, int max)
{
    return min+rand()%(max-min+1);
}

