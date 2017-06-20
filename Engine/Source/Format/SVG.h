#pragma once
namespace DEN
{
	class SVG
	{
	private:
		XMLNode* _node;
		class Widget* _root;
		void Parse();
		static Widget *ParseNode(XMLNode *node);
	public:
		SVG();
		static SVG *Load(const string &filename, class GUI *gui);
	};
}