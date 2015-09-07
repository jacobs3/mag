
#include "RouterManagementDialog.hpp"
#include "Editor.hpp"

RouterManagementDialog::RouterManagementDialog(std::shared_ptr<Router> r, Editor *editor, QWidget *widget)
: QDialog(widget)
{
    nodesEditor = editor;
    id = r->getId();
    router = r;
    repopulatePortNamesList();
    mainLayout = new QVBoxLayout;
    QHBoxLayout *routerNameLayout = new QHBoxLayout;
    setLayout(mainLayout);

    routerNameField = new QLineEdit (tr(r->getName().c_str()) );
    QPushButton *routerNameButton = new QPushButton(tr("Set router name"));
    connect(routerNameButton, SIGNAL(pressed()),
                         this, SLOT(setRouterName()));

    routerNameLayout->addWidget(routerNameField);
    routerNameLayout->addWidget(routerNameButton);


    QHBoxLayout *renamePortLayout = new QHBoxLayout;
    renamePortsComboBox = new QComboBox;
    fillPortsComboBox(renamePortsComboBox);

    portNameField = new QLineEdit (tr(r->getName().c_str()) );
    QPushButton *portNameButton = new QPushButton(tr("Set port name"));
    connect(portNameButton, SIGNAL(pressed()),
                         this, SLOT(setPortName()));


    renamePortLayout->addWidget(renamePortsComboBox);
    renamePortLayout->addWidget(portNameField);
    renamePortLayout->addWidget(portNameButton);

    QHBoxLayout *removePortLayout = new QHBoxLayout;
    removePortsComboBox = new QComboBox;
    fillPortsComboBox(removePortsComboBox);
    QPushButton *removePortButton = new QPushButton(tr("Remove port"));
    connect(removePortButton, SIGNAL(pressed()),
                         this, SLOT(removePort()));

    removePortLayout->addWidget(removePortsComboBox);
    removePortLayout->addWidget(removePortButton);


    QHBoxLayout *addPortLayout = new QHBoxLayout;
    addPortNameField = new QLineEdit (tr(r->getName().c_str()));
    isInputCheckBox = new QCheckBox(tr("is input"));
    QPushButton *addPortButton = new QPushButton(tr("Add port"));
    connect(addPortButton, SIGNAL(pressed()),
                         this, SLOT(addPort()));

    addPortLayout->addWidget(addPortNameField);
    addPortLayout->addWidget(isInputCheckBox);
    addPortLayout->addWidget(addPortButton);


    mainLayout->addLayout(routerNameLayout);
    mainLayout->addLayout(renamePortLayout);
    mainLayout->addLayout(removePortLayout);
    mainLayout->addLayout(addPortLayout);

    this->setWindowTitle("Router management");
}

void RouterManagementDialog::setRouterName()
{
    nodesEditor->setRouterName(id, routerNameField->text().toStdString());
}

void RouterManagementDialog::setPortName()
{
    std::string selectedPortName = renamePortsComboBox->currentText().toStdString();
    for(auto port : ports)
    {
        if(selectedPortName == port.second->getName())
        {
            nodesEditor->setPortName(port.first, portNameField->text().toStdString());
            break;
        }
    }
    repopulatePortNamesList();
    fillPortsComboBox(renamePortsComboBox);
    fillPortsComboBox(removePortsComboBox);
}

void RouterManagementDialog::removePort()
{
    std::string selectedPortName = removePortsComboBox->currentText().toStdString();
    for(auto port : ports)
    {
        if(selectedPortName == port.second->getName())
        {
            nodesEditor->removePort(id, port.first);
            break;
        }
    }
    repopulatePortNamesList();
    fillPortsComboBox(renamePortsComboBox);
    fillPortsComboBox(removePortsComboBox);

}

void RouterManagementDialog::addPort()
{
    nodesEditor->addPort(id, addPortNameField->text().toStdString(), isInputCheckBox->isChecked());
    repopulatePortNamesList();
    fillPortsComboBox(renamePortsComboBox);
    fillPortsComboBox(removePortsComboBox);
}

void RouterManagementDialog::repopulatePortNamesList()
{
        ports.clear();
        ports = router->getPorts();
        std::cout<<ports.size();
}

void RouterManagementDialog::fillPortsComboBox(QComboBox *combo)
{
    combo->clear();
    for(auto port: ports)
    {
        combo->addItem(tr(port.second->getName().c_str()));
        std::cout<<"a";
    }
}
