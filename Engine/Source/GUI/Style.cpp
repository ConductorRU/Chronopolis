#include "stdafx.h"
#include "../Math/Square.h"
#include "../Render/Direct3d11.h"
#include "Widget.h"
#include "Style.h"
namespace DEN
{
	Property::Property()
	{
		order = 0;
		display = 1;
		position = 0;
		align = 0;
		wSpace = 0;
		z_isUpdate = true;
		z_isCaret = false;
		uv = Square(0.0f, 0.0f, 1.0f, 1.0f);
	}
	float Style::PercentWidth(const string &val, Widget *parent)
	{
		float p = ((float)atof(val.c_str()))*0.01f;
		if(parent)
		{
			Square b = parent->GetProperty().GetBorder();
			Square pd = parent->GetProperty().GetPadding();
			return (parent->GetProperty().GetSquare().maxX - b.maxX - b.minX - pd.maxX - pd.minX)*p;
		}
		return float(Render::Get()->GetWidth())*p;
	}
	float Style::PercentHeight(const string &val, Widget *parent)
	{
		float p = ((float)atof(val.c_str()))*0.01f;
		if(parent)
		{
			Square b = parent->GetProperty().GetBorder();
			Square pd = parent->GetProperty().GetPadding();
			return (parent->GetProperty().GetSquare().maxY - b.maxY - b.minY - pd.maxY - pd.minY)*p;
		}
		return float(Render::Get()->GetHeight())*p;
	}
	float Style::GetPixel(const string &name, const string &val, Widget *parent)
	{
		size_t size = val.size();
		string n;
		string t;
		char op = 0;
		string n1;
		string t1;
		int step = 0;
		for(size_t i = 0; i != size; ++i)
		{
			if(i == 0 && val[i] == '-')
				n += val[i];
			else if(step < 2 && val[i] >= '0' && val[i] <= '9')
			{
				n += val[i];
				step = 1;
			}
			else if(step < 3 && ((val[i] >= 'A' && val[i] <= 'Z') || (val[i] >= 'a' && val[i] <= 'z') || val[i] == '%'))
			{
				step = 2;
				t += val[i];
			}
			else if(step == 2 && (val[i] == '+' || val[i] == '-' || val[i] == '/' || val[i] == '*'))
			{
				step = 3;
				op = val[i];
			}
			else if(step >= 3 && step < 5 && val[i] >= '0' && val[i] <= '9')
			{
				n1 += val[i];
				step = 4;
			}
			else if((val[i] >= 'A' && val[i] <= 'Z') || (val[i] >= 'a' && val[i] <= 'z') || val[i] == '%')
			{
				step = 5;
				t1 += val[i];
			}
			//else
			//	return 0.0f;
		}
		float v1 = 0.0f;
		float v2 = 0.0f;
		if(t == "px")
			v1 = (float)atof(n.c_str());
		else if(t == "%")
		{
			if(name == "width" || name == "x")
				v1 = PercentWidth(n, parent);
			else if(name == "height" || name == "y")
				v1 = PercentHeight(n, parent);
		}
		if(t1 == "px")
		{
			v2 = (float)atof(n1.c_str());
		}
		else if(t1 == "%")
		{
			if(name == "width" || name == "x")
				v2 = PercentWidth(n, parent);
			else if(name == "height" || name == "y")
				v2 = PercentHeight(n, parent);
		}
		if(op == '+')
			return v1 + v2;
		if(op == '-')
			return v1 - v2;
		if(op == '/')
			return v1 / v2;
		if(op == '*')
			return v1 * v2;
		return v1;
	}
	Color Style::GetColor(const string &name, const string &val)
	{
		Color c;
		if(c.FromHex(val))
			return c;
		return c;
	}
	void Style::SetValue(const string &name, const string &value, const string &eve)
	{
		if(z_settings.find(eve) == z_settings.end())
			z_settingOrder.push_back(eve);
		z_settings[eve][name] = value;
		z_update = true;
	}
	void Style::SetEnable(bool enable)
	{
		z_enable = enable;
	}
	void Style::SetStyle(const string &style, const string &eve)
	{
		size_t size = style.size();
		string name, value;
		bool isVal = false;
		for(size_t i = 0; i <= size; ++i)
		{
			if(i < size && style[i] != ';')
			{
				if(style[i] == ':')
					isVal = true;
				else if(!isVal)
				{
					if(style[i] != ' ' || name.size() > 0)
						name += style[i];
				}
				else
				{
					if(style[i] != ' ' || value.size() > 0)
						value += style[i];
				}
			}
			else
			{
				while(name.size() && name[name.size() - 1U] == ' ')
					name.pop_back();
				while(value.size() && value[value.size() - 1U] == ' ')
					value.pop_back();
				if(name.size() > 0 && value.size() > 0)
				{
					if(z_settings.find(eve) == z_settings.end())
						z_settingOrder.push_back(eve);
					z_settings[eve][name] = value;
				}
				name = "";
				value = "";
				isVal = false;
				z_update = true;
			}
		}
	}
	void Style::SetEvent(const string &eve, const string &name, const string &value)
	{
		SetValue(name, value, eve);
	}
	void Style::SetEvent(const string &eve, const string &style)
	{
		SetStyle(style, eve);
	}
	string Style::Get(const string &name) const
	{
		auto &cx = z_settings.find("");
		if(cx != z_settings.end())
		{
			auto c = cx->second.find(name);
			if(c != cx->second.end())
				return c->second;
		}
		return "";
	}
	bool Style::IsStyleEvent(const string &name)
	{
		auto &cx = z_settings.find(name);
		if(cx != z_settings.end())
			return true;
		return false;
	}
};