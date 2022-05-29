#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  cv::Mat image;
  cv::Vec3b val;

  image= cv::imread(argv[1],cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu a imagem selecionada" << std::endl;

  cv::imshow("Original", image);  

  int altura = image.size().height;
  int largura = image.size().width;
  cv::Mat novaImagem(altura, largura, cv::IMREAD_GRAYSCALE);

  for(int i=0;i<altura/2;i++){
    for(int j=0;j<largura/2;j++){
      novaImagem.at<uchar>(i,j) = image.at<uchar>(i+altura/2,j+largura/2);
      novaImagem.at<uchar>(i+altura/2,j+largura/2) = image.at<uchar>(i,j);
      novaImagem.at<uchar>(i+altura/2,j) = image.at<uchar>(i,j+largura/2);
      novaImagem.at<uchar>(i,j+largura/2) = image.at<uchar>(i+altura/2,j);
    }
  }
  
  cv::imshow("Resultado", novaImagem);  
  cv::imwrite("gato_saida2.jpg",novaImagem);
  cv::waitKey();

  return 0;
}
