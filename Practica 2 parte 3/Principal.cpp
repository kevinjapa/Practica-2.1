
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <cstring>


#include <opencv2/imgproc.hpp> // Librería que contiene operaciones para realizar procesamiento de imágenes
#include <opencv2/imgcodecs.hpp> // Librería contiene los métodos para cargar imágenes de acuerdo a un códec
#include <opencv2/core.hpp> // Librería que contiene las estructuras y funciones base para representar las imágenes como matrices
#include <opencv2/video.hpp> // Librería para realizar lectura de vídeos
#include <opencv2/videoio.hpp> // Librería para escritura de vídoeos y streaming
#include <opencv2/highgui.hpp> // Librería para crear interfaces básicas de usuario
#include <chrono>


using namespace std;
using namespace cv;


int main(int argc, char* args[]){
    int mascara=37;
    //namedWindow("imagen",WINDOW_AUTOSIZE);
    //createTrackbar("Mascara", "imagen", &mascara, 37, NULL);

    Mat Cerebro= imread("Cerebro.jpg", IMREAD_GRAYSCALE);
    Mat Pie= imread("Pie.jpg", IMREAD_GRAYSCALE);
    Mat Pecho= imread("pecho.jpg",IMREAD_GRAYSCALE);
    resize(Cerebro, Cerebro, Size(), 1.3,1.3);
    resize(Pie, Pie, Size(), 1.4,1.4);
    resize(Pecho, Pecho, Size(), 1.4,1.4);
    Mat ErosionC,DilatacionC,tophatC,blackhatC,vistaC;
    Mat ErosionP,DilatacionP,tophatP,blackhatP,vistaP;
    Mat ErosionPie,DilatacionPie,tophatPie,blackhatPie,vistaPie;
    Mat elemento = getStructuringElement(MORPH_RECT, Size(mascara,mascara));//(37,37)
    
    //Parte 1

    //Operaciones Morfologicas


    //Parte 2

    //Erocion
    erode(Cerebro,ErosionC, elemento);
    erode(Pecho,ErosionP, elemento);
    erode(Pie,ErosionPie, elemento);
    imshow("Erosion Cerebro",ErosionC);
    imshow("Erosion Pecho",ErosionP);
    imshow("Erosion Pie",ErosionPie);

    //Dilatacion
    dilate(Cerebro, DilatacionC,elemento);
    dilate(Pecho, DilatacionP,elemento);
    dilate(Pie, DilatacionPie,elemento);
    imshow("Dilatacion Cerebro",DilatacionC);
    imshow("Dilatacion Pecho",DilatacionP);
    imshow("Dilatacion Pie",DilatacionPie);

    //Top Hat
    morphologyEx(Cerebro, tophatC,MORPH_TOPHAT, elemento);
    morphologyEx(Pecho, tophatP,MORPH_TOPHAT, elemento);
    morphologyEx(Pie, tophatPie,MORPH_TOPHAT, elemento);

    //Black Hat
    morphologyEx(Cerebro,blackhatC,MORPH_BLACKHAT,elemento);
    morphologyEx(Pecho,blackhatP,MORPH_BLACKHAT,elemento);
    morphologyEx(Pie,blackhatPie,MORPH_BLACKHAT,elemento);

    Mat equaC= Cerebro+tophatC-blackhatC;
    Mat equaP= Pecho+tophatP-blackhatP;
    Mat equaPie= Pie+tophatPie-blackhatPie;
    //Imprimir las vistas
    if(vistaC.empty()){
        vistaC= Mat::zeros(Size((Cerebro.cols*4), Cerebro.rows), CV_8UC1);
        vistaP= Mat::zeros(Size((Pecho.cols*4), Pecho.rows), CV_8UC1);
        vistaPie= Mat::zeros(Size((Pie.cols*4), Pie.rows), CV_8UC1);
    }

    Cerebro.copyTo(vistaC(Rect(0,0,Cerebro.cols, Cerebro.rows)));
    tophatC.copyTo(vistaC(Rect(tophatC.cols, 0, tophatC.cols,tophatC.rows)));
    blackhatC.copyTo(vistaC(Rect(Cerebro.cols*2, 0, blackhatC.cols,blackhatC.rows)));
    equaC.copyTo(vistaC(Rect(Cerebro.cols*3, 0, equaC.cols,equaC.rows)));

    Pecho.copyTo(vistaP(Rect(0,0,Pecho.cols, Pecho.rows)));
    tophatP.copyTo(vistaP(Rect(tophatP.cols, 0, tophatP.cols,tophatP.rows)));
    blackhatP.copyTo(vistaP(Rect(Pecho.cols*2, 0, blackhatP.cols,blackhatP.rows)));
    equaP.copyTo(vistaP(Rect(Pecho.cols*3, 0, equaP.cols,equaP.rows)));

    Pie.copyTo(vistaPie(Rect(0,0,Pie.cols, Pie.rows)));
    tophatPie.copyTo(vistaPie(Rect(tophatPie.cols, 0, tophatPie.cols,tophatPie.rows)));
    blackhatPie.copyTo(vistaPie(Rect(Pie.cols*2, 0, blackhatPie.cols,blackhatPie.rows)));
    equaPie.copyTo(vistaPie(Rect(Pie.cols*3, 0, equaPie.cols,equaPie.rows)));

    putText(vistaC,"Orignial", Point(0,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaC,"Top Hat", Point(tophatC.cols,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaC,"Back Hat", Point(blackhatC.cols*2,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaC,"Equa", Point(equaC.cols*3,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaP,"Orignial", Point(0,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaP,"Top Hat", Point(tophatP.cols,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaP,"Back Hat", Point(blackhatP.cols*2,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);
    putText(vistaP,"Equa", Point(equaP.cols*3,10),FONT_HERSHEY_SIMPLEX,0.3,255,1);

    imshow("Cerebro", vistaC);
    imshow("Torax",vistaP);
    imshow("Pie",vistaPie);

    waitKey(0);
    destroyAllWindows();
}