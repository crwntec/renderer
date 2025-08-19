#include <SDL.h>
#include <iostream>
#include <sstream>
#include <render.hpp>
#include "objloader.hpp"
#include <tuple>
#include <transform.hpp>

uint32_t frame_buffer[WIDTH * HEIGHT];
float depth_buffer[WIDTH * HEIGHT];

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not be initialized! SDL_Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    bool running = true;
    SDL_Event event;

    std::cout << "Loading Mesh files.." << "\n";
    std::vector<Mesh> loaded_meshes = LoadObjFiles("assets/cube.obj", "assets/monkey.obj", nullptr);
    std::cout << loaded_meshes.size() << " Mesh files loaded!" << "\n";
    std::vector<Model> models;
    for (int i = 0; i < loaded_meshes.size(); i++)
    {
        float3 position = {i * 2, 0, i * 2};
        models.push_back(Model(&loaded_meshes[i], position, {0, 0, 0}, {1, 1, 1}));
    }
    std::cout << "Created " << models.size() << " Models";
    float3 camera_rotation = {0, 0, 0};
    float3 camera_position = {0, 0, -10};

    const float moveSpeed = 0.05f;
    const float rotateSpeed = 0.05f;

    uint32_t lastTime = SDL_GetTicks();
    int frameCount = 0;

    while (running)
    {
        uint32_t startTicks = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
        const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
        if (keystate[SDL_SCANCODE_ESCAPE])
            running = false;

        models[1].rotation.y += rotateSpeed;
        mat3 camera_rotX_m, camera_rotY_m, camera_rotZ_m;
        std::tie(camera_rotX_m, camera_rotY_m, camera_rotZ_m) = GetRotationMatrices(camera_rotation);
        mat3 camera_orientation_world = camera_rotY_m * camera_rotX_m * camera_rotZ_m;
        float3 camera_right = {camera_orientation_world.m[0][0], camera_orientation_world.m[1][0], camera_orientation_world.m[2][0]};
        float3 camera_up = {camera_orientation_world.m[0][1], camera_orientation_world.m[1][1], camera_orientation_world.m[2][1]};
        float3 camera_forward = {camera_orientation_world.m[0][2], camera_orientation_world.m[1][2], camera_orientation_world.m[2][2]};
        float3 camera_look_direction = camera_forward;
        // Movement controls
        if (keystate[SDL_SCANCODE_W]) // Move forward
            camera_position = camera_position + camera_look_direction * moveSpeed;
        if (keystate[SDL_SCANCODE_S]) // Move backward
            camera_position = camera_position - camera_look_direction * moveSpeed;
        if (keystate[SDL_SCANCODE_A]) // Move left
            camera_position = camera_position - camera_right * moveSpeed;
        if (keystate[SDL_SCANCODE_D]) // Move right
            camera_position = camera_position + camera_right * moveSpeed;
        if (keystate[SDL_SCANCODE_SPACE]) // Move up
            camera_position = camera_position + camera_up * moveSpeed;
        if (keystate[SDL_SCANCODE_LSHIFT]) // Move down
            camera_position = camera_position - camera_up * moveSpeed;

        // Rotation controls
        if (keystate[SDL_SCANCODE_Q])
            camera_rotation.y -= rotateSpeed; // Yaw left
        if (keystate[SDL_SCANCODE_E])
            camera_rotation.y += rotateSpeed; // Yaw right
        if (keystate[SDL_SCANCODE_Z])
            camera_rotation.x -= rotateSpeed; // Pitch down
        if (keystate[SDL_SCANCODE_C])
            camera_rotation.x += rotateSpeed; // Pitch up
        // if (keystate[SDL_SCANCODE_R])
        //     camera_rotation.z -= rotateSpeed; // Roll
        // if (keystate[SDL_SCANCODE_F])
        //     camera_rotation.z += rotateSpeed;
        // Draw Loop

        render(frame_buffer, depth_buffer, models, camera_position, camera_rotation);
        SDL_UpdateTexture(screenTexture, nullptr, frame_buffer, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        frameCount++;
        uint32_t currentTime = SDL_GetTicks();
        if (currentTime - lastTime >= 1000)
        {
            float fps = frameCount * 1000.0f / (currentTime - lastTime);
            std::stringstream title;
            title << "3D Renderer - FPS: " << static_cast<int>(fps);
            SDL_SetWindowTitle(window, title.str().c_str());
            lastTime = currentTime;
            frameCount = 0;
        }

        // // Cap to ~60 FPS
        // uint32_t frameTicks = SDL_GetTicks() - startTicks;
        // if (frameTicks < 16)
        //     SDL_Delay(16 - frameTicks);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
