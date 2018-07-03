#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
RNG rng(12345);

/*
 * main.cpp
 *
 * this code reconize geometric shapes and compute some moments
 *  Created on: July, 2018
 *  Author: Agustin Ortega
 *  Email: aortega.jim@gmail.com
 */


class shapeDetector{

	public:
		shapeDetector(){};

		void detect(const vector<Point>& c){

		  // perimeter and area
		  perimeter = arcLength(c, true);
		  area = contourArea(c);

		  // center
		  Moments mm = moments(c, false);

		  center = Point(mm.m10/mm.m00, mm.m01/mm.m00);


		  // depends of the sides we can compute the shape
	          vector<Point> approx;
		  approxPolyDP(c, approx, 0.04 * perimeter,true );

		  shape = findShape(approx);
		};

		// here we decide what shape is
		string findShape(const vector<Point>& approx){

		  if(approx.size() == 3){
	            return "triangle";
		  }
		  else if (approx.size() == 4){
		    return "rectangle";
		  }
		  else{

		    return "circle";
		  }
		};

		string getShape(){
		  return shape;
		};

		float getArea(){
		  return area;
		};

		float getPerimeter(){

		  return perimeter;
		};

		Point getCenter(){

	          return center;
		};


	private:
		string shape = "none";
		float area = 0;
		float perimeter = 0;
		Point center;
};



int main(int argc, const char * argv[]){

  string fileName;

  if(argc == 2){

    fileName = argv[1];

  }else{

  	cout << "you need an argument please try ./ShapeRecognition <image>";
  }

  // read file
  Mat im = imread(fileName, CV_LOAD_IMAGE_GRAYSCALE);


  // we segment by threshold (very simple one)
  Mat dst,srcCanny;
  double otsu;
  otsu = threshold( im, dst,190, 255,  CV_THRESH_BINARY | CV_THRESH_OTSU );
  Canny(dst, srcCanny, otsu, otsu * 2, 3, true);

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours(srcCanny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  vector<shapeDetector> shapedetector(contours.size());

  Mat drawing = Mat::zeros(srcCanny.size(), CV_8UC3);
  int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
  double fontScale = 0.5;
  int thickness = 1;

  for(int i = 0; i< contours.size(); i++){

    shapedetector[i].detect(contours[i]);

    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
    drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());

    circle(drawing, shapedetector[i].getCenter(), 2, Scalar(0,0,255));

    string text ="shape: "+ shapedetector[i].getShape();
   
    putText(drawing, text,shapedetector[i].getCenter()+Point(10,0), fontFace, fontScale,
        Scalar::all(255), thickness, 8);

    text ="area: "+  std::to_string(shapedetector[i].getArea());//+ " perimeter: "+std::to_string(shapedetector[i].getPerimeter());
    putText(drawing, text,shapedetector[i].getCenter()+Point(10,15), fontFace, fontScale,
        Scalar::all(255), thickness, 8);

    text ="perimeter: "+std::to_string(shapedetector[i].getPerimeter());
    putText(drawing, text,shapedetector[i].getCenter()+Point(10,30), fontFace, fontScale, Scalar::all(255), thickness, 8);

    text = "shape: "+ shapedetector[i].getShape()+" area: "+  
    std::to_string(shapedetector[i].getArea())+" perimeter: "+       
    std::to_string(shapedetector[i].getPerimeter())+
    "center (x,y): ("+ std::to_string(shapedetector[i].getCenter().x)+","+ std::to_string(shapedetector[i].getCenter().y)+")";
    cout<< text<<endl;

  }


  imshow("Contours", drawing);

  waitKey(0);
   

  

  return 0;
}
