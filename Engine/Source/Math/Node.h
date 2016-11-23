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
		Matrix z_matrix;//текущая относительная матрица
		Matrix a_matrix;//начальная абсолютная матрица
		Vector z_position;
		Vector o_position;//начальная относительная позиция
		Quaternion z_rotation;
		Quaternion o_rotation;
		Vector z_scale;
		bool z_update;
		bool z_isUpdateTransform;//нужно ли обновлять матрицу
		int z_count;//количество ссылок
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
		void SetScaleOffset(const Vector &scale, bool saveRatio = false);//умножение текущего масштаба
		Matrix *GetMatrix();
		void UpdateAbsMatrix();
		Matrix GetOffsetMatrix();
		Matrix GetAbsMatrix();
		Vector GetPosition(bool global = false);//false - позиция относительно родителя, true - относительно мирового пространства
		Vector GetDirection();
		Quaternion GetRotation(bool global = false);
		Vector GetOrigPosition();
		Quaternion GetOrigRotation();
		Vector GetScale();
		void SetParent(Node *parent, bool reposition = false);
		void RemoveParent();
		void AddChild(Node *child);
		void UpdateAnimation(float sync);//обновить node и все дочерние node
		void Update();//child - обновить дочерние кости
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