#pragma once
namespace DEN
{
	class Vector2;
	class Quaternion;
	class Matrix2D;

	class Node2D
	{
		friend class Node2DMatrix;
	protected:
		Matrix2D z_matrix;//������� ������������� �������
		Matrix2D a_matrix;//��������� ���������� �������
		Vector2 z_position;
		Vector2 o_position;//��������� ������������� �������
		Quaternion z_rotation;
		Quaternion o_rotation;
		Vector2 z_scale;
		bool z_update;
		bool z_isUpdateTransform;//����� �� ��������� �������
		int z_count;//���������� ������
		Node2D *z_parent;
		set<Node2D*> z_children;
		string z_name;
	public:
		Node2D();
		~Node2D();
		inline void SetUpdate(bool isEnable) { z_isUpdateTransform = isEnable; };
		inline Node2D *GetParent() { return z_parent; };
		void SetMatrix(const Matrix2D &matrix);
		void SetPosition(const Vector2 &pos, bool global = false);
		void SetRotation(const Quaternion &rot, bool global = false);
		void Move(const Vector2 &pos, bool global = false);
		void Rotate(const Quaternion &rot, bool global = false);
		void RotateOrigin(const Quaternion &rot, bool global = false);
		void SetOrigPosition(const Vector2 &pos);
		void SetOrigRotation(const Quaternion &rot);
		void SetScale(const Vector2 &scale, bool saveRatio = false);
		void SetScaleOffset(const Vector2 &scale, bool saveRatio = false);//��������� �������� ��������
		Matrix2D *GetMatrix();
		void UpdateAbsMatrix();
		Matrix2D GetOffsetMatrix();
		Matrix2D GetAbsMatrix();
		Vector2 GetPosition(bool global = false);//false - ������� ������������ ��������, true - ������������ �������� ������������
		Vector2 GetDirection();
		Quaternion GetRotation(bool global = false);
		Vector2 GetOrigPosition();
		Quaternion GetOrigRotation();
		Vector2 GetScale();
		void SetParent(Node2D *parent, bool reposition = false);
		void RemoveParent();
		void AddChild(Node2D *child);
		void Update();//child - �������� �������� �����
		void UpdateHard();
		void UpdateChild();
		void UpdateParent();
		void SetName(const string &name);
		string GetName();
	};

	class Node2DMatrix
	{
	protected:
		Node2D *z_node;
	public:
		Node2DMatrix();
		~Node2DMatrix();
		void SetNode(Node2D *node);
		Node2D *GetNode() const;
	};
}