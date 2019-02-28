#pragma once

#include "Base.h"
#include "rapidxml.hpp"

class Manager
{
public:
	std::string			m_strAssets;
	std::list<Entity*>	m_Entities;
	std::list<Scene*>   m_Scenes;
	sf::RenderWindow*	m_Window;
	Scene*				m_CurrentScene;
	int					m_CursorState;
	Inventory			m_Inventory;

public:
	Manager();
	void LoadSceneData(rapidxml::xml_node<>* pNode);
	void SetCurrentScene(int id);
	void LoadCurrentScene();
	void SetWindow(sf::RenderWindow *window);
	void UpdateCursor();
	void UpdateInventory();

	Entity* GetAssetByID(int ID);
};


