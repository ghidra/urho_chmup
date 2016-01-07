#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Physics/CollisionShape.h>

#include "ObstacleDebris.h"

ObstacleDebris::ObstacleDebris(Context* context) :
    Actor(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);
    mesh_ = String("Box.mdl");
    health_ = 1.0f;
    maxHealth_ = 1.0f;
    collision_layer_ = 32;
    collision_mask_ = 63;
    duration_=0.5f;
}
ObstacleDebris::~ObstacleDebris(){}
//-------------------
//-------------------
void ObstacleDebris::RegisterObject(Context* context)
{
    context->RegisterFactory<ObstacleDebris>();
}

void ObstacleDebris::Setup()
{
    Actor::Setup();//do the basic set up with stored and set values

    shape_ = node_->CreateComponent<CollisionShape>();
    shape_->SetBox(Vector3(1.0f,1.0f,1.0f));
    //shape_->SetSphere( 2.0f );

    //body_->SetUseGravity(false);//dont use gravity
    //body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));
    //body_->SetMass(30.0f);
    //body_->ApplyImpulse(dir*Random(20.0f));

    node_->SetScale(0.35f);
}