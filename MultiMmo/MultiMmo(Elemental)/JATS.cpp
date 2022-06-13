#include "JATS.h"

JsonDocument::JsonDocument()
{
	
}

JsonDocument::JsonDocument(string d)
{
	for (int j = 0; j < d.length(); j++) {
		string d_; d_ = d[j];
		msg.append(d_.c_str());
		if (msg[j] == '}') break; //доходим до конца ключ-запроса 
	}
}

interpretation JsonDocument::value(string key)
{
	int len = key.length();
	string itog;

	for (int j = 1; j < msg.length(); j++)
		if (msg[j] == key[len - 1] && msg[j - 1] == key[len - 2] && bag(key, msg, len, j))
		{
			for (int c = 0; c < msg.length(); c++)
				if (msg[j + 4 + c] == '"') {
					for (int i = j + 4; i < c + j + 4; i++)
						itog += msg[i];
					break;
				}
			break;
		}

	interpretation inc(itog);
	return inc;
} 


bool JsonDocument::bag(string key, string msg, int alk, int j)
{ //метод, нужный для дополнительных расчетов 
	if (key.length() == 2) { return true; } //если аргумент равен 2, то пропускаем
	if (key.length() > 2 && msg[j - 2] == key[alk - 3]) { return true; } //если нет, то сортируем еще
	return false;                                                       //для тщательного поиска
}


//----------------------------------------------interpretation
interpretation::interpretation()
{

}
interpretation::interpretation(string key_)
{
	key = key_;
}
interpretation::~interpretation()
{

}

void interpretation::toIntMassv(int* massv, int length_)
{
	string data; //временный результат
	vector<int> vec(1); //изначально переводим в вектор (обяз.инициализация)

	if (key[key.length() - 1] != ' ') 
		key += ' '; //добавляем пробел на всякий
	int length = key.length() + 1; //узнаем размер

	for (int j = 0, n = 0; j < length; j++)
	{
		if (key[j] != ' ') {
			data += key[j];
			continue;
		}
		
		vec[n] = atoi(data.c_str()); //переводим число в int
		data.clear();
		n++; //кол во чисел (или номер текущего числа)
		vec.push_back(0); //новое число
	}
	
	//переведем в массив
	for (int j = 0; j < vec.size() && j < length_ / sizeof(int); j++) { //length учитывает макс.кол во индексов массива
		*massv = vec[j];
		massv++;
	}
}

void interpretation::toFloatMassv(float* massv, int length_)
{
	string data; //будет хранить число и очищаться
	vector<float> vec(1); //изначально переводим в вектор (обяз.инициализация)

	if (key[key.length() - 1] != ' ') 
		key += " "; //добавляем пробел
	int length = key.length();
	
	for (int j = 0; j < length; j++) //переделаем все точки в запятые ради stof
		if (key[j] == '.') key[j] = ',';

	for (int j = 0, n = 0; j < length; j++)
	{
		if (key[j] != ' ') {
			data += key[j]; 
			continue;
		}
		
		vec[n] = stof(data); //переводим число в float
		data.clear(); //очищаем
		n++; //кол во чисел (или номер текущего числа)
		vec.push_back(0); //новое число
	}
	
	//переведем в массив
	for (int j = 0; j < vec.size() && j < length_ / sizeof(int); j++) { //length учитывает макс.кол во индексов массива
		*massv = vec[j];
		massv++; 
	}

}

string interpretation::toString()
{
	return key;
}

int interpretation::toInt()
{
	string nool;
	for (int j = 0; j < key.length(); j++)
		if (key[j] == ' ') key.erase(j);

	return  (key != nool) ? atoi(key.c_str()) : 0;
}

float interpretation::toFloat()
{
	string nool; 

	for (int j = 0; j < key.length(); j++) //переделаем все точки в запятые ради stof
		if (key[j] == '.') key[j] = ',';

	for (int j = 0; j < key.length(); j++)
		if (key[j] == ' ') key.erase(j);
	
	return (key != nool) ? stof(key) : 0;
} 


//попробовать сделать разделитель запросов
//у нас стоит обязательный выбор перевода в тип, нам нужно 
//чтобы если мы не указывали тип он автоматически возвращал string
