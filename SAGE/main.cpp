#include "Manager.h"

using namespace std;
using namespace sf;
using namespace rapidxml;

Manager				manager;
CharacterSheet*		pCharacterSheet;

float				factor = 0.0f;
float				characterMoveSpeed = 0.1f;

float				deltatime = 0.0f;
Clock				dt;

float				speed = 250;
int					currentSceneId = 0;

bool				hasKey = false;
bool				openedBox = false;
bool				canGoToNextScene = false;
bool				canGoToPrevScene = false;


void CheckMousePosition();
void CheckCharacterCollisionWithKey();
void CheckCharacterCollisionWithTreasureBox();


int main()
{
	// setting up the window
	RenderWindow window(VideoMode(600, 350), "Adventure of skeleton man", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);


	// reading the XML file
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

	// loading assets
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

	// loading scenes
	pNode = FindChildNode(pRootNode, "scenes");
	if (pNode != NULL) {
		manager.LoadSceneData(pNode);
	}

	// setting the first scene
	manager.SetCurrentScene(0);
	manager.SetWindow(&window);
	currentSceneId = 0;

	// loading the main character
	pNode = FindChildNode(pRootNode, "charactermap");
	if (pNode != NULL) {
		pCharacterSheet = new CharacterSheet();
		pCharacterSheet->Start(&manager, pNode);
		pCharacterSheet->SetPosition(Vector2f(300, 200));
		pCharacterSheet->SetGoToPosition(pCharacterSheet->GetPosition());
	}

	while (window.isOpen()) {
		deltatime = dt.restart().asSeconds();
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			// check mouse position for walkable place and also next and previous boundery box
			CheckMousePosition();

			if (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				// goes to next scene and sets the character position and the current scene
				if (currentSceneId < 2 && canGoToNextScene) 
				{
					currentSceneId++;
					manager.SetCurrentScene(currentSceneId);
					pCharacterSheet->SetPosition(Vector2f(0, pCharacterSheet->GetPosition().y));
					pCharacterSheet->SetGoToPosition(Vector2f(30, pCharacterSheet->GetPosition().y));
					if (pCharacterSheet->GetState() != 3)
					{
						pCharacterSheet->SetState(3);
					}
				}

				// goes to previous scene and sets the character position and the current scene
				if (currentSceneId > 0 && canGoToPrevScene)
				{
					currentSceneId--;
					manager.SetCurrentScene(currentSceneId);
					pCharacterSheet->SetPosition(Vector2f(600, pCharacterSheet->GetPosition().y));
					pCharacterSheet->SetGoToPosition(Vector2f(570, pCharacterSheet->GetPosition().y));
					if (pCharacterSheet->GetState() != 1)
					{
						pCharacterSheet->SetState(1);
					}
				}

				// checks if character can walk then move the character
				if(!canGoToNextScene && !canGoToPrevScene)
				{
					if (Mouse::getPosition(window).y > 120)
					{
						if (Mouse::getPosition(window).x > pCharacterSheet->GetPosition().x)
						{
							if (pCharacterSheet->GetState() != 3) 
							{
								pCharacterSheet->SetState(3); // sets the charcter animation state to right
							}
						}
						else
						{
							if (pCharacterSheet->GetState() != 1)
							{
								pCharacterSheet->SetState(1); // sets the charcter animation state to left
							}
						}

						// set the characer goto position
						pCharacterSheet->SetGoToPosition((Vector2f)Mouse::getPosition(window));
					}
				}
			}
		}

		if (currentSceneId == 1 && !hasKey)
			CheckCharacterCollisionWithKey();

		if (currentSceneId == 2 && !openedBox)
			CheckCharacterCollisionWithTreasureBox();

		window.clear(Color::White);

		// load current scene
		manager.LoadCurrentScene();

		// updating the main character position and sprite
		if (pCharacterSheet != NULL)
			pCharacterSheet->Update(&window);

		manager.UpdateInventory();

		// updates the cursor position and sprite
		manager.UpdateCursor();

		window.display();
	}
	return 0;
}


// checks the mouse position and changes the state of the cursor image
void CheckMousePosition()
{
	if (Mouse::getPosition(*manager.m_Window).x > 550)
	{
		manager.m_CursorState = 2;
		canGoToNextScene = true;
		canGoToPrevScene = false;
	}
	else if (Mouse::getPosition(*manager.m_Window).x < 50)
	{
		manager.m_CursorState = 3;
		canGoToNextScene = false;
		canGoToPrevScene = true;
	}
	else
	{
		if (Mouse::getPosition(*manager.m_Window).y > 120)
			manager.m_CursorState = 1;
		else
			manager.m_CursorState = 0;

		canGoToNextScene = false;
		canGoToPrevScene = false;
	}
}


// checks the collision with the key
void CheckCharacterCollisionWithKey()
{
	// calculating the distance between key and character
	float xDistance = pCharacterSheet->GetPosition().x - 250;
	float yDistance = pCharacterSheet->GetPosition().y - 200;
	float distance = sqrt(xDistance * xDistance + yDistance * yDistance);
	if (distance < 10)
	{
		manager.m_Inventory.AddItemInInventory(3);
		manager.m_CurrentScene->m_ObjcetsInScene.pop_back();

		hasKey = true;
	}
}

// checks the collision with the treasure box
void CheckCharacterCollisionWithTreasureBox()
{
	if (hasKey)
	{
		// calculating the distance between treasure box and character
		float xDistance = pCharacterSheet->GetPosition().x - 450;
		float yDistance = pCharacterSheet->GetPosition().y - 200;
		float distance = sqrt(xDistance * xDistance + yDistance * yDistance);
		if (distance < 50)
		{
			manager.m_CurrentScene->m_ObjcetsInScene.pop_back();
			manager.m_CurrentScene->m_ObjcetsInScene.push_back(&ObjectInScene(10, 450, 200));
			openedBox = true;
		}
	}

}


