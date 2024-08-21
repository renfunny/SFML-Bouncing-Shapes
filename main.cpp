#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

class Shape {
	public:
	std::string m_shapeType;
	std::string m_shapeName;
	float m_posX;
	float m_posY;
	float m_speedX;
	float m_speedY;
	int m_R;
	int m_G;
	int m_B;

		Shape(std::string shapeType, std::string shapeName, float posX, float posY, float speedX, float speedY, int R, int G, int B) {
		m_shapeType = shapeType;
		m_shapeName = shapeName;
		m_posX = posX;
		m_posY = posY;
		m_speedX = speedX;
		m_speedY = speedY;
		m_R = R;
		m_G = G;
		m_B = B;
	}
};

class Circle : public Shape {
	public:
	float m_radius;

		Circle(std::string shapeType, std::string shapeName, float posX, float posY, float speedX, float speedY, int R, int G, int B, float radius) : Shape(shapeType, shapeName, posX, posY, speedX, speedY, R, G, B) {
		m_radius = radius;
	}
};

class Rectangle : public Shape {
public:
	float m_width;
	float m_height;

	Rectangle(std::string shapeType, std::string shapeName, float posX, float posY, float speedX, float speedY, int R, int G, int B, float width, float height) : Shape(shapeType, shapeName, posX, posY, speedX, speedY, R, G, B) {
		m_width = width;
		m_height = height;
	}
};



int main(int argc, char* argv[])
{
	std::ifstream file("config.txt");
	std::string line;
	
	int width, height;
	std::string fontPath;
	int fontSize, r, g, b;
	std::vector<Shape*> shapes;
	std::vector<sf::Shape*> sfShapes;

	if (!file) {
		std::cerr << "Unable to open file" << std::endl;
	}

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string type;

		iss >> type;

		if (type == "Window") {
			iss >> width >> height;
			std::cout << "Window: " << width << "x" << height << std::endl;
		}
		else if (type == "Font") {
			iss >> fontPath >> fontSize >> r >> g >> b;
			std::cout << "Font: " << fontPath << " Size: " << fontSize
				<< " Color: (" << r << ", " << g << ", " << b << ")" << std::endl;
		}
		else if (type == "Circle") {
			std::string name;
			int r, g, b;
			float xSpeed, ySpeed, x, y, radius;
			iss >> name >> x >> y >> xSpeed >> ySpeed >> r >> g >> b >> radius;
			shapes.push_back(new Circle(type, name, x, y, xSpeed, ySpeed, r, g, b, radius));
			std::cout << "Circle: " << name << " Position: (" << x << ", " << y << ")"
				<< " Speed: (" << xSpeed << ", " << ySpeed << ")"
				<< " Color: (" << r << ", " << g << ", " << b << ")"
				<< " Radius: " << radius << std::endl;
		}
		else if (type == "Rectangle") {
			std::string name;
			int r, g, b;
			float xSpeed, ySpeed, x, y, width, height;
			iss >> name >> x >> y >> xSpeed >> ySpeed >> r >> g >> b >> width >> height;
			shapes.push_back(new Rectangle(type, name, x, y, xSpeed, ySpeed, r, g, b, width, height));
			std::cout << "Rectangle: " << name << " Position: (" << x << ", " << y << ")"
				<< " Speed: (" << xSpeed << ", " << ySpeed << ")"
				<< " Color: (" << r << ", " << g << ", " << b << ")"
				<< " Width: " << width << " Height: " << height << std::endl;
		}
		else {
			std::cerr << "Unknown type: " << type << std::endl;
		}
	}

	file.close();
	
	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");

	//lets make a shape that we will draw to the screen
	std::for_each(shapes.begin(), shapes.end(), [&](Shape* shape) {
		if (shape->m_shapeType == "Circle") {
			sf::CircleShape* circle = new sf::CircleShape(((Circle*)shape)->m_radius);
			circle->setFillColor(sf::Color(shape->m_R, shape->m_G, shape->m_B));
			circle->setPosition(shape->m_posX, shape->m_posY);
			sfShapes.push_back(circle);

		}
		else if (shape->m_shapeType == "Rectangle") {
			sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(((Rectangle*)shape)->m_width, ((Rectangle*)shape)->m_height));
			rectangle->setFillColor(sf::Color(shape->m_R, shape->m_G, shape->m_B));
			rectangle->setPosition(shape->m_posX, shape->m_posY);
			sfShapes.push_back(rectangle);
		}
	});

	// lets load a font so we can display some text
	sf::Font myFont;

	// check if the font loaded correctly
	if (!myFont.loadFromFile(fontPath))
	{
		// if we cant load font, print an error to the error console and exit
		std::cerr << "Error loading font\n";
		exit(-1);
	}

	

	// main loop - continues for each frame while window is open
	while (window.isOpen())
	{
		// event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			// this event triggers when the wondow is closed 
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(); // clear the window


		for (auto& shape : sfShapes) {
			sf::Vector2f pos = shape->getPosition();

			Shape* customShape = nullptr;
			if (shape->getPointCount() == 30) {
				customShape = static_cast<Circle*>(shapes[&shape - &sfShapes[0]]);
			}
			else {
				customShape = static_cast<Rectangle*>(shapes[&shape - &sfShapes[0]]);
		}

		pos.x += customShape->m_speedX;
		pos.y += customShape->m_speedY;

		if(pos.x < 0 || pos.x + shape->getGlobalBounds().width > width) {
						customShape->m_speedX *= -1;
		}
		if(pos.y < 0 || pos.y + shape->getGlobalBounds().height > height) {
						customShape->m_speedY *= -1;
		}

		shape->setPosition(pos);

		window.draw(*shape);

		sf::Text shapeName;
		shapeName.setFont(myFont);
		shapeName.setString(customShape->m_shapeName);
		shapeName.setCharacterSize(fontSize);
		shapeName.setFillColor(sf::Color(r, g, b));
		
		sf::FloatRect textRect = shapeName.getLocalBounds();

		float textX = pos.x + (shape->getGlobalBounds().width - textRect.width) / 2;
		float textY = pos.y + (shape->getGlobalBounds().height - textRect.height) / 2;

		shapeName.setPosition(textX, textY);

		window.draw(shapeName);
		}

		window.display(); // display the window
	}

	// clean up the memory
	for (auto& shape : sfShapes) {
		delete shape;
	}
	std::for_each(shapes.begin(), shapes.end(), [&](Shape* shape) {
		delete shape;
	});
	return 0;
}
