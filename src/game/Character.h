#ifndef CHARACTER_H
#define CHARACTER_H

#pragma once

#include "../framework/src/Pawn.h"

using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;

}

class Character : public Pawn
{

    URHO3D_OBJECT(Character,Pawn);

public:

    Character(Context* context);

    static void RegisterObject(Context* context);

    virtual void Start();
    virtual void FixedUpdate(float timeStep);
    virtual void Setup();

    virtual void GiveCameraNode(Node* camera);
    virtual SharedPtr<Node> GetCameraNode(){return cameraNode_;};

    void ModifyWeapon(VariantMap& parms);

private:
    
    void HandleSceneDrawableUpdateFinished(StringHash eventType, VariantMap& eventData);
    SharedPtr<Node> cameraNode_;
   
};
#endif
