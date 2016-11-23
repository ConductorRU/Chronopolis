#pragma once
#include "../Math/Vector.h"
#include "../Math/Point2.h"
#include "../Math/Matrix.h"
namespace DEN
{
	class Matrix;
	class Camera
	{
	private:
		Matrix UpdateMatrix(bool global = true);
		Matrix z_matrixView;
		Matrix4 z_matrixProjection;
		Matrix4 z_invView;
		Matrix4 z_invViewY;
		class Node *z_signt;
		Node *z_node;
		string z_name;
		float z_time;
		float z_aspect, z_fov;
		float z_near, z_far;
		Vector z_temp;
		Vector z_right;
		Vector z_up;
		Vector z_look;
		Vector z_pos;
		Vector z_world;
		Vector z_pickDir;
		Point2 z_mPivot;
		struct Frustum
		{
			Vector normal;
			float distance;
			inline void Normalise()
			{
				float norm = 1.0f / sqrt((normal.x*normal.x) + (normal.y*normal.y) + (normal.z*normal.z));
				normal.x = normal.x * norm;
				normal.y = normal.y * norm;
				normal.z = normal.z * norm;
				distance = distance * norm;
			}
			inline float DistanceToPoint(const Vector& pnt)
			{
				return normal.Dot(pnt) + distance;
			}
		} z_frustum[6];

		Vector z_minFrustum;
		Vector z_maxFrustum;
		bool z_target;
		bool z_update;
		bool z_isOrtho;
		bool z_isLook;
		class InputListener *z_listener;
	public:
		inline float GetNear() { return z_near; };
		inline float GetFar() { return z_far; };
		inline Matrix4 *GetViewInverse() { return &z_invView; }
		inline Matrix4 *GetViewY() { return &z_invViewY; }
		Camera();
		~Camera();
		void SetSight(bool enable);
		bool IsSight();
		Node *GetSight();
		void SetName(const string &name);
		string GetName();
		inline Vector GetPickDir() { return z_pickDir; };
		bool CheckFrustum(const class BBox &box);
		void SetClip(float nearDist = 1.0f, float farDist = 1000.0f);
		Matrix GetViewMatrix();
		Matrix4 GetProjMatrix();
		void SetOrthographic(bool enable);
		bool IsOrthographic();
		void Refresh();
		void UpdateOrient();
		void Update();
		void UpdateHit(const Point2 &m);
		void UpdateHit(const Point2 &m, int screenX, int screenY, int sizeX, int sizeY);
		void GetPick(int x, int y, Vector &pos, Vector &dir);
		void MoveX(float x, bool solid = false); // �����/������
		void MoveY(float y, bool solid = false); // �����/����
		void MoveZ(float z, bool solid = false); // ������/�����
		void Move(const Vector &pos, bool solid = false);
		void Pitch(float angle, bool solid = false); // �������� ������������ ������� �������
		void Yaw(float angle, bool solid = false);   // �������� ������������ �������� �������
		void Roll(float angle);  // �������� ������������ ������� �������
		Vector GetPosition();
		void SetPosition(Vector pos);
		void SetRotation(Quaternion &q);
		Quaternion GetRotation();
		Quaternion GetWorldRotation();
		Vector GetRight();
		Vector GetUp();
		Vector GetLook();
		Vector GetRightTarget();
		Vector GetUpTarget();
		Vector GetLookTarget();
		void SetLookVector(const Vector &look);
		void SetAspectRatio(float aspect);//����������� ����� � ������ ������
		void ResetAspect();//���������� ����� ������ � ������������ � �������� ������ � ������� ������
		void SetFOV(float angle);//���� ������ ������
		float GetAspectRatio();
		float GetFOV();
		Vector ToScreenCoord(const Vector &p);
		Vector ToSpaceCoord(const float &height, Matrix *mat = NULL);//����������� ��������� ���� � ���������� � ������� h
		void SetTargetEnable(bool enable);
		bool IsTargetEnable();
		Vector GetWorldPosition();
		Node *GetTarget();
		float GetTargetDistance();
		void AlignTarget(float minX, float minY, float maxX, float maxY);//���� ���� ����� �� �������, ���������� ���� � ��� �������
		void CreateListener();//���������� ������� �� ���������
	};
};