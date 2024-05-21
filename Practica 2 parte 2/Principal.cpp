
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


void eventoTrack(int valor, void *data){}

int decimal=0;

Mat metodoSal(Mat frames, int porRuido){
    Mat ruidoSal = Mat:: zeros(frames.size(), CV_8UC1);
    randu(ruidoSal,0,255);
    frames.setTo(Scalar(255, 255, 255), ruidoSal < 255 * porRuido / 100);
    return frames;
}
Mat metodoPimienta(Mat frames, int porRuido){
    Mat ruidoPimienta = Mat:: zeros(frames.size(), CV_8UC1);
    randu(ruidoPimienta,0,255);
    frames.setTo(Scalar(0,0,0), ruidoPimienta < 255*porRuido /100);
    return frames;
}
Mat filtroMediana( Mat frames, int mask){
    if(mask%2!=0) 
        medianBlur(frames, frames, mask);
    return frames;
}
Mat filtroBlur( Mat frames, int mask){
    if(mask%2!=0) 
        blur(frames, frames, Size(mask,mask));
    return frames;
}
Mat filtroGaussiano(Mat frames, int mask, double sigma){
    decimal=sigma;
    decimal/=10;
    if(mask%2!=0) 
        GaussianBlur(frames, frames, Size(mask,mask),decimal);// mask: tamano de mascara, decimal: desviacion
    return frames;
}
int main(int argc, char* args[]){

    int porSal=0, porPimienta=0, mascara=1, mascaraGaussiano=1, sigmaGaussiano=0, K=1;
    namedWindow("Video Original", WINDOW_AUTOSIZE);
    VideoCapture video("videoEjemplo.mp4");

    if (video.isOpened()) {

        namedWindow("Ruido de Sal", WINDOW_AUTOSIZE);
        createTrackbar("& Sal", "Ruido de Sal", &porSal, 100, eventoTrack, NULL);
        
        namedWindow("Ruido de Pimienta", WINDOW_AUTOSIZE);
        createTrackbar("& Pimienta", "Ruido de Pimienta", &porPimienta, 100, eventoTrack, NULL);

        //Ventana de todos los filtros
        namedWindow("Filtros",WINDOW_AUTOSIZE);
        createTrackbar("Mascara", "Filtros", &mascara, 100, eventoTrack, NULL);
        createTrackbar("Sigma(X,Y)", "Filtros", &sigmaGaussiano, 100, eventoTrack, NULL);

        namedWindow("LAPLACE",WINDOW_AUTOSIZE);
        createTrackbar("K", "LAPLACE", &K, 100, eventoTrack, NULL);

        Mat frame, frameBN;

        Mat sal, pimienta, mediana, Blur, gauss, vistaFiltros ;
        //Mat mediana;
        //Detector de Bordes Sobel
        Mat gX, gY, sobel;

        //Detector de Bordes Laplace
        Mat laplace;

        while (true) {
            video >> frame;
            resize(frame, frame, Size(), 0.4, 0.4);
            cvtColor(frame, frameBN, COLOR_BGRA2GRAY);

            //Metodo de Sal
            frame.copyTo(sal);
            imshow("Ruido de Sal", metodoSal(sal,porSal));

            //Metodo de Pimienta
            frame.copyTo(pimienta);
            imshow("Ruido de Pimienta", metodoPimienta(pimienta, porPimienta));

            // Filtro de la Mediana
            frame.copyTo(mediana);
            filtroMediana(mediana, mascara);

            //Filtro Blur
            frame.copyTo(Blur);
            filtroBlur(Blur, mascara);

            //Filtro Gaussiano
            frame.copyTo(gauss);

            filtroGaussiano(gauss,mascara,(double) sigmaGaussiano);
            //imshow("Gaussiano",filtroGaussiano(gauss,mascara,(double) sigmaGaussiano));

            //Unir las vista de los filtros: Mediana, Blur, Guassiano
            if(vistaFiltros.empty()){
                vistaFiltros= Mat::zeros(Size((frame.cols*3), frame.rows), CV_8UC3);
            }

            //Medoto de Solbel
            Sobel(frameBN, gX, CV_16S, 1, 0, 1);
            Sobel(frameBN, gY,  CV_16S, 0, 1, 1);

            convertScaleAbs(gX, gX);
            convertScaleAbs(gY, gY);
            
            addWeighted(gX, 0.5, gY, 0.5, 0, sobel);

            imshow("Sobel", sobel);

            // Laplace            
            int kImpar;
            if(K%2!=0)
            {
                Laplacian(frameBN, laplace, CV_16S, K);
                convertScaleAbs(laplace,laplace);
                imshow("LAPLACE", laplace);
            }
        
            mediana.copyTo(vistaFiltros(Rect(0,0,mediana.cols, mediana.rows)));
            Blur.copyTo(vistaFiltros(Rect(Blur.cols,0,Blur.cols,Blur.rows)));
            gauss.copyTo(vistaFiltros(Rect(mediana.cols+Blur.cols,0,gauss.cols,gauss.rows)));

            putText(vistaFiltros,"Mediana", Point(0,40),FONT_HERSHEY_SIMPLEX,0.60,250,2);
            putText(vistaFiltros,"Blur", Point(Blur.cols,40),FONT_HERSHEY_SIMPLEX,0.60,250,2);
            putText(vistaFiltros,"GAUSS", Point(gauss.cols*2,40),FONT_HERSHEY_SIMPLEX,0.60,250,2);

            imshow("Filtros", vistaFiltros);
            imshow("Video Original", frame);
            if(waitKey(23)==27)
                break;
        }
    }
}
