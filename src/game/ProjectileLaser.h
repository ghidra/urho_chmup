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

    virtual void FixedUpdate(float timeStep);//we need this to  only to follow the parent
    void SetParentWeaponNode(Node* weapon);//we need this so that we have something to follow

protected:
    virtual void SetupLocal();
    //virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
   virtual void Impact(Node* node, const Vector3 pos, const Vector3 dir);

   SharedPtr<Node> weapon_node_;
};
#endif