#include "Manager.h"

using namespace std;
using namespace sf;
using namespace rapidxml;

Manager				manager;
CharacterSheet*		pCharacterSheet;

//Vector2f Interpolate(const Vector2f&, const Vector2f&, float);
float factor = 0.0f;
float characterMoveSpeed = 0.1f;

float deltatime = 0.0f;
Clock dt;

float speed = 250;

int main()
{
	RenderWindow window(VideoMode(608, 352), "My Window");

	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	ifstream MyFile("data.xml", ios::binary);
	vector<char> buffer((istreambuf_iterator<char>(MyFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	MyFile.close();

	xml_document<>		Doc;
	Doc.parse<0>(&buffer[0]);

	xml_node<>*	pRootNode = Doc.first_node();

	xml_node<>*				pNode;
	xml_node<>*				pChild;
	xml_attribute<>*		pAttr;

	pNode = FindChildNode(pRootNode, "assets");
	if (pNode != NULL) {
		pAttr = FindAttribute(pNode, "path");
		if (pAttr != NULL) manager.m_strAssets = pAttr->value();

		for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
			if (strcmp(pChild->name(), "img") == 0) {
				ImageClass *pImage = new ImageClass();
				pImage->Start(&manager, pChild);
				manager.m_Entities.push_back(pImage);
			}
		}
	}


	pNode = FindChildNode(pRootNode, "charactermap");
	if (pNode != NULL) {
		pCharacterSheet = new CharacterSheet();
		pCharacterSheet->Start(&manager, pNode);
		pCharacterSheet->SetPosition(Vector2f(500, 100));
	}

	while (window.isOpen()) {
		deltatime = dt.restart().asSeconds();
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				factor += (deltatime * characterMoveSpeed);
				pCharacterSheet->SetGoToPosition((Vector2f)Mouse::getPosition(window));
			}

			//if (Keyboard::isKeyPressed(Keyboard::A))
			//{
			//	if (pCharacterSheet->GetState() != 2)
			//	{
			//		pCharacterSheet->SetState(2);
			//	}

			//	pCharacterSheet->SetVelocity(Vector2f(-speed * deltatime, 0));
			//}
			//else if (Keyboard::isKeyPressed(Keyboard::S))
			//{
			//	if (pCharacterSheet->GetState() != 3)
			//	{
			//		pCharacterSheet->SetState(3);
			//	}

			//	pCharacterSheet->SetVelocity(Vector2f(0, speed * deltatime));
			//}
			//else if (Keyboard::isKeyPressed(Keyboard::D))
			//{
			//	if (pCharacterSheet->GetState() != 1)
			//	{
			//		pCharacterSheet->SetState(1);
			//	}

			//	pCharacterSheet->SetVelocity(Vector2f(speed* deltatime, 0));
			//}
			//else if (Keyboard::isKeyPressed(Keyboard::W))
			//{
			//	if (pCharacterSheet->GetState() != 0)
			//	{
			//		pCharacterSheet->SetState(0);
			//	}

			//	pCharacterSheet->SetVelocity(Vector2f(0, -speed * deltatime));
			//}
			//else
			//{
			//	if (pCharacterSheet->GetState() != 4)
			//	{
			//		pCharacterSheet->SetState(4);
			//	}
			//	pCharacterSheet->SetVelocity(Vector2f(0, 0));
			//}
		}

		window.clear(Color::White);

		if (pCharacterSheet != NULL)
			pCharacterSheet->Update(&window);

		window.display();
	}
	return 0;
}


//Vector2f Interpolate(const Vector2f& pointA, const Vector2f& pointB,float factor)
//{
//	if (factor > 1.f)
//		factor = 1.f;
//
//	else if (factor < 0.f)
//		factor = 0.f;
//
//	return pointA + (pointB - pointA) * factor;
//}