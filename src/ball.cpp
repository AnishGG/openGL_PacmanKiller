#include "ball.h"
#include "ellipse.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, double Radius) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->Radius   = Radius;
    this->isSlabAttached = 0;
    this->speed_x = 0.01;
    this->jumped = 0;
    this->drowned = 0;

    GLfloat vertex_buffer_data[10010];
    double theta = 1.0;
    int i = 0;
    while(theta < 362.0){
        vertex_buffer_data[i] = Radius * cos((theta * PI) / 180.0);
        vertex_buffer_data[i+1] = Radius * sin((theta * PI) / 180.0);
        vertex_buffer_data[i+2] = 0.0;
        vertex_buffer_data[i+3] = 0;
        vertex_buffer_data[i+4] = 0;
        vertex_buffer_data[i+5] = 0;
        theta += 1.0;
        vertex_buffer_data[i+6] = Radius * cos(theta * PI / 180.0);
        vertex_buffer_data[i+7] = Radius * sin(theta * PI / 180.0);
        vertex_buffer_data[i+8] = 0;
        i += 9;
    }

    this->object = create3DObject(GL_TRIANGLES, i/3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
//    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(Ellipse pool) {
    this->position.x += this->speed_x;
    this->position.y += this->speed_y;

    /* Setting the ground limit for the ball */
    if(this->position.y <= -1.8 && !drowned){
        jumped = 0;
        speed_y = 0, speed_x = 0;
        this->position.y = -1.8;
    }

    /* Setting the pool limits */
    if(this->position.x >= pool.x_min && this->position.x <= pool.x_max && this->position.y <= -1.6){
        if(this->position.y <= -1.79999){
            jumped = 0;
            drowned = 1;
        }
        else{
            jumped = 1;drowned = 0;
        }
    }
    else if(this->position.x < pool.x_min || this->position.x > pool.x_max){
        drowned = 0;
    }

    if((this->position.y <= pool.calculate_y(this->position.x)) && drowned){
        speed_y = 0;
        this->position.y = pool.calculate_y(this->position.x);

        // To move the ball to the bottom most point
        if(pool.x0 > this->position.x)
            this->position.x += 0.01, speed_y -= 0.01;
        else if(pool.x0 < this->position.x)
            this->position.x -= 0.01, speed_y -= 0.01;
    }
    this->deaccelerate();
}

void Ball::deaccelerate() {
    if(jumped && speed_y > -0.15){
        speed_y -= 0.01;
    }
    if(jumped){
        if(speed_x > 0.0001)
            speed_x -= 0.005;
        else if(speed_x < 0.0001)
            speed_x += 0.005;
    }
    if(drowned && speed_y > -0.15){
        speed_y -= 0.001;
//        speed_y -= (speed_y*speed_y + speed_x*speed_x)*0.4;
    }
    if(drowned && speed_y < -0.15){
        speed_y += 0.001;
//        speed_y += (speed_y*speed_y + speed_x*speed_x)*5;
    }
}

void Ball::jump() {
//    speed_x = 0.0;
    speed_y = 0.20;
    jumped = 1;
}

float Ball::jump_theta(float theta){
    float v_y = this->speed_y;
    float v_x = this->speed_x;
    float alpha = atan(v_y / v_x) * 180.0 / PI;
    if(alpha < -0.001)      // -0.001 is taken as at an angle of 0.00 it was getting converted to 180.0 (float prob)
        alpha += 180.0;
    float speed_net = 0.20;
    float net_angle = 2 * theta - alpha;
    speed_x = speed_net * cos(net_angle * PI / 180.0);
    speed_y = speed_net * sin(net_angle * PI / 180.0);
    jumped = 1;
    return alpha;
}


void Ball::attach_slab(Point* points, float theta, float length, float thickness){
    float new_x1 = this->position.x + (this->Radius + thickness) * cos(theta * PI / 180.0);
    float new_y1 = this->position.y + (this->Radius + thickness) * sin(theta * PI / 180.0);
    float new_x2 = this->position.x + (this->Radius) * cos(theta * PI / 180.0);
    float new_y2 = this->position.y + (this->Radius) * sin(theta * PI / 180.0);

    Point p1, p2, p3, p4;
    p1.x = new_x1 + (length/2) * cos((theta + 90) * PI / 180.0);
    p1.y = new_y1 + (length/2) * sin((theta + 90) * PI / 180.0);
    // p2 and p3 will be diagnols of the slab

    p2.x = new_x1 + (length/2) * cos((theta + 270)* PI / 180.0);
    p2.y = new_y1 + (length/2) * sin((theta + 270)* PI / 180.0);

    p3.x = new_x2 + (length/2) * cos((theta + 90) * PI / 180.0);
    p3.y = new_y2 + (length/2) * sin((theta + 90) * PI / 180.0);
    // p1 and p4 will be diagnol points of the slab
    p4.x = new_x2 + (length/2) * cos((theta + 270)* PI / 180.0);
    p4.y = new_y2 + (length/2) * sin((theta + 270)* PI / 180.0);
    points[0] = p1;points[1] = p2;points[2] = p3;points[3] = p4;
    this->isSlabAttached = 1;
}

/* a is my this-> ball */
//bool Ball::my_collision(Ball b){
//    double R_b, R_a, x_a, x_b, y_a, y_b, R, distance_bw_centers;
//    bool collided = 0;
//    x_a = this->position.x;
//    x_b = b.position.x;
//    y_a = this->position.y;
//    y_b = b.position.y;
//    R_b = b.Radius;
//    R_a = this->Radius;
//    distance_bw_centers = abs(sqrt((x_a - x_b)*(x_a - x_b) + (y_a - y_b)*(y_a - y_b)));
//    if(distance_bw_centers <= R_b)
//    {
//        collided = 1;
//    }
//    if((distance_bw_centers <= R_b + R_a) && (y_a >= y_b) /*&& (x_a <= x_b + R_b) && (x_a >= x_b - R_b)*/)
//        collided = 1;
//    if((this->speed_y) > 0.0)
//        collided = 0;
//    return collided;
//}
