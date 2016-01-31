#ifndef PROJECTILEBLASTER_H
#define PROJECTILEBLASTER_H

#pragma once

#include "../framework/src/Projectile.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
class Scene;
}

class ProjectileBlaster : public Projectile
{

    URHO3D_OBJECT(ProjectileBlaster,Projectile);

public:

    ProjectileBlaster(Context* context);
    ~ProjectileBlaster();
    
    static void RegisterObject(Context* context);

    virtual void Start();
    virtual void Setup(const Vector3 direction);
    virtual void Setup(VariantMap& parms);

protected:
    virtual void SetupLocal();
   
};
#endif