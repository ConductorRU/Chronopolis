#include "stdafx.h"
#include "XML.h"
#include "SVG.h"
#include "GUI/GUI.h"
#include "GUI/Widget.h"
namespace DEN
{
	void SVG::Parse()
	{
		_root = nullptr;
	}
	WidgetX *SVG::ParseNode(XMLNode *node)
	{
		WidgetX *w = nullptr;
		return w;
	}
	SVG::SVG()
	{
		_root = nullptr;
	}
	SVG *SVG::Load(const string &filename, GUI *gui)
	{
		SVG *svg = new SVG;
		svg->_node = XML::Load(filename);
		if(svg->_node)
			svg->_root = ParseNode(svg->_node);
		return svg;
	}
}