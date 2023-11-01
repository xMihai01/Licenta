#include "Model/Utils.h"

std::pair<cv::Mat, cv::Mat> Utils::GetHistograms(const cv::Mat grayImage)
{
	if (grayImage.channels() != 1)
		throw new std::runtime_error("GetHistogram() requires a gray image, but it has been given a color image");

	int histogramSize = 256;
	float range[] = { 0, 256 };
	const float* histogramRange = { range };

	cv::Mat histogram;

	cv::calcHist(&grayImage, 1, 0, cv::Mat(), histogram, 1, &histogramSize, &histogramRange);

	cv::Mat cumulativeHistogram(histogram.rows, histogram.cols, histogram.type());

	cumulativeHistogram.at<float>(0) = histogram.at<float>(0);
	for (int i = 1; i < histogramSize; i++) {
		cumulativeHistogram.at<float>(i) = cumulativeHistogram.at<float>(i - 1) + histogram.at<float>(i);
	}

	cv::normalize(histogram, histogram, 0, grayImage.rows, cv::NORM_MINMAX, -1);
	cv::normalize(cumulativeHistogram, cumulativeHistogram, 0, grayImage.rows, cv::NORM_MINMAX, -1);

	int binWidth = cvRound((double)HISTOGRAM_WIDTH / histogramSize);

	cv::Mat histogramImage(HISTOGRAM_HEIGHT, HISTOGRAM_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat cumulativeHistogramImage(HISTOGRAM_HEIGHT, HISTOGRAM_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));


	for (int i = 1; i < histogramSize; i++) {
		cv::line(histogramImage,
			cv::Point(binWidth * (i - 1), HISTOGRAM_HEIGHT - cvRound(histogram.at<float>(i - 1))),
			cv::Point(binWidth * (i), HISTOGRAM_HEIGHT - cvRound(histogram.at<float>(i))),
			cv::Scalar(255, 255, 255), 2, 8, 0);
	
		cv::line(cumulativeHistogramImage,
			cv::Point(binWidth * (i - 1), HISTOGRAM_HEIGHT - cvRound(cumulativeHistogram.at<float>(i - 1))),
			cv::Point(binWidth * (i), HISTOGRAM_HEIGHT - cvRound(cumulativeHistogram.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0);
		
	}
	return std::make_pair(histogramImage, cumulativeHistogramImage);
}

std::vector<std::string> Utils::GetImageNamesFromFile(const std::string& path)
{
	std::vector<std::string> imageNames;
	std::ifstream file(path);

	std::string imageName;

	while (file >> imageName)
		imageNames.push_back(imageName);
	
	return imageNames;
}
