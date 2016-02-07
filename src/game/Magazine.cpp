#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>

#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Physics/CollisionShape.h>

#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Material.h>

/*#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>*/

#include "Magazine.h"
#include "Gun.h"

Magazine::Magazine(Context* context) :
    Object(context),
    size_(0),
    remaining_(0),
    max_visible_(10)
{
}
Magazine::~Magazine(){}

void Magazine::SetWeaponNode(Node* weapon, const unsigned size)
{
    weapon_ = weapon;
    SetSize(size);
}

void Magazine::SetSize(const unsigned size)
{
    size_ = size;

    //
    if(size<remaining_)//we have more shown that the incoming mag size... we need to lose some shells
    {

    }
    else//we need to add more
    {
        unsigned append = size-remaining_;
        //now we add to the shells_ vector and make a new node for each... up to max number
    }

    remaining_ = size;
}

/*
for (unsigned i = 0; i < shells_.Size(); ++i)
*/