#pragma once
#include "Render/RenderTexture.h"
namespace DEN
{
	class Modifier2d
	{
	protected:

	public:
		Modifier2d();
		~Modifier2d();
		virtual void Update(RenderTexture *tex);
	};
}