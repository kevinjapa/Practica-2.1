
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <cstring>

// Librerías de OpenCV
//#include <opencv2/opencv.hpp>

#include <opencv2/imgproc.hpp> // Librería que contiene operaciones para realizar procesamiento de imágenes
#include <opencv2/imgcodecs.hpp> // Librería contiene los métodos para cargar imágenes de acuerdo a un códec
#include <opencv2/core.hpp> // Librería que contiene las estructuras y funciones base para representar las imágenes como matrices
#include <opencv2/video.hpp> // Librería para realizar lectura de vídeos
#include <opencv2/videoio.hpp> // Librería para escritura de vídoeos y streaming
#include <opencv2/highgui.hpp> // Librería para crear interfaces básicas de usuario
#include <chrono>


using namespace std;
using namespace cv;
using namespace chrono;

int hmin = 0, hmax = 79, smin = 111, smax = 213, vmin = 36, vmax = 101;

void eventoTrack(int valor, void *data){
}


int main(int argc, char* args[]){

    VideoCapture  video("http://166.247.77.253:81/mjpg/video.mjpg");
    //VideoCapture video("http://173.213.113.128/mjpg/video.mjpg");
    //VideoCapture video ("http://173.198.10.174:80/mjpg/video.mjpg");
    namedWindow("vista", WINDOW_AUTOSIZE);
    if(video.isOpened()){


        //Parte 1
        int fps=0, frameCount=0;
        Mat frame;
        Mat ecualizadaHist, imgBN;
        Mat CLANE;
        // Metodo propuesto
        Mat norma,ImgGamma,imgBN2;
        float gamma=1.5;
        //Deteccion de Movimiento
        Mat previous_frame_gray, motion_mask, diff;

        Mat vista;
        auto inicio = high_resolution_clock::now();
        while(3==3){
            video >> frame;
            resize(frame, frame, Size(), 0.8, 0.8);
            cvtColor(frame, imgBN,COLOR_BGR2GRAY);
            cvtColor(frame, imgBN2,COLOR_BGR2GRAY);

            //Ecualizacion de Histograma
            equalizeHist(imgBN, ecualizadaHist);
            imshow("Ecualizacion de Histograma", ecualizadaHist);

            //Metodo de CLAHE

            Ptr<CLAHE> metodoClahe = createCLAHE();
            metodoClahe-> apply(imgBN, CLANE);
            metodoClahe-> setTilesGridSize(Size(10,10));

            imshow("Metodo CLAHE", CLANE);

            //Filtro propuesto GAMMA
            imgBN2.convertTo(norma, CV_32F, 1.0 / 255.0);
            pow(norma, 1 / gamma, imgBN2);
            imgBN2 *= 255;
            imgBN2.convertTo(imgBN2, CV_8U);
            imshow("Gamma", imgBN2);

            
            //Detector de Movimiento Frame Differencing github

            if (previous_frame_gray.empty())
                previous_frame_gray=imgBN.clone();

            absdiff(previous_frame_gray, imgBN, diff);
            threshold(diff, motion_mask, 60, 255,THRESH_BINARY);
            //imshow("Motion Mask", motion_mask);

            //unir las vistas
            if(vista.empty()){
                vista= Mat::zeros(Size((frame.cols*2), frame.rows), CV_8UC1);
            }
            imgBN.copyTo(vista(Rect(0,0,imgBN.cols, imgBN.rows)));
            motion_mask.copyTo(vista(Rect(motion_mask.cols, 0, motion_mask.cols,motion_mask.rows)));
            previous_frame_gray = imgBN.clone();
            
            // FPS
            frameCount++;
            auto fin = high_resolution_clock::now();
            duration<double> tiempo= fin-inicio;
            fps=(frameCount/tiempo.count());
            //putText(frame, "FPS: " + to_string(fps)  , Point(5, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255, 255), 3);
            putText(vista, "FPS: " + to_string(fps)  , Point(5, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 3);

            imshow("vista", vista);
            //imshow("Video Original", frame);
            
            if(waitKey(23)==27)
                break;

            
        }
        video.release();
        destroyAllWindows();

    }
}