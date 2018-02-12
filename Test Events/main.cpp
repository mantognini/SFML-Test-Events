/*
 * SFML Test Events - Copyright (C) 2016 Marco Antognini <antognini.marco@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.  If you use this software in a
 *    product, an acknowledgment in the product documentation would be appreciated
 *    but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */


#include <SFML/Graphics.hpp>

#include <cassert>
#include <clocale>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <limits>
#include <sstream>

#ifdef SFML_SYSTEM_MACOS
#include "ResourcePath.hpp"
#else
std::string resourcePath() { return ""; }
#endif

std::string key2string(sf::Keyboard::Key key)
{
// Code based on lib Thor
#define CASE(id)                                                                                   \
    case sf::Keyboard::id:                                                                         \
        return #id

    switch (key)
    {
    default:
        CASE(Unknown);

        CASE(A);
        CASE(B);
        CASE(C);
        CASE(D);
        CASE(E);
        CASE(F);
        CASE(G);
        CASE(H);
        CASE(I);
        CASE(J);
        CASE(K);
        CASE(L);
        CASE(M);
        CASE(N);
        CASE(O);
        CASE(P);
        CASE(Q);
        CASE(R);
        CASE(S);
        CASE(T);
        CASE(U);
        CASE(V);
        CASE(W);
        CASE(X);
        CASE(Y);
        CASE(Z);
        CASE(Num0);
        CASE(Num1);
        CASE(Num2);
        CASE(Num3);
        CASE(Num4);
        CASE(Num5);
        CASE(Num6);
        CASE(Num7);
        CASE(Num8);
        CASE(Num9);
        CASE(Escape);
        CASE(LControl);
        CASE(LShift);
        CASE(LAlt);
        CASE(LSystem);
        CASE(RControl);
        CASE(RShift);
        CASE(RAlt);
        CASE(RSystem);
        CASE(Menu);
        CASE(LBracket);
        CASE(RBracket);
        CASE(SemiColon);
        CASE(Comma);
        CASE(Period);
        CASE(Quote);
        CASE(Slash);
        CASE(BackSlash);
        CASE(Tilde);
        CASE(Equal);
        CASE(Dash);
        CASE(Space);
        CASE(Return);
        CASE(BackSpace);
        CASE(Tab);
        CASE(PageUp);
        CASE(PageDown);
        CASE(End);
        CASE(Home);
        CASE(Insert);
        CASE(Delete);
        CASE(Add);
        CASE(Subtract);
        CASE(Multiply);
        CASE(Divide);
        CASE(Left);
        CASE(Right);
        CASE(Up);
        CASE(Down);
        CASE(Numpad0);
        CASE(Numpad1);
        CASE(Numpad2);
        CASE(Numpad3);
        CASE(Numpad4);
        CASE(Numpad5);
        CASE(Numpad6);
        CASE(Numpad7);
        CASE(Numpad8);
        CASE(Numpad9);
        CASE(F1);
        CASE(F2);
        CASE(F3);
        CASE(F4);
        CASE(F5);
        CASE(F6);
        CASE(F7);
        CASE(F8);
        CASE(F9);
        CASE(F10);
        CASE(F11);
        CASE(F12);
        CASE(F13);
        CASE(F14);
        CASE(F15);
        CASE(Pause);
    }

    static_assert(sf::Keyboard::KeyCount == 101, "Number of SFML keys has changed");

#undef CASE
}


std::string button2string(sf::Mouse::Button button)
{
#define CASE(id)                                                                                   \
    case sf::Mouse::id:                                                                            \
        return #id

    switch (button)
    {
    default:
        CASE(ButtonCount);
        CASE(Left);
        CASE(Right);
        CASE(Middle);
        CASE(XButton1);
        CASE(XButton2);
    }

    static_assert(sf::Mouse::ButtonCount == 5, "Number of SFML mouse buttons has changed");

#undef CASE
}


std::string axis2string(sf::Joystick::Axis axis)
{
#define CASE(id)                                                                                   \
    case sf::Joystick::id:                                                                         \
        return #id

    switch (axis)
    {
        CASE(X);
        CASE(Y);
        CASE(Z);
        CASE(R);
        CASE(U);
        CASE(V);
        CASE(PovX);
        CASE(PovY);
    }

#undef CASE
}



class GraphicLogger : public sf::Drawable, public sf::Transformable
{
public:
    GraphicLogger(sf::Font const& font, unsigned int fontSize, unsigned int nbLines)
        : logs(nbLines)
        , lines(nbLines)
    {
        for (unsigned int i = 0; i < lines.size(); ++i)
        {
            auto& line = lines[i];
            // Set up each line
            line.setFont(font);
            line.setCharacterSize(fontSize);
            line.setFillColor(sf::Color::White);
            line.setPosition(0, i * (fontSize * 1.1));
        }

        updateLines();
    }

    void log(sf::String const& msg)
    {
        // pop back and push front
        logs.pop_back();
        logs.push_front(msg);

        std::cout << "Log: " << msg.toUtf8().c_str() << std::endl;

        updateLines();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        for (auto const& line : lines)
        {
            target.draw(line, states);
        }
    }

private:
    void updateLines()
    {
        assert(logs.size() == lines.size());
        for (unsigned int i = 0; i < logs.size(); ++i)
        {
            sf::String const str = sf::String(std::to_string(i)) + ": " + logs[i];
            lines[i].setString(str);
        }
    }

private:
    std::deque<sf::String> logs;
    std::vector<sf::Text> lines;
};




class JoystickTable : public sf::Drawable, public sf::Transformable
{
public:
    JoystickTable(sf::Font const& font, unsigned int fontSize)
    : font(font), fontSize(fontSize)
    {
        update();
    }

    void update()
    {
        std::function<float(sf::Joystick::Axis)> producer;
        if (activeJoystickId == -1)
            producer = [](sf::Joystick::Axis) -> float {
                return std::numeric_limits<float>::quiet_NaN();
            };
        else
            producer = [id = activeJoystickId](sf::Joystick::Axis axis) -> float {
                return sf::Joystick::getAxisPosition(id, axis);
            };

        generateTable(producer);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        for (auto const& line : strings)
            target.draw(line, states);
    }

    void setJoystick(unsigned int idx) { activeJoystickId = idx; }

private:
    sf::Text generateText(sf::Joystick::Axis axis, int y, float value) const
    {
        sf::Text text;

        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);

        text.setPosition(0, y);

        std::stringstream ss;
        ss << axis2string(axis) << ": " << std::fixed << std::setprecision(3) << value;
        text.setString(ss.str());

        return text;
    }

    void generateTable(std::function<float(sf::Joystick::Axis)> producer)
    {
        strings.clear();

        // Insert titles + values
        auto const STEP = fontSize + 3;
        for (int i = 0; i < sf::Joystick::AxisCount; ++i) {
            auto axis = static_cast<sf::Joystick::Axis>(i);
            auto y = i * STEP;
            strings.push_back(generateText(axis, y, producer(axis)));
        }
    }

private:
    std::vector<sf::Text> strings;
    unsigned int activeJoystickId = -1;
    sf::Font const& font;
    unsigned int fontSize;
};



template <class T>
std::ostream& operator<<(std::ostream& out, sf::Vector2<T> const& v)
{
    return out << "(" << v.x << "; " << v.y << ")";
}



namespace
{
    int windowCount = 0;
    bool grabbed = false;
}



void drawWindowCount(sf::RenderWindow& window, sf::Font const& font)
{
    sf::Text text;
    text.setString(std::to_string(windowCount));
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Yellow);
    auto pos = window.getSize() / 2u;
    text.setPosition(static_cast<sf::Vector2f>(pos));

    window.draw(text);
}

void createWindow(sf::Window& window, sf::VideoMode const& mode, sf::Uint32 style)
{
    ++windowCount;
    grabbed = false;

    window.create(mode, "SFML Window", style);
    window.setFramerateLimit(30);

    std::cout << "New window:\n"
              << "\tsize: " << window.getSize()
              << "\trequested: " << sf::Vector2u(mode.width, mode.height) << "\n" << std::endl;
}

void goWindowed(sf::Window& window)
{
    createWindow(window, sf::VideoMode(800, 600), sf::Style::Default);
}

void goFullscreen(sf::Window& window)
{
    auto mode = sf::VideoMode::getDesktopMode();
    createWindow(window, mode, sf::Style::Fullscreen);
}

void drawBorder(sf::RenderWindow& window)
{
    sf::RectangleShape borders{ static_cast<sf::Vector2f>(window.getSize()) };
    borders.setFillColor(sf::Color::Transparent);
    borders.setOutlineColor(sf::Color::White);
    borders.setOutlineThickness(-20);
    window.draw(borders);
}

void drawGrid(sf::RenderWindow& window, std::size_t size)
{
    std::vector<sf::Vertex> vertexes;

    float const h = window.getSize().y;
    float const w = window.getSize().x;

    for (float y = 0.f; y <= h; y += size) {
        vertexes.push_back(sf::Vertex({0, y-1}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({w, y-1}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({0, y}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({w, y}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({0, y+1}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({w, y+1}, sf::Color::Green));
    }

    for (float x = 0.f; x <= w; x += size) {
        vertexes.push_back(sf::Vertex({x-1, 0}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({x-1, h}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({x, 0}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({x, h}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({x+1, 0}, sf::Color::Green));
        vertexes.push_back(sf::Vertex({x+1, h}, sf::Color::Green));
    }

    window.draw(vertexes.data(), vertexes.size(), sf::Lines);
}

void printVideoModes()
{
    std::cout << "Fullscreen modes:\n";
    for (auto mode : sf::VideoMode::getFullscreenModes()) {
        std::cout << "\t" << mode.width << "x" << mode.height << "\n";
    }
    auto desktop = sf::VideoMode::getDesktopMode();
    std::cout << "Desktop mode:\n\t" << desktop.width << "x" << desktop.height << "\n";
}

int main(int, char const**)
{
    std::setlocale(LC_ALL, "");

    printVideoModes();

    // Create the main window
    sf::RenderWindow window;
    goWindowed(window);

    // Set the Icon
    sf::Image icon;
    assert(icon.loadFromFile(resourcePath() + "icon.png"));

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load our font
    sf::Font font;
    // auto const file = "sansation.ttf";
    auto const file = "FiraCode-Light.ttf";
    // auto const file = "HelveticaNeue.ttf";
    assert(font.loadFromFile(resourcePath() + file));

    sf::CircleShape cursorShape;
    cursorShape.setRadius(20.f);
    cursorShape.setOrigin(20.f, 20.f);
    cursorShape.setFillColor(sf::Color::Red);

    // Create a logger
    GraphicLogger logger{ font, 20, 20 };
    logger.setPosition(50, 50);
    JoystickTable joyInfo{ font, 20 };
    joyInfo.setPosition(50, 50);
    bool displayJoystickTable = false;


    sf::Event::EventType lastType = sf::Event::Count;

    std::map<sf::Joystick::Axis, sf::Clock> axisClocks;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                // window.close();
            }

// Log events
#define LOGEvent(type)                                                                             \
    case sf::Event::type:                                                                          \
        logger.log(#type)

#define LOGXY(var)                                                                                 \
    logger.log(std::string(#var) + ": (" + std::to_string(var.x) + "; " + std::to_string(var.y) + ")")

#define LOGWheel(var) logger.log(std::string(#var) + ": delta = " + std::to_string(var.delta))

#define LOGWidthHeight(var)                                                                        \
    logger.log(std::string(#var) + ": (" + std::to_string(var.width) + "; " +                      \
               std::to_string(var.height) + ")")

#define LOGKey(var)                                                                                \
    logger.log(key2string(var.key.code) +                                                          \
               (var.type == sf::Event::KeyPressed ? " was pressed" : " was released"))

#define LOGButton(var)                                                                             \
    logger.log(button2string(var.mouseButton.button) +                                             \
               (var.type == sf::Event::MouseButtonPressed ? " was pressed" : " was released"))

            if (event.type != sf::Event::MouseMoved || lastType != sf::Event::MouseMoved)
                switch (event.type)
                {
                    LOGEvent(Closed);
                    break;

                    LOGEvent(Resized);
                    LOGWidthHeight(event.size);
                    break;

                    LOGEvent(LostFocus);
                    break;

                    LOGEvent(GainedFocus);
                    break;

                    LOGEvent(TextEntered);
                    break;

                    LOGEvent(KeyPressed);
                    LOGKey(event);
                    break;

                    LOGEvent(KeyReleased);
                    LOGKey(event);
                    break;

                    LOGEvent(MouseButtonPressed);
                    LOGButton(event);
                    LOGXY(event.mouseButton);
                    break;

                    LOGEvent(MouseButtonReleased);
                    LOGButton(event);
                    LOGXY(event.mouseButton);
                    break;

                    LOGEvent(MouseMoved);
                    LOGXY(event.mouseMove);
                    break;

                    LOGEvent(MouseWheelMoved);
                    LOGWheel(event.mouseWheel);
                    LOGXY(event.mouseWheel);
                    break;

                    LOGEvent(MouseWheelScrolled);
                    LOGWheel(event.mouseWheelScroll);
                    LOGXY(event.mouseWheelScroll);
                    logger.log(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel
                                   ? "VerticalWheel"
                                   : "HorizontalWheel");
                    break;

                    LOGEvent(MouseEntered);
                    break;

                    LOGEvent(MouseLeft);
                    break;

                    LOGEvent(JoystickButtonPressed);
                    break;

                    LOGEvent(JoystickButtonReleased);
                    break;

                    LOGEvent(JoystickMoved);
                    if (axisClocks[event.joystickMove.axis].getElapsedTime() > sf::seconds(2)) {
                        logger.log("\tid:" + std::to_string(event.joystickMove.joystickId));
                        logger.log("\tposition:" + std::to_string(event.joystickMove.position));
                        logger.log("\taxis:" + axis2string(event.joystickMove.axis));
                        axisClocks[event.joystickMove.axis].restart();
                    }
                    break;

                    LOGEvent(JoystickConnected);
                    joyInfo.setJoystick(event.joystickConnect.joystickId);
                    break;

                    LOGEvent(JoystickDisconnected);
                    break;

                default:
                    break;
                }
            lastType = event.type;

            // Actions
            if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                default:
                    break;

                case sf::Keyboard::Escape:
                    window.close();
                    break;

                case sf::Keyboard::F:
                    goFullscreen(window);
                    break;

                case sf::Keyboard::N:
                    goWindowed(window);
                    break;

                case sf::Keyboard::G:
                    grabbed = !grabbed;
                    logger.log(grabbed ? "Grabbed" : "Released");
                    window.setMouseCursorGrabbed(grabbed);
                    break;

                case sf::Keyboard::P:
                    sf::Mouse::setPosition({ 100, 100 }, window);
                    break;

                case sf::Keyboard::Q:
                    sf::Mouse::setPosition({ 100, 100 });
                    break;

                case sf::Keyboard::A:
                    if (event.key.shift) window.setPosition({100, 100});
                    else LOGXY(window.getPosition());
                    break;

                case sf::Keyboard::B:
                    LOGWidthHeight(sf::VideoMode::getDesktopMode());
                    break;

                case sf::Keyboard::C:
                    if (event.key.shift) {
                        window.setSize({400, 600});
                        sf::FloatRect visibleArea(0, 0, 400, 600);
                        window.setView(sf::View(visibleArea));
                    } else LOGXY(window.getSize());
                    break;

                case sf::Keyboard::J:
                    displayJoystickTable = !displayJoystickTable;
                    break;

                }
            }
        }

        joyInfo.update();

        auto position = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
        cursorShape.setPosition(position);

        // Clear screen
        window.clear();

        // Draw the logger/joystick table
        if (displayJoystickTable)
            window.draw(joyInfo);
        else
            window.draw(logger);

        drawWindowCount(window, font);
        drawBorder(window);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
            drawGrid(window, 50);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            window.draw(cursorShape);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
