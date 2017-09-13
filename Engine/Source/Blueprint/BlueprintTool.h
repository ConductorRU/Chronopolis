#pragma once
namespace DEN
{
	class GUI;
	class BlueprintTool
	{
	private:
		GUI *_gui;
	public:
		BlueprintTool(GUI *gui);
		~BlueprintTool();
		void Initialize();
	};
};