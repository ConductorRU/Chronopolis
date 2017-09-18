#include "stdafx.h"
#include "Blueprint.h"
#include "../GUI/Widgets/WidgetText.h"
namespace DEN
{
	BlueprintNode::BlueprintNode(GUI *gui): WidgetBlock(gui)
	{
		SetLeft(70.0f);
		SetTop(60.0f);
		SetWidth(200.0f);
		SetHeight(500.0f);
		SetBackgroundColor(Color("#1C1E1CFA"));
		SetBorderRadius(Square(5.0f, 5.0f, 5.0f, 5.0f));
		SetBorderWidth(1.0f);
		SetBorderColor(Color("#0C0D0C"));

		WidgetBlock *b1 = new WidgetBlock(_gui);
		b1->SetParent(this);
		b1->SetAlign(WIDGET_TOP_STRETCH);
		b1->SetTop(0.0f);
		b1->SetLeft(0.0f);
		b1->SetRight(0.0f);
		b1->SetHeight(20.0f);
		b1->SetBackgroundColor(Color("#0006"));
		b1->SetBorderRadius(Square(5.0f, 5.0f, 5.0f, 5.0f));
	
	
		WidgetText *iText = new WidgetText(_gui);
		iText->SetParent(b1);
		iText->SetTextColor(Color::C_WHITE);
		iText->SetFamily("arial");
		iText->SetFontSize(18);
		iText->SetBold(false);
		iText->SetText("Hello, трава!");
		iText->SetAlign(WIDGET_STRETCH);
	}
	BlueprintNode::~BlueprintNode()
	{
	
	}
	Blueprint::Blueprint()
	{
	
	}
	Blueprint::~Blueprint()
	{
	
	}
	BlueprintNode *Blueprint::AddNode(Widget *parent, const Vector2 &pos)
	{
		BlueprintNode *node = new BlueprintNode(parent->GetGUI());
		node->SetParent(parent);
		_nodes.insert(node);
		return node;
	}
};