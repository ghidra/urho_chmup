#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>

//#include <Urho3D/Graphics/AnimatedModel.h>

#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Physics/CollisionShape.h>

#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>

#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>

#include "Gun01.h"

#include "../framework/src/Weapon.h"
#include "../framework/src/Projectile.h"
#include "ProjectileFireball.h"
#include "ProjectileBlaster.h"
#include "ProjectileLaser.h"

Gun01::Gun01(Context* context) :
    Weapon(context),
    rotationTimer_(0.0f),
    rotationSpeed_(1200.0f),
    rotationRange_(45.0f),
    rotation_(Quaternion()),
    projectileType_(PT_LASER)
{
    //CameraLogic::RegisterObject(context);
    //SetUpdateEventMask(USE_FIXEDUPDATE);
    //collision_layer_ = 4;
    //collision_mask_ = 33;
    firing_interval_=0.02;
    mesh_ = String("Box.mdl");
}
Gun01::~Gun01(){}
//-------------------
//-------------------
void Gun01::RegisterObject(Context* context)
{
    context->RegisterFactory<Gun01>();

}

/*void Gun01::Start()
{
    //SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Gun01, HandleNodeCollision));

}*/
void Gun01::Setup()
{
    //Weapon::Setup();
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    StaticModel* object = node_->CreateComponent<StaticModel>();
    object->SetModel(cache->GetResource<Model>("Models/"+mesh_));
    object->SetMaterial(cache->GetResource<Material>("Materials/Jack.xml"));
    object->SetCastShadows(true);

    if(projectileType_==PT_LASER)
        SetProjectileContinuous(true);
}

/*void Gun01::FixedUpdate(float timeStep)
{
    
}*/
void Gun01::Fire(float timeStep)
{
    Weapon::Fire(timeStep);
    //now we can rotate the turret if we have rotation values
    rotationTimer_+=timeStep;
    float r = Sin(rotationTimer_*rotationSpeed_);
    Quaternion q = Quaternion(r*rotationRange_,Vector3(0.0f,1.0f,0.0f));
    node_->SetRotation(q);
    //debug_->Hud("gun rotation",String(q));
}
void Gun01::ReleaseFire()
{
    Weapon::ReleaseFire();

    //if(continuous_ && continuous_spawned_)
    //{
    //    continuous_spawned_=false;
        //likely need to remove the continuous projectile as well
    //}
    /*
    firing_ = 0;
    firing_timer_ = 0.0f;
    node_->SetTransform(Vector3(),Quaternion());
    */
}

void Gun01::Recoil()
{
    //try to move the ship a little
    RigidBody* ship = node_->GetParent()->GetComponent<RigidBody>();//this should be the main node that holds the whole ship
    if(ship!=NULL)
    {
        //debug_->Hud("recoil",String(dir));
        Vector3 rdir = -ProjectileSpawnDirection_;
        ship->ApplyImpulse(rdir*projectile_speed_*0.075f);
    }
}

void Gun01::SpawnProjectile()
{
    Vector3 pos = node_->GetWorldPosition();
    Quaternion rot = node_->GetWorldRotation();
    ProjectileSpawnDirection_ = rot*Vector3(0.0f,0.0f,1.0f);
    ProjectileSpawnPosition_ = pos+ProjectileSpawnDirection_;

    Node* projectileNode_;
    VariantMap projectileParms;

    //if(!continuous_spawned_)
    //{
    projectileNode_ = node_->GetScene()->CreateChild("projectile");
    projectileNode_->SetPosition(ProjectileSpawnPosition_);

    projectileParms["direction"] = ProjectileSpawnDirection_;
    projectileParms["range"] = projectile_range_;
    projectileParms["speed"] = projectile_speed_;
    projectileParms["usegravity"] = false;
    projectileParms["raytest"] = true;
    //}

    Recoil();

    switch(projectileType_)
    {
        case PT_FIREBALL:
        {
            ProjectileFireball* projectile_ = projectileNode_->CreateComponent<ProjectileFireball>();
            projectile_->Setup( projectileParms );
            break;
        }
        case PT_BLASTER:
        {
            ProjectileBlaster* projectile_ = projectileNode_->CreateComponent<ProjectileBlaster>();
            projectile_->Setup( projectileParms );
            break;
        }
        case PT_LASER:
        {
            //debug_->Hud("PROJECTILE",String("LASER SPAWNED"));
            //if(!continuous_spawned_)
            //{
                //debug_->Hud("PROJECTILE",String("LASER"));
            ProjectileLaser* projectile_ = projectileNode_->CreateComponent<ProjectileLaser>();
            projectile_->Setup( projectileParms );
            projectile_->SetParentWeaponNode(node_);//give the node, so that we can stick to it
                //continuous_spawned_ = true;
            //}
            break;
        }
    }
    //if()
    //{
    //P
    //}
    //else
    //{
    
    //}
    //projectile_->Setup(Vector3(0.0f,0.0f,1.0f));
    //projectile_->Setup( projectileParms );
   
}
//------setting data from pickups most likely
void Gun01::SetRotation(const float speed, const float range, const float offset)
{
    rotationSpeed_=speed;
    rotationRange_=range;
    //use the offset to set the rotation immediatly currently not implemented
}
void Gun01::SetRotationSpeed(const float speed){rotationSpeed_=speed;}
void Gun01::SetRotationRange(const float range){rotationRange_=range;}

void Gun01::SetProjectileType(const unsigned type){projectileType_=static_cast<ProjectileType>(type);}

