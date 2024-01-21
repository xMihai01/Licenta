#pragma once
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>

class ImageConversion {

public:

	static QImage MatToQImage(const cv::Mat& opencv_image);
	static QImage QPixmapToQImage(const QPixmap& Qt_pixmap);

	static QPixmap QImageToQPixmap(const QImage& Qt_image);
	static cv::Mat QImageToMat(const QImage& Qt_image);

private:

};