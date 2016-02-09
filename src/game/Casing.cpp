#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Core/Context.h>

#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Physics/RigidBody.h>

#include <Urho3D/Resource/ResourceCache.h>

#include "Casing.h"

Casing::Casing(Context* context) :
    Actor(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);
    mesh_ = String("Cylinder.mdl");
    health_ = 1.0f;
    maxHealth_ = 1.0f;
    collision_layer_ = 32;
    collision_mask_ = 63;
    duration_ = 2.0f;
}
Casing::~Casing(){}
//-------------------
//-------------------
void Casing::RegisterObject(Context* context)
{
    context->RegisterFactory<Casing>();
}

void Casing::Setup()
{
    Actor::Setup();//do the basic set up with stored and set values

    shape_ = node_->CreateComponent<CollisionShape>();
    //shape_->SetBox(Vector3(1.0f,1.0f,1.0f));
    shape_->SetSphere( 2.0f );

    body_->SetUseGravity(false);//dont use gravity
    body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));
    body_->SetMass(1.0f);
}