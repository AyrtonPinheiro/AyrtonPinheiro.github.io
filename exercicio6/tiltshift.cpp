#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>


//decaimento
int dec_slider = 10;
int dec_slider_max = 100;

//altura do centro
int top_slider = 40;
int top_slider_max = 100;

//posição do centro
int centro_slider = 40;
int centro_slider_max = 100;

double alfa;
int l1, l2;
cv::Mat image1, image2, blended, mask;

char TrackbarName[50];

//funcao que aplica os efeitos das 3 trackbars
void on_trackbar_efeito(int, void*){
  image1.copyTo(blended);
  l1 = image1.size().height*(centro_slider - top_slider/2)/100.0;
  l2 = image1.size().height*(centro_slider + top_slider/2)/100.0;
  if(l1<0) l1 = 0;
  if(l2>image1.size().height) l2 = image1.size().height;
  
  // tag::parte2[]
  //ponderação entre a imagem original com o alfa e a imagem borrada com o negativo de alfa
  //e a combinação delas, resultando no efeito tiltshift
  for(int i=0;i<image1.size().height;i++){
    alfa = 0.5*(tanh((i-l1)/(double)(dec_slider+1.0))-tanh((i-l2)/(double)(dec_slider+1.0)));
    cv::addWeighted(image1.row(i), alfa, image2.row(i), 1-alfa, 0.0, blended.row(i));
  } 
  cv::imshow("tiltshift", blended);
   // end::parte2[]
}

int main(int argvc, char** argv){
  // tag::parte1[]
  image1 = cv::imread("Pedestres.jpg"); 
  resize(image1, image1, cv::Size(750,500));
  image1.copyTo(image2);  //image2 é uma copia de image1 que vai passar por um borramento
  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  
  image2.convertTo(image2, CV_32F);
  mask = cv::Mat(3, 3, CV_32F, media);  
  //aplicando o filtro 10 vezes para reforçar o borramento
  for(int i=0;i<10;i++){
    cv::filter2D(image2, image2, image2.depth(), mask,
                 cv::Point(1, 1), 0);
  }
  image2.convertTo(image2, CV_8U);
  // end::parte1[]
  
  cv::namedWindow("tiltshift", 1);
  
  std::sprintf( TrackbarName, "Decaimento x %d", dec_slider_max );
  cv::createTrackbar( TrackbarName, "tiltshift",
                      &dec_slider,
                      dec_slider_max,
                      on_trackbar_efeito );
  
  std::sprintf( TrackbarName, "Altura do centro x %d", top_slider_max );
  cv::createTrackbar( TrackbarName, "tiltshift",
                      &top_slider,
                      top_slider_max,
                      on_trackbar_efeito );
                      
  std::sprintf( TrackbarName, "Posição do centro x %d", centro_slider_max );
  cv::createTrackbar( TrackbarName, "tiltshift",
                      &centro_slider,
                      centro_slider_max,
                      on_trackbar_efeito );
                      
  on_trackbar_efeito(top_slider, 0);

  cv::waitKey(0);
  return 0;
}
