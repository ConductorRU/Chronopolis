#include "stdafx.h"
#include "XML.h"
namespace DEN
{
	vector<string> XML::errors;
	XMLNode* XML::Load(const string &filename)
	{
		XML::errors.clear();
		ifstream myfile;
		myfile.open(filename, ios::in);
		if(myfile.fail())
			return nullptr;
		XMLNode *root = nullptr;
		XMLNode *cur = nullptr;
		uint depth = 0;
		string lines;
		string name = "";
		string body = "";
		bool isTag = false;
		bool isName = false;//��� ����
		bool isClose = false;
		bool isBody = true;
		char isProp = 0;//��� �������� ����

		string aName = "";
		string aProp = "";
		while(getline(myfile, lines))
		{
			uint cnt = lines.size();
			for(uint i = 0; i < cnt; ++i)
			{
				char c = lines[i];
				if(isName)//���� ��� ����
				{
					if(isalpha(c) || isdigit(c) || c == '_')
						name.push_back(c);
					else if(isspace(c) || c == '/' || c == '>')
					{
						if(!isClose)
							cur->tag = name;
						else
						{
							if(cur->tag != name)
								XML::errors.push_back("Warning: tag name not match");
							cur = cur->parent;
						}
						name = "";
						isName = false;
						if(isspace(c))
							isProp = 1;
					}
				}
				else if(isTag && isspace(c) && isProp == 0)//���� <...[������]...>
					isProp = 1;
				else if(isProp == 1)//���� <...������[...]...>
				{
					if(isalpha(c) || isdigit(c) || c == '_')
						aName.push_back(c);
					else if(aName != "")
					{
						if(c == '=')
							isProp = 3;
						else
							isProp = 2;
					}
				}
				else if(isProp == 2 && c == '=')//���� <...������...[=]...>
				{
					isProp = 3;
				}
				else if(isProp == 3 && c == '"')//���� <...������...=["]...>
				{
					isProp = 4;
				}
				else if(isProp == 4)//���� <...������...="[...]...>
				{
					if(c == '"')
					{
						cur->attributes[aName] = aProp;
						aName = "";
						aProp = "";
						isProp = 0;
					}
					else
						aProp.push_back(c);
				}
				else if(c == '<')
				{
					isTag = true;
					if(root == nullptr)
					{
						root = new XMLNode;
						cur = root;
					}
					else
					{
						XMLNode* n = new XMLNode;
						n->parent = cur;
						cur->childs.push_back(n);
						cur = n;
					}
					isName = true;
					isBody = false;
					body = "";
				}
				else if(isTag && c == '/')//���� <...[/]
				{
					isTag = false;
					isClose = true;
					if(body != "")
					{
						XMLNode* n = new XMLNode;
						n->parent = cur;
						cur->childs.push_back(n);
						cur->text = body;
					}
				}
				else if(isClose && c == '>')//���� <.../[>]
				{
					cur = cur->parent;
					isClose = false;
					isBody = true;
					isTag = false;
				}
				else if(c == '>')//���� <...[>]
				{
					isBody = true;
					isTag = false;
					body = "";
				}
				else if(isBody)//���� <...>[...]
				{
					body.push_back(c);
				}
			}
		}
		return root;
	}
}