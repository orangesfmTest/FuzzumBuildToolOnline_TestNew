// Actor.h
#pragma once
#include "FuzzumObject.h"
#include <string>

FUZZUM_CLASS()
class AActor : public FuzzumObject {
    FUZZUM_BODY()	

public:
    FUZZUM_FIELD(DisplayName="Actor Name")
    std::string Name;

    FUZZUM_IMPLEMENT_VISIT_PROPS(AActor)
};

#include "../Generated/Actor.refl.h"