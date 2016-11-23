#include "stdafx.h"
#include "Node.h"
namespace DEN
{
	Node::Node()
	{
		z_matrix = Matrix::IDENTITY;
		a_matrix = Matrix::IDENTITY;
		z_scale = Vector(1.0f, 1.0f, 1.0f);
		z_rotation = Quaternion::IDENTITY;
		z_parent = NULL;
		o_rotation = Quaternion::IDENTITY;
		z_position = Vector::ZERO;
		o_position = Vector::ZERO;
		z_count = 1;
		z_update = false;
		z_isUpdateTransform = true;
	}

	Node::~Node()
	{
		if(z_parent != NULL)
		{
			set<Node*>::const_iterator elem = z_parent->z_children.find(this);
			if(elem != z_parent->z_children.end())
				z_parent->z_children.erase(this);
		}
		z_parent = NULL;
		for each(set<Node*>::value_type elem in z_children)
			elem->z_parent = NULL;
	}

	void Node::SetMatrix(const Matrix &matrix)
	{
		z_update = true;
		z_position = matrix.GetTranslation();
		z_rotation = matrix.ToQuaternion();
	}

	void Node::SetPosition(const Vector &pos, bool global)
	{
		z_update = true;
		if(!global || !z_parent)
			z_position = pos;
		else
		{
			Matrix mat;
			z_parent->GetRotation(true).ToRotationMatrix(mat);///это правильно?
			mat = mat.Inverse();
			z_position = mat.TransformCoord(pos - z_parent->GetPosition(true));
			z_update = true;
		}
	}

	void Node::Move(const Vector &pos, bool global)
	{
		z_update = true;
		if(!global)
			z_position += pos;
		else
		{
			Matrix mat;
			Vector vec;
			z_rotation.ToRotationMatrix(mat);///это правильно?
			vec = mat.TransformCoord(pos);
			z_position += vec;
		}
	}

	void Node::SetRotation(const Quaternion &q, bool global)
	{
		z_update = true;
		z_rotation = q;
		if(global)
		{
			Matrix mat;
			Vector vec;
			vec = mat.TransformCoord(z_position);
			z_position = vec;
		}
	}

	void Node::Rotate(const Quaternion &q, bool global)
	{
		z_update = true;
		if(!global)
			z_rotation = z_rotation*q;
		else
			z_rotation = q*z_rotation;
	}

	void Node::RotateOrigin(const Quaternion &q, bool global)
	{
		z_update = true;
		if(!global)
			o_rotation = o_rotation*q;
		else
			o_rotation = q*o_rotation;
	}

	void Node::SetOrigPosition(const Vector &pos)
	{
		z_update = true;
		o_position = pos;
	}

	void Node::SetOrigRotation(const Quaternion &q)
	{
		o_rotation = q;
	}

	void Node::UpdateAbsMatrix()
	{
		Matrix mat;
		mat.Transformation(o_position, Vector(1.0f, 1.0f, 1.0f), o_rotation);
		if(z_parent != NULL)
			z_parent->UpdateAbsMatrix(),
			a_matrix = z_parent->a_matrix*mat.Inverse();
		else
			a_matrix = mat.Inverse();
	}

	Matrix Node::GetOffsetMatrix()
	{
		Update();
		return a_matrix*z_matrix;
	}

	Matrix Node::GetAbsMatrix()
	{
		Update();
		return a_matrix;
	}

	void Node::SetScale(const Vector &scale, bool saveRatio)
	{
		z_update = true;
		Vector sc = z_scale;
		z_scale = scale;
		if(saveRatio)
		{
			sc = Vector(sc.x / z_scale.x, sc.y / z_scale.y, sc.z / z_scale.z);
			for each(set<Node*>::value_type elem in z_children)
				elem->SetScale(sc);
		}
	}

	void Node::SetScaleOffset(const Vector &scale, bool saveRatio)
	{
		z_update = true;
		Vector sc = z_scale;
		z_scale = z_scale*scale;
		if(saveRatio)
		{
			sc = Vector(sc.x / z_scale.x, sc.y / z_scale.y, sc.z / z_scale.z);
			for each(set<Node*>::value_type elem in z_children)
				elem->SetScaleOffset(sc);
		}
	}

	Matrix *Node::GetMatrix()
	{
		Update();
		return &z_matrix;
	}

	Vector Node::GetPosition(bool global)
	{
		if(!global)
			return z_position;
		//Matrix mat;
		//z_rotation.ToRotationMatrix(mat);
		Update();
		return z_matrix.GetTranslation();
	}

	Vector Node::GetDirection()
	{
		Update();
		return z_matrix.GetLook();
	}

	Vector Node::GetOrigPosition()
	{
		return o_position;
	}

	Quaternion Node::GetOrigRotation()
	{
		return o_rotation;
	}

	Quaternion Node::GetRotation(bool global)
	{
		//return z_rotation;
		if(!global)
			return z_rotation;
		//Matrix mat;
		//z_rotation.ToRotationMatrix(mat);
		Update();
		return z_matrix.ToQuaternion();
	}

	Vector Node::GetScale()
	{
		return z_scale;
	}

	void Node::SetParent(Node *parent, bool reposition)
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
			Matrix m = parent->GetMatrix()->Inverse();
			z_position = m.TransformCoord(-z_position);
		}
		parent->AddChild(this);
		z_update = true;
	}

	void Node::RemoveParent()
	{
		if(z_parent != NULL)
		{
			set<Node*>::const_iterator elem = z_parent->z_children.find(this);
			if(elem != z_parent->z_children.end())
				z_parent->z_children.erase(this);
		}
		z_parent = NULL;
		UpdateAbsMatrix();
		z_update = true;
	}

	void Node::AddChild(Node *child)
	{
		if(child->z_parent)
		{
			set<Node*>::const_iterator elem = child->z_parent->z_children.find(child);
			if(elem != child->z_parent->z_children.end())
				child->z_parent->z_children.erase(child);
		}
		z_children.insert(child);
		child->z_parent = this;
		child->UpdateAbsMatrix();
		child->z_update = true;
	}

	void Node::UpdateAnimation(float sync)
	{
		bool update = true;
		if(z_isUpdateTransform)
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
		for each(set<Node*>::value_type elem in z_children)
			elem->UpdateAnimation(sync);
	}

	void Node::Update()
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
			for each(set<Node*>::value_type elem in z_children)//понижает fps на 16% при 5 Human Mesh в debug
			{
				elem->z_update = true;
				elem->Update();
			}
		}
	}

	void Node::UpdateHard()
	{
		z_update = true;
		Update();
	}

	void Node::UpdateChild()
	{
		for each(set<Node*>::value_type elem in z_children)//понижает fps на 16% при 5 Human Mesh в debug
			elem->Update();
	}

	void Node::UpdateParent()
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

	void Node::SetName(const string &name)
	{
		z_name = name;
	}

	string Node::GetName()
	{
		return z_name;
	}

	NodeMatrix::NodeMatrix()
	{
		z_node = new Node();// GetManager()->GetEngine()->CreateNode();
	}

	NodeMatrix::~NodeMatrix()
	{
		if(z_node)
		{
			z_node->z_count--;
			if(z_node->z_count == 0)//если этот нод больше никто не использует
				delete z_node;
		}
	}

	void NodeMatrix::SetNode(Node *node)
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

	Node *NodeMatrix::GetNode() const
	{
		return z_node;
	}
}