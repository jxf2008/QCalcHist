#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "QCalcHist.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

QCalcHist::QCalcHist(QWidget* parent) :QDialog(parent), current_img_channels_nu(0){
	pushbutton_imgpath  = new QPushButton(tr("..."));
	pushbutton_img      = new QPushButton();
	pushbutton_close    = new QPushButton(tr("关闭"));
	pushbutton_calchist = new QPushButton(tr("直方图"));
	lineedit_imgpath    = new QLineEdit();
	dialog_show_histogram = new ShowHistogram(this);

	label_channels = new QLabel(tr("统计通道"));
	label_channel_1_bin = new QLabel(tr("通道一区间数"));
	label_channel_2_bin = new QLabel(tr("通道二区间数"));
	label_channel_3_bin = new QLabel(tr("通道三区间数"));

	slider_channel_1 = new QSlider(Qt::Horizontal);
	slider_channel_2 = new QSlider(Qt::Horizontal);
	slider_channel_3 = new QSlider(Qt::Horizontal);

	spinbox_channel_1 = new QSpinBox();
	spinbox_channel_2 = new QSpinBox();
	spinbox_channel_3 = new QSpinBox();

	groupbox_histogram = new QGroupBox(tr("直方图信息"));

	pushbutton_img->setFixedSize(QSize(IMG_WIDTH, IMG_HEIGHT));
	QPixmap img(tr(":/images/empty.jpg"));
	QSize size = img.size();
	pushbutton_img->setIcon(QIcon(img));
	pushbutton_img->setIconSize(size);

	lineedit_imgpath->setEnabled(false);

	pushbutton_imgpath->setFixedWidth(OPEN_PATH_WIDTH);

	slider_channel_1->setRange(10, 25);
	slider_channel_2->setRange(10, 25);
	slider_channel_3->setRange(10, 25);

	spinbox_channel_1->setRange(10, 25);
	spinbox_channel_2->setRange(10, 25);
	spinbox_channel_3->setRange(10, 25);

	dialog_show_histogram->hide();

	QHBoxLayout* layout_imgpath = new QHBoxLayout;
	layout_imgpath->addWidget(lineedit_imgpath);
	layout_imgpath->addWidget(pushbutton_imgpath);
	QHBoxLayout* layout_button = new QHBoxLayout;
	layout_button->addWidget(pushbutton_calchist);
	layout_button->addWidget(pushbutton_close);

	QHBoxLayout* layout_channel_1 = new QHBoxLayout;
	layout_channel_1->addWidget(label_channel_1_bin);
	layout_channel_1->addWidget(slider_channel_1);
	layout_channel_1->addWidget(spinbox_channel_1);
	QHBoxLayout* layout_channel_2 = new QHBoxLayout;
	layout_channel_2->addWidget(label_channel_2_bin);
	layout_channel_2->addWidget(slider_channel_2);
	layout_channel_2->addWidget(spinbox_channel_2);
	QHBoxLayout* layout_channel_3 = new QHBoxLayout;
	layout_channel_3->addWidget(label_channel_3_bin);
	layout_channel_3->addWidget(slider_channel_3);
	layout_channel_3->addWidget(spinbox_channel_3);
	QVBoxLayout* layout_channel = new QVBoxLayout;
	layout_channel->addLayout(layout_channel_1);
	layout_channel->addLayout(layout_channel_2);
	layout_channel->addLayout(layout_channel_3);
	groupbox_histogram->setLayout(layout_channel);

	QVBoxLayout* layout_main = new QVBoxLayout;
	layout_main->addLayout(layout_imgpath);
	layout_main->addWidget(pushbutton_img);
	layout_main->addWidget(groupbox_histogram);
	layout_main->addLayout(layout_button);

	setLayout(layout_main);

	connect(pushbutton_imgpath, SIGNAL(clicked()), this, SLOT(choose_img()));
	connect(pushbutton_close, SIGNAL(clicked()), this, SLOT(close_this()));
	connect(pushbutton_calchist, SIGNAL(clicked()), this, SLOT(show_histogram()));
	connect(slider_channel_1, SIGNAL(valueChanged(int)), spinbox_channel_1, SLOT(setValue(int)));
	connect(slider_channel_2, SIGNAL(valueChanged(int)), spinbox_channel_2, SLOT(setValue(int)));
	connect(slider_channel_3, SIGNAL(valueChanged(int)), spinbox_channel_3, SLOT(setValue(int)));
	connect(spinbox_channel_1, SIGNAL(valueChanged(int)), slider_channel_1, SLOT(setValue(int)));
	connect(spinbox_channel_2, SIGNAL(valueChanged(int)), slider_channel_2, SLOT(setValue(int)));
	connect(spinbox_channel_3, SIGNAL(valueChanged(int)), slider_channel_3, SLOT(setValue(int)));

	slider_channel_1->setValue(15);
	slider_channel_2->setValue(15);
	slider_channel_3->setValue(15);
}

void QCalcHist::choose_img() {
	QString path = QFileDialog::getOpenFileName(this, tr("选择图片"), ".", tr("Images (*.png *.bmp *.jpg *.jepg)"));
	if (path.isEmpty())
		return;

	mat_current = cv::imread(path.toStdString());
	current_img_channels_nu = mat_current.channels();
	if (current_img_channels_nu != 1 && current_img_channels_nu != 3) {
		QMessageBox::warning(this, tr("图片错误"), tr("目前版本只能处理单通道或三通道图片"), QMessageBox::Yes);
		return;
	}
	
	if (current_img_channels_nu == 3) {
		slider_channel_2->setEnabled(true);
		slider_channel_3->setEnabled(true);
		spinbox_channel_2->setEnabled(true);
		spinbox_channel_3->setEnabled(true);
	}
	if (current_img_channels_nu == 1) {
		slider_channel_2->setEnabled(false);
		slider_channel_3->setEnabled(false);
		spinbox_channel_2->setEnabled(false);
		spinbox_channel_3->setEnabled(false);
	}

	slider_channel_1->setValue(15);
	slider_channel_2->setValue(15);
	slider_channel_3->setValue(15);

	str_current_img_path = path;
	lineedit_imgpath->setText(path);

	QImage img(path);
	if (img.size().width() > IMG_WIDTH || img.size().height() > IMG_HEIGHT) {
		QImage tmp = img.scaled(QSize(IMG_WIDTH, IMG_HEIGHT), Qt::KeepAspectRatio);
		img = tmp;
	}


	pushbutton_img->setIcon(QIcon(QPixmap::fromImage(img)));
	pushbutton_img->setIconSize(QSize(IMG_WIDTH, IMG_HEIGHT));
}

void QCalcHist::close_this() {
	close();
}

void QCalcHist::show_histogram() {
	if (mat_current.empty()) {
		QMessageBox::warning(this, tr("空图片"), tr("请选择图片"), QMessageBox::Yes);
		return;
	}
	dialog_show_histogram->set_img(mat_current);
	float range_1[] = { 0,255 }; //目前默认范围时0-255
	float range_2[] = { 0,255 };
	float range_3[] = { 0,255 };
	const float* range_all[] = { range_1,range_2,range_3 };
	int bin1_nu = slider_channel_1->value();
	int bin2_nu = slider_channel_2->value();
	int bin3_nu = slider_channel_3->value();
	const int bin[] = { bin1_nu,bin2_nu,bin3_nu };
	const int channels[] = { 0,1,2 };
	dialog_show_histogram->calc_hist(range_all, bin, channels, current_img_channels_nu);
	dialog_show_histogram->exec();
}
