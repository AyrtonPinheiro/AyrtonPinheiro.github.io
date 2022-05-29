#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::Mat image;
  int width, height;
  cv::VideoCapture cap;
  std::vector<cv::Mat> planes;
  cv::Mat histR, histG; // R é o atual, G é o antigo
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  int key;

	cap.open(1);
  
  if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
  }
  
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  std::cout << "largura = " << width << std::endl;
  std::cout << "altura  = " << height << std::endl;

  int histw = nbins, histh = nbins/2;
  cv::Mat histImgR(histh, histw, CV_8UC3, cv::Scalar(0,0,0));

  cap >> image;
  cv::split (image, planes);
  cv::calcHist(&planes[0], 1, 0, cv::Mat(), histG, 1,
               &nbins, &histrange,
               uniform, acummulate);

  while(1){
    cap >> image;
    cv::split (image, planes);
    cv::calcHist(&planes[0], 1, 0, cv::Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    
    cv::normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    histImgR.setTo(cv::Scalar(0));
    
    for(int i=0; i<nbins; i++){
      cv::line(histImgR,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(histR.at<float>(i))),
               cv::Scalar(255, 255, 255), 1, 8, 0);
    }
    double compara = compareHist(histR, histG, 1); //1 = metodo CV_COMP_CHISQR
    //std::cout << compara << std::endl;
    //2.5 foi aproximadamente o menor valor que não detecta ruído do histograma
    if (compara > 2.5) std::cout << "MOVIMENTO!!" << std::endl;
    
    histImgR.copyTo(image(cv::Rect(0, 0       ,nbins, histh)));
    cv::imshow("image", image);
    histR.copyTo(histG);
    
    key = cv::waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
