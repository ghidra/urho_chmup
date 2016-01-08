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
    mesh_="Sphere.mdl";
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

    body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));

    //make some pickup values to be passed to the weapons
    rotationSpeed_ = Random(0.0f,1200.0f);
    rotationRange_ = Random(0.0f,90.0f);
    projectileSpeed_ = Random(20.0f,200.0f);
    projectileRange_ = Random(20.0f,100.f);

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
        

        //we need to give the weapon to the character
        Character* character =  otherNode->GetComponent<Character>();
        if(character != NULL)
        {
            VariantMap weaponParms;
            weaponParms["rotationSpeed"] = rotationSpeed_;
            weaponParms["rotationRange"] = rotationRange_;
            weaponParms["projectileSpeed"] = projectileSpeed_;
            weaponParms["projectileRange"] = projectileRange_;


            character->ModifyWeapon(weaponParms);
        }

        node_->Remove();
    }
}


