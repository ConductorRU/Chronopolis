#include "DC.h"
#include "Date.h"
Date::Date()
{
	z_week = WEEK_SUNDAY;
	z_month = MONTH_MAY;
	z_cZodiac = YEAR_RABBIT;
	z_zodiac = ZODIAC_TAURUS;
	z_day = 1;
	z_year = 2011;
	z_hour = 0;
	z_minute = 0;
	z_second = 0;
}

void Date::zCalcEnum()
{
	if (z_day > 31)
	{
		if ((z_month < 7 && z_month % 2 == 0) || (z_month >= 7 && z_month % 2 == 1))
		{
			if (z_month < 11)
				z_month = (MONTH_TYPE)(z_month + 1);
			else
			{
				z_month = MONTH_JANUARY;
				z_year++;
			}
			z_day = 1;
		}
	}
	else if (z_day > 30 && z_month != 1)
	{
		if ((z_month < 7 && z_month % 2 == 1) || (z_month >= 7 && z_month % 2 == 0))
		{
			if (z_month < 11)
				z_month = (MONTH_TYPE)(z_month + 1);
			else
				z_month = MONTH_JANUARY;
			z_day = 1;
		}
	}
	else if (z_day > 28 && z_month == 1)
	{
		if ((IsBissextile() && z_day > 29) || IsBissextile() == false)
		{
			z_day = 1;
			z_month = MONTH_MARCH;
		}
	}
	switch (z_month)
	{
	case 0: if (z_day < 21)  z_zodiac = ZODIAC_CAPRICORN;   else z_zodiac = ZODIAC_AQUARIUS; break;
	case 1: if (z_day < 20)  z_zodiac = ZODIAC_AQUARIUS;    else z_zodiac = ZODIAC_PISCES; break;
	case 2: if (z_day < 21)  z_zodiac = ZODIAC_PISCES;      else z_zodiac = ZODIAC_ARIES; break;
	case 3: if (z_day < 21)  z_zodiac = ZODIAC_ARIES;       else z_zodiac = ZODIAC_TAURUS; break;
	case 4: if (z_day < 22)  z_zodiac = ZODIAC_TAURUS;      else z_zodiac = ZODIAC_GEMINI; break;
	case 5: if (z_day < 22)  z_zodiac = ZODIAC_GEMINI;      else z_zodiac = ZODIAC_CANCER; break;
	case 6: if (z_day < 23)  z_zodiac = ZODIAC_CANCER;      else z_zodiac = ZODIAC_LEO; break;
	case 7: if (z_day < 24)  z_zodiac = ZODIAC_LEO;         else z_zodiac = ZODIAC_VIRGO; break;
	case 8: if (z_day < 24)  z_zodiac = ZODIAC_VIRGO;       else z_zodiac = ZODIAC_LIBRA; break;
	case 9: if (z_day < 24)  z_zodiac = ZODIAC_LIBRA;       else z_zodiac = ZODIAC_SCORPIO; break;
	case 10: if (z_day < 23) z_zodiac = ZODIAC_SCORPIO;     else z_zodiac = ZODIAC_SAGITTARIUS; break;
	case 11: if (z_day < 22) z_zodiac = ZODIAC_SAGITTARIUS; else z_zodiac = ZODIAC_CAPRICORN; break;
	}
	int age = z_year % 12 - 4;
	if (z_month == 0 || (z_month == 1 && z_day < 4))
		age -= 1;
	if (age < 0)
		age += 12;
	z_cZodiac = (YEAR_TYPE)age;
}

void Date::SetYear(int year)
{
	z_year = year;
	zCalcEnum();
}

void Date::SetMonth(MONTH_TYPE month)
{
	z_month = month;
	zCalcEnum();
}

void Date::SetDay(int day)
{
	z_day = day;
	zCalcEnum();
}

void Date::SetData(int year, MONTH_TYPE month, int day)
{
	z_year = year;
	z_month = month;
	z_day = day;
	zCalcEnum();
}

void Date::GetData(int &year, MONTH_TYPE &month, int &day)
{
	year = z_year;
	month = z_month;
	day = z_day;
}

void Date::SetTime(int hour, int minute)
{
	if (hour < 0 || hour >= 24)
		hour = 0;
	if (minute < 0 || minute >= 60)
		minute = 0;
	z_hour = hour;
	z_minute = minute;
	z_second = 0;
}

void Date::GetTime(int &hour, int &minute)
{
	hour = z_hour;
	minute = z_minute;
}

bool Date::CompareHour(const Date &date)
{
	if (z_hour != date.z_hour || z_day != date.z_day || z_month != date.z_month || z_year != date.z_year)
		return false;
	return true;
}

bool Date::operator < (const Date& date) const
{
	if (z_year < date.z_year)
		return true;
	if (z_year == date.z_year)
	{
		if (z_month < date.z_month)
			return true;
		if (z_month == date.z_month)
		{
			if (z_day < date.z_day)
				return true;
			if (z_day == date.z_day)
			{
				if (z_hour < date.z_hour)
					return true;
			}
		}
	}
	return false;
}

int Date::DifHour(const Date &date)
{
	int count = 0;
	Date dif;
	dif.z_year = z_year - date.z_year;
	dif.z_second = z_month - date.z_month;
	dif.z_day = z_day - date.z_day;
	dif.z_hour = z_hour - date.z_hour;
	if (dif.z_year < 0)
	{
		dif.z_year = -dif.z_year;
		dif.z_second = -dif.z_second;
		dif.z_day = -dif.z_day;
		dif.z_hour = -dif.z_hour;
	}
	else if (dif.z_second < 0)//в second хран€тс€ мес€ца
	{
		dif.z_second = -dif.z_second;
		dif.z_day = -dif.z_day;
		dif.z_hour = -dif.z_hour;
	}
	else if (dif.z_day < 0)
	{
		dif.z_day = -dif.z_day;
		dif.z_hour = -dif.z_hour;
	}
	else if (dif.z_hour < 0)
		dif.z_hour = -dif.z_hour;
	if (dif.z_hour < 0)
	{
		dif.z_hour = 24 + dif.z_hour;
		dif.z_day -= 1;
	}
	if (dif.z_day < 0)
	{
		dif.z_day = 30 + dif.z_day;
		dif.z_second -= 1;
	}
	if (dif.z_second < 0)
	{
		dif.z_second = 12 + dif.z_second;
		dif.z_year -= 1;
	}
	count = dif.z_year * 365 * 24 + dif.z_second * 30 + dif.z_day * 24 + dif.z_hour;
	return count;
}

bool Date::IsBissextile()
{

	if (z_year % 4 == 0)
		return true;
	return false;
}

string Date::GetText(bool sec)
{
	string txt;
	if (z_hour < 10)
		txt += "0";
	txt += to_string(z_hour);
	txt += ":";
	if (z_minute < 10)
		txt += "0";
	txt += to_string(z_minute);
	if (sec)
	{
		txt += ":";
		if (z_second < 10)
			txt += "0";
		txt += to_string(z_second);
	}
	return txt;
}

int Date::GetMaxDay(MONTH_TYPE month, bool bis)
{
	if (month == MONTH_FEBRUARY)
	{
		if (bis)
			return 29;
		return 28;
	}
	if ((month < 7 && month % 2 == 0) || (month >= 7 && month % 2 == 1))
		return 31;
	return 30;
}

Clock::Clock()
{
	z_speed = 1.0f;
	z_enable = true;
	z_last = 0.0f;
	z_fSec = 0.0f;
}

void Clock::SetSpeed(float speed)
{
	z_speed = speed;
}

void Clock::Update(float sync)
{
	z_last = 0.0f;
	if (z_enable == false)
		return;
	if (sync > 1000000.0f)
		sync = 1000000.0f;
	if (sync < 0.0f)
		sync = 0.0f;
	z_fSec += z_speed*sync;
	int oldTime = z_second;
	if (z_fSec < 1.0f)
		return;
	z_second += (int)z_fSec;
	int time;
	time = z_second - oldTime;
	z_fSec -= (float)time;
	z_last += (float)time / 60.0f;
	if (z_second >= 60)
	{
		time = z_second / 60;
		z_second = z_second - time * 60;
		z_minute += time;
	}
	if (z_minute >= 60)
	{
		time = z_minute / 60;
		z_minute = z_minute - time * 60;
		z_hour += time;
	}
	if (z_hour >= 24)
	{
		time = z_hour / 24;
		z_hour = z_hour - time * 24;
		z_day += time;
		SetData(z_year, z_month, z_day);//если день вышел за пределы мес€ца, функци€ zCalcEnum() это исправит
	}
}