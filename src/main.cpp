#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>

uint64_t unix_timestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int main()
{
    sf::Texture color;
    if (!color.loadFromFile("sphere_color.png"))
    {
        return -1;
    }

    sf::Texture normal;
    if (!normal.loadFromFile("sphere_normal.png"))
    {
        return -1;
    }

    sf::Shader lighting;
    if (!lighting.loadFromFile("lighting.frag", sf::Shader::Fragment))
    {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        sf::Sprite sprite;
        sprite.setTexture(color);
        lighting.setUniform("normalmap", normal);

        std::uint64_t unixtime = unix_timestamp();
        std::uint64_t botpart = unixtime;
        botpart /= 1000000;
        unixtime -= botpart * 1000000;
        float movespeed = 0.002f;
        //sf::Vector3f lightpos(5.0f*(float)std::cos(((float)unixtime)*movespeed),1.0f,5.0f*(float)std::sin(((float)unixtime)*movespeed));
        sf::Vector3f lightpos(5+10.0f*(float)std::cos(((float)unixtime)*movespeed), 5, 5);
        std::cout << 5.0f*(float)std::cos(((float)unixtime)*movespeed) << std::endl;
        lighting.setUniform("lightpos", lightpos);

        window.draw(sprite, &lighting);

        // end the current frame
        window.display();
    }

    return 0;
}