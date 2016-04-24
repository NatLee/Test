#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <cstdio>

#define range 10

using namespace std;
using namespace cv;

bool color(double b, double g, double r) {
	if ((b > 75 - range && b < 75 + range) && (g>97 - range && g < 97 + range) && (r>125 - range && r < 125 + range))
		return true;
	else
		return false;
}


int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;
	namedWindow("Original", WINDOW_AUTOSIZE);
	namedWindow("Shirt", WINDOW_AUTOSIZE);
	for (;;) {
		Mat frame;
		cap >> frame;

		flip(frame, frame, 1);

		Mat shirt = Mat(Size(frame.cols, frame.rows), CV_8UC1, Scalar::all(0));
		double b, g, r;

		bool check = false;

		for (int row = frame.rows - 1; row >= 0; --row)
		{
			for (int col = frame.cols - 1; col >= 0; --col)
			{
				b = frame.at<Vec3b>(row, col)[0];//B
				g = frame.at<Vec3b>(row, col)[1];//G
				r = frame.at<Vec3b>(row, col)[2];//R
				int count = 0;
				for (int k = col; k > col - 100 && col>100; k--) {
					if (color(b, g, r)) {
						count++;
					}
					else {
						break;
					}
				}
				if (count == 100) {
					for (int lineLength = col; lineLength > col - 100 && col > 100; lineLength--) {
						for (int lineWidth = row; lineWidth > row - 5 && row > 5; lineWidth--) {
							frame.at<Vec3b>(lineWidth, lineLength)[0] = 0;
							frame.at<Vec3b>(lineWidth, lineLength)[1] = 255;
							frame.at<Vec3b>(lineWidth, lineLength)[2] = 0;
						}
					}
					check = true;
					break;
				}

			}
			if (check) {
				break;
			}
		}
		if (check)
			cout << "Yee" << endl;
		imshow("Original", frame);
		imshow("Shirt", shirt);

		if (waitKey(30) >= 0) break;
	}
	return 0;
}
