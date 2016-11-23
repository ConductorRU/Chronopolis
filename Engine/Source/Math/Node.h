#pragma once
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
namespace DEN
{
	class Vector;
	class Quaternion;
	class Matrix;
	class Node
	{
		friend class NodeMatrix;
	protected:
		Matrix z_matrix;//������� ������������� �������
		Matrix a_matrix;//��������� ���������� �������
		Vector z_position;
		Vector o_position;//��������� ������������� �������
		Quaternion z_rotation;
		Quaternion o_rotation;
		Vector z_scale;
		bool z_update;
		bool z_isUpdateTransform;//����� �� ��������� �������
		int z_count;//���������� ������
		Node *z_parent;
		set<Node*> z_children;
		string z_name;
	public:
		Node();
		~Node();
		inline void SetUpdate(bool isEnable) { z_isUpdateTransform = isEnable; };
		inline Node *GetParent() { return z_parent; };
		void SetMatrix(const Matrix &matrix);
		void SetPosition(const Vector &pos, bool global = false);
		void SetRotation(const Quaternion &q, bool global = false);
		void Move(const Vector &pos, bool global = false);
		void Rotate(const Quaternion &q, bool global = false);
		void RotateOrigin(const Quaternion &q, bool global = false);
		void SetOrigPosition(const Vector &pos);
		void SetOrigRotation(const Quaternion &q);
		void SetScale(const Vector &scale, bool saveRatio = false);
		void SetScaleOffset(const Vector &scale, bool saveRatio = false);//��������� �������� ��������
		Matrix *GetMatrix();
		void UpdateAbsMatrix();
		Matrix GetOffsetMatrix();
		Matrix GetAbsMatrix();
		Vector GetPosition(bool global = false);//false - ������� ������������ ��������, true - ������������ �������� ������������
		Vector GetDirection();
		Quaternion GetRotation(bool global = false);
		Vector GetOrigPosition();
		Quaternion GetOrigRotation();
		Vector GetScale();
		void SetParent(Node *parent, bool reposition = false);
		void RemoveParent();
		void AddChild(Node *child);
		void UpdateAnimation(float sync);//�������� node � ��� �������� node
		void Update();//child - �������� �������� �����
		void UpdateHard();
		void UpdateChild();
		void UpdateParent();

		void SetName(const string &name);
		string GetName();
	};

	class NodeMatrix
	{
	protected:
		Node *z_node;
	public:
		NodeMatrix();
		~NodeMatrix();
		void SetNode(Node *node);
		Node *GetNode() const;
	};
}