#ifndef IMAGEVIEWTOOLFORM_H
#define IMAGEVIEWTOOLFORM_H

#include <QWidget>

namespace Ui {
class imageViewToolForm;
}

class ImageViewToolBox : public QWidget
{
    Q_OBJECT

public:
	explicit ImageViewToolBox(QWidget *parent = 0);
	~ImageViewToolBox();

	void SetBtnNormal(bool bvisible, bool benable);
	void SetBtnMax(bool bvisible, bool benable);
	void SetBtnRestore(bool bvisible, bool benable);
	void SetBtnMagnify(bool bvisible, bool benable);
	void SetBtnReduce(bool bvisible, bool benable);
	void SetBtnRotate(bool bvisible, bool benable);
	void SetBtnSave(bool bvisible, bool benable);

private:
	void ModifyUI();
	void SetCursor();

private slots:
	void OnImageNormal();
	void OnImageMax();
	void OnImageRestore();
	void OnImageMagnify();
	void OnImageReduce();
	void OnImageRotate();
	void OnImageSave();

private:
    Ui::imageViewToolForm *ui;
};

#endif // IMAGEVIEWTOOLFORM_H
