#pragma once

#include "Base.h"
#include "rapidxml.hpp"
//#include "Level.h"

class Manager
{
public:
	std::string			m_strAssets;
	std::list<Entity*>	m_Entities;
	std::list<Scene*>   m_Scenes;
	Scene				m_CurrentScene;
	sf::RenderWindow*	m_Window;

public:
	Manager();
	void LoadSceneData(rapidxml::xml_node<>* pNode);
	void SetCurrentScene(int id);
	void LoadCurrentScene();
	void SetWindow(sf::RenderWindow *window);

	Entity* GetAssetByID(int ID);
};
