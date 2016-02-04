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

#include "../framework/src/Weapon.h"

#include "ProjectileLaser.h"

ProjectileLaser::ProjectileLaser(Context* context) :
    Projectile(context)
{
    mesh_ = String("Cylinder.mdl");
    //mesh_ = String("Sphere.mdl");
    collision_size_=3.0f;
    speed_ = 0.0f;
    ray_test_=false;
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
void ProjectileLaser::SetParentWeaponNode(Node* weapon)
{
    weapon_node_ =  weapon;
    body_->SetKinematic(true);
}
void ProjectileLaser::FixedUpdate(float timeStep)
{
    //Projectile::FixedUpdate(timeStep);
    Actor::FixedUpdate(timeStep);
    //now I can get the parent position and rotation and start following
    Vector3 newpos = weapon_node_->GetWorldPosition();
    node_->SetWorldPosition(newpos);

    //check if weapon is firing
    Weapon* w = weapon_node_->GetDerivedComponent<Weapon>();
    if(!w->IsFiring())
    {
        node_->Remove();
    }
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

    node_->SetScale(Vector3(1.0,1.0,10.0));
    //force speed to 0
    speed_=0.0f;
    RigidBody* body = node_->GetComponent<RigidBody>();
    node_->SetWorldPosition(node_->GetWorldPosition()+(Vector3::FORWARD*20.0));
    body->SetLinearVelocity(Vector3::ZERO);

    shape_->SetBox( Vector3(collision_size_, collision_size_,collision_size_) );
}
//use our own impact method
void ProjectileLaser::Impact(Node* node, const Vector3 pos, const Vector3 dir)
{
    RigidBody* body = node->GetComponent<RigidBody>();
    Actor* actor = node->GetDerivedComponent<Actor>();
    body->ApplyImpulse(dir);
    if(actor!=NULL)
    {
        actor->TakeDamage(damage_,pos,dir,level_);
    }
    //if(node_!=NULL)
    //    node_->Remove();
}


