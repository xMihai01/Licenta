#pragma once
#include <opencv2/opencv.hpp>
#include <QLabel>
#include "IVideoListener.h"
#include "Model/Utils/ImageConversion.h"

#include <QMessageBox>

class InterfaceVideoListener : public IVideoListener 
{
public:

	InterfaceVideoListener(QLabel* label);

	void Update(const cv::Mat& mat);
	
private:

	QLabel* m_label;

};