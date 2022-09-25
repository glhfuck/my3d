#include <iostream>
#include <string>
#include <chrono>

#include "../libs/My3dLib/include/Shape.h"
#include "../libs/My3dLib/include/Camera.h"
#include "../libs/My3dLib/include/Lens.h"
#include "../libs/My3dLib/include/Rasterizer.h"
#include "OBJ_Parser.h"

#include "SDL2/SDL.h"

int windowCreate() {
  const size_t SCREEN_WIDTH = 1280;
  const size_t SCREEN_HEIGHT = 800;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("my3d",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  int win_w, win_h;
  SDL_GetWindowSize(window, &win_w, &win_h);
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           win_w, win_h);

  Rasterizer raster(win_w, win_h);

  ublas::vector<double> cam_pos(3);
  cam_pos <<= 20, -20, 15;
  Camera camera(cam_pos);

  double FOV = 90;
  Lens lens(FOV, (double) win_w / win_h, 1, 100);

  Shape ghost = OBJ_Parser::getShape("../ghost.obj");

  Shape::Coords coord = Shape::Coords::Global;

  SDL_SetRelativeMouseMode(SDL_TRUE);
  const Uint8* key_states = SDL_GetKeyboardState(NULL);

  double trans_speed = 0.500;
  double sens = 0.002;

  for (;;) {
    auto start = std::chrono::high_resolution_clock::now();

    double x_trans = 0;
    double y_trans = 0;
    double z_trans = 0;

    double x_rot = 0;
    double y_rot = 0;
    double z_rot = 0;

    int x = 0;
    int y = 0;

    SDL_PumpEvents();
    SDL_GetRelativeMouseState(&x, &y);
    z_rot -= x * sens;
    x_rot -= y * sens;

    {
      if (key_states[SDL_SCANCODE_ESCAPE]) {
        break;
      }
      if (key_states[SDL_SCANCODE_A]) {
        x_trans -= trans_speed;
      }
      if (key_states[SDL_SCANCODE_D]) {
        x_trans += trans_speed;
      }
      if (key_states[SDL_SCANCODE_S]) {
        y_trans -= trans_speed;
      }
      if (key_states[SDL_SCANCODE_W]) {
        y_trans += trans_speed;
      }
      if (key_states[SDL_SCANCODE_LCTRL]) {
        z_trans -= trans_speed;
      }
      if (key_states[SDL_SCANCODE_SPACE]) {
        z_trans += trans_speed;
      }
      if (key_states[SDL_SCANCODE_MINUS]) {
        --FOV;
      }
      if (key_states[SDL_SCANCODE_EQUALS]) {
        ++FOV;
      }
      if (key_states[SDL_SCANCODE_G]) {
        coord = Shape::Coords::Global;
      }
      if (key_states[SDL_SCANCODE_L]) {
        coord = Shape::Coords::Local;
      }
      if (key_states[SDL_SCANCODE_1]) {
        raster.SetRenderingMode(Rasterizer::RenderingMode::Facets);
      }
      if (key_states[SDL_SCANCODE_2]) {
        raster.SetRenderingMode(Rasterizer::RenderingMode::Edges);
      }
      if (key_states[SDL_SCANCODE_3]) {
        raster.SetRenderingMode(Rasterizer::RenderingMode::Vertices);
      }
      if (key_states[SDL_SCANCODE_O]) {
        raster.SetFillingMode(Rasterizer::FillingMode::Colored);
      }
      if (key_states[SDL_SCANCODE_P]) {
        raster.SetFillingMode(Rasterizer::FillingMode::Depth);
      }
    }

    lens.SetFOV(FOV);

    camera.Move(x_trans, y_trans, z_trans);
    camera.Rotate(z_rot, x_rot, y_rot);

    raster.Clear();

    raster.DrawShape(ghost, camera, lens);

    SDL_UpdateTexture(texture, NULL, raster.GetFrameBuffer(), win_w * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;
    std::cout << duration.count() << std::endl;
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
};

int main() {
  windowCreate();
}
