#pragma once
namespace DEN
{
	class GUI;
	class Blueprint;
	class BlueprintTool
	{
	private:
		GUI *_gui;
		Blueprint *_blue;
		uint _isMove;
		uint _posId;
		Point2 _pos;
	public:
		BlueprintTool(GUI *gui);
		~BlueprintTool();
		void Initialize();
		Blueprint *CreateBlueprint();
	};
};