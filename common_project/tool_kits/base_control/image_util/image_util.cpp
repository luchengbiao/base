#include "image_util.h"
#include "QPainter"

QImage ImageUtil::MakeThumbnail(std::wstring file_path, int width, int height)
{
	QImage image;
	QImage background(width, height, QImage::Format_ARGB32);
	background.fill(Qt::white);
	if (image.load(QString::fromStdWString(file_path))
		&& !background.isNull())
	{
		image = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		int x = (background.width() - image.width()) / 2;
		int y = (background.height() - image.height()) / 2;
		QPainter painter(&background);
		painter.drawImage(x, y, image);
	}
	return background;
}

