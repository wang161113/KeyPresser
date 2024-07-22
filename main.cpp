#include <QApplication>
#include "keypresser.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    KeyPresser keyPresser;
    keyPresser.setWindowIcon(QIcon(":/keypresser.ico"));
    keyPresser.show();

    return app.exec();
}
