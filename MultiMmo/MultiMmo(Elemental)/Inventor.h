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
    ��������� ������ ���������. 
    */
private:
    struct Element
    {
        //*������*
        int thingsId;
        int quanityThings;
    }; 
    vector<Element*> element; //�������
    vector<int> character; //��������
    vector<int> SA; //���� �����������
public:
    Inventor();
    void udpate(); //�������� ��������
    void addThings(int id, int quanity); //�������� �������

    void saveFile(); //��������� � ���� (���� �������� ���������� �� ��������)
    void DownloadResources(); //��������� ������� ������ �������� � ���������
};