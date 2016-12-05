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
		Vector GetCenter() const;//середина BBox (-4 и 6 == 1)
		Vector GetMid() const;//средний размер BBox (-4 и 6 == 5)
		Vector GetSize() const;//размер BBox
		void UpdateMatrix(Matrix *matrix);
		void SetBox(const Vector &min, const Vector &max);
		bool Intersect(const Vector &point);//находится ли точка внутри бокса
		bool IntersectSphere(const Vector &point);//находится ли точка внутри сферы, с радиусом, равным диаметру диагонали
	};
}