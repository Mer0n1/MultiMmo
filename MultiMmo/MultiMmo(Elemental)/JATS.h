#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>
using namespace std;

class interpretation 
{
private:
	string key;
public:
	interpretation();
	interpretation(string key_);
	~interpretation();

	void toIntMassv(int* massv, int lentgh); //massv - первый элемент массива
	void toFloatMassv(float* massv, int lentgh);

	string toString(); //вернем тип string
	int toInt(); //вернем тип int
	float toFloat(); //вернем тип float
};

class JsonDocument 
{
private:
	string msg;
public:
	JsonDocument();
	JsonDocument(string d);

	interpretation value(string key);
private:
	bool bag(string key, string msg, int alk, int j);

};

/*
string d = "{\"test\":\"154.5 2 3 5 6 7 9 0\"}";
	JsonDocument doc = d;

	int* mk = doc.value("test").toIntMassv();
	string str = doc.value("test").toString();
	float* fl = doc.value("test").toFloatMassv();

	cout << "Выводим int массив:\n";
	for (int j = 0; j < 10; j++)
		cout << *(mk++) << " "; cout << endl;

	cout << "Выводим string:\n";
	for (int j = 0; j < str.size(); j++)
		cout << str[j]; cout << endl;

	cout << "Выводим float массив:\n";
	for (int j = 0; j < 10; j++)
		cout << *(fl++) << " "; cout << endl;


	interpretation iter; 
	iter.key = "1 456 3 33 5 6 7 8 9 10 11";

	int massv[10]; 
	for (int j = 0; j < 10; j++) massv[j] = 0;

	iter.toIntMassv_(&massv[0], sizeof(massv)/4); 
	for (int j = 0; j < 10; j++) cout << massv[j] << " "; cout << endl;
*/