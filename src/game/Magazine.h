#ifndef MAGAZINE_H
#define MAGAZINE_H

#pragma once

//#include "Gun.h"
//#include "constants.h"

using namespace Urho3D;
namespace Urho3D
{
class Node;
class Scene;
}

class Magazine : public Object
{

    URHO3D_OBJECT(Magazine,Object);

public:

    Magazine(Context* context);
    ~Magazine();

    void SetWeaponNode(Node* weapon, const unsigned size = 9);
    void SetSize(const unsigned size);

private:

    SharedPtr<Node> weapon_;
    unsigned size_;
    unsigned remaining_;
    unsigned max_visible_;

    Vector< SharedPtr<Node> > shells_;//hold the shells
};
#endif