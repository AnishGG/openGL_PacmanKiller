#include "main.h"
#include "timer.h"
#include "ellipse.h"
#include "magnet.h"
#include "porcupines.h"
#include "score.h"
#include "digit.h"
#include "level.h"
#include "display.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball Player;
vector<pair <Ball, Rectangle> > balls;
vector<pair <Ball, Rectangle> >::iterator j;

Rectangle ground, grass, tramp_one, tramp_two;
Ellipse pond, pond_frame, trampoline;
Magnet mag1, mag2;
Porcupine porpine[10];
Score score;
Digit digit[10];
Level level;
Display display;
int count_enemies = 0;

float screen_zoom = 0.9, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);
Timer t1(1.0 / 1.0);
int game_timer = 40;

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
    mag1.draw(VP);
    mag2.draw(VP);
    grass.draw(VP);
    ground.draw(VP);
    for(int ii = 0;ii < 4; ii++)
        porpine[ii].draw(VP);
    pond_frame.draw(VP);
    pond.draw(VP);
    for(j = balls.begin(); j < balls.end(); j++){
        j.base()->first.draw(VP);
        if(j.base()->first.isSlabAttached)
            j.base()->second.draw(VP);
    }
    tramp_one.draw(VP);
    tramp_two.draw(VP);
    trampoline.draw(VP);
    Player.draw(VP);
    char test[10];
    sprintf(test, "%d", game_timer);
    display.store(test, 3.6, 3.8);
    display.draw(VP);
}

void tick_input(GLFWwindow *window) {
    glfwSetWindowTitle(window, level.get_string(score));
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int pan_left = glfwGetKey(window, GLFW_KEY_LEFT);
    int pan_right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    if(left && right)
        Player.speed_x = 0;
    else if (left) {
        Player.speed_x = -BALL_SPEED;
    }
    else if (right){
        Player.speed_x = +BALL_SPEED;
    }
    else if(!Player.jumped || Player.drowned)
        Player.speed_x = 0;

    if(up && !Player.jumped){
        Player.jump();
    }

    /* Checking the panning */
    if(pan_left && pan_right)
        ; // Do nothing
    else if(pan_left && screen_center_x > (-3.8 * screen_zoom))
        screen_center_x -= 0.1;
    else if(pan_right && screen_center_x < (3.8 * screen_zoom))
        screen_center_x += 0.1;
    reset_screen();
    /***********************/

    if(level.get_level() > 3){   cout << "You win the game" << endl; quit(window);}
    if(game_timer <= 0){
        cout << "Sorry, You loose" << endl;
        quit(window);
    }
    if(level.get_level_up_score() <= score.get_score()){
        balls.clear();                //destroy_all_enemies()
        level.level_up();
        score.subtract(score.get_score());
        game_timer = 40;
    }
}

void tick_elements() {
    bool jumped_at_theta = 0;
    bool isCollision = 0;
    int count_collisions = 0;
    Player.tick(pond, grass);
    for(j = balls.begin(); j < balls.end(); j++){

        j.base()->first.tick(pond, grass); // first is the ball
        if(j.base()->first.isSlabAttached)
            j.base()->second.tick();    // second is the slab attached

        /* Detect collision with the Player */
        if(Player.my_collision(j.base()->first)){
            if(j.base()->first.isSlabAttached){
                Player.jump_theta(j.base()->second.theta);
                jumped_at_theta = 1;
            }
            balls.erase(j);
            isCollision = 1;
            if(jumped_at_theta){
                score.add(5);
                break;
            }
            count_collisions++;
        }
        /************************************/

    }
    if(isCollision && !jumped_at_theta){
        Player.jump();
        score.add(3*count_collisions);
    }
    /* To bind the ball inside the screen */
    if(Player.position.x > SCREEN_X_MAX)
        Player.position.x = SCREEN_X_MAX;
    else if(Player.position.x < SCREEN_X_MIN)
        Player.position.x = SCREEN_X_MIN;
    if(Player.position.y > SCREEN_Y_MAX){
        Player.position.y = SCREEN_Y_MAX;
        Player.speed_y = -0.01;  // To remove the sticking of the ball on the top of the screen and a downward push is given to the ball
    }
    /**************************************/

    /* Check if the ball lands on the trampoline */
    if(Player.position.x > trampoline.x_min && Player.position.x < trampoline.x_max){
        if(Player.position.y - Player.Radius <= trampoline.position.y && Player.speed_y < 0){
            Player.speed_y = TRAMPOLINE_PUSH;
        }
    }

    /* Check if the ball is in the magnetic region and reduce it's speed*/
    mag1.detect_in_range(&Player);
    mag2.detect_in_range(&Player);

    /* For ticking the porcupines */
    for(int ii = 0;ii < 4; ii++){
        porpine[ii].tick();
        porpine[ii].detect_collision(Player, &score);
        if(level.get_level() < 3)   porpine[ii].set_validity(0);
    }
    /*****************************/

    /********************** Automatic Panning Section *********************/
    if(Player.position.x > (screen_center_x + 3.6 / screen_zoom)){
        screen_center_x += 0.1;
    }
    else if(Player.position.x < (screen_center_x - 3.6 / screen_zoom)){
        screen_center_x -= 0.1;
    }
    if(Player.position.y > (screen_center_y + 3.6 / screen_zoom)){
        screen_center_y += 0.1;
    }
    else if(Player.position.y < (screen_center_y - 2.6 / screen_zoom)){
        screen_center_y -= 0.1;
    }
    reset_screen();
    /********************** Section Ends here *******************************/
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    /* Initialising the Player */
    Player       = Ball(2, 2.0, COLOR_PLAYER, 0.3, 1);
    Player.jumped = 1;
    /***************************/

    /* Ground Coordinates */
    Point p1, p2, p3, p4, p5, p6, p7, p8;
    p1.x = -16.0; p1.y = -8.0;
    p2.x = -16.0; p2.y = -2.5;
    p3.x =  16.0; p3.y = -8.0;
    p4.x =  16.0; p4.y = -2.5;
    /* Green Coordinates */
    p5 = p2;
    p6 = p4;
    p7.x = -16.0; p7.y = -2.0;
    p8.x =  16.0; p8.y = -2.0;
    /***********************************/
    grass       =     Rectangle(p7, p8, p5, p6, COLOR_GREEN);
    ground      =     Rectangle(p1, p2, p3, p4, COLOR_LIGHT_RED);
    pond        =     Ellipse(0, -2.0, COLOR_WATER, 1.0, 0.7);
    pond_frame  =     Ellipse(0, -2.0, COLOR_WATER, 1.0 + Player.Radius, 0.7 + Player.Radius);

    /* The making of the Trampoline */
    /* Using p1, p2, p3, p4 again for trampoline co-ordinates for the first support of Trampoline */
    p1.x =  2.1; p1.y = -2.0;
    p2.x =  2.2; p2.y = -2.0;
    p3.x =  2.1; p3.y = -1.5;
    p4.x =  2.2; p4.y = -1.5;
    tramp_one   =     Rectangle(p1, p2, p3, p4, COLOR_RED);

    /* Using those co-ordinates again for the second support of the Trampoline*/
    p1.x =  2.9; p1.y = -2.0;
    p2.x =  3.0; p2.y = -2.0;
    p3.x =  2.9; p3.y = -1.5;
    p4.x =  3.0; p4.y = -1.5;
    tramp_two   =     Rectangle(p1, p2, p3, p4, COLOR_RED);

    trampoline = Ellipse(2.55, -1.5, COLOR_RED, 0.7 / 2, 0.2);
    /********************************/

    /* Creating magnets */
    // Boiler(x_center, y_center, length, breadth, theta, thickness, color)
    mag1 = Magnet(-3.0, 3, 0.8, 0.7, 0, 0.2, COLOR_MAGNET);
    mag2 = Magnet( 3.0, 3, 0.8, 0.7, 180, 0.2, COLOR_MAGNET);
    /*****************************************************************/

    /* Creating porcupines */
    for(int ii = 0;ii < 4; ii++){
        porpine[ii] = Porcupine(-2 - ii*0.2, grass.max_point, 0.2, 0.3, COLOR_GREEN);
        porpine[ii].set_validity(0);       // Only valid at level 3
    }
    /**********************/

    /* Initialising Score and level and display*/
    score = Score();
    level = Level();
    display = Display();
    /********************************************/

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
            generate_enemies();
            destroy_enemies();
            if(game_timer > 0)
                game_timer--;
            else if(game_timer == 0){
                quit(window);
            }
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

void generate_enemies(){
    /* Section for Random balls */
    double random_number = ((rand() % 4) - 1) + (double)rand() / (double)((unsigned)RAND_MAX + 1); // integer + decimal
    double decimal_part = (double)((double)rand() / (double)((unsigned)RAND_MAX + 1)) / 33;
    decimal_part += 0.01;
    double Radius = (double)((rand() % 3) + 1) / 10;
    color_t colors[] = {COLOR_BLACK, COLOR_GREEN, COLOR_RED, COLOR_WHITE};
    Ball temp;
    Rectangle temp1;
    temp = Ball(-7, random_number, colors[rand() % 4], Radius, 0);
    temp.speed_x = decimal_part;
    if(count_enemies % 4 == 0 && level.get_level() >= 2){
        float theta = 45 ;
        Point *p = new Point[4];
        temp.attach_slab(p, theta, 2 * Radius, 0.05);
        temp1 = Rectangle(p[0], p[1], p[2], p[3], COLOR_GREEN);
        temp1.set_speed(decimal_part);
        temp1.set_theta(theta);
    }
    j = balls.begin();
    balls.insert(j, make_pair(temp, temp1));
    /***** Section ends here *****/
    count_enemies++;
}

void destroy_enemies(){
    for(j = balls.begin(); j < balls.end(); j++){
        if(j.base()->first.position.x > 2*SCREEN_X_MAX)
            balls.erase(j);
    }
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
