
#include <QHBoxLayout>
#include <vector>
#include "ShowHistogram.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ShowHistogram::ShowHistogram(QWidget* parent) :QDialog(parent) {
    series = new QBarSeries();

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Ö±·½Í¼"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    
    axisX = new QBarCategoryAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedSize(800, 300);
    
    QHBoxLayout* layout_main = new QHBoxLayout;
    layout_main->addWidget(chartView);
    setLayout(layout_main);
    layout_main->setSizeConstraint(QLayout::SetFixedSize);
}

void ShowHistogram::calc_hist(const float** range, const int* bins, const int* channels, int channel_count) {
    cv::Mat mat_dst;
    //cv::cvtColor(mat_current, mat_current, cv::COLOR_BGR2GRAY);
    std::vector<cv::Mat> mats;
    cv::calcHist(&mat_current, 1, channels, cv::noArray(), mat_dst, channel_count, bins, range);

    double maxvalue;
    cv::minMaxLoc(mat_dst, nullptr, &maxvalue);

    QBarSet* set0 = new QBarSet("");
    QStringList categories;
    for (int i = 0; i < (*bins); ++i) {
        float v = mat_dst.at<float>(i);
        v = 255 * (v / maxvalue);
        *set0 << static_cast<int>(v);
        QString nm = QString::number(i + 1);
        categories << nm;
    }
    series->clear();
    series->append(set0);
    axisX->clear();
    axisX->append(categories);
 

    axisY->setRange(*(*(range)), *(*(range)+1));
}

void ShowHistogram::closeEvent(QCloseEvent* event) {
    event->ignore();
    hide();
}