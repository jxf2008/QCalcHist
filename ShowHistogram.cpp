#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ShowHistogram.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

ShowHistogram::ShowHistogram(QWidget* parent) :QDialog(parent) {
    series = new QBarSeries();

    radiobutton_channel_1 = new QRadioButton(tr("通道一"));
    radiobutton_channel_2 = new QRadioButton(tr("通道二"));
    radiobutton_channel_3 = new QRadioButton(tr("通道三"));

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("直方图"));
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

    radiobutton_channel_1->setChecked(true);
    radiobutton_channel_2->hide();
    radiobutton_channel_3->hide();

    QHBoxLayout* layout_channel = new QHBoxLayout;
    layout_channel->addWidget(radiobutton_channel_1);
    layout_channel->addWidget(radiobutton_channel_2);
    layout_channel->addWidget(radiobutton_channel_3);
    layout_channel->addStretch();
    
    QVBoxLayout* layout_main = new QVBoxLayout;
    layout_main->addWidget(chartView);
    layout_main->addLayout(layout_channel);
    setLayout(layout_main);
    layout_main->setSizeConstraint(QLayout::SetFixedSize);

    connect(radiobutton_channel_1, SIGNAL(clicked()), this, SLOT(draw_hist_channel1()));
    connect(radiobutton_channel_2, SIGNAL(clicked()), this, SLOT(draw_hist_channel2()));
    connect(radiobutton_channel_3, SIGNAL(clicked()), this, SLOT(draw_hist_channel3()));
}

void ShowHistogram::set_img(const cv::Mat& mat) {
    mat_current = mat;
    if (mat_current.channels() == 1) {
        radiobutton_channel_2->hide();
        radiobutton_channel_3->hide();
    }
    else {
        radiobutton_channel_2->show();
        radiobutton_channel_3->show();
    }
    radiobutton_channel_1->setChecked(true);
}

void ShowHistogram::calc_hist(const float** range, const int* bins, const int* channels, int channel_count) {
    mat_dst.clear();
    hist_bins.clear();
    //单独计算每个通道的直方图
    for (int i = 0; i < channel_count; ++i) {
        cv::Mat dst;
        cv::calcHist(&mat_current, 1, channels + i, cv::noArray(), dst, 1, bins + i, range + i);
        mat_dst.push_back(dst.clone());

        hist_bins.push_back(*(bins + i));
    }
    draw_hist_channel1();
}

void ShowHistogram::closeEvent(QCloseEvent* event) {
    event->ignore();
    radiobutton_channel_1->clicked(true);
    hide();
}

void ShowHistogram::draw_hist_channel1() {
    double maxvalue = 0;
    cv::minMaxLoc(mat_dst.at(0), nullptr, &maxvalue);

    for (int i = 0; i < hist_bins.at(0); ++i) {
        float v = mat_dst.at(0).at<float>(i);
        maxvalue = (maxvalue < v ? v : maxvalue);
    }

    QBarSet* set0 = new QBarSet("");
    QStringList categories;
    for (int i = 0; i < hist_bins.at(0); ++i) {
        float v = mat_dst.at(0).at<float>(i);
        v = 100 * (v / maxvalue);
        *set0 << static_cast<int>(v);
        QString nm = QString::number(i + 1);
        categories << nm;
    }
    axisX->clear();
    axisX->append(categories);
    series->clear();
    series->append(set0);

    axisY->setRange(0, 100);
}

void ShowHistogram::draw_hist_channel2() {
    double maxvalue = 0;
    cv::minMaxLoc(mat_dst.at(1), nullptr, &maxvalue);

    QBarSet* set0 = new QBarSet("");
    QStringList categories;
    for (int i = 0; i < hist_bins.at(1); ++i) {
        float v = mat_dst.at(1).at<float>(i);
        v = 100 * (v / maxvalue);
        *set0 << static_cast<int>(v);
        QString nm = QString::number(i + 1);
        categories << nm;
    }
    series->clear();
    series->append(set0);
    axisX->clear();
    axisX->append(categories);

    axisY->setRange(0, 100);
}

void ShowHistogram::draw_hist_channel3() {
    double maxvalue = 0;
    cv::minMaxLoc(mat_dst.at(2), nullptr, &maxvalue);

    for (int i = 0; i < hist_bins.at(2); ++i) {
        float v = mat_dst.at(2).at<float>(i);
        maxvalue = (maxvalue < v ? v : maxvalue);
    }

    QBarSet* set0 = new QBarSet("");
    QStringList categories;
    for (int i = 0; i < hist_bins.at(2); ++i) {
        float v = mat_dst.at(2).at<float>(i);
        v = 100 * (v / maxvalue);
        *set0 << static_cast<int>(v);
        QString nm = QString::number(i + 1);
        categories << nm;
    }
    series->clear();
    series->append(set0);
    axisX->clear();
    axisX->append(categories);

    axisY->setRange(0, 100);
}