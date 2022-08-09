#include "Engine.h"
#include "windows.h" 


Engine::Engine()
{
	Entity::setView(&view); 
	Entity::setVector2i(&pos); 
	Entity::setMap(&Map);
	Entity::setTime(&Timer, &TimeOptimization);
	Entity::setGameWorld(&world); 
	attack::setAttribute(&world, &view); //инициализируем метод атаки
	Client::InisializationWorld(&world);
	
	pr = new Player;
	Interface = new RpgInterface(pr->getHealthBar(), pr->getModuleRA(), &view);
	group = &GroupSystem::getObject();
	animation = &AnimationAttackModule::getObject();

	Timer = 0;
	view.reset(FloatRect(0, 0, 800, 600));
	view.zoom(1.3);
}

Engine::~Engine()
{
	delete pr;
	delete Interface; 
	group->DeleteGroup(); //удаление одиночного класса
}

void Engine::start(RenderWindow& window)
{
	pr->inisialization(); //переинициализация характеристик 
	AnimationAttackModule::setWindow(window);
	//delete pr; //using this client as a module
	while (true)
	{
		window.setView(view);
		pixelPos = Mouse::getPosition(window); //забираем координаты мыши
		pos = window.mapPixelToCoords(pixelPos); //переводим их в игровые
		Timer = clock.getElapsedTime().asSeconds();
		
		TimeOptimization = OneFrame.getElapsedTime().asMicroseconds();
		OneFrame.restart();
		TimeOptimization /= 10000;
		
		Event events;
		while (window.pollEvent(events))
		{
			if (events.type == Event::Closed)
				window.close();
			else if (events.type == Event::MouseWheelMoved);
		}
		
		changeview(); 
		window.clear();
		window.setView(view);
		
		Map.draw_map(window); //вывод карты
		world.update(window); //обновление всех игроков
		animation->draw(window); //вывод анимации
		
		if (pr->getLife()) //временное условие пока не исправим неправильную архитектуру
			Interface->Interface(window); 
		
		window.display();
	}
}

void Engine::changeview()
{
	if (view.getSize().x < 3000 && view.getSize().y < 3000) //допустимые границы
		if (Keyboard::isKeyPressed(Keyboard::U))
			view.zoom(1.0050f); //масштабируем, уменьшение

	if (view.getSize().x > 800 && view.getSize().y > 600) //допустимые границы
		if (Keyboard::isKeyPressed(Keyboard::I))
			view.zoom(0.9950f); //масштабируем, увеличение
}

void Engine::LoadMap(string name, bool load)
{
	Map.TileMapEdit(name); //загружаем текстуру карты
	if (load) world.DownloadWorld(name); //теперь загружаем информацию и персонажах карты
	group->LoadMap(&world, name); //и информацию и группах
}
