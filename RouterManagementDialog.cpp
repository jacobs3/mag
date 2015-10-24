#include "RouterManagementDialog.hpp"
#include <iterator>

RouterManagementDialog::RouterManagementDialog(std::shared_ptr<Router> r, std::shared_ptr<IController> editor, QWidget *widget)
: QDialog(widget)
{
    nodesEditor = editor;
    id = r->getId();
    router = r;
    subscribeForUpdate();
    repopulatePortsList();
    mainLayout = new QHBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    setLayout(mainLayout);

    routerNameField = new QLineEdit (tr(r->getName().c_str()));
    routerNameField->setText(tr(r->getName().c_str()));
    routerNameField->setFixedWidth(150);
    QPushButton *routerNameButton = new QPushButton(tr("Set router name"));
    setButtonSize(routerNameButton);


    connect(routerNameButton, SIGNAL(pressed()),
                         this, SLOT(setRouterName()));

    leftLayout->addWidget(routerNameField);
    rightLayout->addWidget(routerNameButton);

    QHBoxLayout *renamePortLayout = new QHBoxLayout;
    renamePortsComboBox = new QComboBox;
    renamePortsComboBox->setFixedWidth(100);
    fillPortsComboBox(renamePortsComboBox);

    portNameField = new QLineEdit (tr("portName"));
    portNameField->setText(tr("portName"));
    portNameField->setFixedWidth(100);
    QPushButton *portNameButton = new QPushButton(tr("Set port name"));
    setButtonSize(portNameButton);
    connect(portNameButton, SIGNAL(pressed()),
                         this, SLOT(setPortName()));

    renamePortLayout->addWidget(renamePortsComboBox);
    renamePortLayout->addWidget(portNameField);
    leftLayout->addLayout(renamePortLayout);
    rightLayout->addWidget(portNameButton);

    removePortsComboBox = new QComboBox;
    removePortsComboBox->setFixedWidth(100);
    fillPortsComboBox(removePortsComboBox);
    QPushButton *removePortButton = new QPushButton(tr("Remove port"));
    setButtonSize(removePortButton);
    connect(removePortButton, SIGNAL(pressed()),
                         this, SLOT(removePort()));

    leftLayout->addWidget(removePortsComboBox);
    rightLayout->addWidget(removePortButton);

    QHBoxLayout *addPortLayout = new QHBoxLayout;
    addPortNameField = new QLineEdit (tr("portName"));
    addPortNameField->setText(tr("portName"));
    addPortNameField->setFixedWidth(150);
    isInputCheckBox = new QCheckBox(tr("is input"));
    QPushButton *addPortButton = new QPushButton(tr("Add port"));
    setButtonSize(addPortButton);
    connect(addPortButton, SIGNAL(pressed()),
                         this, SLOT(addPort()));

    addPortLayout->addWidget(addPortNameField);
    addPortLayout->addWidget(isInputCheckBox);
    leftLayout->addLayout(addPortLayout);
    rightLayout->addWidget(addPortButton);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    this->setWindowTitle("Router management [" + QString::fromUtf8(router->getName().c_str()) + "]");
}

void RouterManagementDialog::setButtonSize(QPushButton *b)
{
    b->setFixedSize(QSize(100,22));
}

void RouterManagementDialog::setRouterName()
{
    if(!nodesEditor->setRouterName(id, routerNameField->text().toStdString()))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText(QString::fromUtf8("Given router name exists. Please change name"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        if(msgBox.exec() == QMessageBox::Ok)
        {
          msgBox.close();
        }
    }
}

void RouterManagementDialog::setPortName()
{
    int selectedPortIndex = renamePortsComboBox->currentIndex();
    auto it = ports.begin();
    std::advance (it,selectedPortIndex);
    nodesEditor->setPortName(it->first, portNameField->text().toStdString());
}

void RouterManagementDialog::removePort()
{
    int selectedPortIndex = removePortsComboBox->currentIndex();
    auto it = ports.begin();
    std::advance (it,selectedPortIndex);
    nodesEditor->removePort(id, it->first);
}

void RouterManagementDialog::addPort()
{
    nodesEditor->addPort(id, addPortNameField->text().toStdString(), isInputCheckBox->isChecked());
}

void RouterManagementDialog::repopulatePortsList()
{
    ports.clear();
    ports = router->getPorts();
}

void RouterManagementDialog::subscribeForUpdate()
{
    router->attach(this);
    for(auto p : router->getPorts())
    {
        p.second->attach(this);
    }
}

void RouterManagementDialog::fillPortsComboBox(QComboBox *combo)
{
    combo->clear();
    for(auto port: ports)
    {
        combo->addItem(tr(port.second->getName().c_str()));
    }
}

void RouterManagementDialog::update()
{
    repopulatePortsList();
    subscribeForPortsUpdate();
    fillPortsComboBox(renamePortsComboBox);
    fillPortsComboBox(removePortsComboBox);
    routerNameField->setText(tr(router->getName().c_str()));
    this->setWindowTitle("Router management [" + QString::fromUtf8(router->getName().c_str()) + "]");
}

void RouterManagementDialog::subscribeForPortsUpdate()
{
    for(auto p : router->getPorts())
    {
        p.second->dettach(this);
        p.second->attach(this);
    }
}
