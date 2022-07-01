#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define RADIUS 20

  cv::Mat complexImage, tmp, filter, padded;
  std::vector<cv::Mat> planos;
    
  int gammaH_slider = 70;
  int gammaH_slider_max = 200;

  int gammaL_slider = 10;
  int gammaL_slider_max = 200;

  //variavel C
  int c_slider = 10;
  int c_slider_max = 200;
  
  //variavel D0
  int d0_slider = 180;
  int d0_slider_max = 200;

  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  int dft_M, dft_N;

// troca os quadrantes da imagem da DFT
void deslocaDFT(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;
  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(cv::Rect(0, 0, cx, cy));
  B = image(cv::Rect(cx, 0, cx, cy));
  C = image(cv::Rect(0, cy, cx, cy));
  D = image(cv::Rect(cx, cy, cx, cy));
  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);
  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

// tag::on_trackbar_efeito[]
void on_trackbar_efeito(int, void*) {
  filter = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));
  tmp = cv::Mat(dft_M, dft_N, CV_32F);
  float c0, d0, gammaH, gammaL, dQuad;
  c0 = (float)c_slider/10.0;
  d0 = (float)d0_slider/10.0;
  gammaH = (float)gammaH_slider/10.0;
  gammaL = (float)gammaL_slider/10.0;
  for(int i=0;i<dft_M;i++){
    for(int j=0;j<dft_N;j++){
      dQuad = pow((float)i - dft_M/2.0, 2) + pow((float)j - dft_N/2.0, 2);
      tmp.at<float>(i,j) = (gammaH - gammaL)*(1 - exp(-1.0*(c0)*(dQuad/(d0*d0)))) + gammaL;
    }
  }
  
  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  cv::Mat comps[] = {tmp, tmp};
  cv::merge(comps, 2, filter);
  
  cv::Mat cloneComplexImage = complexImage.clone();
  
  // aplica o filtro de frequencia
  cv::mulSpectrums(cloneComplexImage, filter, cloneComplexImage, 0);
  
  // troca novamente os quadrantes
  deslocaDFT(cloneComplexImage);

  // calcula a DFT inversa
  cv::idft(cloneComplexImage, cloneComplexImage);

  // limpa o array de planos
  planos.clear();

  // separa as partes real e imaginaria da
  // imagem filtrada
  cv::split(cloneComplexImage, planos);

  // normaliza a parte real para exibicao
  cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);
  cv::imshow("filtroHomomorfico", planos[0]);    
}
// end::on_trackbar_efeito[]

int main(int argvc, char** argv) {
  cv::Mat mag;
  cv::Mat image, imagegray, magI;
  cv::Mat_<float> realInput, zeros;
  cv::Mat backgroundImage;
  //char key;

  //  char TrackbarRadiusName[50];
  //  std::strcpy(TrackbarFrequencyName, "Raio");

  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu a imagem selecionada" << std::endl;
  resize(image, image, cv::Size(800,600));
  cv::imshow("original", image);
  
  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = cv::getOptimalDFTSize(image.rows);
  dft_N = cv::getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                     dft_N - image.cols, cv::BORDER_CONSTANT,
                     cv::Scalar::all(0));
  //cv::imshow("padded", padded);
  
  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = cv::Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

  // limpa o array de matrizes que vao compor a
  // imagem complexa
  planos.clear();
  // cria a compoente real
  realInput = cv::Mat_<float>(padded);
  // insere as duas componentes no array de matrizes
  planos.push_back(realInput);
  planos.push_back(zeros);

  // combina o array de matrizes em uma unica
  // componente complexa
  cv::merge(planos, complexImage);

  // calcula o dft
  cv::dft(complexImage, complexImage);
  // realiza a troca de quadrantes
  deslocaDFT(complexImage);

  cv::namedWindow("filtroHomomorfico", 1);
  cv::createTrackbar("gammaL", "filtroHomomorfico", &gammaL_slider, gammaL_slider_max,
                     on_trackbar_efeito);
                     
  cv::createTrackbar("gammaH", "filtroHomomorfico", &gammaH_slider, gammaH_slider_max,
                     on_trackbar_efeito);

  cv::createTrackbar("C", "filtroHomomorfico", &c_slider, c_slider_max,
                     on_trackbar_efeito);

  cv::createTrackbar("D0", "filtroHomomorfico", &d0_slider, d0_slider_max,
                     on_trackbar_efeito);

  on_trackbar_efeito(gammaL_slider, 0);

  cv::waitKey(0);
  return 0;
}
