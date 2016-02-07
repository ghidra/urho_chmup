#include <Urho3D/Urho3D.h>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Scene/SceneEvents.h>

#include "Character.h"
#include "states/StateCharacterFlying.h"//force it into falling first
#include "Gun.h"//force it into falling first

Character::Character(Context* context) :
    Pawn(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);
    mesh_ = String("Box.mdl");
    canCollect_=true;
    brakeForce_=0.1;
}

//-------------------
//-------------------
void Character::RegisterObject(Context* context)
{
    context->RegisterFactory<Character>();
}

void Character::Start()
{
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Character, HandleNodeCollision));
}

void Character::Setup()
{
    Pawn::Setup();//do the basic set up with stored and set values

    shape_ = node_->CreateComponent<CollisionShape>();
    //shape_->SetCapsule(3.0f, 10.0f, Vector3(0.0f, 5.0f, 0.0f));
    shape_->SetBox(Vector3(1.0f,1.0f,1.0f));

    body_->SetUseGravity(false);//dont use gravity
    body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));

    SetState( new StateCharacterFlying(context_) );
}
void Character::GiveCameraNode(Node* camera)
{
    //i can use this for setting GUI elements to it from this character
    cameraNode_ = camera;
}
//--------
void Character::FixedUpdate(float timeStep)
{
    Pawn::FixedUpdate(timeStep);

    if(controller_)
    {
        Controls& ctrl = controller_->controls_;
        //--------------------------------------------------
        //--------------------------------------------------
        ///state stuff
        if(state_ != NULL)
        {
            State* state = state_->HandleInput(ctrl);
            if (state != NULL)
            {
                //do exit state before removing state
                state_->Exit();
                delete state_;
                state_ = state;
                //we are entering the new state
                state_->Enter(static_cast<Pawn*>(this));
            }
            state_->Update();
            //state_->Debug();
        }

        if(weapon_ != NULL)
        {
            weapon_->Update(ctrl,timeStep);
        }
    }
}
//-------
void Character::ModifyWeapon(VariantMap& parms)
{
    if(weapon_!=NULL)
    {
        Gun* w = static_cast<Gun*>(weapon_);
        if( parms.Contains("rotationSpeed") ) w->SetRotationSpeed( parms["rotationSpeed"].GetFloat() );
        if( parms.Contains("rotationRange") ) w->SetRotationRange( parms["rotationRange"].GetFloat() );
        if( parms.Contains("projectileSpeed") ) w->SetProjectileSpeed( parms["projectileSpeed"].GetFloat() );
        if( parms.Contains("projectileRange") ) w->SetProjectileRange( parms["projectileRange"].GetFloat() );
        if( parms.Contains("magSize") && parms.Contains("magSpeed") ) w->SetMagSize( parms["magSize"].GetInt(), parms["magSpeed"].GetFloat() );
        if( parms.Contains("fireRate") ) w->SetFireRate( parms["fireRate"].GetFloat() );
        if( parms.Contains("projectileType") ) w->SetProjectileType( parms["projectileType"].GetInt() );
        if( parms.Contains("continuous") ) w->SetProjectileContinuous( parms["continuous"].GetBool() );
    }

}
