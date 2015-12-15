#ifndef CHMUPDEBUGLEVEL_H
#define CHMUPDEBUGLEVEL_H

#pragma once

#include <Urho3D/Core/Object.h>
#include "../game/Character.h"

using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;

}

using namespace Urho3D;

class ChmupDebugLevel : public Object
{

    URHO3D_OBJECT(ChmupDebugLevel,Object);

public:

    ChmupDebugLevel(Context* context);

    virtual void Setup(SharedPtr<Scene> scene, SharedPtr<Node> cameraNode);

protected:

    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    SharedPtr<Character> character_;
   
};
#endif