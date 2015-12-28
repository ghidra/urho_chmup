#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "CustomGeo.h"

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

CustomGeo::CustomGeo(Context* context):
    Object(context)
{
}
CustomGeo::~CustomGeo(){}

void CustomGeo::AddPoint(const Vector3 p){
	points_.Push(p);
}

void CustomGeo::AddTri(const unsigned p1, const unsigned p2, const unsigned p3)
{
	ids_.Push(p1);
	ids_.Push(p2);
	ids_.Push(p3);
	normals_.Push(Normal(points_[p1],points_[p2],points_[p3]));
}

void CustomGeo::Setup(Node* node)
{
	node_ = node;

	unsigned num = ids_.Size();
	const unsigned numVertices = num;
	float vertexData[num*6];
	unsigned short indexData[num];

	for(unsigned i = 0; i < numVertices; ++i)
	{
		unsigned ii = i*6;
		vertexData[ii] = points_[ids_[i]].x_;
		vertexData[ii+1] = points_[ids_[i]].y_;
		vertexData[ii+2] = points_[ids_[i]].z_;

		vertexData[ii+4] = normals_[ids_[i]].x_;
		vertexData[ii+5] = normals_[ids_[i]].y_;
		vertexData[ii+6] = normals_[ids_[i]].z_;

		indexData[i]=i;
	}
	// Finally create one model (pyramid shape) and a StaticModel to display it from scratch
    	// Note: there are duplicated vertices to enable face normals. We will calculate normals programmatically
        	/*const unsigned numVertices = 18;

        	float vertexData[] = {
           	 // Position             Normal
	            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
	            0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,
	            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

	            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,
	            0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,

	            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,

	            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
	            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,

	            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
	            0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,

	            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,
	            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f
        	};

        	const unsigned short indexData[] = {
	            0, 1, 2,
	            3, 4, 5,
	            6, 7, 8,
	            9, 10, 11,
	            12, 13, 14,
	            15, 16, 17
        	};

        	// Calculate face normals now
        	for (unsigned i = 0; i < numVertices; i += 3)
        	{
	           Vector3& v1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i]));
	           Vector3& v2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1)]));
	           Vector3& v3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2)]));
	           Vector3& n1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i + 3]));
	           Vector3& n2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1) + 3]));
	           Vector3& n3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2) + 3]));

	           //Vector3 edge1 = v1 - v2;
	           //Vector3 edge2 = v1 - v3;
	           //n1 = n2 = n3 = edge1.CrossProduct(edge2).Normalized();
	           n1 = n2 = n3 = Normal(v1,v2,v3);
        	}*/

        	SharedPtr<Model> fromScratchModel(new Model(context_));
        	SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
        	SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
        	SharedPtr<Geometry> geom(new Geometry(context_));

        	// Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
        	vb->SetShadowed(true);
        	vb->SetSize(numVertices, MASK_POSITION|MASK_NORMAL);
        	vb->SetData(vertexData);

        	ib->SetShadowed(true);
        	ib->SetSize(numVertices, false);
        	ib->SetData(indexData);

        	geom->SetVertexBuffer(0, vb);
        	geom->SetIndexBuffer(ib);
        	geom->SetDrawRange(TRIANGLE_LIST, 0, numVertices);

        	fromScratchModel->SetNumGeometries(1);
        	fromScratchModel->SetGeometry(0, 0, geom);
        	fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

        	Node* newnode = node_->GetScene()->CreateChild("FromScratchObject");
        	newnode->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
        	StaticModel* object = newnode->CreateComponent<StaticModel>();
        	object->SetModel(fromScratchModel);
}

Vector3 CustomGeo::Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vector3 edge1 = p1 - p2;
           Vector3 edge2 = p1 - p3;
	return edge1.CrossProduct(edge2).Normalized();
}

void CustomGeo::Debug(const String label, const String value){
    GetSubsystem<DebugHud>()->SetAppStats(label+" : ", value );
}