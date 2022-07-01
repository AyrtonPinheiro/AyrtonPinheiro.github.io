#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 4

int top_slider = 120;
int top_slider_max = 200;

char TrackbarName[50];

vector<int> yrange;
vector<int> xrange;
Mat image, border, frame, points, clonePoints;
int width, height, gray;
int x, y;

// tag::on_trackbar_canny[]
void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  imwrite("bordaCanny.jpg", border);

  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(border.at<uchar>(i,j) >= 250){
        x = i+rand()%(2*JITTER)-JITTER+1;
        y = j+rand()%(2*JITTER)-JITTER+1;
        gray = image.at<uchar>(x,y);
        circle(border,
             cv::Point(y,x),
             RAIO-1,
             CV_RGB(gray,gray,gray),
             -1,
             cv::LINE_AA);
      }
    }
  }
  clonePoints = points.clone();
  //para pixels com valor maior que 50, a matriz clonePoints recebe o valor da matriz borda
  //ao usar a condicao "maior que 0", a imagem final ficava com muitos pixels escuros
  //ao redor dos circulos da borda
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(border.at<uchar>(i,j) >= 50){
        clonePoints.at<uchar>(i,j) = border.at<uchar>(i,j);
      }
    }
  }
  imshow("canny", clonePoints);
  //imshow("canny", border);
}
// end::on_trackbar_canny[]

int main(int argc, char**argv){



  image= imread(argv[1],cv::IMREAD_GRAYSCALE);
  
  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);
  
  iota(xrange.begin(), xrange.end(), 0); 
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());
  
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             cv::LINE_AA);
    }
  }
  
  // tag::tirar_branco[]
  //para tirar os espaços entre os circulos 
  //se o pixel for igual a 255, ele recebe a cor da imagem original
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(points.at<uchar>(i,j) == 255){
        points.at<uchar>(i,j) = image.at<uchar>(i,j);
      }
    }
  }
  // end::tirar_branco[]
  cv::imshow("original", image);
  cv::imshow("pontilhismo", points);

  sprintf( TrackbarName, "Threshold inferior", top_slider_max );

  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );

  on_trackbar_canny(top_slider, 0 );

  waitKey();
  imwrite("originalCinza.jpg", image);
  imwrite("bordaPontilhismo.jpg", border);
  imwrite("Pontilhismo.jpg", points);
  imwrite("resultado.jpg", clonePoints);
  return 0;
}
