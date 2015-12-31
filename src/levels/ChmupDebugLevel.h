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

    void Setup(SharedPtr<Scene> scene, SharedPtr<Node> cameraNode);
    SharedPtr<Node> GetCharacterNode(){return characterNode_;};

protected:

    //SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    SharedPtr<Node> characterNode_;
    //SharedPtr<Node> light_;

private:
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
   
};
#endif