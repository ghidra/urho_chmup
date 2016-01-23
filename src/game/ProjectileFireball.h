#ifndef PROJECTILEFIREBALL_H
#define PROJECTILEFIREBALL_H

#pragma once

#include "../framework/src/Projectile.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
class Scene;
}

class ProjectileFireball : public Projectile
{

    URHO3D_OBJECT(ProjectileFireball,Projectile);

public:

    ProjectileFireball(Context* context);
    ~ProjectileFireball();
    
    static void RegisterObject(Context* context);

    virtual void Start();
    //virtual void FixedUpdate(float timeStep);
    virtual void Setup(const Vector3 direction);
    virtual void Setup(VariantMap& parms);

protected:
    virtual void SetupLocal();
    virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
   
};
#endif