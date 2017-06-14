#pragma once
namespace DEN
{
	struct XMLNode
	{
		XMLNode* parent = nullptr;
		vector<XMLNode*> childs;
		map<string, string> attributes;
		string tag;
		string text;
	};
	class XML
	{
	private:
		static vector<string> errors;
	public:
		static XMLNode *Load(const string &filename);
	};
}