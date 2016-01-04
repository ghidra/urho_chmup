#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>

#include <Urho3D/Graphics/AnimatedModel.h>

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

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

Gun01::Gun01(Context* context) :
    Weapon(context)
{
    //CameraLogic::RegisterObject(context);
    //SetUpdateEventMask(USE_FIXEDUPDATE);
    //collision_layer_ = 4;
    //collision_mask_ = 33;
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
}

/*void Gun01::FixedUpdate(float timeStep)
{
    
}*/

void Gun01::SpawnProjectile()
{
    Vector3 pos = node_->GetWorldPosition();

    Vector3 offpos = pos+Vector3(0.0f,0.0f,1.0f);


    Node* projectileNode_ = node_->GetScene()->CreateChild("projectile");
    projectileNode_->SetPosition(offpos);

    VariantMap projectileParms;
    projectileParms["direction"] = Vector3(0.0f,0.0f,1.0f);
    projectileParms["range"] = 100.0f;
    projectileParms["speed"] = 200.0f;
    projectileParms["usegravity"] = false;
    projectileParms["raytest"] = true;

    Projectile* projectile_ = projectileNode_->CreateComponent<Projectile>();
    //projectile_->Setup(Vector3(0.0f,0.0f,1.0f));
    projectile_->Setup( projectileParms );
   
}

