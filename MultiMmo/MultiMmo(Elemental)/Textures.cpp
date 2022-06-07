#include "Textures.h"


TexturesFile::TexturesFile()
{
	textureProfileCharacter[7].loadFromFile("Textures/profile/character/SeniusR.png");
	textureProfileCharacter[99].loadFromFile("Textures/profile/character/Developer.png");

	for (int j = 0; j < 200; j++) {
		ProfileCharacter[j].setTexture(textureProfileCharacter[j]);
		ProfileCharacter[j].setScale(0.2, 0.2);
		ProfileCharacter[j].setPosition(130, 110);
	}

	//////////////////////////
	textureModelCharacter[96].loadFromFile("Textures/models/Chara.png");
	textureModelCharacter[97].loadFromFile("Textures/models/Papyrus.png");
	textureModelCharacter[98].loadFromFile("Textures/models/Sans.png");
	textureModelCharacter[99].loadFromFile("Textures/models/Developer.png");
	//персонажи до 100 - игровые персонажи

	//мобы
	textureModelCharacter[101].loadFromFile("Textures/models/mobs/redis1.png");
	textureModelCharacter[102].loadFromFile("Textures/models/mobs/ananas.png");


	for (int j = 0; j < 200; j++) {
		ModelCharacter[j].setTexture(textureModelCharacter[j]);
		ModelCharacter[j].setTextureRect(IntRect(178, 0, 86, 133));
	}


	///-----------------------attacks
	textureAttack[1].loadFromFile("Textures/models/attack/ray.png");

	for (int j = 0; j < 100; j++)
		attack[j].setTexture(textureAttack[j]);
}