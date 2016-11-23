#pragma once
#include "../Math/Color.h"
#include "../Math/Vector.h"
#include "../Math/Node.h"
namespace DEN
{
	enum LIGHT_RENDER_TYPE
	{
		LIGHT_DYNAMIC = 0,//дл¤ динамического освещени¤
		LIGHT_STATIC = 1,//дл¤ генерации лайтмапов, в освещении мешей без лайтмапов не участвует
		LIGHT_HYBRID = 2,//статическое - дл¤ мешей с лайтмапом, динамическое - дл¤ обычных мешей
	};
	enum TYPE_LIGHT
	{
		LIGHT_POINT = 1,
		LIGHT_SPOT = 2,
		LIGHT_DIRECTIONAL = 3,
		LIGHT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
	};
	struct D3DLight
	{
		TYPE_LIGHT type;            /* Type of light source */
		Color      diffuse;         /* Diffuse color of light */
		Color      specular;        /* Specular color of light */
		Color      ambient;         /* Ambient color of light */
		Vector     position;        /* Position in world space */
		Vector     direction;       /* Direction in world space */
		float      range;           /* Cutoff range */
		float      falloff;         /* Falloff */
		float      attenuation0;    /* Constant attenuation */
		float      attenuation1;    /* Linear attenuation */
		float      attenuation2;    /* Quadratic attenuation */
		float      theta;           /* Inner angle of spotlight cone */
		float      phi;             /* Outer angle of spotlight cone */
	};
	class Light:public NodeMatrix
	{
	private:
		D3DLight z_light;
		bool z_enable;
		bool z_global;
		LIGHT_RENDER_TYPE z_type;
		string z_name;
	public:
		Light();
		~Light();
		void Copy(Light *from);
		bool IsRange(const Vector &pos, float length = 0.0f);
		void Update();
		string GetName();
		void SetName(const string &name);
		void SetAmbient(const Color &color);
		void SetDiffuse(const Color &color);
		void SetSpecular(const Color &color);
		void SetLightType(TYPE_LIGHT type);
		void SetRange(float range);
		void SetEnable(bool on);
		void SetGlobal(bool isGlobal);
		void SetSpotCone(float in, float out);
		void SetFalloff(float falloff);
		float GetTheta();
		float GetPhi();
		float GetFallow();
		float GetAttenuation0();
		float GetAttenuation1();
		float GetAttenuation2();
		void SetAttenuation0(float at);
		void SetAttenuation1(float at);
		void SetAttenuation2(float at);
		Color GetAmbient();
		Color GetDiffuse();
		Color GetSpecular();
		TYPE_LIGHT GetType();
		float GetRange();
		bool IsEnable();
		void SetRenderType(LIGHT_RENDER_TYPE type);
		LIGHT_RENDER_TYPE GetRenderType();
		bool IsDynamic();//принимает ли участие в рендеринге динамического освещения
		bool IsStatic();//принимает ли участие в освещении с использованием лайтмапов
		bool IsGlobal();//для всех мешей или только для тех, у кого включен список света
	};
}