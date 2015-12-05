#include <Urho3D/Urho3D.h>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Scene/SceneEvents.h>

#include "Character.h"
#include "states/StateCharacterFlying.h"//force it into falling first


#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

Character::Character(Context* context) :
    Pawn(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);
    mesh_ = String("Box.mdl");
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
    shape_->SetCapsule(3.0f, 10.0f, Vector3(0.0f, 5.0f, 0.0f));

    body_->SetUseGravity(false);//dont use gravity

    SetState( new StateCharacterFlying(context_) );
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
        
        // Reset grounded flag for next frame //might be obsolete
        onGround_ = false;//i might not really need this anymore
    }
}
