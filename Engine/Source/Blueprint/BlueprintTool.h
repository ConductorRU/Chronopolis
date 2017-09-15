#pragma once
namespace DEN
{
	class GUI;
	class BlueprintTool
	{
	private:
		GUI *_gui;
		uint _isMove;
		uint _posId;
		Point2 _pos;
	public:
		BlueprintTool(GUI *gui);
		~BlueprintTool();
		void Initialize();
	};
};