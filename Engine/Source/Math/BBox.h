#pragma once
namespace DEN
{
	class Vector;
	class Matrix;
	class BBox
	{
	private:
		Vector z_min;
		Vector z_max;
		Vector z_oMin;
		Vector z_oMax;
	public:
		BBox();
		BBox(Vector min, Vector max);
		~BBox();
		Vector GetMin() const;
		Vector GetMax() const;
		Vector GetOrigMin() const;
		Vector GetOrigMax() const;
		float GetMinSize() const;
		float GetMaxSize() const;
		float GetRadius() const;
		float GetLength() const;
		Vector GetNormalizeLength() const;
		Vector GetCenter() const;//�������� BBox (-4 � 6 == 1)
		Vector GetMid() const;//������� ������ BBox (-4 � 6 == 5)
		Vector GetSize() const;//������ BBox
		void UpdateMatrix(Matrix *matrix);
		void SetBox(const Vector &min, const Vector &max);
		bool Intersect(const Vector &point);//��������� �� ����� ������ �����
		bool IntersectSphere(const Vector &point);//��������� �� ����� ������ �����, � ��������, ������ �������� ���������
	};
}