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

    camera.set(3, 512);
    camera.set(4, 288);

    waitKey(30);
    imshow("", frame);

    // TODO 
    face_cascase.load("/Users/ggbrw/Downloads/opencv/data/haarcascades/haarcascade_frontalface_alt.xml");
    if(!face_cascase.load("/Users/ggbrw/Downloads/opencv/data/haarcascades/haarcascade_frontalface_alt.xml")) {
        cerr << "Error XML" << endl;
    }
}

void GDExample::_process(float delta) {
    camera.read(frame);




    //face detectie


    flip(frame,frame,1);
        
    // if(frame.empty()) return;
        
        // Point matchLocation;
        vector<Rect> faces;
        face_cascase.detectMultiScale(frame, faces, 1.1, 3, 0, Size(100,100), Size(3000,3000));

       for(int i = 0; i < faces.size(); i++) {
           Point center(faces[i].x + faces[i].width * .5, faces[i].y + faces[i].height * .5);
           ellipse(frame, center, Size(faces[i].width * .5, faces[i].height * .5), 0, 0, 360, Scalar(255,0,255), 4, 8, 0);
       }
        
        // if(faces.size() > 0) {
        //     prevCenter = Point(faces[0].x + faces[0].width * .5, faces[0].y + faces[0].height * .5);
        //     ellipse(frame, Point(faces[0].x + faces[0].width * .5, faces[0].y + faces[0].height * .5), Size(faces[0].width * .5, faces[0].height * .5), 0, 0, 360, Scalar(255,0,255), 4, 8, 0);
        // }
        
        // location = Point(location.x + (prevCenter.x - location.x) / 8, location.y + (prevCenter.y - location.y) / 8);
        
        // ellipse(frame, location, Size(6,6), 0, 0, 360, Scalar(0,255,0), 2, 2, 0);
        

        if(waitKey(10) == 27) return; // stop capturing by pressing ESC
     


    //copy face location to the game
    set_position(Vector2(faces[0].x + faces[0].width * .5, faces[0].y + faces[0].height * .5));

 
    waitKey(30);
}

void GDExample::set_speed(float p_speed) {
    speed = p_speed;
}

float GDExample::get_speed() {
    return speed;
}
