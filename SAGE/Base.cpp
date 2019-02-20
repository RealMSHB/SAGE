#include "Manager.h"
#include "Base.h"
#include "WordDelimitedByCharacter.h"

using namespace std;
using namespace sf;
using namespace rapidxml;

// ------------------------------------------------

xml_node<>* FindChildNode(xml_node<>* pNode, const char* szName)
{
	for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
		if (strcmp(pChild->name(), szName) == 0) return pChild;
	}
	return NULL;
}

xml_attribute<>* FindAttribute(xml_node<>* pNode, const char* szName)
{
	for (xml_attribute<>* pAttr = pNode->first_attribute(); pAttr != NULL; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), szName) == 0) return pAttr;
	}
	return NULL;
}

// ------------------------------------------------

Entity::Entity()
{
}

void Entity::Start(Manager *pManager, xml_node<>* pNode)
{
}

void Entity::Update(RenderWindow* pWindow)
{
}

// ------------------------------------------------

ImageClass::ImageClass()
{
	m_nID = 0;
	m_pImage = NULL;
}

ImageClass::ImageClass(ImageClass* pImage)
{
	m_pImage = pImage;
}

void ImageClass::Start(Manager *pManager, xml_node<>* pNode)
{
	xml_attribute<>*	pAttr = FindAttribute(pNode, "id");
	if (pAttr != NULL) m_nID = atoi(pAttr->value());

	pAttr = FindAttribute(pNode, "x");
	if (pAttr != NULL) m_vPos.x = atof(pAttr->value());
	pAttr = FindAttribute(pNode, "y");
	if (pAttr != NULL) m_vPos.y = atof(pAttr->value());

	string  file = pManager->m_strAssets + pNode->value();
	if (m_Texture.loadFromFile(file)) {
		m_Sprite.setTexture(m_Texture);
	}


}

void ImageClass::SetPosition(sf::Vector2f pos)
{
	m_vPos = pos;
}

void ImageClass::Update(RenderWindow* pWindow)
{
	m_Sprite.setPosition(m_vPos);
	pWindow->draw(m_Sprite);
}

// ------------------------------------------------

CharacterSheet::CharacterSheet()
{
	m_nID = 0;
	m_AnimStateId = 0;
	m_Rect = IntRect(0, 0, 0, 0);
	m_AnimIndex = 1;
}


void CharacterSheet::Start(Manager *pManager, xml_node<>* pNode)
{
	xml_attribute<>*	pAttr = FindAttribute(pNode, "id");
	if (pAttr != NULL) m_nID = atoi(pAttr->value());

	pAttr = FindAttribute(pNode, "width");
	if (pAttr != NULL) m_Rect.width = atof(pAttr->value());
	pAttr = FindAttribute(pNode, "height");
	if (pAttr != NULL) m_Rect.height = atof(pAttr->value());

	pAttr = FindAttribute(pNode, "src");
	if (pAttr != NULL)
	{
		string  file = pManager->m_strAssets + pAttr->value();
		if (m_Texture.loadFromFile(file)) {
			m_Sprite.setTexture(m_Texture);
		}

		if (pNode != NULL) {
			for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
				if (strcmp(pChild->name(), "state") == 0) {
					float	timing = 0;
					int		imgCount = 0;
					pAttr = FindAttribute(pChild, "timing");
					if (pAttr != NULL)
						timing = atof(pAttr->value());
					pAttr = FindAttribute(pChild, "imgCount");
					if (pAttr != NULL)
						imgCount = atoi(pAttr->value());
					IntRect tempRect;
					vector<string> tempVecStr = WordDelimitedByCharacter::split(pChild->value(), ",");
					tempRect = IntRect(stoi(tempVecStr[0]), stoi(tempVecStr[1]), stoi(tempVecStr[2]), stoi(tempVecStr[3]));
					AnimState* animState = new AnimState(timing, imgCount, tempRect);
					m_AnimStates.push_back(animState);
				}
			}
		}
	}
	else
	{
		cout << "Please specify the source of the character sheet" << endl;
	}
}

void CharacterSheet::SetPosition(sf::Vector2f pos)
{
	m_vPos = pos;
}

void CharacterSheet::SetGoToPosition(sf::Vector2f pos)
{
	m_vGotoPosition = pos;
}


void CharacterSheet::SetVelocity(sf::Vector2f velocity)
{
	m_vVelocity = velocity;
}

sf::Vector2f CharacterSheet::GetPosition()
{
	return m_vPos;
}

void CharacterSheet::Update(RenderWindow* pWindow)
{
	m_Sprite.setPosition(Interpolate(m_Sprite.getPosition(), m_vGotoPosition,0.05f));
	//m_Sprite.move(m_vVelocity);

	if (m_Clock.getElapsedTime().asSeconds() > m_AnimStates[m_AnimStateId]->m_Timing)
	{
		if (m_AnimStateId != 4)
		{
			m_Sprite.setTextureRect(IntRect(
				m_AnimIndex * m_AnimStates[m_AnimStateId]->m_Rect.width + m_AnimStates[m_AnimStateId]->m_Rect.left,
				m_AnimStates[m_AnimStateId]->m_Rect.top,
				m_AnimStates[m_AnimStateId]->m_Rect.width,
				m_AnimStates[m_AnimStateId]->m_Rect.height));

			m_PrevAnimStateId = m_AnimStateId;
		}
		else
		{
			m_Sprite.setTextureRect(IntRect(
				m_AnimIndex * m_AnimStates[m_AnimStateId]->m_Rect.width + m_AnimStates[m_AnimStateId]->m_Rect.left,
				m_PrevAnimStateId * m_Rect.height,
				m_AnimStates[m_AnimStateId]->m_Rect.width,
				m_AnimStates[m_AnimStateId]->m_Rect.height));
		}

		m_AnimIndex++;
		if (m_AnimIndex == m_AnimStates[m_AnimStateId]->m_ImageCount)
			m_AnimIndex = 0;

		m_Clock.restart();
	}
	pWindow->draw(m_Sprite);
}

void CharacterSheet::SetState(int stateID)
{
	m_AnimStateId = stateID;
	m_AnimIndex = 0;
	m_Clock.restart();
}

int CharacterSheet::GetState()
{
	return m_AnimStateId;
}

sf::Vector2f CharacterSheet::Interpolate(const sf::Vector2f pointA, const sf::Vector2f pointB, float factor)
{
	if (factor > 1.f)
		factor = 1.f;

	else if (factor < 0.f)
		factor = 0.f;

	return pointA + (pointB - pointA) * factor;
}
// -------------------------------------------------
AnimState::AnimState(float timing, int imgCount, IntRect rect)
{
	m_Timing = timing;
	m_ImageCount = imgCount;
	m_Rect = rect;
}