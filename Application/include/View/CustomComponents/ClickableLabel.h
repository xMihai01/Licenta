
#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QMessageBox>

class ClickableLabel : public QLabel
{
    Q_OBJECT 

public:
    ClickableLabel(QWidget* parent = nullptr);

    void ChangeImageOnLabel(const QPixmap& pixmap);
    void ChangeOriginalImage(const QPixmap& pixmap);
    void ResetPoints();

    bool ConfirmPoints();

    QPoint GetFirstPoint() const;
    QPoint GetSecondPoint() const;
    QPixmap GetOriginalImage() const;

signals:

    void draw();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:

    QPoint firstPoint;
    QPoint secondPoint;

    QPixmap originalImage;
};

#endif