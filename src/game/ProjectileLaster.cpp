#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>

#include "ProjectileLaser.h"

ProjectileLaser::ProjectileLaser(Context* context) :
    Projectile(context)
{
    //mesh_ = String("Cylinder.mdl");
    mesh_ = String("Sphere.mdl");
    speed_ = 0.0f;
}
ProjectileLaser::~ProjectileLaser(){}
//-------------------
//-------------------
void ProjectileLaser::RegisterObject(Context* context)
{
    context->RegisterFactory<ProjectileLaser>();
}

void ProjectileLaser::Start()
{
    Projectile::Start();
    //SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(ProjectileLaser, HandleNodeCollision));
}
void ProjectileLaser::Setup(const Vector3 direction)
{
    Projectile::Setup(direction);
    SetupLocal();
}
void ProjectileLaser::Setup(VariantMap& parms)
{
    Projectile::Setup(parms);
    SetupLocal();
}
void ProjectileLaser::SetupLocal()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    Material* bmat = cache->GetResource<Material>( "Materials/ProjectileBlaster.xml");
    SharedPtr<Material> cmat = bmat->Clone();

    cmat->SetShaderParameter("Direction", dir_.Normalized() );//single quotes didnt work
    cmat->SetShaderParameter("Speed", 200.0f );//dir_.Length()//single quotes didnt work

    StaticModel* obj = node_->GetComponent<StaticModel>();
    obj->SetMaterial(cmat);

    node_->SetScale(1.5f);
    //force speed to 0
    speed_=0.0f;
    RigidBody* body = node_->GetComponent<RigidBody>();
    node_->SetWorldPosition(node_->GetWorldPosition()+(Vector3::FORWARD*12.0));
    body->SetLinearVelocity(body->GetLinearVelocity()*0.05f);
}


