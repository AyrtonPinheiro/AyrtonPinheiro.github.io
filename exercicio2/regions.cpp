#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  cv::Mat image;
  cv::Vec3b val;
  int x1, y1, x2, y2;

  image= cv::imread("gato.jpg",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "a imagem não abriu " << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

  std::cout << "Insira as coordenadas do Ponto 1 (x1, y1): ";
  std::cin >> x1 >> y1;
  std::cout << "Insira as coordenadas do Ponto 2 (x2, y2): ";
  std::cin >> x2 >> y2;

  if(x1<x2 && y1<y2){
    for(int i=x1;i<x2;i++){
      for(int j=y1;j<y2;j++){
        image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
      }
    }
  }
  else if(x1>x2 && y1>y2){
    for(int i=x2;i<x1;i++){
      for(int j=y2;j<y1;j++){
        image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
      }
    }
  }
  else if(x1<x2 && y1>y2){
    for(int i=x1;i<x2;i++){
      for(int j=y2;j<y1;j++){
        image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
      }
    }
  }
  else if(x1>x2 && y1<y2){
    for(int i=x2;i<x1;i++){
      for(int j=y1;j<y2;j++){
        image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
      }
    }
  }  
  
  cv::imshow("janela", image);  
  cv::imwrite("gato_saida.jpg",image);
  cv::waitKey();

  return 0;
}
