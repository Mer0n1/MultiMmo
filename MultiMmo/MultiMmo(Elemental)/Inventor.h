#pragma once
#include "TinyXML/tinyxml.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Inventor
{
    /*
    Наработки класса инвентаря. 
    */
private:
    struct Element
    {
        //*данные*
        int thingsId;
        int quanityThings;
    }; 
    vector<Element*> element; //предмет
    vector<int> character; //персонаж
    vector<int> SA; //спец способности
public:
    Inventor();
    void udpate(); //обновить предметы
    void addThings(int id, int quanity); //добавить предмет

    void saveFile(); //сохранить в файл (файл хранящий информацию об аккаунте)
    void DownloadResources(); //загружаем ресурсы нашего аккаунта в инвентарь
};