#pragma once
enum WEEK_TYPE
{
	WEEK_MONDAY = 0,//�����������
	WEEK_TUESDAY = 1,//�������
	WEEK_WEDNESDAY = 2,//�����
	WEEK_THURSDAY = 3,//�������
	WEEK_FRIDAY = 4,//�������
	WEEK_SATURDAY = 5,//�������
	WEEK_SUNDAY = 6,//�����������
};

enum MONTH_TYPE
{
	MONTH_JANUARY = 0,//������
	MONTH_FEBRUARY = 1,//�������
	MONTH_MARCH = 2,//����
	MONTH_APRIL = 3,//������
	MONTH_MAY = 4,//���
	MONTH_JUNE = 5,//����
	MONTH_JULY = 6,//����
	MONTH_AUGUST = 7,//������
	MONTH_SEPTEMBER = 8,//��������
	MONTH_OCTOBER = 9,//�������
	MONTH_NOVEMBER = 10,//������
	MONTH_DECEMBER = 11,//�������
};

enum YEAR_TYPE
{
	YEAR_RAT = 0,//�����
	YEAR_OX = 1,//���
	YEAR_TIGER = 2,//����
	YEAR_RABBIT = 3,//������
	YEAR_DRAGON = 4,//������
	YEAR_SNAKE = 5,//����
	YEAR_HORSE = 6,//������
	YEAR_GOAT = 7,//����
	YEAR_MONKEY = 8,//��������
	YEAR_ROOSTER = 9,//�����
	YEAR_DOG = 10,//������
	YEAR_BOAR = 11,//�����
};

enum ZODIAC_TYPE
{
	ZODIAC_ARIES = 0,//����
	ZODIAC_TAURUS = 1,//�����
	ZODIAC_GEMINI = 2,//��������
	ZODIAC_CANCER = 3,//���
	ZODIAC_LEO = 4,//���
	ZODIAC_VIRGO = 5,//����
	ZODIAC_LIBRA = 6,//����
	ZODIAC_SCORPIO = 7,//��������
	ZODIAC_SAGITTARIUS = 8,//�������
	ZODIAC_CAPRICORN = 9,//�������
	ZODIAC_AQUARIUS = 10,//�������
	ZODIAC_PISCES = 11,//����
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
	void zCalcEnum();//������������ ���� ������, ������, ���. ��������
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
	bool CompareHour(const Date &date);//�������� ����� �� ���� (true - ���� ���������)
	int DifHour(const Date &date);//���������� ���������� �����
	bool IsBissextile();
	int GetMaxDay(MONTH_TYPE month, bool bis = false);//���������� ��������� ���� ������
	string GetText(bool sec = true);
};
class Clock :public Date
{
private:
	float z_fSec;
	float z_speed;//�������� � �������� (z_speed ������ �������� ������� == 1 ������� ��������� �������)
	bool z_enable;
	float z_last;//������ ���������� ����� � ������
public:
	Clock();
	inline bool IsEnable() { return z_enable; };
	inline float GetLast() { return z_last; };//������� ������ ������� � ����������� ����� (1.0f = 1 ���)
	inline void SetEnable(bool enable) { z_enable = enable; };
	void SetSpeed(float speed);
	void Update(float sync);
};