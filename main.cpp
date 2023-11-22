#include "ComplexPlane.h"
using namespace sf;
using namespace std;


int main()
{
    VideoMode vm((VideoMode::getDesktopMode().width / 2), (VideoMode::getDesktopMode().height / 2));
    RenderWindow window(vm, "Mandelbrot Set", Style::Default);

    ComplexPlane plane(vm.width, vm.height);

    Font programFont;
    programFont.loadFromFile("Pixellettersfull-BnJ5.ttf");

    Text programText("", programFont, 30);

    while (window.isOpen())
    {
        /*
            Handle user input
        */

        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    plane.zoomIn();
                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                else if (event.mouseButton.button == Mouse::Right)
                {
                    plane.zoomOut();
                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }

            if (event.type == Event::MouseMoved)
            {
                plane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        /*
            Update scene
        */
        plane.updateRender();
        plane.loadText(programText);

        /*
            Draw scene to window
        */
        window.clear();

        plane.draw(window, RenderStates::Default); // RenderStates::Default is, for now, a placeholder
        window.draw(programText);

        window.display();
    }
}

