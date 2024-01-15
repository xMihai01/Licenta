#include <Controller/ParkingSetupWindowController.h>

ParkingSetupWindowController::ParkingSetupWindowController(ClickableLabel* label)
	: m_label(label)
{
}

void ParkingSetupWindowController::GetAllParkingCamerasInMap(QHash<QListWidgetItem*, DatabaseEntity::Camera>& map)
{
	if (map.size() > 0) {
		for (const auto& item : map.asKeyValueRange()) {
			delete item.first;
		}
	}
	std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

	for (const auto& camera : cameras) {
		if (camera.GetCameraType().GetType() == DatabaseEntity::CameraType::Type::PARKING)
			map[new QListWidgetItem()] = camera;
	}
}

void ParkingSetupWindowController::GetAllSpacesForParkingCamera(QHash<QListWidgetItem*, DatabaseEntity::ParkingSpace>& map, const DatabaseEntity::Camera& camera)
{
	if (map.size() > 0) {
		for (const auto& item : map.asKeyValueRange()) {
			delete item.first;
		}
	}
	
	std::vector<DatabaseEntity::ParkingSpace> spaces = m_database.ToParkingSpace().FindAllByCamera(camera);
	for (const auto& space : spaces) {
		map[new QListWidgetItem()] = space;
	}
}

void ParkingSetupWindowController::ShowParkingSpace(const QPoint& first, const QPoint& second)
{
	cv::Mat originalImage = ImageConversion::QImageToMat(ImageConversion::QPixmapToQImage(m_label->GetOriginalImage()));
	cv::Mat imageWithDrawnParkingSpace;
	cv::resize(originalImage, originalImage, cv::Size(WIDTH, HEIGHT));
	Utils::ShowRectangleOnImage(originalImage, imageWithDrawnParkingSpace, cv::Point2d(first.x(), first.y()), cv::Point2d(second.x(), second.y()));
	m_label->ChangeImageOnLabel(ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(imageWithDrawnParkingSpace)));

}

void ParkingSetupWindowController::ShowParkingSpace(const DatabaseEntity::ParkingSpace& parkingSpace)
{
	cv::Mat originalImage = ImageConversion::QImageToMat(ImageConversion::QPixmapToQImage(m_label->GetOriginalImage()));
	cv::Mat imageWithDrawnParkingSpace;
	Utils::ShowRectangleOnImage(originalImage, imageWithDrawnParkingSpace, cv::Point2d(parkingSpace.GetX1(), parkingSpace.GetY1())
		, cv::Point2d(parkingSpace.GetX2(), parkingSpace.GetY2()));
	cv::resize(imageWithDrawnParkingSpace, imageWithDrawnParkingSpace, cv::Size(WIDTH, HEIGHT));
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

void ParkingSetupWindowController::AddParkingSpace(const DatabaseEntity::Camera& camera, const QPoint& first, const QPoint& second, const QString& name)
{
	try {
		QPoint scaledFirst = QPoint((double)first.x() * scaleFactorX, (double)first.y() * scaleFactorY);
		QPoint scaledSecond = QPoint((double)second.x() * scaleFactorX, (double)second.y() * scaleFactorY);
		m_database.ToParkingSpace().Add(DatabaseEntity::ParkingSpace(camera, scaledFirst.x(), scaledSecond.x(), scaledFirst.y(), scaledSecond.y(), name.toStdString()));
	}
	catch (const std::exception& exception) {
		throw exception;
	}
}

void ParkingSetupWindowController::UpdateParkingSpace(const DatabaseEntity::ParkingSpace& parkingSpace, const QPoint& first, const QPoint& second, const QString& name)
{
	try {
		QPoint scaledFirst = QPoint((double)first.x() * scaleFactorX, (double)first.y() * scaleFactorY);
		QPoint scaledSecond = QPoint((double)second.x() * scaleFactorX, (double)second.y() * scaleFactorY);
		m_database.ToParkingSpace().Update(DatabaseEntity::ParkingSpace(parkingSpace.GetID(), parkingSpace.GetCamera(), scaledFirst.x(), scaledSecond.x(), scaledFirst.y(), scaledSecond.y(), name.toStdString()));
	}
	catch (const std::exception& exception) {
		throw exception;
	}
}

void ParkingSetupWindowController::RemoveParkingSpace(const DatabaseEntity::ParkingSpace& parkingSpace)
{
	try {
		m_database.ToParkingSpace().Remove(parkingSpace);
	}
	catch (const std::exception& exception) {
		throw exception;
	}
}

void ParkingSetupWindowController::Reset()
{
	cv::Mat originalImage = ImageConversion::QImageToMat(ImageConversion::QPixmapToQImage(m_label->GetOriginalImage()));
	m_label->ResetPoints();
	cv::resize(originalImage, originalImage, cv::Size(WIDTH, HEIGHT));
	m_label->ChangeImageOnLabel(ImageConversion::QImageToQPixmap(ImageConversion::MatToQImage(originalImage)));
}
