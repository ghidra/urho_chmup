#ifndef MAIN_H
#define MAIN_H

#pragma once

#include "framework/src/ApplicationHandler.h"

namespace Urho3D
{

class Node;
class Scene;

}

class CameraLogic;


class Main : public ApplicationHandler
{
    URHO3D_OBJECT(Main,ApplicationHandler);

public:
    Main(Context* context);
    ~Main();

    virtual void Start();

private:
    SharedPtr<Node> characterNode_;
};
#endif