#ifndef PROJECTILELASER_H
#define PROJECTILELASER_H

#pragma once

#include "../framework/src/Projectile.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
class Scene;
}

class ProjectileLaser : public Projectile
{

    URHO3D_OBJECT(ProjectileLaser,Projectile);

public:

    ProjectileLaser(Context* context);
    ~ProjectileLaser();
    
    static void RegisterObject(Context* context);

    virtual void Start();
    virtual void Setup(const Vector3 direction);
    virtual void Setup(VariantMap& parms);

protected:
    virtual void SetupLocal();
    //virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
   
};
#endif