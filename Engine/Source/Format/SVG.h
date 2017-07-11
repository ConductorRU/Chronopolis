#pragma once
namespace DEN
{
	class SVG
	{
	private:
		XMLNode* _node;
		class WidgetX* _root;
		void Parse();
		static WidgetX *ParseNode(XMLNode *node);
	public:
		SVG();
		static SVG *Load(const string &filename, class GUI *gui);
	};
}