#include "Manager.h"


using namespace std;
using namespace sf;
using namespace rapidxml;

// ---------------------------------------------------------------

Manager::Manager()
{
}

Entity* Manager::GetAssetByID(int ID)
{
	list<Entity*>::iterator i;
	for (i = m_Entities.begin(); i != m_Entities.end(); ++i)
	{
		if ((*i)->m_nID == ID)
			return (*i);
	}
	return NULL;
}

void Manager::LoadSceneData(rapidxml::xml_node<>* pNode)
{
	for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
		if (strcmp(pChild->name(), "scene") == 0) {
			Scene *scene = new Scene(atoi(FindAttribute(pChild, "id")->value()));
			for (xml_node<>* pSceneChild = pChild->first_node(); pSceneChild != NULL; pSceneChild = pSceneChild->next_sibling()) {
				if (strcmp(pSceneChild->name(), "object") == 0) {
					ObjectInScene *objectInScene = new ObjectInScene(
						atoi(FindAttribute(pSceneChild, "id")->value()),
						atoi(FindAttribute(pSceneChild, "x")->value()),
						atoi(FindAttribute(pSceneChild, "y")->value()));

					scene->m_ObjcetsInScene.push_back(objectInScene);
				}
			}
			m_Scenes.push_back(scene);
		}
	}
}

void Manager::SetCurrentScene(int id)
{
	list<Scene*>::iterator i;
	for (i = m_Scenes.begin(); i != m_Scenes.end(); ++i)
	{
		if ((*i)->m_Id == id)
		{
			m_CurrentScene = (*i);
			return;
		}
	}
	m_CurrentScene = (*m_Scenes.begin());
}

void Manager::LoadCurrentScene()
{
	list<ObjectInScene*>::iterator i;
	for (i = (*m_CurrentScene).m_ObjcetsInScene.begin(); i != (*m_CurrentScene).m_ObjcetsInScene.end(); ++i)
	{

		ImageClass* pItem = (ImageClass*)GetAssetByID((*i)->m_AssetId);
		if (pItem != NULL)
		{
			pItem->SetPosition(Vector2f((*i)->m_Position.x, (*i)->m_Position.y));
			pItem->Update(m_Window);
		}
	}

}

void Manager::SetWindow(sf::RenderWindow *window)
{
	m_Window = window;
}

// update cursor image according to its postion on the screen
void Manager::UpdateCursor()
{
	ImageClass* pItem;
	switch (m_CursorState)
	{
	case 0: // lock cursor
		pItem = (ImageClass*)GetAssetByID(9);
		if (pItem != NULL)
		{
			pItem->SetPosition(Vector2f(Mouse::getPosition(*m_Window)));
			pItem->Update(m_Window);
		}
		break;
	case 1: // normal cursor
		pItem = (ImageClass*)GetAssetByID(6);
		if (pItem != NULL)
		{
			pItem->SetPosition(Vector2f(Mouse::getPosition(*m_Window)));
			pItem->Update(m_Window);
		}
		break;
	case 2: // next scene cursor
		pItem = (ImageClass*)GetAssetByID(7);
		if (pItem != NULL)
		{
			pItem->SetPosition(Vector2f(Mouse::getPosition(*m_Window)));
			pItem->Update(m_Window);
		}
		break;
	case 3: // previous scene cursor
		pItem = (ImageClass*)GetAssetByID(8);
		if (pItem != NULL)
		{
			pItem->SetPosition(Vector2f(Mouse::getPosition(*m_Window)));
			pItem->Update(m_Window);
		}
		break;
	default:
		break;
	}
}

void Manager::UpdateInventory()
{
	if (!m_Inventory.m_ItemsInInventory.empty())
	{
		int counter = 1;
		list<int>::iterator i;
		for (i = m_Inventory.m_ItemsInInventory.begin(); i != m_Inventory.m_ItemsInInventory.end(); ++i)
		{
			ImageClass* pItem = (ImageClass*)GetAssetByID(*i);
			if (pItem != NULL)
			{
				pItem->SetPosition(Vector2f((counter * 20), 10));
				pItem->Update(m_Window);
			}
			counter++;
		}
	}
}

// ---------------------------------------------------------------

