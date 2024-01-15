#include "View/CustomComponents/ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent) {
    firstPoint = QPoint(-1, 0);
    secondPoint = QPoint(-1, 0);
}

void ClickableLabel::ChangeImageOnLabel(const QPixmap& pixmap)
{
    this->setPixmap(pixmap);
}

void ClickableLabel::ChangeOriginalImage(const QPixmap& pixmap)
{
    originalImage = pixmap;
}

void ClickableLabel::ResetPoints()
{
    firstPoint = QPoint(-1, 0);
    secondPoint = QPoint(-1, 0);
}

bool ClickableLabel::ConfirmPoints()
{
    return firstPoint.x() != -1 && secondPoint.x() != -1;
}

QPoint ClickableLabel::GetFirstPoint() const
{
    return firstPoint;
}

QPoint ClickableLabel::GetSecondPoint() const
{
    return secondPoint;
}

QPixmap ClickableLabel::GetOriginalImage() const
{
    return originalImage;
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = event->pos();

        if (clickPos.x() > 640)
            clickPos.setX(640);
        if (clickPos.y() > 480)
            clickPos.setY(480);

        if (firstPoint.x() == -1) {
            firstPoint = clickPos;
            QMessageBox::about(NULL, "Info", "First point set to " + QString::number(firstPoint.x()) + ", " + QString::number(firstPoint.y()));
        }
        else if (secondPoint.x() == -1) {
            secondPoint = clickPos;
            QMessageBox::about(NULL, "Info", "Second point set to " + QString::number(secondPoint.x()) + ", " + QString::number(secondPoint.y()));
            emit draw();
        }
        else {
            QMessageBox::warning(NULL, "Warning!", "Points are already set. If you think you might have made a mistake, click Reset Box.");
        }
    }

    QLabel::mousePressEvent(event);
}
