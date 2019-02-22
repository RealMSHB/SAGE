#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include "rapidxml.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Manager;
class AnimState;

rapidxml::xml_node<>* FindChildNode(rapidxml::xml_node<>* pNode, const char* szName);
rapidxml::xml_attribute<>* FindAttribute(rapidxml::xml_node<>* pNode, const char* szName);

// ----------------------------------------------------------------------------

class Entity
{
protected:
	int					m_nID;
public:
	Entity();

	virtual void Start(Manager *pManager, rapidxml::xml_node<>* pNode);
	virtual void Update(sf::RenderWindow* pWindow);
	friend class Manager;
};

// ----------------------------------------------------------------------------

class ImageClass : public Entity
{
	sf::Texture			m_Texture;
	sf::Sprite			m_Sprite;
	sf::Vector2f		m_vPos;
	ImageClass*				m_pImage;

public:
	ImageClass();
	ImageClass(ImageClass* pImage);

	void SetPosition(sf::Vector2f pos);

	virtual void Start(Manager *pManager, rapidxml::xml_node<>* pNode);
	virtual void Update(sf::RenderWindow* pWindow);
};


class CharacterSheet : public Entity
{
	sf::Texture			m_Texture;
	sf::IntRect         m_Rect;
	sf::Sprite			m_Sprite;
	sf::Vector2f		m_vPos;
	sf::Vector2f		m_vVelocity;
	sf::Vector2f		m_vGotoPosition;
	sf::Clock			m_Clock;
	sf::Clock			m_dTime;
	int					m_AnimStateId;
	int					m_PrevAnimStateId;
	int					m_AnimIndex;

public:
	std::vector<AnimState*> m_AnimStates;

public:
	CharacterSheet();

	void SetPosition(sf::Vector2f pos);
	void SetGoToPosition(sf::Vector2f pos);
	void SetVelocity(sf::Vector2f velocity);
	sf::Vector2f GetPosition();
	void SetState(int stateID);
	int GetState();
	sf::Vector2f Interpolate(const sf::Vector2f, const sf::Vector2f, float);

	virtual void Start(Manager *pManager, rapidxml::xml_node<>* pNode);
	virtual void Update(sf::RenderWindow* pWindow);
};

class AnimState
{
	float			m_Timing;
	int				m_ImageCount;
	sf::IntRect     m_Rect;
public:
	AnimState(float timing, int imgCount, sf::IntRect rect);

	friend class CharacterSheet;

};

class ObjectInScene
{
public:
	int m_AssetId;
	sf::Vector2f m_Position;


	ObjectInScene(int id, int x, int y);
	friend class Scene;
};

class Scene
{
	int m_Id;
	std::list<ObjectInScene*> m_ObjcetsInScene;

public:
	Scene();
	Scene(int id);


	friend class Manager;
};

