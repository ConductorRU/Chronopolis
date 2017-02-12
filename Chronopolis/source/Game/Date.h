#pragma once
enum WEEK_TYPE
{
	WEEK_MONDAY = 0,//понедельник
	WEEK_TUESDAY = 1,//вторник
	WEEK_WEDNESDAY = 2,//среда
	WEEK_THURSDAY = 3,//четверг
	WEEK_FRIDAY = 4,//пятница
	WEEK_SATURDAY = 5,//суббота
	WEEK_SUNDAY = 6,//воскресенье
};

enum MONTH_TYPE
{
	MONTH_JANUARY = 0,//январь
	MONTH_FEBRUARY = 1,//февраль
	MONTH_MARCH = 2,//март
	MONTH_APRIL = 3,//апрель
	MONTH_MAY = 4,//май
	MONTH_JUNE = 5,//июнь
	MONTH_JULY = 6,//июль
	MONTH_AUGUST = 7,//август
	MONTH_SEPTEMBER = 8,//сентябрь
	MONTH_OCTOBER = 9,//октябрь
	MONTH_NOVEMBER = 10,//ноябрь
	MONTH_DECEMBER = 11,//декабрь
};

enum YEAR_TYPE
{
	YEAR_RAT = 0,//крыса
	YEAR_OX = 1,//бык
	YEAR_TIGER = 2,//тигр
	YEAR_RABBIT = 3,//кролик
	YEAR_DRAGON = 4,//дракон
	YEAR_SNAKE = 5,//змея
	YEAR_HORSE = 6,//лошадь
	YEAR_GOAT = 7,//коза
	YEAR_MONKEY = 8,//обезьяна
	YEAR_ROOSTER = 9,//петух
	YEAR_DOG = 10,//собака
	YEAR_BOAR = 11,//кабан
};

enum ZODIAC_TYPE
{
	ZODIAC_ARIES = 0,//овен
	ZODIAC_TAURUS = 1,//телец
	ZODIAC_GEMINI = 2,//близнецы
	ZODIAC_CANCER = 3,//рак
	ZODIAC_LEO = 4,//лев
	ZODIAC_VIRGO = 5,//дева
	ZODIAC_LIBRA = 6,//весы
	ZODIAC_SCORPIO = 7,//скорпион
	ZODIAC_SAGITTARIUS = 8,//стрелец
	ZODIAC_CAPRICORN = 9,//козерог
	ZODIAC_AQUARIUS = 10,//водолей
	ZODIAC_PISCES = 11,//рыбы
};
class Date
{
protected:
	WEEK_TYPE z_week;
	MONTH_TYPE z_month;
	YEAR_TYPE z_cZodiac;
	ZODIAC_TYPE z_zodiac;
	int z_day;
	int z_year;
	int z_hour;
	int z_minute;
	int z_second;
	void zCalcEnum();//просчитывает день недели, зодиак, кит. гороскоп
public:
	Date();
	inline WEEK_TYPE GetWeek() { return z_week; };
	inline MONTH_TYPE GetMonth() { return z_month; };
	inline ZODIAC_TYPE GetZodiac() { return z_zodiac; };
	inline YEAR_TYPE GetChZodiac() { return z_cZodiac; };
	inline int GetDay() { return z_day; };
	inline int GetYear() { return z_year; };
	inline int GetSecond() { return z_second; };
	bool operator < (const Date& date) const;
	void SetYear(int year);
	void SetMonth(MONTH_TYPE month);
	void SetDay(int day);
	void SetData(int year, MONTH_TYPE month, int day);
	void GetData(int &year, MONTH_TYPE &month, int &day);
	void SetTime(int hour, int minute);
	void GetTime(int &hour, int &minute);
	bool CompareHour(const Date &date);//сравнить время до часа (true - если совпадает)
	int DifHour(const Date &date);//количество пройденных часов
	bool IsBissextile();
	int GetMaxDay(MONTH_TYPE month, bool bis = false);//возвращает последний день месяца
	string GetText(bool sec = true);
};
class Clock :public Date
{
private:
	float z_fSec;
	float z_speed;//скорость в секундах (z_speed секунд игрового времени == 1 секунда реального времени)
	bool z_enable;
	float z_last;//точное количество минут и секунд
public:
	Clock();
	inline bool IsEnable() { return z_enable; };
	inline float GetLast() { return z_last; };//сколько прошло времени с предыдущего кадра (1.0f = 1 мин)
	inline void SetEnable(bool enable) { z_enable = enable; };
	void SetSpeed(float speed);
	void Update(float sync);
};