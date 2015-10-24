#include "MainWindow.hpp"
#include "Editor.hpp"
#include "Site.hpp"
#include <QtWidgets>
#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    std::shared_ptr<IController> controller = std::make_shared<Editor>(&w);
    std::shared_ptr<Site> site = std::make_shared<Site>();
    w.setController(controller);
    w.setModel(site);
    controller->setModel(site);
    w.start();
    w.show();

    return a.exec();
}
