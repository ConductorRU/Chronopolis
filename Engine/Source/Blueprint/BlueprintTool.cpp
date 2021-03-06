#include "stdafx.h"
#include "GUI/Class.h"
#include "BlueprintTool.h"
#include "Blueprint.h"
#include "../Core/Engine.h"
#include "../Math/Vector.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
namespace DEN
{
	BlueprintTool::BlueprintTool(GUI *gui)
	{
		_gui = gui;
	}
	BlueprintTool::~BlueprintTool()
	{
	
	}
	void BlueprintTool::Initialize()
	{
		Widget *block = new WidgetBlock(_gui);
		block->SetParent(_gui->GetRoot());
		block->SetAlign(WIDGET_STRETCH);
		block->SetBackgroundColor(Color("#2A2A2A"));
		WidgetGrid *grid = new WidgetGrid(_gui);
		grid->GetRelative().SetTranslation(500.0f, 500.0f);
		grid->GenerateGrid(10, Square(-1000.0f, -1000.0f, 1000.0f, 1000.0f), Color("#353535"));
		grid->GenerateGrid(50, Square(-1000.0f, -1000.0f, 1000.0f, 1000.0f), Color("#1C1C1C"));
		grid->AddLine(Vector2(-1000.0f, 0.0f), Vector2(1000.0f, 0.0f), Color("#000"));
		grid->AddLine(Vector2(0.0f, -1000.0f), Vector2(0.0f, 1000.0f), Color("#000"));
		grid->SetParent(block);

		GUIListener *lis = grid->CreateListener();
		_isMove = false;
		_posId = 6;
		lis->onMouseHit = [this, grid](MouseEventClick eve, InputListener *lis)
		{
			if(eve.btn == 1)
			{
				this->_pos = Point2(eve.x, eve.y);
				this->_isMove = true;
			}
			return true;
		};
		lis->onMouseMove = [this, grid](MouseEvent eve, InputListener *lis)
		{
			if(_isMove)
			{
				Point2 pos = this->_pos - Point2(eve.x, eve.y);
				Vector2 trans = grid->GetRelative().GetTranslation() + Vector2((float)pos.x, (float)pos.y)*Engine::Get()->GetTime().spf*4.0f;
				grid->GetRelative().SetTranslation(trans);
			}
			return true;
		};
		lis->onMouseReleased = [this, grid](MouseEventClick eve, InputListener *lis)
		{
			if(eve.btn == 1)
				this->_isMove = false;
			return true;
		};
		lis->onMouseWhell = [this, grid](MouseEventWheel eve, InputListener *lis)
		{
			Vector2 scale = grid->GetRelative().GetScale();
			float w[12] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.75f, 1.0f, 1.5f, 2.0f, 4.0f, 7.0f, 10.0f};
			if(eve.wheel > 0)
				++_posId;
			else if(_posId > 0)
				--_posId;
			_posId = min(_posId, 11);
			float d = w[_posId] - 1.0f;
			scale.x = scale.y = w[_posId];

			Vector curPos = Engine::Get()->GetScene()->GetCamera()->ToSpaceCoord(0.0f);
			Point2 cPos = Engine::Get()->GetInput()->GetCursorPos();
			grid->GetRelative().SetScale(scale);

			Vector2 trans = grid->GetRelative().GetTranslation();
			//trans.x = cPos.x;
			//trans.y = cPos.x;
			grid->GetRelative().SetTranslation(trans + Vector2(curPos.x*d, curPos.y*d));
			return true;
		};
		Blueprint *blue = CreateBlueprint();
		blue->AddNode(grid, Vector2());
	}
	Blueprint *BlueprintTool::CreateBlueprint()
	{
		_blue = new Blueprint();
		return _blue;
	}
};