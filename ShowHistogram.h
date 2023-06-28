#include <QDialog>
#include <QCloseEvent>
#include <QRadioButton>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <opencv.hpp>
#include <vector>
#include "constdata.h"

QT_CHARTS_USE_NAMESPACE

class ShowHistogram : public QDialog{
	Q_OBJECT
private:
	cv::Mat mat_current;
	std::vector<cv::Mat> mat_dst;
	std::vector<int> hist_bins;

	QBarSeries* series;
	QBarCategoryAxis* axisX;
	QValueAxis* axisY;

	QRadioButton* radiobutton_channel_1;
	QRadioButton* radiobutton_channel_2;
	QRadioButton* radiobutton_channel_3;
public:
	ShowHistogram(QWidget* parent = nullptr);
	void set_img(const cv::Mat& mat);
	void calc_hist(const float** range, const int* bins, const int* channels, int channel_count);
protected:
	void closeEvent(QCloseEvent* event);
private slots:
	void draw_hist_channel1();
	void draw_hist_channel2();
	void draw_hist_channel3();
};

