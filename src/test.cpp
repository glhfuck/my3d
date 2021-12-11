#include <iostream>
#include "../libs/My3dLib/include/Shape.h"
#include "../libs/My3dLib/include/Camera.h"
#include "../libs/My3dLib/include/Screen.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Point {
 public:
  Point(double x, double y, double z): x(x), y(y), z(z) {}

  operator ublas::vector<double>() {
    ublas::vector<double> vec(4);
    vec <<= x, y, z, 1;
    return vec;
  }

  double x;
  double y;
  double z;
};

class Cube: public Shape {
 public:

  explicit Cube(const ublas::vector<double>& pos = ublas::zero_vector<double>(3),
                double l = 1): Shape(pos) {
    vertices_count_ = 8;

    vertices_ = {
        Point(0, 0, 0),
        Point(0, 0, l),
        Point(0, l, 0),
        Point(0, l, l),
        Point(l, 0, 0),
        Point(l, 0, l),
        Point(l, l, 0),
        Point(l, l, l)
    };
  }

  void draw(sf::RenderWindow& window, ublas::matrix<double>& VP) {
    ublas::matrix<double> MVP = ublas::prod(VP, M);

    std::vector<sf::RectangleShape> res_vector;
    res_vector.reserve(vertices_count_);

    for (auto& vertex : vertices_) {
      ublas::vector<double> res = ublas::prod(MVP, vertex);
      res_vector.emplace_back(sf::Vector2f(3,3)).setPosition((res[0] / res[3] + 1) / 2 * window.getSize().x,
                                                             (-res[2] / res[3] + 1) / 2 * window.getSize().y);
    }

    for(auto& res : res_vector) {
      window.draw(res);
    }
  }
};

class Axes: public Shape {
 public:

  explicit Axes(const ublas::vector<double>& pos = ublas::zero_vector<double>(3),
                double l = 1): Shape(pos) {
    vertices_ = {
        Point(0, 0, 0),
        Point(0, 0, l),
        Point(0, l, 0),
        Point(l, 0, 0),
    };
  }

  void draw(sf::RenderWindow& window, ublas::matrix<double>& VP) {
    ublas::matrix<double> MVP = ublas::prod(VP, M);

    sf::VertexArray res_arr (sf::Lines, vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      ublas::vector<double> res = ublas::prod(MVP, vertices_[i]);
      res_arr[i] = sf::Vector2f(((res[0] / res[3] + 1) / 2 * window.getSize().x),
                                (-res[2] / res[3] + 1)  / 2 * window.getSize().y);
    }

    sf::Vertex Ox[] =
        {
            res_arr[0],
            res_arr[1]
        };

    Ox[0].color = sf::Color::Red;
    Ox[1].color = sf::Color::Red;

    sf::Vertex Oy[] =
        {
            res_arr[0],
            res_arr[2]
        };

    Oy[0].color = sf::Color::Green;
    Oy[1].color = sf::Color::Green;

    sf::Vertex Oz[] =
        {
            res_arr[0],
            res_arr[3]
        };

    Oz[0].color = sf::Color::Blue;
    Oz[1].color = sf::Color::Blue;

    window.draw(Ox, 2, sf::Lines);
    window.draw(Oy, 2, sf::Lines);
    window.draw(Oz, 2, sf::Lines);
  }
};

int main()
{
  int windowWidth = 1920;
  int windowHeight = 1200;

  ublas::vector<double> cube_pos(3);
  cube_pos <<= 1, 1, 0;
  Cube cube(cube_pos);

  ublas::vector<double> cam_pos(3);
  cam_pos <<= 2.2, 4, 0.5;

  ublas::vector<double> look_at(3);
  look_at = cube_pos + ublas::scalar_vector<double>(3,0.5);
  Camera camera(cam_pos, look_at);
  Screen screen(90, (float)windowWidth / windowHeight);

  Axes axes;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "my3d", sf::Style::Default, settings);

  while (window.isOpen()) {
    ublas::matrix<double> VP = screen.P;
    VP = ublas::prod(VP, camera.V);

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    cube.draw(window, VP);
    axes.draw(window, VP);
    window.display();

    ublas::vector<double> translate_to(3);
    translate_to <<= 0, 0, 0;

    {
      double speed = 8;
      speed /= 10000;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        translate_to[1] -= speed;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        translate_to[1] += speed;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        translate_to[0] -= speed;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        translate_to[0] += speed;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        translate_to[2] -= speed;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        translate_to[2] += speed;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
      }
      cube.translate(translate_to);

      double x_scale = 1;
      double y_scale = 1;
      double z_scale = 1;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
        x_scale -= 0.001;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
        x_scale += 0.001;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
        y_scale -= 0.001;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        y_scale += 0.001;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
        z_scale -= 0.001;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        z_scale += 0.001;
      }
      cube.scale(x_scale, y_scale, z_scale);

      int sign = 0;
      Cube::Axes axis;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        sign -= 1;
        axis = Axes::Ox;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        sign += 1;
        axis = Axes::Ox;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        sign -= 1;
        axis = Axes::Oy;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
        sign += 1;
        axis = Axes::Oy;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
        sign -= 1;
        axis = Axes::Oz;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
        sign += 1;
        axis = Axes::Oz;
      }
      if (sign != 0) {
        cube.rotate(sign * 0.05, axis, Cube::Coords::Local);
      }
    }

    //cube_pos[1] -= 0.0001;
    //cube.setPosition(cube_pos);
    //cube.translate(Point(-0.0001, -0.0001,0));
    //cube.localOxRotate(0.05);
    //cube.globalOzRotate(0.05);


    //sf::sleep(sf::seconds(1));

  }
  return EXIT_SUCCESS;
}
