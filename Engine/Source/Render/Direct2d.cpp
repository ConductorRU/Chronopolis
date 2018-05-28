#include "stdafx.h"
#include "Direct2d.h"
namespace DEN
{
	Render2D::Render2D()
	{
		_factory = nullptr;
	}
	Render2D::~Render2D()
	{
	
	}
	void Render2D::Initialize()
	{
		if(FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), reinterpret_cast<IUnknown**>(&_factory))))
		{
			runtime_error("Critical error: Unable to create the DirectWrite factory!");
			return;
		}
		if(FAILED(_factory->GetSystemFontCollection(&_fonts)))
		{
			runtime_error("Critical error: Unable to get System Font Collection!");
			return;
		}
		wchar_t locale[LOCALE_NAME_MAX_LENGTH];
		if(!GetUserDefaultLocaleName(locale, _countof(locale)))
		{
			runtime_error("Critical error: Default Locale Name not found!");
			return;
		}
		unsigned const count = _fonts->GetFontFamilyCount();
		for (unsigned familyIndex = 0; familyIndex != count; ++familyIndex)
		{
			IDWriteFontFamily *family = nullptr;
			if(FAILED(_fonts->GetFontFamily(familyIndex, &family)))
			{
				runtime_error("Critical error: Unable to get Font Family!");
				return;
			}
			IDWriteLocalizedStrings *names;
			if(FAILED(family->GetFamilyNames(&names)))
			{
				runtime_error("Critical error: Unable to get Family Names!");
				return;
			}
			unsigned nameIndex;
			BOOL exists;
			if(FAILED(names->FindLocaleName(locale, &nameIndex, &exists)))
			{
				runtime_error("Critical error: Unable to Find Locale Name!");
				return;
			}
			if (exists)
			{
				wchar_t name[64];
				if(FAILED(names->GetString(nameIndex, name, _countof(name))))
				{
					runtime_error("Critical error: Unable to Get Font String!");
					return;
				}
				wprintf(L"%s\n", name);
			}
		}
	}
};