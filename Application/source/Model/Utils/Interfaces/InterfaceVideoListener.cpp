#include "Model/Utils/Interfaces/InterfaceVideoListener.h"

InterfaceVideoListener::InterfaceVideoListener(QLabel* label)
	: m_label(label)
{
}

void InterfaceVideoListener::Update(const cv::Mat& mat) {
	try {
		cv::Mat resizedMat;
		cv::resize(mat, resizedMat, cv::Size(640, 480), cv::INTER_LINEAR);

		QPixmap pixmap = ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(resizedMat));

		QMetaObject::invokeMethod(m_label, "setPixmap", Qt::QueuedConnection, Q_ARG(QPixmap, pixmap));
	
	}
	catch (const std::exception& exception) {
		throw std::runtime_error(exception.what());
	}
}




