#include "Engine.h"
#include "windows.h" 


Engine::Engine()
{
	Entity::setView(&view); 
	Entity::setVector2i(&pos); 
	Entity::setMap(&Map);
	Entity::setTime(&Timer);
	Entity::setGameWorld(&world); 
	AttackSystem::setAnimationSystem(&animation);
	attack::setAttribute(&world, &view); //�������������� ����� �����
	Client::InisializationWorld(&world);

	pr = new Player;
	Interface = new RpgInterface(&pr->hb, pr->getModuleRA(), &view);

	Timer = 0;
	view.reset(FloatRect(0, 0, 800, 600)); 
}

Engine::~Engine()
{
	delete pr;
	delete Interface;
}

void Engine::start(RenderWindow& window)
{
	pr->inisialization(); //����������������� ������������� 
	AnimationAttackModule::setWindow(window);

	while (true)
	{
		pixelPos = Mouse::getPosition(window); //�������� ���������� ����
		pos = window.mapPixelToCoords(pixelPos); //��������� �� � �������
		Timer = clock.getElapsedTime().asSeconds();

		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			else if (events.type == Event::MouseWheelMoved);
		}
		
		window.setView(view);
		changeview(); 
		window.clear();
		
		Map.draw_map(window); //����� �����
		world.update(window); //���������� ���� �������
		animation.draw(window); //����� ��������

		Interface->setPosition(view.getCenter().x, view.getCenter().y); //����� ���������� ����������
		Interface->Interface(window);

		window.display();
	}
}

void Engine::changeview()
{
	if (view.getSize().x < 3000 && view.getSize().y < 3000) //���������� �������
		if (Keyboard::isKeyPressed(Keyboard::U))
			view.zoom(1.0050f); //������������, ����������

	if (view.getSize().x > 800 && view.getSize().y > 600) //���������� �������
		if (Keyboard::isKeyPressed(Keyboard::I))
			view.zoom(0.9950f); //������������, ����������
}

void Engine::LoadMap(string name)
{
	Map.TileMapEdit(name); //��������� �������� �����
	world.DownloadWorld(name); //������ ��������� ���������� � ���������� �����
	group.LoadMap(&world, name); //� ���������� � �������
}

//�������� � readme ��� ������� �� ���������� ������-������, ��� ��������� ���������� ������� ��� ���������� �� ������ ����� ���� ���������� x � y
//��������� ��� � limitmap
//��������� ��� � ��������
//��������� ��� � 2 Vide