// Player.h
#pragma once
#include "Actor.h"

FUZZUM_CLASS()
class APlayer : public AActor {
    FUZZUM_BODY()

public:
    FUZZUM_FIELD(DisplayName="Health", Replicated)
    int Health = 100;

    FUZZUM_IMPLEMENT_VISIT_PROPS(APlayer)
};

#include "../Generated/Player.refl.h"