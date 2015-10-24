#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include <list>
#include <memory>

#include "Typedefs.hpp"
#include "Router.hpp"
#include "UiMainWindow.hpp"
#include "RouterView.hpp"
#include "IController.hpp"
#include "IObserver.hpp"

class RouterManagementDialog : public QDialog, IObserver
{
    Q_OBJECT

public:
    explicit RouterManagementDialog(std::shared_ptr<Router>, std::shared_ptr<IController>, QWidget*);
    ~RouterManagementDialog(){}

private slots:
    void setRouterName();
    void setPortName();
    void removePort();
    void addPort();
    void update();

private:
    void subscribeForUpdate();
    void subscribeForPortsUpdate();
    void repopulatePortsList();
    void fillPortsComboBox(QComboBox *combo);
    void setButtonSize(QPushButton *b);
    QLineEdit *routerNameField;
    QLineEdit *portNameField;
    QLineEdit *addPortNameField;
    QCheckBox *isInputCheckBox;
    QHBoxLayout *mainLayout;
    std::shared_ptr<IController> nodesEditor;
    QComboBox *renamePortsComboBox;
    QComboBox *removePortsComboBox;
    QGraphicsView *view;
    std::shared_ptr<Router> router;
    RouterId id;
    std::map<PortId, std::shared_ptr<Port> > ports;

};
