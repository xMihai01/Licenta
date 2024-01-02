#include "Model/Utils/ImageConversion.h"

QImage ImageConversion::MatToQImage(const cv::Mat& opencv_image)
{

    switch (opencv_image.type())
    {
    case CV_8UC4:

        return QImage(opencv_image.data, opencv_image.cols, opencv_image.rows, static_cast<int>(opencv_image.step), QImage::Format_ARGB32);

    case CV_8UC3:

        return QImage(opencv_image.data, opencv_image.cols, opencv_image.rows, static_cast<int>(opencv_image.step), QImage::Format_BGR888);
    
    case CV_8UC1:

        return QImage(opencv_image.data, opencv_image.cols, opencv_image.rows, static_cast<int>(opencv_image.step), QImage::Format_Grayscale8);

    }

    return QImage();
}

QImage ImageConversion::QPixmapToQImage(const QPixmap& Qt_pixmap)
{
    QImage image = Qt_pixmap.toImage();
    return image.convertToFormat(Qt_pixmap.toImage().format());
}

QPixmap ImageConversion::QImageToQPixmap(const QImage& Qt_image)
{
    return QPixmap::fromImage(Qt_image);
}

cv::Mat ImageConversion::QImageToMat(const QImage& Qt_image)
{
    cv::Mat mat1, mat2;

    switch (Qt_image.format())
    {
    case QImage::Format_ARGB32:
    
        return cv::Mat(Qt_image.height(), Qt_image.width(), CV_8UC4, const_cast<uchar*>(Qt_image.bits()), static_cast<size_t>(Qt_image.bytesPerLine()));
    
    case QImage::Format_RGB32:

        mat1 = cv::Mat(Qt_image.height(), Qt_image.width(), CV_8UC4, const_cast<uchar*>(Qt_image.bits()), static_cast<size_t>(Qt_image.bytesPerLine()));

        cv::cvtColor(mat1, mat2, cv::COLOR_BGRA2BGR);

        return mat2;
  
    case QImage::Format_BGR888:

        return cv::Mat(Qt_image.height(), Qt_image.width(), CV_8UC3, const_cast<uchar*>(Qt_image.bits()), static_cast<size_t>(Qt_image.bytesPerLine())).clone();
    
    case QImage::Format_Indexed8:
    
        return cv::Mat(Qt_image.height(), Qt_image.width(), CV_8UC1, const_cast<uchar*>(Qt_image.bits()), static_cast<size_t>(Qt_image.bytesPerLine()));
    
    }

    return cv::Mat();
}
