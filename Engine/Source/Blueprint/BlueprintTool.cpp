#include "stdafx.h"
#include "GUI/Class.h"
#include "BlueprintTool.h"
#include "../Core/Engine.h"
#include "../Math/Vector.h"
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
		lis->onMouseHit = [this, grid](MouseEventClick eve)
		{
			if(eve.btn == 1)
			{
				this->_pos = Point2(eve.x, eve.y);
				this->_isMove = true;
			}
			return true;
		};
		lis->onMouseMove = [this, grid](MouseEvent eve)
		{
			if(_isMove)
			{
				Point2 pos = this->_pos - Point2(eve.x, eve.y);
				Vector2 trans = grid->GetRelative().GetTranslation() + Vector2(pos.x, pos.y)*Engine::Get()->GetTime().spf*4.0f;
				grid->GetRelative().SetTranslation(trans);
			}
			return true;
		};
		lis->onMouseReleased = [this, grid](MouseEventClick eve)
		{
			if(eve.btn == 1)
				this->_isMove = false;
			return true;
		};
		lis->onMouseWhell = [this, grid](MouseEventWheel eve)
		{
			Vector2 scale = grid->GetRelative().GetScale();
			float w[12] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.75f, 1.0f, 1.5f, 2.0f, 4.0f, 7.0f, 10.0f};
			if(eve.wheel > 0)
				++_posId;
			else if(_posId > 0)
				--_posId;
			_posId = min(_posId, 11);
			scale.x = scale.y = w[_posId];
			grid->GetRelative().SetScale(scale);
			return true;
		};
		WidgetBlock *bblock = new WidgetBlock(_gui);
		bblock->SetParent(grid);
		bblock->SetLeft(50.0f);
		bblock->SetTop(60.0f);
		bblock->SetWidth(200.0f);
		bblock->SetHeight(500.0f);
		bblock->SetBackgroundColor(Color("#1C1E1CFA"));
		bblock->SetBorderRadius(Square(5.0f, 5.0f, 5.0f, 5.0f));
		bblock->SetBorderWidth(1.0f);
		bblock->SetBorderColor(Color("#0C0D0C"));

		WidgetBlock *b1 = new WidgetBlock(_gui);
		b1->SetParent(bblock);
		b1->SetAlign(WIDGET_TOP_STRETCH);
		b1->SetTop(0.0f);
		b1->SetLeft(0.0f);
		b1->SetRight(0.0f);
		b1->SetHeight(50.0f);
		b1->SetBackgroundColor(Color("#aaa"));
		b1->SetBorderRadius(Square(5.0f, 5.0f, 0.0f, 0.0f));
		//b1->SetBorderWidth(1.0f);
		b1->SetBorderColor(Color("#0C0D0C"));
	
	
		WidgetText *iText = new WidgetText(_gui);
		iText->SetParent(bblock);
		iText->SetTextColor(Color::C_WHITE);
		iText->SetFamily("arial");
		iText->SetFontSize(24);
		iText->SetBold(false);
		iText->SetText("Hello, трава!");
		iText->SetAlign(WIDGET_STRETCH);
	}
};