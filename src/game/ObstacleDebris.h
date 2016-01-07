#ifndef OBSTACLEDEBRIS_H
#define OBSTACLEDEBRIS_H

#pragma once

#include "../framework/src/Actor.h"

using namespace Urho3D;

namespace Urho3D
{
class Node;
class Scene;
}

class ObstacleDebris : public Actor
{

    URHO3D_OBJECT(ObstacleDebris,Actor);

public:

    ObstacleDebris(Context* context);
    ~ObstacleDebris();
    static void RegisterObject(Context* context);
    virtual void Setup();
    
};
#endif
