#include <Controller/ParkingSetupWindowController.h>

ParkingSetupWindowController::ParkingSetupWindowController(ClickableLabel* label)
	: m_label(label)
{
}

void ParkingSetupWindowController::GetAllParkingCamerasInMap(QHash<QListWidgetItem*, DatabaseEntity::Camera>& map)
{
	std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

	for (const auto& camera : cameras) {
		if (camera.GetCameraType().GetType() == DatabaseEntity::CameraType::Type::PARKING)
			map[new QListWidgetItem()] = camera;
	}
}

void ParkingSetupWindowController::GetAllSpacesForParkingCamera(const DatabaseEntity::Camera& camera)
{
}

void ParkingSetupWindowController::ShowParkingSpace(const QPoint& first, const QPoint& second)
{
	cv::Mat originalImage = ImageConversion::QImageToMat(ImageConversion::QPixmapToQImage(m_label->GetOriginalImage()));
	cv::Mat imageWithDrawnParkingSpace;
	cv::resize(originalImage, originalImage, cv::Size(WIDTH, HEIGHT));
	Utils::ShowRectangleOnImage(originalImage, imageWithDrawnParkingSpace, cv::Point2d(first.x(), first.y()), cv::Point2d(second.x(), second.y()));
	m_label->ChangeImageOnLabel(ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(imageWithDrawnParkingSpace)));
}

void ParkingSetupWindowController::TakeSingleFrameFromCamera(const DatabaseEntity::Camera& camera)
{
	VideoCamera* videoCamera = new VideoCamera();
	camera.IsLocationAnIndex() ? videoCamera->OpenCamera(std::stoi(camera.GetLocation())) : videoCamera->OpenCamera(camera.GetLocation());
	cv::Mat firstFrame = videoCamera->ReadNextFrame();
	videoCamera->StopCamera();
	delete videoCamera;
	scaleFactorX = (double)firstFrame.size().width / (double)WIDTH;
	scaleFactorY = (double)firstFrame.size().height / (double)HEIGHT;
	m_label->ChangeOriginalImage(ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(firstFrame)));
	cv::resize(firstFrame, firstFrame, cv::Size(WIDTH, HEIGHT));
	m_label->ChangeImageOnLabel(ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(firstFrame)));
}

void ParkingSetupWindowController::Reset()
{
	cv::Mat originalImage = ImageConversion::QImageToMat(ImageConversion::QPixmapToQImage(m_label->GetOriginalImage()));
	m_label->ResetPoints();
	cv::resize(originalImage, originalImage, cv::Size(WIDTH, HEIGHT));
	m_label->ChangeImageOnLabel(ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(originalImage)));
}
