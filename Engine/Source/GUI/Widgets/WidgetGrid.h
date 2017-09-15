#pragma once
#include "../Widget.h"

namespace DEN
{
	class WidgetGrid : public Widget
	{
	protected:
	public:
		WidgetGrid(GUI *gui);
		~WidgetGrid();
		void AddLine(const Vector2 &v0, const Vector2 &v1, const Color &color);
		void GenerateGrid(float cellSize, const Square &box, const Color &color);
	};
};