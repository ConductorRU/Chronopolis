#include "StdAfx.h"
#include "../Math/Math.h"
#include "../Render/Render.h"
#include "../Input/Input.h"
#include "Scene.h"
#include "Camera.h"
#include "../Core/Engine.h"
namespace DEN
{
	Camera::Camera()
	{
		z_node = new Node();///z_scene->GetEngine()->CreateNode();
		z_signt = new Node();///z_scene->GetEngine()->CreateNode();
		z_temp = Vector(0.0f, 0.0f, 0.0f);
		z_time = 0.0f;
		z_right = Vector(1.0f, 0.0f, 0.0f);
		z_up = Vector(0.0f, 1.0f, 0.0f);
		z_look = Vector(0.0f, 0.0f, 1.0f);
		z_pos = Vector(0.0f, 0.0f, 0.0f);
		z_fov = 45.0f;
		z_aspect = Engine::Get()->GetRender()->GetAspect();
		z_near = 0.1f;
		z_far = 10000.0f;
		z_matrixProjection = Matrix::Perspective(AngleToRad(z_fov), z_aspect, z_near, z_far);
		///Engine::Get()->GetDevice()->SetTransform(D3DTS_VIEW, GetViewMatrix().ToMatrix());
		z_target = false;
		z_update = true;
		z_isOrtho = false;
		z_isLook = false;
		z_listener = nullptr;
	}

	Camera::~Camera()
	{
		delete z_node;
		delete z_signt;
		if(z_listener)
		{
			Engine::Get()->GetInput()->RemoveListener(z_listener);
			delete z_listener;
		}
	}
	Vector Camera::GetCursorPos()
	{
		Vector2 npos = Engine::Get()->GetCursorPos();
		Vector pos;
		pos.x = ((-npos.x/Engine::Get()->GetRender()->GetAspect())*tan(z_fov/2.0f))*1.0f;
		pos.y = (npos.y*tan(z_fov/2.0f))*1.0f;
		pos.z = 1.0f;
		pos = pos*z_far;
		pos = z_matrixView.Inverse().TransformCoord(pos);
		return pos;
	}
	void Camera::SetSight(bool enable)
	{
		z_isLook = enable;
	}

	bool Camera::IsSight()
	{
		return z_isLook;
	}

	Node *Camera::GetSight()
	{
		return z_signt;
	}

	void Camera::SetName(const string &name)
	{
		z_name = name;
	}

	string Camera::GetName()
	{
		return z_name;
	}

	bool Camera::CheckFrustum(const BBox &box)
	{
		Vector bMin = box.GetMin();
		Vector bMax = box.GetMax();
		for(int i = 0; i < 6; ++i)
		{
			if(z_frustum[i].normal.x*bMin.x + z_frustum[i].normal.y*bMin.y + z_frustum[i].normal.z*bMin.z + z_frustum[i].distance <= 0.0f)
				if(z_frustum[i].normal.x*bMax.x + z_frustum[i].normal.y*bMax.y + z_frustum[i].normal.z*bMax.z + z_frustum[i].distance <= 0.0f)
					if(z_frustum[i].normal.x*bMin.x + z_frustum[i].normal.y*bMax.y + z_frustum[i].normal.z*bMin.z + z_frustum[i].distance <= 0.0f)
						if(z_frustum[i].normal.x*bMax.x + z_frustum[i].normal.y*bMax.y + z_frustum[i].normal.z*bMin.z + z_frustum[i].distance <= 0.0f)
							if(z_frustum[i].normal.x*bMax.x + z_frustum[i].normal.y*bMin.y + z_frustum[i].normal.z*bMin.z + z_frustum[i].distance <= 0.0f)
								if(z_frustum[i].normal.x*bMin.x + z_frustum[i].normal.y*bMin.y + z_frustum[i].normal.z*bMax.z + z_frustum[i].distance <= 0.0f)
									if(z_frustum[i].normal.x*bMin.x + z_frustum[i].normal.y*bMax.y + z_frustum[i].normal.z*bMax.z + z_frustum[i].distance <= 0.0f)
										if(z_frustum[i].normal.x*bMax.x + z_frustum[i].normal.y*bMin.y + z_frustum[i].normal.z*bMax.z + z_frustum[i].distance <= 0.0f)
											return false;
		}
		return true;
	}

	void Camera::SetClip(float nearDist, float farDist)
	{
		z_near = nearDist;
		z_far = farDist;
	}

	Matrix Camera::GetViewMatrix()
	{
		if(z_update)
		{
			z_look.Normalize();
			z_up = z_look.Cross(z_right);
			//z_up.y = abs(z_up.y);//нужна дл¤ LookAt, но есть баг
			z_up.Normalize();
			z_right = z_up.Cross(z_look);
			z_right.Normalize();
			float x = -z_right.Dot(z_pos);
			float y = -z_up.Dot(z_pos);
			float z = -z_look.Dot(z_pos);
			z_matrixView(0, 0) = z_right.x;
			z_matrixView(1, 0) = z_up.x;
			z_matrixView(2, 0) = z_look.x;
			z_matrixView(0, 1) = z_right.y;
			z_matrixView(1, 1) = z_up.y;
			z_matrixView(2, 1) = z_look.y;
			z_matrixView(0, 2) = z_right.z;
			z_matrixView(1, 2) = z_up.z;
			z_matrixView(2, 2) = z_look.z;
			z_matrixView(0, 3) = x;
			z_matrixView(1, 3) = y;
			z_matrixView(2, 3) = z;
			if(z_target)
			{
				z_world = z_node->GetMatrix()->TransformCoord(z_pos) - z_matrixView.TransformCoord(z_pos);
				Matrix mat = z_node->GetMatrix()->Inverse();
				z_matrixView = mat*z_matrixView;
			}
			else
				z_world = z_pos;
			Matrix4 matComb;
			matComb = MatrixMultiply(z_matrixView.ToMatrix(), z_matrixProjection);

			//Ћева¤ плоскость viewing frustum
			z_frustum[0].normal.x = matComb._14 + matComb._11;
			z_frustum[0].normal.y = matComb._24 + matComb._21;
			z_frustum[0].normal.z = matComb._34 + matComb._31;
			z_frustum[0].distance = matComb._44 + matComb._41;

			//ѕрава¤ плоскость viewing frustum
			z_frustum[1].normal.x = matComb._14 - matComb._11;
			z_frustum[1].normal.y = matComb._24 - matComb._21;
			z_frustum[1].normal.z = matComb._34 - matComb._31;
			z_frustum[1].distance = matComb._44 - matComb._41;

			//¬ерхн¤¤ плоскость viewing frustum
			z_frustum[2].normal.x = matComb._14 - matComb._12;
			z_frustum[2].normal.y = matComb._24 - matComb._22;
			z_frustum[2].normal.z = matComb._34 - matComb._32;
			z_frustum[2].distance = matComb._44 - matComb._42;

			//Ќижн¤¤ плоскость viewing frustum
			z_frustum[3].normal.x = matComb._14 + matComb._12;
			z_frustum[3].normal.y = matComb._24 + matComb._22;
			z_frustum[3].normal.z = matComb._34 + matComb._32;
			z_frustum[3].distance = matComb._44 + matComb._42;

			//Ѕлижн¤¤ плоскость viewing frustum
			z_frustum[4].normal.x = matComb._13;
			z_frustum[4].normal.y = matComb._23;
			z_frustum[4].normal.z = matComb._33;
			z_frustum[4].distance = matComb._43;

			//?альн¤¤ плоскость viewing frustum
			z_frustum[5].normal.x = matComb._14 - matComb._13;
			z_frustum[5].normal.y = matComb._24 - matComb._23;
			z_frustum[5].normal.z = matComb._34 - matComb._33;
			z_frustum[5].distance = matComb._44 - matComb._43;
		}
		z_update = false;
		return z_matrixView;
	}

	Matrix4 Camera::GetProjMatrix()
	{
		return z_matrixProjection;
	}

	void Camera::SetOrthographic(bool enable)
	{
		z_isOrtho = enable;
	}

	bool Camera::IsOrthographic()
	{
		return z_isOrtho;
	}

	void Camera::Refresh()
	{
		z_update = true;
	}

	void Camera::UpdateHit(const Point2 &m)
	{
		UpdateHit(m, 0, 0, Engine::Get()->GetRender()->GetWidth(), Engine::Get()->GetRender()->GetHeight());
	}

	void Camera::UpdateHit(const Point2 &m, int screenX, int screenY, int sizeX, int sizeY)
	{
		Vector v;
		v.x = (((2.0f*(m.x - screenX)) / sizeX) - 1) / z_matrixProjection._11;
		v.y = -(((2.0f*(m.y - screenY)) / sizeY) - 1) / z_matrixProjection._22;
		v.z = 1.0f;
		Matrix mx;
		mx = z_matrixView.Inverse();
		z_pickDir.x = v.x * mx(0, 0) + v.y * mx(0, 1) + v.z * mx(0, 2);
		z_pickDir.y = v.x * mx(1, 0) + v.y * mx(1, 1) + v.z * mx(1, 2);
		z_pickDir.z = v.x * mx(2, 0) + v.y * mx(2, 1) + v.z * mx(2, 2);
	}

	void Camera::UpdateOrient()
	{
		if(z_isLook)
		{
			if(z_target)
				SetLookVector(z_signt->GetPosition(true) - z_node->GetPosition(true));
			else
				SetLookVector(z_signt->GetPosition(true));
		}
		GetViewMatrix();
	}

	void Camera::Update()
	{
		UpdateOrient();
		UpdateHit(Engine::Get()->GetInput()->GetCursorPos());
		///z_scene->GetEngine()->GetDevice()->SetTransform(D3DTS_VIEW, z_matrixView.ToMatrix());
		z_invView = z_matrixView.Inverse().ToMatrix();
		//z_aspect = z_scene->GetEngine()->GetDevice()->GetScreenAspectRatio();
		if(z_isOrtho)
			z_matrixProjection = Matrix::Orthographic(z_aspect*GetWorldPosition().y, GetWorldPosition().y, z_near, z_far);
		else
			z_matrixProjection = Matrix::Perspective(AngleToRad(z_fov), z_aspect, z_near, z_far);
		///z_scene->GetEngine()->GetDevice()->SetTransform(D3DTS_PROJECTION, z_matrixProjection);
		Matrix m = z_matrixView;
		m.RotationAxis(Vector::ONE_Y);
		z_invViewY = m.ToMatrix();
		/*Vector vPickRayDir;
		Vector vPickRayOrig;
		Vector v;
		v.x = -1/z_matrixProjection._11;
		v.y = 1/z_matrixProjection._22;
		v.z = 1.0f;
		Matrix m;
		m = GetViewMatrix().Inverse();
		vPickRayDir.x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0);
		vPickRayDir.y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1);
		vPickRayDir.z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2);
		vPickRayOrig.x = m(3, 0);
		vPickRayOrig.y = m(3, 1);
		vPickRayOrig.z = m(3, 2);*/
	}

	void Camera::GetPick(int x, int y, Vector &pos, Vector &dir)
	{
		GetViewMatrix();
		Vector v;
		v.x = (((2.0f*x) / Engine::Get()->GetRender()->GetWidth()) - 1) / z_matrixProjection._11;
		v.y = -(((2.0f*y) / Engine::Get()->GetRender()->GetHeight()) - 1) / z_matrixProjection._22;
		Matrix m;
		m = z_matrixView.Inverse();
		dir.x = v.x * m(0, 0) + v.y * m(0, 1) + m(0, 2);
		dir.y = v.x * m(1, 0) + v.y * m(1, 1) + m(1, 2);
		dir.z = v.x * m(2, 0) + v.y * m(2, 1) + m(2, 2);
		pos.x = m(0, 3);
		pos.y = m(1, 3);
		pos.z = m(2, 3);
	}

	void Camera::Move(const Vector &pos, bool solid)
	{
		if(!solid)
		{
			z_pos += z_right * pos.x;
			z_pos += z_up * pos.y;
			z_pos += z_look * pos.z;
		}
		else
		{
			z_pos += Vector(z_right.x, 0.0f, z_right.z) * pos.x;
			z_pos.y += (abs(z_right.z) + abs(z_look.z) + abs(z_up.z)) * pos.y;
			z_pos += Vector(z_up.x + z_look.x*z_up.y, 0.0f, z_up.z + z_look.z*z_up.y) * pos.z;
		}
	}

	void Camera::MoveX(float units, bool solid)
	{
		z_update = true;
		Matrix *m = z_node->GetMatrix();
		if(z_target)
		{
			Vector p1 = Vector(m->m[0][0], m->m[1][0], m->m[2][0]);
			Vector v = z_node->GetPosition(true) + (Vector(p1.x, 0.0f, p1.z) * units);
			z_node->SetPosition(v);
			return;
		}
		if(solid == true)
		{
			z_pos += Vector(z_right.x, 0.0f, z_right.z) * units;
		}
		else
			z_pos += z_right * units;
	}

	void Camera::MoveY(float units, bool solid)
	{
		z_update = true;
		if(solid == true)
			z_pos.y += (abs(z_right.z) + abs(z_look.z) + abs(z_up.z)) * units;
		else
			z_pos += z_up * units;
	}

	void Camera::MoveZ(float units, bool solid)
	{
		z_update = true;
		Matrix *m = z_node->GetMatrix();
		if(z_target)
		{
			Vector p1 = Vector(m->m[0][2], m->m[1][2], m->m[2][2]);
			Vector v = z_node->GetPosition(true) + (Vector(p1.x, 0.0f, p1.z) * units);
			if(m->m[1][1] >= 0.0f)
				v -= Vector(m->m[0][1] * p1.y, 0.0f, m->m[2][1] * p1.y) * units;
			else
				v += Vector(m->m[0][1] * p1.y, 0.0f, m->m[2][1] * p1.y) * units;
			z_node->SetPosition(v);
			return;
		}
		if(solid == true)
			z_pos += Vector(z_up.x + z_look.x*z_up.y, 0.0f, z_up.z + z_look.z*z_up.y) * units;
		else
			z_pos += z_look * units;
	}

	void Camera::Pitch(float angle, bool solid)
	{
		z_update = true;
		Matrix mat = Matrix::IDENTITY;
		if(solid == true)
			mat.RotationX(angle);
		else
			mat.RotationAxis(z_right, angle);
		z_up = mat.TransformCoord(z_up);
		z_look = mat.TransformCoord(z_look);
	}

	void Camera::Yaw(float angle, bool solid)
	{
		z_update = true;
		Matrix mat = Matrix::IDENTITY;
		if(solid == true)
			mat.RotationY(angle);
		else
			mat.RotationAxis(z_up, angle);
		z_right = mat.TransformCoord(z_right);
		z_look = mat.TransformCoord(z_look);
	}

	void Camera::Roll(float angle)
	{
		z_update = true;
		Matrix mat;
		mat.RotationAxis(z_look, angle);
		z_right = mat.TransformCoord(z_right);
		z_up = mat.TransformCoord(z_up);
	}

	Vector Camera::GetRight()
	{
		return z_right;
	}

	Vector Camera::GetUp()
	{
		return z_up;
	}

	Vector Camera::GetLook()
	{
		return z_look;
	}

	Vector Camera::GetRightTarget()
	{
		Matrix mat = GetViewMatrix();
		return Vector(mat(0, 0), mat(0, 1), mat(0, 2));
	}

	Vector Camera::GetUpTarget()
	{
		Matrix mat = GetViewMatrix();
		return Vector(mat(1, 0), mat(1, 1), mat(1, 2));
	}

	Vector Camera::GetLookTarget()
	{
		Matrix mat = GetViewMatrix();
		return Vector(mat(2, 0), mat(2, 1), mat(2, 2));
	}

	Vector Camera::GetPosition()
	{
		return z_pos;
	}

	Vector Camera::GetWorldPosition()
	{
		GetViewMatrix();
		return z_world;
	}

	float Camera::GetTargetDistance()
	{
		return z_world.GetDistance(z_node->GetPosition(true));
	}

	void Camera::SetPosition(Vector pos)
	{
		z_update = true;
		z_pos = pos;
	}

	void Camera::SetRotation(Quaternion &q)
	{
		z_update = true;
		q.ToAxes(z_right, z_up, z_look);
	}

	Quaternion Camera::GetRotation()
	{
		Quaternion q;
		q.FromAxes(z_right, z_up, z_look);
		return q;
	}

	Quaternion Camera::GetWorldRotation()
	{
		Quaternion q;
		Matrix *mat = z_node->GetMatrix();
		q.FromRotationMatrix(*mat);
		//q = q*Quaternion(-PI, Vector::ONE_X);
		//Vector up = z_up;
		//Vector look = z_look;
		//up.y = -look.y;
		//look.y = z_up.y;
		//up.z = look.z;
		//look.z = z_up.z;
		//q.FromAxes(z_right, up, look);
		return q;
	}

	void Camera::SetLookVector(const Vector &look)
	{
		z_update = true;
		Vector zAdjustVec = look - z_pos;
		if(zAdjustVec == Vector::ZERO)
			return;
		zAdjustVec.Normalize();
		z_look = zAdjustVec;
		z_right = Vector::ONE_Y.Cross(z_look);
		z_right.Normalize();
		z_up = zAdjustVec.Cross(z_right);
		z_up.Normalize();
	}

	void Camera::SetAspectRatio(float aspect)
	{
		z_aspect = aspect;
		z_matrixProjection = Matrix::Perspective(AngleToRad(z_fov), z_aspect, z_near, z_far);
	}

	void Camera::ResetAspect()
	{
		z_aspect = Engine::Get()->GetRender()->GetAspect();
		z_matrixProjection = Matrix::Perspective(AngleToRad(z_fov), z_aspect, z_near, z_far);
	}

	void Camera::SetFOV(float angle)
	{
		z_fov = angle;
		z_matrixProjection = Matrix::Perspective(AngleToRad(z_fov), z_aspect, z_near, z_far);
	}

	float Camera::GetAspectRatio()
	{
		return z_aspect;
	}

	float Camera::GetFOV()
	{
		return z_fov;
	}

	Vector Camera::ToScreenCoord(const Vector &p)
	{
		Vector dZero;
		Vector dest = Vector(0.0f, 0.0f, 0.0f);
		Matrix mat = GetViewMatrix()*Matrix(z_matrixProjection);
		dZero = mat.TransformCoord(p);
		dZero.z -= z_matrixProjection._43;//-0.1f
		dZero.x = dZero.x / dZero.z;
		dZero.y = dZero.y / dZero.z;
		dest.x = (Engine::Get()->GetRender()->GetWidth()*(1 + dZero.x))*0.5f;
		dest.y = (Engine::Get()->GetRender()->GetHeight()*(1 - dZero.y))*0.5f;
		dest.z = dZero.z;
		return dest;
	}

	Vector Camera::ToSpaceCoord(const float &height, Matrix *mat)
	{
		float fBary1, fBary2, fDist;
		Vector dest = Vector(0.0f, 0.0f, 0.0f);
		Vector v0(-100000.0f, height, -100000.0f);
		Vector v1;
		Vector v2(100000.0f, height, 100000.0f);
		if(mat)
		{
			v0 = mat->TransformCoord(v0);
			v2 = mat->TransformCoord(v2);
		}
		for(int i = 0; i != 2; ++i)
		{
			if(i == 1)
				v1 = Vector(-100000.0f, height, 100000.0f);
			else
				v1 = Vector(100000.0f, height, -100000.0f);
			if(mat)
				v1 = mat->TransformCoord(v1);
			if(Vector::IntersectTriangle(z_world, z_pickDir, v0, v1, v2, &fDist, &fBary1, &fBary2))
				dest = z_world + z_pickDir*fDist;
		}
		return dest;
	}

	void Camera::SetTargetEnable(bool enable)
	{
		z_update = true;
		z_target = enable;
		if(z_target)
		{
			/*z_right = Vector(1.0f, 0.0f, 0.0f);
			z_up = Vector(0.0f, 0.0f, 1.0f);
			z_look = Vector(0.0f, -1.0f, 0.0f);*/
			z_pos = Vector(0.0f, z_pos.y, 0.0f);
		}
		else
		{
			z_pos = Vector(z_node->GetPosition(true).x, z_pos.y, z_node->GetPosition(true).z);
		}
	}

	bool Camera::IsTargetEnable()
	{
		return z_target;
	}

	Node *Camera::GetTarget()
	{
		z_update = true;
		return z_node;
	}

	void Camera::AlignTarget(float minX, float minY, float maxX, float maxY)
	{
		z_update = true;
		Vector pos = z_node->GetPosition(true);
		if(pos.x < minX)
			pos.x = minX;
		else if(pos.x > maxX)
			pos.x = maxX;
		if(pos.z < minY)
			pos.z = minY;
		else if(pos.z > maxY)
			pos.z = maxY;
		z_node->SetPosition(pos, true);
	}

	void Camera::CreateListener()
	{
		z_listener = new InputListener;
		Engine::Get()->GetInput()->AddListener(z_listener);
		static float setQY1 = 0.0f;
		static float setQX1 = 0.0f;
		z_listener->onMouseHit = [this](MouseEventClick m)
		{
			if(m.btn == 0)
			{
				z_mPivot = Point2(m.x, m.y);
			}
			else if(m.btn == 2)
			{
				z_mPivot = Point2(m.x, m.y);
			}
			else if(m.btn == 1)
			{
				z_mPivot = Point2(m.x, m.y);
				setQY1 = 0.0f;
				setQX1 = 0.0f;
			}
			return false;
		};
		z_listener->onMouseWhell = [this](MouseEventWheel m)
		{
			MoveY((-m.wheel)*GetTargetDistance()*0.1f, true);
			return false;
		};
		z_listener->onMousePressed = [this](MouseEventClick m)
		{
			Engine *en = Engine::Get();
			float f = GetTargetDistance()*0.01f*en->GetTime().spf;
			if(m.btn == 2)
			{
				MoveX((m.x - z_mPivot.x)*f, true);
				MoveZ(-(m.y - z_mPivot.y)*f, true);
			}
			else if(m.btn == 1)
			{
				float setQX2 = (m.y - z_mPivot.y)*0.02f;
				float setQX = setQX2 - setQX1;
				setQX1 = setQX2;
				float setQY2 = (m.x - z_mPivot.x)*0.02f;
				float setQY = setQY2 - setQY1;
				setQY1 = setQY2;
				if(IsTargetEnable())
				{
					GetTarget()->Rotate(Quaternion(setQX, Vector(1.0f, 0.0f, 0.0f)), false);
					GetTarget()->Rotate(Quaternion(setQY, Vector(0.0f, 1.0f, 0.0f)), true);
				}
				else
				{
					Pitch(setQX);
					Yaw(setQY, true);
				}
			}
			//MoveY((-m.wheel)*GetTargetDistance()*0.1f, true);
			return false;
		};
		/*
		if(input->MouseHit(2))
		{
		float setQX2 = (input->mouseY - input->mousePivotY)*0.02f;
		float setQX = setQX2 - setQX1;
		setQX1 = setQX2;
		float setQY2 = (input->mouseX - input->mousePivotX)*0.02f;
		float setQY = setQY2 - setQY1;
		setQY1 = setQY2;
		if(IsTargetEnable())
		{
		GetTarget()->Rotate(Quaternion(setQX, Vector(1.0f, 0.0f, 0.0f)), false);
		GetTarget()->Rotate(Quaternion(setQY, Vector(0.0f, 1.0f, 0.0f)), true);
		}
		else
		{
		Pitch(setQX);
		Yaw(setQY, true);
		}
		}*/
	}
};