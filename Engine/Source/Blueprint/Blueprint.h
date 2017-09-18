#pragma once
#include "../GUI/Widgets/WidgetBlock.h"
namespace DEN
{
	class WidgetBlock;
	class Vector2;
	class BlueprintNode: public WidgetBlock
	{
	private:
		WidgetBlock *_widget;
	public:
		BlueprintNode(GUI *gui);
		~BlueprintNode();
	};
	class Blueprint
	{
	private:
		set<BlueprintNode*> _nodes;
	public:
		Blueprint();
		~Blueprint();
		BlueprintNode *AddNode(Widget *parent, const Vector2 &pos);
	};
};
