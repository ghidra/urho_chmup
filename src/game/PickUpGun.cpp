#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Physics/PhysicsEvents.h>

#include "PickUpGun.h"
//#include "Gun01.h"
#include "Character.h"

PickUpGun::PickUpGun(Context* context) :
    PickUp(context)
{
    collision_layer_ = 4;
    collision_mask_ = 33;
    mesh_="Box.mdl";
}
PickUpGun::~PickUpGun(){}
//-------------------
//-------------------
void PickUpGun::RegisterObject(Context* context)
{
    context->RegisterFactory<PickUpGun>();
}

void PickUpGun::Start()
{
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(PickUpGun, HandleNodeCollision));

}
void PickUpGun::Setup()
{
    PickUp::Setup();

    shape_ = node_->CreateComponent<CollisionShape>();
    shape_->SetBox(Vector3(1.0f, 1.0f, 1.0f));

    //node_->SetPosition(Vector3(4.0f, 8.0f, 0.0f));//objectNode
    //node_->SetRotation(Quaternion(33.0f,78.0f,24.0f));

}

//void PickUpGun::FixedUpdate(float timeStep)
//{
    

//}
void PickUpGun::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollision;

    PickUp::HandleNodeCollision(eventType, eventData);
    
    if(collected_)
    {
        Node* otherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
        Node* boneNode = otherNode->GetChild(String("gun"), true);//get the bone that holds the gun

        //Node* gunNode = boneNode->CreateChild("gun_01");
        //Gun01* weapon = gunNode->CreateComponent<Gun01>();
        //weapon->Setup();

        //we need to give the weapon to the character
        /*Character* character =  otherNode->GetComponent<Character>();
        if(character != NULL)
        {
            character->EquipWeapon(weapon);
        }*/

        node_->Remove();
    }
}


