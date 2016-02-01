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

#include "ProjectileBlaster.h"

ProjectileBlaster::ProjectileBlaster(Context* context) :
    Projectile(context)
{
    mesh_ = String("Sphere.mdl");
    speed_ = 10.0f;
}
ProjectileBlaster::~ProjectileBlaster(){}
//-------------------
//-------------------
void ProjectileBlaster::RegisterObject(Context* context)
{
    context->RegisterFactory<ProjectileBlaster>();
}

void ProjectileBlaster::Start()
{
    Projectile::Start();
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(ProjectileBlaster, HandleNodeCollision));
}
void ProjectileBlaster::Setup(const Vector3 direction)
{
    Projectile::Setup(direction);
    SetupLocal();
}
void ProjectileBlaster::Setup(VariantMap& parms)
{
    Projectile::Setup(parms);
    SetupLocal();
}
void ProjectileBlaster::SetupLocal()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    Material* bmat = cache->GetResource<Material>( "Materials/ProjectileBlaster.xml");
    SharedPtr<Material> cmat = bmat->Clone();

    //Color myCol = Color(Random(1.0f),Random(1.0f),Random(1.0f),1.0f);

    cmat->SetShaderParameter("Direction", dir_.Normalized() );//single quotes didnt work
    cmat->SetShaderParameter("Speed", dir_.Length() );//single quotes didnt work

    StaticModel* obj = node_->GetComponent<StaticModel>();
    obj->SetMaterial(cmat);

    node_->SetScale(0.5f);
}


