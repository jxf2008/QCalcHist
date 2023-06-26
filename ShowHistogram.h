#include <QDialog>
#include <QCloseEvent>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <opencv.hpp>
#include "constdata.h"

QT_CHARTS_USE_NAMESPACE

class ShowHistogram : public QDialog{
private:
	cv::Mat mat_current;
	QBarSeries* series;
	QBarCategoryAxis* axisX;
	QValueAxis* axisY;
public:
	ShowHistogram(QWidget* parent = nullptr);
	void set_img(const cv::Mat& mat) { mat_current = mat; }
	void calc_hist(const float** range, const int* bins, const int* channels, int channel_count);
protected:
	void closeEvent(QCloseEvent* event);
};

