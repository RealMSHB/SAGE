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