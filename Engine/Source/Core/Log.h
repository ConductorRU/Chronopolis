#pragma once
namespace DEN
{
	class Log
	{
	private:
		static Log *_this;
	public:
		Log();
		~Log();
		static Log *Get() { return _this; };
		void Error(const string &title, const string &message);
	};
}