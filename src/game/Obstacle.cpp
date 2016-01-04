#include <Urho3D/Urho3D.h>

#include <Urho3D/Core/Context.h>
//#include <Urho3D/Graphics/AnimatedModel.h>

#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Scene/SceneEvents.h>

#include "Obstacle.h"

Obstacle::Obstacle(Context* context) :
    Pawn(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);
    mesh_ = String("Box.mdl");
    brakeForce_=0.05;
    collision_layer_ = 16;
    collision_mask_ = 51;
}

//-------------------
//-------------------
void Obstacle::RegisterObject(Context* context)
{
    context->RegisterFactory<Obstacle>();
}

void Obstacle::Start()
{
    //SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Obstacle, HandleNodeCollision));
}

void Obstacle::Setup()
{
    Pawn::Setup();//do the basic set up with stored and set values

    shape_ = node_->CreateComponent<CollisionShape>();
    //shape_->SetBox(Vector3(1.0f,1.0f,1.0f));
    shape_->SetSphere( 2.0f );

    body_->SetUseGravity(false);//dont use gravity
    body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));
    body_->SetMass(30.0f);
}
//--------
void Obstacle::FixedUpdate(float timeStep)
{
    Pawn::FixedUpdate(timeStep);
}
