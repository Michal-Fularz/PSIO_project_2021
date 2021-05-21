#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <GL/glu.h> // Windows/Linux

#include "planet.h"

void draw_cube(double size) {
    double half_cube_size = size / 2.0;

    // bottom
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // top
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glEnd();

    // left
    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // right
    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // front
    glColor3d(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // back
    glColor3d(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, -half_cube_size);
    glEnd();
}

void set_viewport(int width, int height) {
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    gluLookAt(0, -5, 5, 0, 0, 0, 0, 0, 1);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
}



void ex_1()
{
    // create the window
    sf::Window window(sf::VideoMode(1024, 768), "SFML OpenGL Template", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // set viewport according to current window size
    set_viewport(window.getSize().x, window.getSize().y);

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);

    // setup lights

    GLfloat light_position[] = { 2.0, 0.0, 2.0, 1.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    GLfloat global_ambient[] = {0.3, 0.3, 0.3, 0.1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_NORMALIZE) ;

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;

    sf::Clock clk;

    float camera_x = 0.0;
    float camera_z = 0.0;
    float camera_zoom = 0.0;

    while (running) {
        // EVENTS
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // end the program
                running = false;
            } else if (event.type == sf::Event::Resized) {
                // adjust the viewport when the window is resized
                set_viewport(event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) {
                    camera_z += 0.2;
                }
                else if (event.key.code == sf::Keyboard::S) {
                    camera_z -= 0.2;
                }
                if (event.key.code == sf::Keyboard::A) {
                    camera_x += 0.2;
                }
                else if (event.key.code == sf::Keyboard::D) {
                    camera_x -= 0.2;
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                camera_zoom += event.mouseWheelScroll.delta;
            }
        }

        // LOGIC
        float rot = clk.getElapsedTime().asSeconds() * 90;

        // DISPLAY

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
        glEnable (GL_COLOR_MATERIAL);

        // draw stuff

        glPushMatrix();
        {
            std::cout << camera_zoom << std::endl;
            glScalef(1.0 + camera_zoom/100.0, 1.0 + camera_zoom/100.0, 1.0 + camera_zoom/100.0);
            glTranslatef(camera_x, 0.0, camera_z);

            glPushMatrix();
            {
                glRotated(rot/10, 0.0, 0.0, 1.0);
                glTranslated(3.0, 3.0, 0.0);
                glScaled(0.5, 0.5, 0.5);

                glPushMatrix();
                {
                    // TODO
                    // test functions below (glTranslated, glRotated, glColor3d)
                    // what happens when you change their arguments?
                    // does their order change the result?

                    glTranslated(1.0, 0.0, 0.0);
                    //glRotated(0, 1.0, 0.0, 0.0);
                    //glRotated(rot, 0.0, 1.0, 0.0);
                    glRotated(rot, 0.0, 0.0, 1.0);


                    glScaled(0.5, 1.0, 0.25);

                    draw_cube(1.0);
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glRotated(rot/3, 0.0, 0.0, 1.0);
                    glTranslated(-2.0, 0.0, 0.0);
                    glRotated(rot/2, 0.0, 1.0, 0.0);
                    draw_cube(0.5);
                }
                glPopMatrix();

                glPushMatrix();
                {
                    glRotated(rot/3, 1.0, 0.0, 0.0);
                    draw_cube(0.3);
                }
                glPopMatrix();
            }
            glPopMatrix();

            glPushMatrix();
            {
                draw_cube(2.0);
            }
            glPopMatrix();
        }
        glPopMatrix();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}

void ex_2()
{
    sf::Window window(sf::VideoMode(1024, 768), "SFML OpenGL Template", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    // set viewport according to current window size
    set_viewport(window.getSize().x, window.getSize().y);

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);

    // setup lights
    GLfloat light_position[] = { 2.0, 0.0, 2.0, 1.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    GLfloat global_ambient[] = {0.3, 0.3, 0.3, 0.1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_NORMALIZE) ;

    // load resources, initialize the OpenGL states, ...

    std::vector<Planet> objects;
    // objects.push_back(Planet(2.0, 0.0, 5.0, 0.0)); //double diameter = 1.0, double distance = 0.0, double spin_period = 0.0, double rotation_period = 0.0

    // load planets
    std::fstream solar_system_file("./../s01e10/solar_system.txt");
    if (!solar_system_file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(solar_system_file, line)) {
        if (!line.empty() && line[0] != '#') {
            std::stringstream line_str(line);

            std::string name;
            double distance;
            double diameter;
            double spin_period;
            double orbit_period;
            double gravity;
            int number_of_moons;
            double r;
            double g;
            double b;
            line_str >> name >> distance >> diameter >> spin_period >> orbit_period >> gravity >> number_of_moons >> r >> g >> b;
            std::cout << name << " " << diameter << std::endl;

            objects.push_back(Planet(diameter, distance, spin_period, orbit_period));
            objects.back().set_color(r/255.0, g/255.0, b/255.0);
        }
    }

    // run the main loop
    bool running = true;
    sf::Clock clk;
    while (running) {
        double delta_t = clk.getElapsedTime().asSeconds();
        clk.restart();

        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // end the program
                running = false;
            } else if (event.type == sf::Event::Resized) {
                // adjust the viewport when the window is resized
                set_viewport(event.size.width, event.size.height);
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                std::cerr << event.mouseWheelScroll.delta <<std::endl;
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
        glEnable (GL_COLOR_MATERIAL);

        // draw stuff

        glPushMatrix();

        glScaled(1.0/1500, 1.0/1500, 1.0/1500);

        for (auto &p : objects) {
            p.step(delta_t);
            p.draw();
        }

        glPopMatrix();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}


int main()
{
    std::cout << "Hello s01e09!" << std::endl;

    //ex_1();
    ex_2();

    return 0;
}
