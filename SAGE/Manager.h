#pragma once

#include "Base.h"
//#include "Level.h"

class Manager
{
public:
	std::string			m_strAssets;
	std::list<Entity*>	m_Entities;

public:
	Manager();

	Entity* GetAssetByID(int ID);
};
