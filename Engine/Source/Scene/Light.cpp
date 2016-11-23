#include "stdafx.h"
#include "../Math/Matrix.h"
#include "Light.h"
namespace DEN
{
	Light::Light()
	{
		ZeroMemory(&z_light, sizeof(D3DLight));
		z_global = true;
		z_enable = true;
		z_light.attenuation0 = 1.0f;
		z_light.falloff = 1.0f;
		SetRange(100.0f);
		SetLightType(LIGHT_POINT);
		z_type = LIGHT_HYBRID;
	}

	Light::~Light()
	{
	}

	void Light::Copy(Light *from)
	{
		z_light = from->z_light;
		z_enable = from->z_enable;
		z_global = from->z_global;
		z_type = from->z_type;
		z_name = from->z_name;
		//class Shadow *z_shadow;
	}
	void Light::Update()
	{
		GetNode()->Update();
		Vector pos = GetNode()->GetPosition(true);
		z_light.position = pos;
		pos = GetNode()->GetMatrix()->GetLook();
		z_light.direction = pos;
	}

	bool Light::IsRange(const Vector &pos, float length)
	{
		Vector v = z_light.position;
		if(v.GetDistance(pos) <= z_light.range + length)
			return true;
		return false;
	}

	string Light::GetName()
	{
		return z_name;
	}

	void Light::SetName(const string &name)
	{
		z_name = name;
	}

	void Light::SetAmbient(const Color &color)
	{
		z_light.ambient = color;
	}

	void Light::SetDiffuse(const Color &color)
	{
		z_light.diffuse = color;
	}

	void Light::SetSpecular(const Color &color)
	{
		z_light.specular = color;
	}

	void Light::SetLightType(TYPE_LIGHT type)
	{
		z_light.type = type;
	}

	void Light::SetRange(float range)
	{
		z_light.range = range;
	}

	void Light::SetEnable(bool on)
	{
		z_enable = on;
	}

	void Light::SetGlobal(bool isGlobal)
	{
		z_global = isGlobal;
	}

	void Light::SetSpotCone(float in, float out)
	{
		z_light.theta = min(in, 3.1415926f);
		z_light.phi = min(out, 3.1415926f);
	}

	void Light::SetFalloff(float falloff)
	{
		z_light.falloff = falloff;
	}

	float Light::GetTheta()
	{
		return z_light.theta;
	}

	float Light::GetPhi()
	{
		return z_light.phi;
	}

	float Light::GetFallow()
	{
		return z_light.falloff;
	}

	float Light::GetAttenuation0()
	{
		return z_light.attenuation0;
	}

	float Light::GetAttenuation1()
	{
		return z_light.attenuation1;
	}

	float Light::GetAttenuation2()
	{
		return z_light.attenuation2;
	}

	void Light::SetAttenuation0(float at)
	{
		z_light.attenuation0 = at;
	}

	void Light::SetAttenuation1(float at)
	{
		z_light.attenuation1 = at;
	}

	void Light::SetAttenuation2(float at)
	{
		z_light.attenuation2 = at;
	}

	Color Light::GetAmbient()
	{
		return z_light.ambient;
	}

	Color Light::GetDiffuse()
	{
		return z_light.diffuse;
	}

	Color Light::GetSpecular()
	{
		return z_light.specular;
	}

	TYPE_LIGHT Light::GetType()
	{
		return z_light.type;
	}

	float Light::GetRange()
	{
		return z_light.range;
	}

	bool Light::IsEnable()
	{
		return z_enable;
	}

	void Light::SetRenderType(LIGHT_RENDER_TYPE type)
	{
		z_type = type;
	}

	LIGHT_RENDER_TYPE Light::GetRenderType()
	{
		return z_type;
	}

	bool Light::IsDynamic()
	{
		return z_type == LIGHT_DYNAMIC || z_type == LIGHT_HYBRID;
	}

	bool Light::IsStatic()
	{
		return z_type == LIGHT_STATIC || z_type == LIGHT_HYBRID;
	}

	bool Light::IsGlobal()
	{
		return z_global;
	}
}