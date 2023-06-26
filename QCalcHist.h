#ifndef QCALCHIST_H__
#define QCALCHIST_H__

#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include "ShowHistogram.h"

class ShowHistogram;

class QCalcHist : public QDialog{
	Q_OBJECT
private:
	int current_img_channels_nu;
	cv::Mat mat_current;
	QPushButton* pushbutton_imgpath;
	QPushButton* pushbutton_img;
	QPushButton* pushbutton_close;
	QPushButton* pushbutton_calchist;

	QLineEdit* lineedit_imgpath;

	QCheckBox* checkbox_channel_1;
	QCheckBox* checkbox_channel_2;
	QCheckBox* checkbox_channel_3;

	QLabel* label_channels;
	QLabel* label_channel_1_bin;
	QLabel* label_channel_2_bin;
	QLabel* label_channel_3_bin;

	QSlider* slider_channel_1;
	QSlider* slider_channel_2;
	QSlider* slider_channel_3;

	QSpinBox* spinbox_channel_1;
	QSpinBox* spinbox_channel_2;
	QSpinBox* spinbox_channel_3;

	QGroupBox* groupbox_histogram;

	ShowHistogram* dialog_show_histogram;

	QString str_current_img_path = DEFAULT_IMG;
public:
	QCalcHist(QWidget* parent = nullptr);
private slots:
	void choose_img();
	void close_this();
	void show_histogram();
};

#endif