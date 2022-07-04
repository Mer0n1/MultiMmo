#include "Map.h"
#include <sstream>
#include <fstream>

#define standartSize 32 //стандартный размер тайлов - 32px

maps::maps()
{
	for (int j = 0; j < 100; j++)
		for (int i = 0; i < 100; i++)
			TileMap[j][i] = 0;
	TileMap[0][5] = 1;
	SizeTileMap = 64;

	menu0.loadFromFile("Textures/map/TestFon.jpg");
	menu.setTexture(menu0); menu.setPosition(-500, -500);

	change0.loadFromFile("Textures/Redactor/change.png");
	change.setTexture(change0);

	general[0].loadFromFile("Textures/map/JapaneseVillage.png"); //сборка уличных текстур №1
	general[1].loadFromFile("Textures/map/cafe.png"); //сборка кафе текстур №2

	general_[0].setTexture(general[0]);
}

bool maps::interactionMap(int x, int y, int Height, int Width)
{
	for (int i = y / SizeTileMap; i < (y + Height) / SizeTileMap; i++) //150 высота модельки
		for (int j = x / SizeTileMap; j < (x + Width - 10) / SizeTileMap; j++) //32 - размер в пикселях 1 блока мира
		{
			if (TileMap[i][j] == 154)
				return true;

			if (TileMap[i][j] == 150)
				return true;

		}
	return false;
}


void maps::limitMap(float& x, float& y, View &view)
{
	float tempX = x; 
	float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края

	int xt = view.getSize().x / 2;
	int yt = view.getSize().y / 2; //+height/2
	
	//Камера относительно границ мира
	if (x < xt) tempX = xt; //убираем из вида левую сторону
	if (y < yt) tempY = yt; //верхнюю сторону
	if (y > HEIGHT_MAP * SizeTileMap - yt) tempY = HEIGHT_MAP * SizeTileMap - yt;//нижнюю сторону	
	if (x > WIDTH_MAP  * SizeTileMap - xt) tempX = WIDTH_MAP * SizeTileMap - xt;	

	if (tempX != x || tempY != y)
		view.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 

	//не даем выйти за границу карты игроку
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x > WIDTH_MAP * SizeTileMap)  x = WIDTH_MAP * SizeTileMap;
	if (y > HEIGHT_MAP * SizeTileMap) y = HEIGHT_MAP * SizeTileMap;
}


void maps::TileMapEdit(string fail)
{
	string code; //code - весь текст

	fail = "maps/" + fail + "/" + fail + ".xml";

	TiXmlDocument levelFile(fail.c_str());
	if (!levelFile.LoadFile()) {
		cout << "Loading graphics for map failed." << endl; return;
	}

	TiXmlElement* sizeQ = levelFile.FirstChildElement("map");
	WIDTH_MAP = atoi(sizeQ->Attribute("width"));
	HEIGHT_MAP = atoi(sizeQ->Attribute("height"));

	//теперь сделаем фон
	TiXmlElement* dd;
	dd = sizeQ->FirstChildElement("layer");
	
	TiXmlElement* layerMain; //доступ к данным слоя
	layerMain = dd->FirstChildElement("data");

	vector<string> mainCode; //код для хранения string кодов слоев мира
	
	for (int j = 0; true; j++)
	{
		code = layerMain->GetText();
		mainCode.push_back(code);

		if (dd = dd->NextSiblingElement())
			layerMain = dd->FirstChildElement("data"); //переходим на новый слой
		else
			break;
	}

	
	for (int j = 0; j < mainCode.size(); j++)
	{
		for (int c = 0; c < mainCode[j].length(); c++)  //заменить запятые пробелом
			if (mainCode[j][c] == ',') mainCode[j][c] = ' ';
		
		for (int c = 0; c < mainCode[j].length(); c++) //подчищаем пробелы за TinyXML
			if (mainCode[j][c] == ' ' & mainCode[j][c + 1] == ' ')
				mainCode[j].erase(c, 1);

		//Переводим код string в массив
		interpretation inter(mainCode[j]);
		
		int length = mainCode[j].length() / 2;
		int* CopyInt = new int[length]; //обычный массив который мы переведем в двумерный

		inter.toIntMassv(&CopyInt[0], length * sizeof(CopyInt[0]));

		if (j == 1) //как правило основной слой находится 2 по счету
			for (int j = 0, kof = 0; j < HEIGHT_MAP; j++)
				for (int c = 0; c < WIDTH_MAP; c++, kof++)
					TileMap[j][c] = CopyInt[kof];

		if (j == 0) //фоновый слой (интерактиву сложно отслеживать все массивы, поэтому он отслеживает только основной слой)
			for (int j = 0, kof = 0; j < HEIGHT_MAP; j++)
				for (int c = 0; c < WIDTH_MAP; c++, kof++)
					TileFonMap[j][c] = CopyInt[kof];

		delete [] CopyInt;
	}
}

void maps::draw_map(RenderWindow& window)
{
	window.draw(menu);

	Sprite lux;
	lux = general_[0]; //загружаем сборку
	
	for (int i = 0; i < HEIGHT_MAP; i++) //высота
		for (int j = 0; j < WIDTH_MAP; j++) { //ширина

			//фон
			if (TileFonMap[i][j] != 0) {
				int h2 = TileFonMap[i][j] / 8;
				int w2 = TileFonMap[i][j] % 8;

				lux.setTextureRect(IntRect(w2 * standartSize - standartSize, h2 * standartSize, standartSize, standartSize));
				lux.setPosition(j * SizeTileMap, i * SizeTileMap);
				lux.setScale(SizeTileMap / standartSize, SizeTileMap / standartSize);

				window.draw(lux);
			}

			//основной слой
			if (TileMap[i][j] != 0) {
				int h1 = TileMap[i][j] / 8; //номер тайла по высоте 
				int w1 = TileMap[i][j] % 8; //номер тайла по ширине
				if (w1 == 0) { w1 = 8; h1 -= 1; } //из за бага в конце тайла
				
				lux.setTextureRect(IntRect(w1 * standartSize - standartSize, h1 * standartSize, standartSize, standartSize));
				lux.setPosition(j * SizeTileMap, i * SizeTileMap); //64 можно менять размеры
				lux.setScale(SizeTileMap / standartSize, SizeTileMap / standartSize);

				window.draw(lux);
			}
		}

	//отрисовка без использовании данных об обьектах (по-тайловая отрисовка)
	//минус -нельзя будет использовать анимации на обьектах
}
