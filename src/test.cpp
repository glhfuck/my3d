#include <iostream>
#include <strstream>
#include <string>
#include <chrono>

#include "../libs/My3dLib/include/Shape.h"
#include "../libs/My3dLib/include/Camera.h"
#include "../libs/My3dLib/include/Lens.h"
#include "../libs/My3dLib/include/Rasterizer.h"
#include "OBJ_Parser.h"

#include "SDL2/SDL.h"

bool BACKFACECULL = true;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;

int windowCreate() {
  const size_t SCREEN_WIDTH = 1280;
  const size_t SCREEN_HEIGHT = 720;

//  const size_t SCREEN_WIDTH = 640;
//  const size_t SCREEN_HEIGHT = 400;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("my3d",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  int win_w, win_h;
  SDL_GetWindowSize(window, &win_w, &win_h);
  texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           win_w, win_h);

  Rasterizer raster(win_w, win_h);

  ublas::vector<double> cam_pos(3);
  cam_pos <<= 10, -23, 10;
  Camera camera(cam_pos);

  double FOV = 100;
  Lens lens(FOV, (double) win_w / win_h, 1, 100);

  //Shape ghost = OBJ_Parser::getShape("../ghost.obj");
  Shape ghost = OBJ_Parser::getShape("../earth.obj");
  //Shape ghost = OBJ_Parser::getShape("../tri.obj");
  //Shape ghost = OBJ_Parser::getShape("../tea.obj");

  Shape::Coords coord = Shape::Coords::Global;

  SDL_SetRelativeMouseMode(SDL_TRUE);
  const Uint8* key_states = SDL_GetKeyboardState(NULL);

  double trans_speed = 0.50;
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
      if (key_states[SDL_SCANCODE_B]) {
        BACKFACECULL = !BACKFACECULL;
        sleep(1);
      }
    }

    lens.SetFOV(FOV);

    camera.Move(x_trans, y_trans, z_trans);
    camera.Rotate(z_rot, x_rot, y_rot);

    raster.Clear();

    //SDL_RenderClear(renderer);

    raster.DrawShape(ghost, camera, lens);


    SDL_UpdateTexture(texture, NULL, raster.GetFrameBuffer(), win_w * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;

    std::strstream str;
    str << "FPS: " << (int)(1000 / duration.count());
    SDL_SetWindowTitle(window, str.str());
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
};


int main() {
  windowCreate();
}








//int SDL_SW_FillTriangle(SDL_Surface *dst, SDL_Point *d0, SDL_Point *d1, SDL_Point *d2)
//{
//  int ret = 0;
//
//  SDL_Rect dstrect;
//
//  int dstbpp;
//  Uint8 *dst_ptr;
//  int dst_pitch;
//
//  int area, is_clockwise;
//
//  int d2d1_y, d1d2_x, d0d2_y, d2d0_x, d1d0_y, d0d1_x;
//  int w0_row, w1_row, w2_row;
//  int bias_w0, bias_w1, bias_w2;
//
//  area = cross_product(d0, d1, d2->x, d2->y);
//
//  /* Flat triangle */
//  if (area == 0) {
//    return 0;
//  }
//
//  bounding_rect_fixedpoint(d0, d1, d2, &dstrect);
//
//  {
//    /* Clip triangle rect with surface rect */
//    SDL_Rect rect;
//    rect.x = 0;
//    rect.y = 0;
//    rect.w = dst->w;
//    rect.h = dst->h;
//    SDL_IntersectRect(&dstrect, &rect, &dstrect);
//  }
//
//  {
//    /* Clip triangle with surface clip rect */
//    SDL_Rect rect;
//    SDL_GetClipRect(dst, &rect);
//    SDL_IntersectRect(&dstrect, &rect, &dstrect);
//  }
//
//  }
//    /* Write directly to destination surface */
//    dstbpp = dst->format->BytesPerPixel;
//    dst_ptr = (Uint8 *)dst->pixels + dstrect.x * dstbpp + dstrect.y * dst->pitch;
//    dst_pitch = dst->pitch;
//  }
//
//  is_clockwise = area > 0;
//  area = SDL_abs(area);
//
//  d2d1_y = (d1->y - d2->y) << FP_BITS;
//  d0d2_y = (d2->y - d0->y) << FP_BITS;
//  d1d0_y = (d0->y - d1->y) << FP_BITS;
//  d1d2_x = (d2->x - d1->x) << FP_BITS;
//  d2d0_x = (d0->x - d2->x) << FP_BITS;
//  d0d1_x = (d1->x - d0->x) << FP_BITS;
//
//  /* Starting point for rendering, at the middle of a pixel */
//  {
//    SDL_Point p;
//    p.x = dstrect.x;
//    p.y = dstrect.y;
//    trianglepoint_2_fixedpoint(&p);
//    p.x += (1 << FP_BITS) / 2;
//    p.y += (1 << FP_BITS) / 2;
//    w0_row = cross_product(d1, d2, p.x, p.y);
//    w1_row = cross_product(d2, d0, p.x, p.y);
//    w2_row = cross_product(d0, d1, p.x, p.y);
//  }
//
//  /* Handle anti-clockwise triangles */
//  if (! is_clockwise) {
//    d2d1_y *= -1;
//    d0d2_y *= -1;
//    d1d0_y *= -1;
//    d1d2_x *= -1;
//    d2d0_x *= -1;
//    d0d1_x *= -1;
//    w0_row *= -1;
//    w1_row *= -1;
//    w2_row *= -1;
//  }
//
//  /* Add a bias to respect top-left rasterization rule */
//  bias_w0 = (is_top_left(d1, d2, is_clockwise) ? 0 : -1);
//  bias_w1 = (is_top_left(d2, d0, is_clockwise) ? 0 : -1);
//  bias_w2 = (is_top_left(d0, d1, is_clockwise) ? 0 : -1);
//
//
//  Uint32 color;
//
//  int x, y;
//  for (y = 0; y < dstrect.h; y++) {
//    /* y start */
//    int w0 = w0_row;
//    int w1 = w1_row;
//    int w2 = w2_row;
//    for (x = 0; x < dstrect.w; x++) {
//      /* In triangle */
//      if (w0 + bias_w0 >= 0 && w1 + bias_w1 >= 0 && w2 + bias_w2 >= 0) {
//        Uint8 *dptr = (Uint8 *) dst_ptr + x * dstbpp;
//          {
//            *(Uint32 *)dptr = color;
//          }
//      }
//      /* x += 1 */
//      w0 += d2d1_y;
//      w1 += d0d2_y;
//      w2 += d1d0_y;
//    }
//    /* y += 1 */
//    w0_row += d1d2_x;
//    w1_row += d2d0_x;
//    w2_row += d0d1_x;
//    dst_ptr += dst_pitch;
//  }
//
//  return ret;
//}

















