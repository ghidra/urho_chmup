#ifndef OBSTACLE_H
#define OBSTACLE_H

#pragma once

#include "../framework/src/Pawn.h"

using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;

}

class Obstacle : public Pawn
{

    URHO3D_OBJECT(Obstacle,Pawn);

public:

    Obstacle(Context* context);

    static void RegisterObject(Context* context);

    virtual void Start();
    virtual void FixedUpdate(float timeStep);
    virtual void Setup();
   
};
#endif
