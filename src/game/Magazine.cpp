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
    max_visible_(10),
    shell_offset_(0.25f)
{
}
Magazine::~Magazine(){}

void Magazine::SetWeaponNode(Node* weapon, const unsigned size)
{
    weapon_ = weapon;
    //now create a node to put the shells under, i'll put it under the weapon for now
    mag_node_ = weapon->GetParent()->CreateChild("magazine");//this puts it on the character
    mag_node_->SetPosition( Vector3::LEFT*1.5 );
    SetSize(size);
}

void Magazine::SetSize(const unsigned size)
{
    if(weapon_!=NULL)
    {
        size_ = size;

        //
        if(size<remaining_)//we have more shown that the incoming mag size... we need to lose some shells
        {
            unsigned excess = remaining_-size;
            for(unsigned i=0; i<excess; ++i)
            {
                SharedPtr<Node> n = shells_.At(shells_.Size()-1);
                n->Remove();
                shells_.Pop();
            }
        }
        else//we need to add more
        {
            unsigned append = size-remaining_;
            float ssize = float(shells_.Size());
            //now we add to the shells_ vector and make a new node for each... up to max number
            ResourceCache* cache = GetSubsystem<ResourceCache>();
            for(unsigned i=0; i<append; ++i)
            {
                Node* shell = mag_node_->CreateChild("shell");//weapon_->GetScene()->CreateChild("shell");
                StaticModel* sm = shell->CreateComponent<StaticModel>();
                sm->SetModel( cache->GetResource<Model>("Models/Cylinder.mdl") );

                shell->SetPosition( Vector3::FORWARD*((ssize+float(i))*shell_offset_) );
                shell->SetScale(Vector3(0.25f,0.25f,1.0f));
                shell->SetRotation( Quaternion(0.0f,0.0f,90.0f) );
                shells_.Push(SharedPtr<Node>(shell));
            }
        }

        remaining_ = size;
    }
}

void Magazine::Eject()
{
    //turn the last shell into a rigid body and eject it, or for now.. just remove it
    if(shells_.Size()>0)
    {
        SharedPtr<Node> n = shells_.At(shells_.Size()-1);
        n->Remove();
        shells_.Pop();
        remaining_-=1;
    }
}
void Magazine::Reload()
{

}