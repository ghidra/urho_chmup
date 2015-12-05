#ifndef STATECHARACTERFLYING_H
#define STATECHARACTERFLYING_H

#pragma once

#include "../../framework/src/State.h"

class StateCharacterFlying : public State
{
    URHO3D_OBJECT(StateCharacterFlying,State);

public:
    StateCharacterFlying(Context* context);
    ~StateCharacterFlying();

    virtual State* HandleInput(Controls& ctrl);
    virtual void Update();

protected:
	Vector3 moveDir_;
};
#endif