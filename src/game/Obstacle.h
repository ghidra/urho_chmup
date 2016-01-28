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
    ~Obstacle();

    static void RegisterObject(Context* context);

    virtual void Start();
    virtual void FixedUpdate(float timeStep);
    virtual void Setup();
    virtual void TakeDamage(const float amount, const Vector3 pos = Vector3(), const Vector3 dir= Vector3(), const unsigned level = 0, const enum DamageType type = DT_BALISTIC);//called from something that hits this... like a projectile


//protected:

    //virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
   
};
#endif
