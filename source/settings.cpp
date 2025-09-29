#include "settings.h"

Difficulty Settings::easyDiff = {
    1.5f,
    40,
    10,
    {bottomScreen.x / 40, bottomScreen.y / 40},
    2.f
};

Difficulty Settings::mediumDiff = {
    0.75f,
    20,
    50,
    {bottomScreen.x / 20, bottomScreen.y / 20},
    1.f
};

Difficulty Settings::hardDiff = {
    0.75f,
    20,
    75,
    {bottomScreen.x / 20, bottomScreen.y / 20},
    1.f
};

std::vector<Difficulty> Settings::diffs = {
    easyDiff,
    mediumDiff,
    hardDiff
};