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
  //Shape static_axes = Obj_loader::getShape("../untitled-scene.obj");
  Shape static_axes = Obj_loader::getShape("../axes.obj");
  //Shape dynamic_axes = Obj_loader::getShape("../axes.obj");

  ublas::vector<double> cam_pos(3);
  cam_pos <<= 5, -5, 5;
  Camera camera(cam_pos);

  double FOV = 120;
  Lens lens(FOV, (double) win_w / win_h, 1, 30);

  Shape::Coords coord = Shape::Coords::Global;
  int direction = 1;

  bool quit = false;
  while (!quit) {
    auto start = std::chrono::high_resolution_clock::now();
    SDL_Event e;

    double trans_speed = 0.100;
    double rot_speed = 0.02;

    double x_trans = 0;
    double y_trans = 0;
    double z_trans = 0;

    double x_rot = 0;
    double y_rot = 0;
    double z_rot = 0;


    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_a) {
          x_trans -= trans_speed;
        }
        if (e.key.keysym.sym == SDLK_d) {
          x_trans += trans_speed;
        }
        if (e.key.keysym.sym == SDLK_s) {
          y_trans -= trans_speed;
        }
        if (e.key.keysym.sym == SDLK_w) {
          y_trans += trans_speed;
        }
        if (e.key.keysym.sym == SDLK_TAB) {
          z_trans -= trans_speed;
        }
        if (e.key.keysym.sym == SDLK_SPACE) {
          z_trans += trans_speed;
        }
        if (e.key.keysym.sym == SDLK_u) {
          --FOV;
        }
        if (e.key.keysym.sym == SDLK_i) {
          ++FOV;
        }
        if (e.key.keysym.sym == SDLK_q) {
          direction = 1;
        }
        if (e.key.keysym.sym == SDLK_e) {
          direction = -1;
        }
        if (e.key.keysym.sym == SDLK_z) {
          x_rot += rot_speed * direction;
        }
        if (e.key.keysym.sym == SDLK_x) {
          y_rot += rot_speed * direction;
        }
        if (e.key.keysym.sym == SDLK_c) {
          z_rot += rot_speed * direction;
        }
        if (e.key.keysym.sym == SDLK_g) {
          coord = Shape::Coords::Global;
        }
        if (e.key.keysym.sym == SDLK_l) {
          coord = Shape::Coords::Local;
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

      lens.SetFOV(FOV);

      camera.Move(x_trans, y_trans, z_trans);
      camera.Rotate(z_rot, x_rot, y_rot);
      camera.GetParam();
//      dynamic_axes.Translate(x_trans, y_trans, z_trans, coord);
//      dynamic_axes.Rotate(x_rot, y_rot, z_rot, coord);

      raster.clear();

      raster.drawShape(static_axes, camera, lens);
      raster.drawShape(ghost, camera, lens);

      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float, std::milli> duration = end - start;
      std::cout << duration.count() << std::endl;

      SDL_UpdateTexture(texture , NULL, raster.getFrameBuffer(), win_w * sizeof (uint32_t));
      SDL_RenderCopy(renderer, texture , NULL, NULL);
      SDL_RenderPresent(renderer);

    }

    //static_axes.Translate(translate_to, coord);
    //camera.
   // static_axes.Rotate(sign * 0.5, axis, coord);

  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
};

int main() {
  windowCreate();
}
