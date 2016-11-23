#include "stdafx.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "Matrix2D.h"
#include "Node2D.h"
namespace DEN
{
	Node2D::Node2D()
	{
		z_matrix = Matrix2D::IDENTITY;
		a_matrix = Matrix2D::IDENTITY;
		z_scale = Vector2(1.0f, 1.0f);
		z_rotation = Quaternion::IDENTITY;
		z_parent = NULL;
		o_rotation = 0.0f;
		z_position = Vector2::ZERO;
		o_position = Vector2::ZERO;
		z_count = 1;
		z_update = false;
		z_isUpdateTransform = true;
	}

	Node2D::~Node2D()
	{
		if(z_parent != NULL)
		{
			set<Node2D*>::const_iterator elem = z_parent->z_children.find(this);
			if(elem != z_parent->z_children.end())
				z_parent->z_children.erase(this);
		}
		z_parent = NULL;
		for each(set<Node2D*>::value_type elem in z_children)
			elem->z_parent = NULL;
	}

	void Node2D::SetMatrix(const Matrix2D &matrix)
	{
		z_update = true;
		z_position = matrix.GetTranslation();
		z_rotation = matrix.ToQuaternion();
	}

	void Node2D::SetPosition(const Vector2 &pos, bool global)
	{
		z_update = true;
		if(!global || !z_parent)
			z_position = pos;
		else
		{
			Matrix2D mat;
			z_parent->GetRotation(true).ToRotationMatrix(mat);///это правильно?
			mat = mat.Inverse();
			z_position = mat.TransformCoord(pos - z_parent->GetPosition(true));
			z_update = true;
		}
	}

	void Node2D::Move(const Vector2 &pos, bool global)
	{
		z_update = true;
		if(!global)
			z_position += pos;
		else
		{
			Matrix2D mat;
			Vector2 vec;
			z_rotation.ToRotationMatrix(mat);///это правильно?
			vec = mat.TransformCoord(pos);
			z_position += vec;
		}
	}

	void Node2D::SetRotation(const Quaternion &rot, bool global)
	{
		z_update = true;
		z_rotation = rot;
		if(global)
		{
			Matrix2D mat;
			Vector2 vec;
			vec = mat.TransformCoord(z_position);
			z_position = vec;
		}
	}

	void Node2D::Rotate(const Quaternion &rot, bool global)
	{
		z_update = true;
		if(!global)
			z_rotation = z_rotation*rot;
		else
			z_rotation = rot*z_rotation;
	}

	void Node2D::RotateOrigin(const Quaternion &rot, bool global)
	{
		z_update = true;
		if(!global)
			o_rotation = o_rotation*rot;
		else
			o_rotation = rot*o_rotation;
	}

	void Node2D::SetOrigPosition(const Vector2 &pos)
	{
		z_update = true;
		o_position = pos;
	}

	void Node2D::SetOrigRotation(const Quaternion &rot)
	{
		o_rotation = rot;
	}

	void Node2D::UpdateAbsMatrix()
	{
		Matrix2D mat;
		mat.Transformation(o_position, Vector2(1.0f, 1.0f), o_rotation);
		if(z_parent != NULL)
			z_parent->UpdateAbsMatrix(),
			a_matrix = z_parent->a_matrix*mat.Inverse();
		else
			a_matrix = mat.Inverse();
	}

	Matrix2D Node2D::GetOffsetMatrix()
	{
		Update();
		return a_matrix*z_matrix;
	}

	Matrix2D Node2D::GetAbsMatrix()
	{
		Update();
		return a_matrix;
	}

	void Node2D::SetScale(const Vector2 &scale, bool saveRatio)
	{
		z_update = true;
		Vector2 sc = z_scale;
		z_scale = scale;
		if(saveRatio)
		{
			sc = Vector2(sc.x / z_scale.x, sc.y / z_scale.y);
			for each(set<Node2D*>::value_type elem in z_children)
				elem->SetScale(sc);
		}
	}

	void Node2D::SetScaleOffset(const Vector2 &scale, bool saveRatio)
	{
		z_update = true;
		Vector2 sc = z_scale;
		z_scale = z_scale*scale;
		if(saveRatio)
		{
			sc = Vector2(sc.x / z_scale.x, sc.y / z_scale.y);
			for each(set<Node2D*>::value_type elem in z_children)
				elem->SetScaleOffset(sc);
		}
	}

	Matrix2D *Node2D::GetMatrix()
	{
		Update();
		return &z_matrix;
	}

	Vector2 Node2D::GetPosition(bool global)
	{
		if(!global)
			return z_position;
		//Matrix mat;
		//z_rotation.ToRotationMatrix(mat);
		Update();
		return z_matrix.GetTranslation();
	}

	Vector2 Node2D::GetDirection()
	{
		Update();
		return z_matrix.GetLook();
	}

	Vector2 Node2D::GetOrigPosition()
	{
		return o_position;
	}

	Quaternion Node2D::GetOrigRotation()
	{
		return o_rotation;
	}

	Quaternion Node2D::GetRotation(bool global)
	{
		//return z_rotation;
		if(!global)
			return z_rotation;
		//Matrix mat;
		//z_rotation.ToRotationMatrix(mat);
		Update();
		return z_matrix.ToQuaternion();
	}

	Vector2 Node2D::GetScale()
	{
		return z_scale;
	}

	void Node2D::SetParent(Node2D *parent, bool reposition)
	{
		if(z_parent == parent)
			return;
		if(z_parent == NULL)
		{
			z_position = -z_position;
			o_position = -o_position;
		}
		if(reposition)
		{
			Matrix2D m = parent->GetMatrix()->Inverse();
			z_position = m.TransformCoord(-z_position);
		}
		parent->AddChild(this);
		z_update = true;
	}

	void Node2D::RemoveParent()
	{
		if(z_parent != NULL)
		{
			set<Node2D*>::const_iterator elem = z_parent->z_children.find(this);
			if(elem != z_parent->z_children.end())
				z_parent->z_children.erase(this);
		}
		z_parent = NULL;
		UpdateAbsMatrix();
		z_update = true;
	}

	void Node2D::AddChild(Node2D *child)
	{
		if(child->z_parent)
		{
			set<Node2D*>::const_iterator elem = child->z_parent->z_children.find(child);
			if(elem != child->z_parent->z_children.end())
				child->z_parent->z_children.erase(child);
		}
		z_children.insert(child);
		child->z_parent = this;
		child->UpdateAbsMatrix();
		child->z_update = true;
	}

	void Node2D::Update()
	{
		if(z_update)//если убрать, fps опустится с 1000 до 200
		{
			z_update = false;
			z_matrix.Transformation(z_position, z_scale, z_rotation);
			if(z_parent != NULL)
			{
				if(z_parent->z_update)
					z_parent->UpdateParent();
				if(z_isUpdateTransform)
					z_matrix = z_matrix*z_parent->z_matrix;
				else
					z_matrix = z_matrix*z_parent->z_matrix.ToPositionMatrix();
			}
			for each(set<Node2D*>::value_type elem in z_children)//понижает fps на 16% при 5 Human Mesh в debug
			{
				elem->z_update = true;
				elem->Update();
			}
		}
	}

	void Node2D::UpdateHard()
	{
		z_update = true;
		Update();
	}

	void Node2D::UpdateChild()
	{
		for each(set<Node2D*>::value_type elem in z_children)//понижает fps на 16% при 5 Human Mesh в debug
			elem->Update();
	}

	void Node2D::UpdateParent()
	{
		if(z_update)
		{
			z_update = false;
			z_matrix.Transformation(z_position, z_scale, z_rotation);
		}
		if(z_parent != NULL)
		{
			if(z_parent->z_update)
				z_parent->UpdateParent();
			if(z_isUpdateTransform)
				z_matrix = z_matrix*z_parent->z_matrix;
			else
				z_matrix = z_matrix*z_parent->z_matrix.ToPositionMatrix();
		}
	}

	void Node2D::SetName(const string &name)
	{
		z_name = name;
	}

	string Node2D::GetName()
	{
		return z_name;
	}

	Node2DMatrix::Node2DMatrix()
	{
		z_node = new Node2D();// GetManager()->GetEngine()->CreateNode2D();
	}

	Node2DMatrix::~Node2DMatrix()
	{
		if(z_node)
		{
			z_node->z_count--;
			if(z_node->z_count == 0)//если этот нод больше никто не использует
				delete z_node;
		}
	}

	void Node2DMatrix::SetNode(Node2D *node)
	{
		if(node != z_node)
		{
			z_node->z_count--;
			if(z_node->z_count == 0)
				delete z_node;
			z_node = node;
			z_node->z_count++;
		}
	}

	Node2D *Node2DMatrix::GetNode() const
	{
		return z_node;
	}
}