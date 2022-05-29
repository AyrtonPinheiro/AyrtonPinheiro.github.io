#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  cv::Mat image, realce;
  int width, height;
  int nObjCom = 0, nObjSem = 0;
  
  cv::Point p;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

  width=image.cols;
  height=image.rows;
  std::cout << width << "x" << height << std::endl;

  cv::imshow("original", image);
  
  p.x=0;
  p.y=0;
  
  // tag::parte1[]
  // busca objetos presentes nas bordas e os desconsidera
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255 && (i == 0 || j == 0 || i == height-1 || j == width-1)){
        p.x=j;
        p.y=i;
      	cv::floodFill(image,p,0);
      }
    }
  }
  p.x=0;
  p.y=0;
  // fundo preenchido com cor 70
  cv::floodFill(image,p,70);
  // end::parte1[]
  
  cv::imshow("cor de fundo alterada", image);
  cv::imwrite("bolhas2.png", image);
  
  // tag::parte2[]
  // busca objetos com buracos
  /* se a imagem tiver mais de 1 buraco, ao encontrar o segundo buraco 
  de um objeto que ja foi contado, a cor dele não será mais 255,
  logo ele não será contado mais de 1 vez */
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0 && image.at<uchar>(i,j-1) == 255){
        nObjCom++;
        p.x=j-1;
        p.y=i;
        cv::floodFill(image,p,140);
      }
    }
  }
  // end::parte2[]
  
  cv::imshow("objeto com buraco", image);
  cv::imwrite("bolhas3.png", image);

  // tag::parte3[]
  // busca objetos sem buracos
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        nObjSem++;
        p.x=j;
        p.y=i;
        cv::floodFill(image,p,210);
      }
    }
  }
  // end::parte3[]
  
  std::cout << "objetos com buracos: " << nObjCom << "\n objetos sem buracos: " << nObjSem << "\n";
  cv::imshow("final", image);
  cv::imwrite("bolhas4.png", image);
  cv::waitKey();
  return 0;
}
