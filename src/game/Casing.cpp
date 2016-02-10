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
    collision_mask_ = 57;
    duration_ = 2.0f;
}
Casing::~Casing(){}
//-------------------
//-------------------
void Casing::RegisterObject(Context* context)
{
    context->RegisterFactory<Casing>();
}

void Casing::Setup(const Vector3 position)
{
    Actor::Setup();//do the basic set up with stored and set values

    shape_ = node_->CreateComponent<CollisionShape>();
    shape_->SetBox(Vector3(1.0f,1.0f,1.0f));
    //shape_->SetSphere( 2.0f );

    //body_->SetUseGravity(false);//dont use gravity
    //body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));
    body_->SetMass(1.0f);

    Vector3 RandomDirection = Vector3(Random(),Random(),Random());
    Vector3 RandomAngular = Vector3(Random(),Random(),Random());

    body_->SetAngularVelocity(RandomAngular*Random()*10.0f);
    body_->SetLinearVelocity(RandomDirection*Random()*10.0f);

    node_->SetWorldPosition(position);
    node_->SetScale( Vector3(0.1f,0.4f,0.1f) );
}