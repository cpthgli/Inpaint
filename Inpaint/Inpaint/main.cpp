#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
	string dir;
	string filename;
	string ext;
	cout << "dir:";
	cin >> dir;
	cout << "filename:";
	cin >> filename;
	cout << "ext:";
	cin >> ext;
	Vec3b red = Vec3b(0, 0, 255);
	Mat src = imread(dir + filename + ext);
	double difference;
	double preDiff = 1000000000;
	int cropSize;
	cout << "cropSize:";
	cin >> cropSize;
	int halfCropSize = cropSize / 2;
	int pasteSize;
	cout << "pasteSize:";
	cin >> pasteSize;
	int pasteSizeAjast = halfCropSize + pasteSize;
	int srcRows = src.rows;
	int srcCols = src.cols;
	int srcRowsLimit = srcRows - halfCropSize;
	int srcColsLimit = srcCols - halfCropSize;
	int srcRowsCrop = srcRows - cropSize;
	int srcColsCrop = srcCols - cropSize;
	int xCropSize, yCropSize;
	int x, y, i, j, k, l;
	for (y = halfCropSize; y < srcRowsLimit; y++) {
		for (x = halfCropSize; x < srcColsLimit; x++) {
			if (src.at<Vec3b>(y, x) == red) {
				cout << endl << y << "," << x << endl;
				Mat paste(Size(pasteSizeAjast, pasteSizeAjast), CV_8UC3, red);
				xCropSize = x - halfCropSize; yCropSize = y - halfCropSize;
				Mat crop(src, Rect(xCropSize, yCropSize, cropSize, cropSize));
				preDiff = 1000000000;
				for (j = 0; j < srcRowsCrop; j++) {
					for (i = 0; i < srcRowsCrop; i++) {
						Mat comp(src, Rect(i, j, cropSize, cropSize));
						difference = 0.0;
						for (l = 0; l < cropSize; l++) {
							for (k = 0; k < cropSize; k++) {
								if (comp.at<Vec3b>(l, k) == red) {
									difference = 1000000000; l = cropSize;	k = cropSize;
								}
								else {
									if (crop.at<Vec3b>(l, k) != red) {
										difference += sqrt(pow(crop.at<Vec3b>(l, k)[0] - comp.at<Vec3b>(l, k)[0], 2)
											+ pow(crop.at<Vec3b>(l, k)[1] - comp.at<Vec3b>(l, k)[1], 2)
											+ pow(crop.at<Vec3b>(l, k)[2] - comp.at<Vec3b>(l, k)[2], 2));
									}
								}
							}
						}
						if (difference < preDiff) {
							preDiff = difference;
							for (l = 0; l < pasteSizeAjast; l++) {
								for (k = 0; k < pasteSizeAjast; k++) {
									if (crop.at<Vec3b>(l, k) == red) paste.at<Vec3b>(l, k) = comp.at<Vec3b>(l, k);
								}
							}
						}
					}
				}
				for (j = 0; j < pasteSizeAjast; j++) {
					for (i = 0; i < pasteSizeAjast; i++) {
						if (paste.at<Vec3b>(j, i) != red) src.at<Vec3b>(yCropSize + j, xCropSize + i) = paste.at<Vec3b>(j, i);
					}
				}
			}
		}
	}
	imwrite(dir + filename + "_inpainted(" + to_string(cropSize) + "," + to_string(pasteSize) + ")" + ext, src);
}