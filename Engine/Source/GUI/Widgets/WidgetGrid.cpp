#include "stdafx.h"
#include "WidgetGrid.h"
#include "../../Render/RenderMesh.h"
#include "../../Math/Vector.h"
namespace DEN
{
	WidgetGrid::WidgetGrid(GUI *gui): Widget(gui)
	{
		_buffer->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		_indexes = new vector<uint>;
	}
	WidgetGrid::~WidgetGrid()
	{
		
	}
	void WidgetGrid::AddLine(const Vector2 &v0, const Vector2 &v1, const Color &color)
	{
		Widget::AddLine(v.size(), v.size() + 1);
		Vertex2D p;
		p.col = color;
		p.pos = v0;
		v.push_back(p);
		p.pos = v1;
		v.push_back(p);
	}
	void WidgetGrid::GenerateGrid(float cellSize, const Square &box, const Color &color)
	{
		for(float x = 0.0f; x <= box.maxX; x += cellSize)
			AddLine(Vector2(x, box.minX), Vector2(x, box.maxX), color);
		for(float x = 0.0f - cellSize; x >= box.minX; x -= cellSize)
			AddLine(Vector2(x, box.minX), Vector2(x, box.maxX), color);
		for(float y = 0.0f; y <= box.maxY; y += cellSize)
			AddLine(Vector2(box.minY, y), Vector2(box.maxY, y), color);
		for(float y = -cellSize; y >= box.minY; y -= cellSize)
			AddLine(Vector2(box.minY, y), Vector2(box.maxY, y), color);
	}
};