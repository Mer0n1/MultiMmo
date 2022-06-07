#include "Inventor.h"

Inventor::Inventor()
{
    DownloadResources();
}

void Inventor::udpate()
{
    for (int j = 0; j < element.size(); j++)
        if (element[j]->quanityThings <= 0) //���� ����������� �������� ����� 0 ������� ���� �������
            element.erase(element.begin() + j);
}

void Inventor::addThings(int id, int quanity)
{
    for (int j = 0; j < element.size(); j++)
        if (element[j]->thingsId == id) { //���� ������ � ����� ���� ���� ������ ���������� ��� ��
            element[j]->quanityThings += quanity;
            return; //� ������� �� ������
        }

    element.push_back(new Element); //���� ������ �������� ��� ������� ���
    element.back()->thingsId = id;
    element.back()->quanityThings = quanity;
}

void Inventor::saveFile()
{

}

void Inventor::DownloadResources()
{
    TiXmlDocument object("Save/account.xml");
    object.LoadFile();

    TiXmlElement* sizeQ = object.FirstChildElement("account")->FirstChildElement("inventor");
    TiXmlElement* CharacterElement;
    CharacterElement = object.FirstChildElement("account")->FirstChildElement("character");

    for (int j = 0; j < 100; j++) {
        character.push_back(atoi(CharacterElement->Attribute("id")));

        if (!(CharacterElement = CharacterElement->NextSiblingElement("character")))
            break; //���� ��������� ������ ��� �������
    }

    //��������� SA
    sizeQ = object.FirstChildElement("account")->FirstChildElement("inventor");

    TiXmlElement* SAElement; //������ ������
    SAElement = sizeQ->FirstChildElement("SA");

    for (int j = 0; j < 50; j++) {
        SA.push_back(atoi(SAElement->Attribute("id")));
        
        if (!(SAElement = SAElement->NextSiblingElement("SA")))
            break; //���� ��������� ������ ��� �������
    }

    ///---------------------
    /*TiXmlElement* inventor;
    inventor = object.FirstChildElement("account")->FirstChildElement("inventor")->FirstChildElement("things");

    TiXmlElement* inventorId;
    inventorId = inventor->FirstChildElement("id");
    string invId = inventorId->Attribute("Value");

    inventorId = inventor->FirstChildElement("quanity");
    string invQuanity = inventorId->Attribute("Value"); */
}