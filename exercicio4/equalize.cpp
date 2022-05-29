#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv){
  cv::Mat image, imageEq;
  int width, height;
  cv::VideoCapture cap;
  cv::Mat histR, histG;
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
  cv::Mat histImgR(histh, histw, cv::COLOR_BGR2GRAY, cv::Scalar(0,0,0));
  cv::Mat histImgG(histh, histw, cv::COLOR_BGR2GRAY, cv::Scalar(0,0,0));
  while(1){
    cap >> image;
    
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::flip(image, image, 1);
    equalizeHist(image, imageEq);
    
    cv::calcHist(&image, 1, 0, cv::Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    cv::calcHist(&imageEq, 1, 0, cv::Mat(), histG, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
    
    cv::normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(histG, histG, 0, histImgG.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    histImgR.setTo(cv::Scalar(0));
    histImgG.setTo(cv::Scalar(0));
    
    for(int i=0; i<nbins; i++){
      cv::line(histImgR,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(histR.at<float>(i))),
               cv::Scalar(255, 255, 255), 1, 8, 0);
      cv::line(histImgG,
               cv::Point(i, histh),
               cv::Point(i, histh-cvRound(histG.at<float>(i))),
               cv::Scalar(255, 255, 255), 1, 8, 0);
    }
    histImgR.copyTo(image(cv::Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(imageEq(cv::Rect(0, 0       ,nbins, histh)));
    
    cv::imshow("original", image);
    cv::imshow("imagem equalizada", imageEq);
    key = cv::waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
