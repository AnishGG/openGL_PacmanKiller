#include "main.h"
#include "timer.h"
#include "ball.h"
#include "rectangle.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1, ball2, ball[100];
vector<pair<Ball, Rectangle> > balls;
vector<pair<Ball, Rectangle> >::iterator j;
bool my_collision(Ball a, Ball b);

Rectangle ground, grass, rec[100];
int i = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 120);
Timer t1(1.0 / 1.0);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball2.draw(VP);
    grass.draw(VP);
    ground.draw(VP);
    for(j = balls.begin(); j < balls.end(); j++){
        j.base()->first.draw(VP);
        if(j.base()->first.isSlabAttached)
            j.base()->second.draw(VP);
    }
    ball1.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if(left && right)
        ball1.speed_x = 0;
    else if (left) {
        ball1.speed_x = -0.04;
    }
    else if (right){
        ball1.speed_x = +0.04;
    }
    else
        ball1.speed_x = 0;
    if(up){
        ball1.jump();
    }
}

void tick_elements() {
    ball1.tick();
    for(j = balls.begin(); j < balls.end(); j++){
        j.base()->first.tick();
        if(j.base()->first.isSlabAttached)
            j.base()->second.tick();
        /* Detect collision with the Player */
        if(my_collision(ball1, j.base()->first))
            ball1.speed_y = 0.20;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models


    ball1       = Ball(2, -1.8, COLOR_PLAYER, 0.2);
    ball2       = Ball(-2, 3, COLOR_GREEN, 0.2);

    /* Ground Coordinates */
    Point p1, p2, p3, p4, p5, p6, p7, p8;
    p1.x = -4.0; p1.y = -4.0;
    p2.x = -4.0; p2.y = -2.5;
    p3.x =  4.0; p3.y = -4.0;
    p4.x =  4.0; p4.y = -2.5;
    /* Green Coordinates */
    p5 = p2;
    p6 = p4;
    p7.x = -4.0; p7.y = -2.0;
    p8.x =  4.0; p8.y = -2.0;
    /***********************************/
    grass       =     Rectangle(p7, p8, p5, p6, COLOR_GREEN);
    ground      =     Rectangle(p1, p2, p3, p4, COLOR_LIGHT_RED);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

                // Process timers

        if (t60.processTick()) {
            // 60 fps
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }
        if(t1.processTick()){
            /* Section for Random balls */
            double random_number = ((rand() % 5) - 1) + (double)rand() / (double)((unsigned)RAND_MAX + 1); // integer + decimal
            double decimal_part = (double)((double)rand() / (double)((unsigned)RAND_MAX + 1)) / 33;
            decimal_part += 0.01;
            double Radius = (double)((rand() % 3) + 1) / 10;
            color_t colors[] = {COLOR_BLACK, COLOR_GREEN, COLOR_RED, COLOR_WHITE};
            Ball temp;
            Rectangle temp1;
            temp = Ball(-5, random_number, colors[rand() % 4], Radius);
            temp.speed_x = decimal_part;
            if(i % 4 == 0){
                Point *p = new Point[4];
                temp.attach_slab(p, 235, 2 * Radius, 0.05);
                temp1 = Rectangle(p[0], p[1], p[2], p[3], COLOR_GREEN);
                temp1.set_speed(decimal_part);
            }
            j = balls.begin();
            balls.insert(j, make_pair(temp, temp1));
            /***** Section ends here *****/
            i++;
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

/* Tells whether the player a collides with b according to the defined rules */
bool my_collision(Ball a, Ball b){
    double R_b, R_a, x_a, x_b, y_a, y_b, R, distance_bw_centers;
    bool collided = 0;
    x_a = a.position.x;
    x_b = b.position.x;
    y_a = a.position.y;
    y_b = b.position.y;
    R_b = b.Radius;
    R_a = a.Radius;
    distance_bw_centers = abs(sqrt((x_a - x_b)*(x_a - x_b) + (y_a - y_b)*(y_a - y_b)));
    if(distance_bw_centers <= R_b)
    {
        collided = 1;
    }
    if((distance_bw_centers <= R_b + R_a) && (y_a >= y_b) /*&& (x_a <= x_b + R_b) && (x_a >= x_b - R_b)*/)
        collided = 1;
    if(a.speed_y > 0.0)
        collided = 0;
    return collided;
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}