#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>

// Make code easier to type with "using namespace"
using namespace sf;
const int Xmax = 1920;
const int Ymax = 1080;
const float AXE_POSITION_LEFT = 850;
const float AXE_POSITION_RIGHT = 1050;

const int NUM_BRANCHES = 6;
enum class side { LEFT, RIGHT, NONE };

const int NUM_TREE = 3;


const int NUM_CLOUDS = 6;
struct MyCloud
{
	Sprite spriteCloud;
	bool cloudActive;
	float cloudSpeed;
};

const int NUM_BEE = 7;

struct MyBee
{
	Sprite spriteBee;
	bool BeeActive;
	float BeeSpeed;
};

void updateBranches(int seed, side branca[]);

int main()
{

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Create a video mode object
	VideoMode vm(Xmax, Ymax);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	Texture texturePlayer;
	Texture textureAxe;
	Texture textureRIP;
	Texture textureLog;
	// Load a graphic into the texture


	textureBackground.loadFromFile("./graphics/background.png");
	texturePlayer.loadFromFile("./graphics/Jugador.png");
	textureAxe.loadFromFile("./graphics/axe.png");
	textureRIP.loadFromFile("./graphics/rip.png");
	textureLog.loadFromFile("./graphics/log.png");

	// Create a sprite


	Sprite spriteBackground;


	Sprite spritePlayer;
	Sprite spriteAxe;
	Sprite spriteRIP;
	Sprite spriteLog;

	// Attach the texture to the sprite


	spriteBackground.setTexture(textureBackground);
	//spriteTree.setTexture(textureTree);
	spritePlayer.setTexture(texturePlayer);
	spriteRIP.setTexture(textureRIP);
	spriteAxe.setTexture(textureAxe);
	spriteLog.setTexture(textureLog);
	spriteRIP.setPosition(-500, -500);
	spritePlayer.setPosition(-500, -500);
	spriteAxe.setPosition(-500, -500);
	spriteLog.setPosition(-500, -500);
	// Set the spriteBackground to cover the screen

	//Sprites
	spriteBackground.setPosition(0, 0);
	//spriteTree.setPosition(810, 0);
	//Strings
	String levelName = "DastardlyCave";
	String playerName = "John Carmack";
	String message = "GAME OVER!!";
	Text scoreText;
	Text messageText;
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	int score = 0;

	messageText.setFont(font);
	scoreText.setFont(font);
	messageText.setString("Press enter to start!");
	scoreText.setString("Score = 0");
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	//Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(Xmax / 2.0f, Ymax / 2.0f);
	scoreText.setPosition(20, 20);


	//Cloud

	MyCloud spriteCloud[NUM_CLOUDS];
	Texture textureCloud;
	textureCloud.loadFromFile("./graphics/nuvol.png");
	for (int i = 0; i < NUM_CLOUDS; i++) {
		spriteCloud[i].spriteCloud.setTexture(textureCloud);
		spriteCloud[i].spriteCloud.setPosition(rand() % Xmax, (rand() % 200));
		spriteCloud[i].cloudActive = true;
		spriteCloud[i].spriteCloud.setOrigin(220, 20);
		spriteCloud[i].cloudSpeed = rand() % 150 + 50;
	}

	//Tree
	Sprite spriteTree[NUM_TREE];
	Texture textureTree;
	textureTree.loadFromFile("./graphics/arbol.png");
	spriteTree[0].setTexture(textureTree);
	spriteTree[0].setPosition(810, 0);
	for (int i = 1; i < NUM_TREE; i++) {
		spriteTree[i].setTexture(textureTree);
		spriteTree[1].setPosition(1500, 2);
		spriteTree[2].setPosition(300, 2);
		spriteTree[1].setScale(0.5 , 1);
		spriteTree[2].setScale(0.6, 1);

	}

	//Player
	side playerSide = side::LEFT;

	//Bee
	MyBee spriteBee[NUM_BEE];
	Texture textureBee;
	textureBee.loadFromFile("./graphics/bee.png");
	for (int i = 0; i < NUM_BEE; i++) {
		spriteBee[i].spriteBee.setTexture(textureBee);
		spriteBee[i].spriteBee.setPosition(rand() % Xmax, (rand() % 300));
		spriteBee[i].BeeActive = false;
		spriteBee[i].spriteBee.setOrigin(220, 20);
		spriteBee[i].BeeSpeed = rand() % 150 + 50;
	}

	//Branches
	Sprite branches[NUM_BRANCHES];
	side branchPositions[NUM_BRANCHES];
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}







	//Bools
	bool paused = true;
	bool logActive = false;
	bool acceptInput = false;


	//Float
	float logSpeedX = 1000;
	float logSpeedY = -1500;


	//Time Bar
	RectangleShape timeBar;
	float timeBarStartWidth = 500;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::White);
	timeBar.setOutlineColor(Color::Black);
	timeBar.setOutlineThickness(3  );
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


	Clock clock;

	//for (int i = 0; i < NUM_CLOUDS; i++)
	//{
		//spriteCloud[i].spriteCloud.setTexture(textureCloud);
		//spriteCloud[i].spriteCloud.setPosition(0, i * 250);
	//}
	srand((int)time(0) * 10);

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;
				spriteAxe.setPosition(-500, 750);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		/*
		****************************************
		Handle the players input
		****************************************
		*/


		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 6;

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}



			spriteRIP.setPosition(675, 2000);
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}
		if (acceptInput) {


			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1150, 720);
				updateBranches(score, branchPositions);
				spriteAxe.setRotation(0);
				spriteAxe.setScale(1, 1);

				//Log Flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(530, 720);
				
				spriteAxe.setScale(-1, 1);
				updateBranches(score, branchPositions);
				//Log Flying to the left
				spriteLog.setPosition(815, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
			}


		}


		if (!paused)
		{
			Time dt = clock.restart();
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f) {
				// Pause the game
				paused = true;
				// Change the message shown to the player
				messageText.setString("Out of time!!");
				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}
			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;
				spriteRIP.setPosition(525, 760);
				spritePlayer.setPosition(2000, 600);
				messageText.setString("SQUISHED!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				death.play();
			}
			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
				);

				// Has the insect reached the right hand edge of the screen?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new cloud next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}




			//Score
			//score++;
			std::stringstream ss;

			ss << "Score = " << score;
			scoreText.setString(ss.str());


			//Axe




			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!spriteCloud[i].cloudActive)
				{
					spriteCloud[i].cloudSpeed = (rand() % 200);
					spriteCloud[i].spriteCloud.setPosition(-200, (rand() % 500));
					spriteCloud[i].cloudActive = true;
				}
				else
				{
					spriteCloud[i].spriteCloud.setPosition(spriteCloud[i].spriteCloud.getPosition().x + (spriteCloud[i].cloudSpeed * dt.asSeconds()),
						spriteCloud[i].spriteCloud.getPosition().y);
					if (spriteCloud[i].spriteCloud.getPosition().x > Xmax)
					{
						spriteCloud[i].cloudActive = false
							;
					}
				}

			}

			for (int i = 0; i < NUM_BEE; i++)
			{
				if (!spriteBee[i].BeeActive)
				{
					spriteBee[i].BeeSpeed = (rand() % 200);
					spriteBee[i].spriteBee.setPosition(2000, (rand() % 500));
					spriteBee[i].BeeActive = true;
				}
				else
				{
					spriteBee[i].spriteBee.setPosition(spriteBee[i].spriteBee.getPosition().x - (spriteBee[i].BeeSpeed * dt.asSeconds()),
						spriteBee[i].spriteBee.getPosition().y);

					if (spriteBee[i].spriteBee.getPosition().x < 0.0)
					{
						spriteBee[i].BeeActive = false;
					}
				}

			}


			//Actualitzar Branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}


		}
		/*
				****************************************
				Update the scene
				****************************************
				*/


				/*
				****************************************
				Draw the scene
				****************************************
				*/

				// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);


		for (int i = 0; i < NUM_CLOUDS; i++) {
			window.draw(spriteCloud[i].spriteCloud);

		}

		for (int i = 0; i < NUM_TREE; i++) {
			window.draw(spriteTree[i]);

		}

		for (int i = 0; i < NUM_BEE; i++) {
			window.draw(spriteBee[i].spriteBee);

		}


		window.draw(scoreText);
		window.draw(timeBar);
		window.draw(spriteAxe);
		window.draw(spritePlayer);
		window.draw(spriteRIP);
		window.draw(spriteLog);





		if (paused)
		{
			window.draw(messageText);
		}

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}



		// Show everything we just drew
		window.display();


	}


	return 0;
}



void updateBranches(int seed, side branca[]) {
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branca[j] = branca[j - 1];
	}
	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	case 0:
		branca[0] = side::LEFT;
		break;
	case 1:
		branca[0] = side::RIGHT;
		break;
	default:
		branca[0] = side::NONE;
		break;
	}
}
	

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
