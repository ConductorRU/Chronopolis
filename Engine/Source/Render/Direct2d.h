#pragma once
namespace DEN
{
	class Render2D
	{
	private:
		IDWriteFactory2 *_factory;
		IDWriteFontCollection *_fonts;
	public:
		Render2D();
		~Render2D();
		void Initialize();
	};
}