#ifndef PICKUPGUN_H
#define PICKUPGUN_H

#pragma once

#include "../framework/src/PickUp.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
class Scene;
}

class PickUpGun : public PickUp
{

    URHO3D_OBJECT(PickUpGun,PickUp);

public:

    PickUpGun(Context* context);
    ~PickUpGun();

    static void RegisterObject(Context* context);

     virtual void Start();
    //virtual void FixedUpdate(float timeStep);
    virtual void Setup();

protected:

    virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
   
};
#endif