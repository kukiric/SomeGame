#include "Game.h"
#include <Engine/Filesystem/File.h>
#include <SDL2/SDL.h>
#include <cstring>

GameObjects::GameObjects()
    : universe(player)
{
    loadBitmapTextSDL(NULL, "Fonts/curses_640x300.bmp", &text);

    float aspect = (float) mainApp->window.getWidth()
                 / (float) mainApp->window.getHeight();

    camera = new Camera(60.f, aspect);
    skyCam = new Camera(60.f, aspect);
    camera->setClip(1.0f, 10000.f);
    skyCam->setClip(1000.f, 1000000.f);
    player.addChild(camera);
    player.addChild(skyCam);
    universe.spawn();
}

void Game::pollInput()
{
    input.mouse.xrel = 0;
    input.mouse.yrel = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT or
           (event.type == SDL_KEYDOWN and event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
        {
            this->quit();
            break;
        }

        if (event.type == SDL_KEYDOWN)
        {
            input.keyPressed[event.key.keysym.scancode] = 1;
        }
        else if (event.type == SDL_KEYUP)
        {
            input.keyPressed[event.key.keysym.scancode] = 0;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT) input.mouse.left = true;
            if (event.button.button == SDL_BUTTON_RIGHT) input.mouse.right = true;
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT) input.mouse.left = false;
            if (event.button.button == SDL_BUTTON_RIGHT) input.mouse.right = false;
        }
        if (event.type == SDL_MOUSEMOTION)
        {
            input.mouse.xrel = event.motion.xrel;
            input.mouse.yrel = event.motion.yrel;
            input.mouse.x = event.motion.x;
            input.mouse.y = event.motion.y;
        }
    }
}

void Game::initialize()
{
    Filesystem::setRootPath("../Data");
    memset(&input, 0, sizeof(InputArray));
    gameObjects = new GameObjects();
}

void Game::finalize()
{
    delete gameObjects;
}

void Game::loopBody()
{
    window.clear();
    gameObjects->text.setString("nil");
    gameObjects->player.updateChildren();
    gameObjects->universe.updateChildren();
    gameObjects->universe.draw(gameObjects->skyCam);
    gameObjects->universe.draw(gameObjects->camera);
    gameObjects->text.draw(0, 0);
    window.display();
}

Game::Game(int argc, char** argv)
    : Application(argc, argv)
{}
