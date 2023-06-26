#include <QApplication>
#include "QCalcHist.h"

int main(int argc, char** argv) {
	QApplication A(argc, argv);
	QCalcHist calchist;
	calchist.show();
	return A.exec();
}