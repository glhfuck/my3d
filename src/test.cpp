#include <iostream>
#include <string>
#include <chrono>

#include "../libs/My3dLib/include/Shape.h"
#include "../libs/My3dLib/include/Camera.h"
#include "../libs/My3dLib/include/Lens.h"
#include "../libs/My3dLib/include/Rasterizer.h"
#include "../libs/My3dLib/include/Obj_loader.h"

#include "SDL2/SDL.h"

int windowCreate () {
  const size_t SCREEN_WIDTH = 1280;
  const size_t SCREEN_HEIGHT = 800;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("my3d",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);


  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);


  if(!renderer) {
    fprintf(stderr, "Could not create renderer\n");
    return 1;
  }

  int win_w, win_h;
  SDL_GetWindowSize(window, &win_w, &win_h);
  std::cout << win_w << ' ' << win_h << std::endl;

  int gw, gh;
  SDL_GL_GetDrawableSize(window, &gw, &gh);
  std::cout << gw << ' ' << gh << std::endl;


  SDL_Texture* texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           win_w, win_h);

  Rasterizer raster(win_w, win_h);

  Shape ghost = Obj_loader::getShape("../pantasma02.obj");
  //Shape ghost = Obj_loader::getShape("../untitled-scene.obj");

  ublas::vector<double> cam_pos(3);
  //cam_pos <<= 5, 5, 5;
  cam_pos <<= 20, 20, 20;
  //cam_pos <<= 45, 45, 45;
  Camera camera(cam_pos);

  Lens screen(80, (double) win_w / win_h, 18, 30);

  Transformable::Axes axis = Transformable::Axes::Ox;
  Transformable::Coords coord = Transformable::Coords::Global;

  bool quit = false;
  while (!quit) {
    auto start = std::chrono::high_resolution_clock::now();
    SDL_Event e;

    ublas::vector<double> translate_to(3);
    translate_to <<= 0, 0, 0;
    double speed = 0.100;
    int sign = 0;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_w) {
          translate_to[0] += speed;
        }
        if (e.key.keysym.sym == SDLK_s) {
          translate_to[0] -= speed;
        }
        if (e.key.keysym.sym == SDLK_a) {
          translate_to[1] -= speed;
        }
        if (e.key.keysym.sym == SDLK_d) {
          translate_to[1] += speed;
        }
        if (e.key.keysym.sym == SDLK_TAB) {
          translate_to[2] -= speed;
        }
        if (e.key.keysym.sym == SDLK_SPACE) {
          translate_to[2] += speed;
        }
        if (e.key.keysym.sym == SDLK_q) {
          sign -= 1;
        }
        if (e.key.keysym.sym == SDLK_e) {
          sign += 1;
        }
        if (e.key.keysym.sym == SDLK_z) {
          axis = Transformable::Axes::Ox;
        }
        if (e.key.keysym.sym == SDLK_x) {
          axis = Transformable::Axes::Oy;
        }
        if (e.key.keysym.sym == SDLK_c) {
          axis = Transformable::Axes::Oz;
        }
        if (e.key.keysym.sym == SDLK_g) {
          coord = Transformable::Coords::Global;
        }
        if (e.key.keysym.sym == SDLK_l) {
          coord = Transformable::Coords::Local;
        }
        if (e.key.keysym.sym == SDLK_v) {
          raster.setRenderingMode(Rasterizer::RenderingMode::Facets);
        }
        if (e.key.keysym.sym == SDLK_b) {
          raster.setRenderingMode(Rasterizer::RenderingMode::Edges);
        }
        if (e.key.keysym.sym == SDLK_n) {
          raster.setRenderingMode(Rasterizer::RenderingMode::Vertices);
        }
        if (e.key.keysym.sym == SDLK_o) {
          raster.setFillingMode(Rasterizer::FillingMode::Colored);
        }
        if (e.key.keysym.sym == SDLK_p) {
          raster.setFillingMode(Rasterizer::FillingMode::Depth);
        }
      }

      if (e.type == SDL_MOUSEBUTTONDOWN) {
        quit = true;
      }
    }

    ghost.translate(translate_to, coord);
    ghost.rotate(sign * 0.5, axis, coord);

    raster.clear();

    raster.drawShape(ghost, camera, screen);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;
    std::cout << duration.count() << std::endl;

    SDL_UpdateTexture(texture , NULL, raster.getFrameBuffer(), win_w * sizeof (uint32_t));
    SDL_RenderCopy(renderer, texture , NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
};

int main() {
  windowCreate();
}
