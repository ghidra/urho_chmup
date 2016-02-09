#ifndef CASING_H
#define CASING_H

#pragma once

#include "../framework/src/Actor.h"

using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;

}

class Casing : public Actor
{

    URHO3D_OBJECT(Casing,Actor);

public:

    Casing(Context* context);
    ~Casing();

    static void RegisterObject(Context* context);

    virtual void Setup();
    //virtual void Setup();  
};
#endif
