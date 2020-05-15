#include "gdexample.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace godot;
using namespace cv;
using namespace std;

void GDExample::_register_methods() {
    register_method("_process", &GDExample::_process);
    register_property<GDExample, float>("amplitude", &GDExample::amplitude, 10.0);
    register_property<GDExample, float>("speed", &GDExample::set_speed, &GDExample::get_speed, 1.0);

    register_signal<GDExample>((char *)"position_changed", "node", GODOT_VARIANT_TYPE_OBJECT, "new_pos", GODOT_VARIANT_TYPE_VECTOR2);
}

GDExample::GDExample() {
}

GDExample::~GDExample() {
    // add your cleanup here
}

void GDExample::_init() {
    // initialize any variables here
    time_passed = 0.0;
    amplitude = 10.0;
    speed = 1.0;

    camera.open(0); //open camera
    camera.read(frame);
    cursorPos = Point(frame.cols / 2, frame.rows / 2);

    // camera.set(3, 512);
    // camera.set(4, 288);

    // // TODO 
    face_cascase.load("/Users/ggbrw/Downloads/opencv/data/haarcascades/haarcascade_frontalface_alt.xml");
    if(!face_cascase.load("/Users/ggbrw/Downloads/opencv/data/haarcascades/haarcascade_frontalface_alt.xml")) {
        cerr << "Error XML" << endl;
    }
}

int dist(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

void GDExample::_process(float delta) {
    camera.read(frame);
    if(frame.empty()) return;
    flip(frame,frame,1); // Beeld flippen zodat de user's rechts en links overeenkomt met beeld 

    vector<Rect> faces;
    face_cascase.detectMultiScale(frame, faces, 1.1, 3, 0, Size(100,100), Size(3000,3000));
    
    Vector2 movement(0,0);

    for(int i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width * .5, faces[i].y + faces[i].height * .5);
        ellipse(frame, center, Size(faces[i].width * .5, faces[i].height * .5), 0, 0, 360, Scalar(255,0,255), 4, 8, 0);
        
        if(dist(center, Point(cursorPos.x,cursorPos.y)) < max(faces[i].width,faces[i].height) / 2) {
            line(frame, cursorPos, center, Scalar(255,0,255), 4, 8);
            movement = Vector2(movement.x + (center.x - cursorPos.x), movement.y + (center.y - cursorPos.y));
        }
    }
        

    cursorPos.x += movement.x / 4;
    cursorPos.y += movement.y / 4;
    ellipse(frame, Point(cursorPos.x,cursorPos.y), Size(8,8), 0, 0, 360, Scalar(0,255,0), 4, 8, 0);
    imshow("", frame);

    if(faces.size() == 0) return;
    if(waitKey(10) == 27) return;

    set_position(Vector2(cursorPos.x,cursorPos.y));
}

void GDExample::set_speed(float p_speed) {
    speed = p_speed;
}

float GDExample::get_speed() {
    return speed;
}
