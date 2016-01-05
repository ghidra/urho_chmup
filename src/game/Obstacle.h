#ifndef OBSTACLE_H
#define OBSTACLE_H

#pragma once

#include "../framework/src/Actor.h"

using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;

}

class Obstacle : public Actor
{

    URHO3D_OBJECT(Obstacle,Actor);

public:

    Obstacle(Context* context);

    static void RegisterObject(Context* context);

    virtual void Start();
    virtual void FixedUpdate(float timeStep);
    virtual void Setup();

protected:

    virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
   
};
#endif
