#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
using std::string;

/**
 * @brief Encodes a message within the pixels of an image
 * 
 * @param msg message to be encoded
 * @param img image that the message will be encoded in
 */
void addMessage(string msg, sf::Image& img);

/**
 * @brief Extract message that is encoded in an image
 * 
 * @param encoded image containing encoded message
 * @return string decoded message
 */
string getMessage(sf::Image encoded);

int main(int argc, char* argv[])
{
    string message, c;
    std::ostringstream sstr;
    sf::Image img;
    sf::Texture texture;
    sf::Sprite sprite;

    if(argc != 3)
    {
        std::cerr << "Invalid Command: \n"
        "Command should take the format - ./steganography {image} {option}\n"
        "options are:\n1 - to encode an image\n2 - to decode an image\n";
        return 1;
    }

    img.loadFromFile(argv[1]);

    switch(atoi(argv[2]))
    {
        case 1:	// Insert message into image
            sstr << std::cin.rdbuf();
            message = sstr.str();
            message += '\0';
            addMessage(message, img);
            img.saveToFile("encoded.png");
        break;
        case 2:	// Extract message from image
            message = getMessage(img);
            std::cout << message << std::endl;
        break;
        default:
            std::cerr << "Invalid Option Selected - Exiting\n"; 
            return 2;
        break;
    }

    texture.loadFromImage(img);
    sprite.setTexture(texture);

    sf::RenderWindow window(sf::VideoMode(img.getSize().x, img.getSize().y), "Steganography");

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}

/**
 * Uses bitwise operations to break each 8 bit character
 * into four 2 bit chunks. It then clears the 2 least
 * significant bits of each rgba channel of each pixel
 * and stores the 2 bit chunks in the cleared spaces.
 */
void addMessage(string msg, sf::Image& img)
{
    sf::Vector2u size = img.getSize();
    int msgSize = msg.size();
	
    for(int i = 0; i < size.y; i++)
    {
        for(int j = 0; j < size.x; j++)
        {
            if(i * size.x + j < msgSize)
            {
                /*
                    msg masks   = 0x03, 0x0c, 0x30, 0xc0
                    color masks = 0xfc, 0xf3, 0xcf, 0x3f
                */
                sf::Color c = img.getPixel(j, i); // original color
                char msgBits[4] = {0};
                msgBits[0] = (msg[i * size.x + j] & 0xc0) >> 6;
                msgBits[1] = (msg[i * size.x + j] & 0x30) >> 4;
                msgBits[2] = (msg[i * size.x + j] & 0x0c) >> 2;
                msgBits[3] = (msg[i * size.x + j] & 0x03);

                c.r = (c.r & 0xfc) | msgBits[0];
                c.g = (c.g & 0xfc) | msgBits[1];
                c.b = (c.b & 0xfc) | msgBits[2];
                c.a = (c.a & 0xfc) | msgBits[3];
                img.setPixel(j, i, c);
            }
            else
				return;
        }
    }
}

/**
 * Uses bitwise operations to extract the 2 least
 * significant bits from each rgba chanell of each
 * pixel and combines them into a character. All 
 * characters are added to a string until a NULL
 * character is found.
 */
string getMessage(sf::Image encoded)
{
    string msg = "";
    sf::Vector2u size = encoded.getSize();

    for(int i = 0; i < size.y; i++)
    {
        for(int j = 0; j < size.x; j++)
        {
            sf::Color color = encoded.getPixel(j, i);
            char msgBits[4] = {0}, c;
			msgBits[0] = (color.r & 0x03);
			msgBits[1] = (color.g & 0x03);
			msgBits[2] = (color.b & 0x03);
			msgBits[3] = (color.a & 0x03);

			c = msgBits[0] << 6 | msgBits[1] << 4 | msgBits[2] << 2 | msgBits[3];
			if(c == '\0')
			    return msg;

            msg += c;
        }
    }
    return msg;
}
