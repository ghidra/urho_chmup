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

#include "ProjectileFireball.h"

ProjectileFireball::ProjectileFireball(Context* context) :
    Projectile(context)
{
    mesh_ = String("Sphere.mdl");
    speed_ = 10.0f;
}
ProjectileFireball::~ProjectileFireball(){}
//-------------------
//-------------------
void ProjectileFireball::RegisterObject(Context* context)
{
    context->RegisterFactory<ProjectileFireball>();
}

void ProjectileFireball::Start()
{
    Projectile::Start();
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(ProjectileFireball, HandleNodeCollision));
}
void ProjectileFireball::Setup(const Vector3 direction)
{
    Projectile::Setup(direction);
    SetupLocal();
}
void ProjectileFireball::Setup(VariantMap& parms)
{
    Projectile::Setup(parms);
    SetupLocal();
}
void ProjectileFireball::SetupLocal()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    Material* bmat = cache->GetResource<Material>( "Materials/projectile_fireball.xml");
    SharedPtr<Material> cmat = bmat->Clone();

    //Color myCol = Color(Random(1.0f),Random(1.0f),Random(1.0f),1.0f);

    //rmat.shaderParameters["ObjectColor"]=Variant(myCol);//single quotes didnt work

    StaticModel* obj = node_->GetComponent<StaticModel>();
    obj->SetMaterial(cmat);

    node_->SetScale(1.0f);
}
void ProjectileFireball::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollision;
    Projectile::HandleNodeCollision(eventType, eventData);
}


