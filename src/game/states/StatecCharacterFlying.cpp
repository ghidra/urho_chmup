#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>//will not complie without this?

#include "StateCharacterFlying.h"

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

StateCharacterFlying::StateCharacterFlying(Context* context):
    State(context)
{
    name_=String("flying");
}
StateCharacterFlying::~StateCharacterFlying(){}

State* StateCharacterFlying::HandleInput(Controls& ctrl)
{
	//get the move direction
	moveDir_ = Vector3::ZERO;
	if (ctrl.IsDown(CTRL_UP))
        moveDir_ += Vector3::FORWARD;
    if (ctrl.IsDown(CTRL_DOWN))
        moveDir_ += Vector3::BACK;
    if (ctrl.IsDown(CTRL_LEFT))
        moveDir_ += Vector3::LEFT;
    if (ctrl.IsDown(CTRL_RIGHT))
        moveDir_ += Vector3::RIGHT;
    
    // Normalize move vector so that diagonal strafing is not faster
    if (moveDir_.LengthSquared() > 0.0f)
    {
        moveDir_*=Vector3(1.0f,1.0f,0.0f);//flatten movement out to a plane
        moveDir_.Normalize();
    }
    //------------------

	if ( ctrl.IsDown(CTRL_JUMP) ) 
	{
        return NULL;
		//we want to jump
		//return new StateCharacterJumping(context_);
	}else{
		return NULL;
	}
}
void StateCharacterFlying::Update()
{
    RigidBody* body = pawn_->GetBody();
    body->ApplyImpulse(-pawn_->GetPlaneVelocity() * pawn_->GetBrakeForce());//this is the brake force
    
    body->ApplyImpulse(moveDir_ * pawn_->GetMoveForce() );


}