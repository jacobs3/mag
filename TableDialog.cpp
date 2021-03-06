
#include "TableDialog.hpp"
#include <iterator>
#include <string>

const std::string nhlfeTableId = "NHL";
const std::string ilmTableId = "ILM";
const std::string ftnTableId = "FTN";
const std::string correctEmptyFieldsError = "Please correct empty fields";
const std::string labelsDefWhenPopOpError = "Please remove labels when pop operation chosen";
const std::string provideLabelsError = "Please provide labels for push or swap operation";
const std::string multipleLabelsTooltip = "use / to divide multiple labels. Put top label on the right";

std::map<LabelOperation, std::string> TableDialog::labelOperationToString = {
    {LabelOperation::pop, "Pop"},
    {LabelOperation::push, "Push"},
    {LabelOperation::swap, "Swap"}
};

std::map<std::string, LabelOperation> TableDialog::labelOperationFromString = {
    {"Pop", LabelOperation::pop},
    {"Push", LabelOperation::push},
    {"Swap", LabelOperation::swap}
};

std::map<std::string, AddRowFunction> TableDialog::addRowFunctions = {
    {nhlfeTableId, &TableDialog::addRowInNhlfeTable},
    {ilmTableId, &TableDialog::addRowInIlmTable},
    {ftnTableId, &TableDialog::addRowInFtnTable}
};

TableDialog::TableDialog(std::shared_ptr<Router> r, std::shared_ptr<IController> editor, QWidget *widget)
: QDialog(widget)
{
    nodesEditor = editor;
    id = r->getId();
    router = r;
    tabs = nullptr;
    subscribeForUpdate();
    update();
    this->setWindowTitle("Label management [" + QString::fromUtf8(router->getName().c_str()) + "]");
    this->setAttribute(Qt::WA_AlwaysShowToolTips);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

TableDialog::~TableDialog()
{
    unsubsribeForUpdate();
}

std::string TableDialog::serializeLabels(std::vector<Label> labels)
{
    if(labels.size()==0)
        return "";
    std::string res = std::to_string(labels[0]);
    for(auto it = labels.begin() + 1; it != labels.end(); it++)
    {
        res+="/"+std::to_string(*it);
    }
    if(res=="/")
        return "";
    else return res;
}

void TableDialog::update()
{
    QWidget *centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);
    centralWidget->setAttribute(Qt::WA_AlwaysShowToolTips);
    int currentIndex = 0;
    if(tabs!=nullptr)
    {
        currentIndex = tabs->currentIndex();
    }
    tabs = new QTabWidget(centralWidget);
    tabs->setAttribute(Qt::WA_AlwaysShowToolTips);
    layout->addWidget(tabs);
    setFixedSize(550, 400);
    tabs->setFixedSize(550, 400);
    tabs->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    nhlfeTable = router->getNhlfeTable();
    ftnTable = router->getFtnTable();
    ilmTable = router->getIlmTable();
    repopulateInputPortsList();
    repopulateOutputPortsList();
    subscribeForPortsUpdate();
    tabs->addTab(createFtnTable(),"FTN");
    tabs->addTab(createIlmTable(),"ILM");
    tabs->addTab(createNhlfeTable(),"NHLFE");
    setTableSizePolicies(ftnTab);
    setTableSizePolicies(ilmTab);
    setTableSizePolicies(nhlfeTab);
    tabs->setCurrentIndex(currentIndex);
    centralWidget->show();
}

QTableWidget* TableDialog::createFtnTable()
{
    int row=0;
    ftnTab = new QTableWidget((int)ftnTable->size()+1,3 );

    for(FtnEntry ftnElement : *ftnTable)
    {
        QTableWidgetItem *tableItem = new QTableWidgetItem;
        tableItem->setText(QString::fromUtf8(ftnElement.fec.c_str()));
        ftnTab->setItem(row, 0, tableItem);
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *tableItem2 = new QTableWidgetItem;
        tableItem2->setText(QString::number(ftnElement.token));
        ftnTab->setItem(row, 1, tableItem2);
        tableItem2->setFlags(tableItem2->flags() & ~Qt::ItemIsEditable);

        addRemoveButton(ftnTab, ftnTableId, row, 2);
        row++;
    }
    ftnTab->setColumnWidth(0,200);
    createAddRow(ftnTab, (int)ftnTable->size(), 2, ftnTableId);
    ftnTab->setHorizontalHeaderLabels(QList<QString>() << QString("FEC")<<QString("Token")<<QString("Action"));
    return ftnTab;
}

QTableWidget* TableDialog::createIlmTable()
{
    int row=0;
    ilmTab = new QTableWidget((int)ilmTable->size()+1,4 );
    ilmTab->setAttribute(Qt::WA_AlwaysShowToolTips);
    for(IlmEntry ilmElement : *ilmTable)
    {
        QTableWidgetItem *tableItem = new QTableWidgetItem;
        tableItem->setText(QString::fromUtf8(serializeLabels(ilmElement.inLabels).c_str()));
        ilmTab->setItem(row, 0, tableItem);
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *tableItem2 = new QTableWidgetItem;
        tableItem2->setText(QString::fromUtf8((inputPorts[ilmElement.inPort])->getName().c_str()));
        ilmTab->setItem(row, 1, tableItem2);
        tableItem2->setFlags(tableItem2->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *tableItem3 = new QTableWidgetItem;
        tableItem3->setText(QString::number(ilmElement.token));
        ilmTab->setItem(row, 2, tableItem3);
        tableItem3->setFlags(tableItem3->flags() & ~Qt::ItemIsEditable);

        addRemoveButton(ilmTab, ilmTableId, row, 3);
        row++;
    }

    createAddRow(ilmTab, (int)ilmTable->size(), 3, ilmTableId);
    ilmTab->setHorizontalHeaderLabels(QList<QString>() << QString("In label")<<QString("In port")<<QString("Token")<<QString("Action"));
    return ilmTab;
}

QTableWidget* TableDialog::createNhlfeTable()
{
    int row=0;
    nhlfeTab = new QTableWidget((int)nhlfeTable->size()+1,5 );

    nhlfeTab->setAttribute(Qt::WA_AlwaysShowToolTips);
    for(NhlfeEntry nhlfeElement : *nhlfeTable)
    {
        QTableWidgetItem *tableItem = new QTableWidgetItem;
        tableItem->setText(QString::number(nhlfeElement.token));
        nhlfeTab->setItem(row, 0, tableItem);
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *tableItem2 = new QTableWidgetItem;

        tableItem2->setText(QString::fromUtf8(serializeLabels(nhlfeElement.outLabel).c_str()));
        nhlfeTab->setItem(row, 1, tableItem2);
        tableItem2->setFlags(tableItem2->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *tableItem3 = new QTableWidgetItem;
        tableItem3->setText(QString::fromUtf8(labelOperationToString[nhlfeElement.operation].c_str()));
        nhlfeTab->setItem(row, 2, tableItem3);
        tableItem3->setFlags(tableItem3->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *tableItem4 = new QTableWidgetItem;
        tableItem4->setText(QString::fromUtf8((outputPorts[nhlfeElement.outPort])->getName().c_str()));
        nhlfeTab->setItem(row, 3, tableItem4);
        tableItem4->setFlags(tableItem4->flags() & ~Qt::ItemIsEditable);

        addRemoveButton(nhlfeTab, nhlfeTableId, row, 4);
        row++;
    }

    createAddRow(nhlfeTab, (int)nhlfeTable->size(), 4, nhlfeTableId);
    nhlfeTab->setHorizontalHeaderLabels(QList<QString>() << QString("Token")<<QString("Out label")<<QString("Operation")<<QString("Out port")<<QString("Action"));
    return nhlfeTab;
}

void TableDialog::addRemoveButton(QTableWidget *table, std::string objectName, int row, int column)
{
    QPushButton* btnRemove = new QPushButton();
    btnRemove->setText("Remove");
    btnRemove->setObjectName(QString::fromUtf8(objectName.c_str()) + " " + QString::number(row));
    table->setCellWidget(row,column,(QWidget*)btnRemove);
    connect(btnRemove, SIGNAL(pressed()), this, SLOT(removeRow()));
}

void TableDialog::createAddRow(QTableWidget *table, int row, int columns, std::string objectName)
{
    for(int i = 0; i<columns;i++)
    {
        table->setItem(row, i, new QTableWidgetItem);
    }

    QPushButton* btnAdd = new QPushButton();
    btnAdd->setText("Add");
    btnAdd->setObjectName(QString::fromUtf8(objectName.c_str()) + " " + QString::number(row));
    table->setCellWidget(row,columns,(QWidget*)btnAdd);
    connect(btnAdd, SIGNAL(pressed()), this, SLOT(addRow()));
    if(objectName == nhlfeTableId)
    {
        table->item(row,1)->setToolTip(QString::fromUtf8(multipleLabelsTooltip.c_str()));
        QComboBox* combo = new QComboBox();
        for(auto item : labelOperationFromString)
        {
            combo->addItem(QString::fromUtf8(item.first.c_str()));
        }
        table->setCellWidget(row,2,combo);

        QComboBox* portsComboBox = new QComboBox;
        fillPortsComboBox(portsComboBox, outputPorts);
        table->setCellWidget(row,3,portsComboBox);
    }
    else if(objectName == ilmTableId)
    {
        table->item(row,0)->setToolTip(QString::fromUtf8(multipleLabelsTooltip.c_str()));
        QComboBox* portsComboBox = new QComboBox;
        fillPortsComboBox(portsComboBox, inputPorts);
        table->setCellWidget(row,1,portsComboBox);
    }
}

void TableDialog::removeRow()
{
    std::string objectName = ((QPushButton*)sender())->objectName().toStdString();
    std::string tableName = objectName.substr(0,3);
    std::string rowFromName = objectName.substr(4,4);
    int row = std::stoi(rowFromName);
    if(tableName==ilmTableId)
    {
        nodesEditor->removeIlmEntry(id, ilmTable->at(row));
    }
    else if(tableName==ftnTableId)
    {
        nodesEditor->removeFtnEntry(id, ftnTable->at(row));
    }
    else if(tableName==nhlfeTableId)
    {
        nodesEditor->removeNhlfeEntry(id, nhlfeTable->at(row));
    }
}

void TableDialog::addRow()
{

    std::string objectName = ((QPushButton*)sender())->objectName().toStdString();
    std::string tableName = objectName.substr(0,3);
    try
    {
        (this->*(addRowFunctions[tableName]))();
    }
    catch(std::invalid_argument)
    {
        showMessageBox("Please provide numeric value");
    }
}

void TableDialog::addRowInNhlfeTable()
{
    int row = (int)nhlfeTable->size();
    std::string item = nhlfeTab->item(row, 0)->text().toStdString();
    if(item.empty())
    {
        showMessageBox(correctEmptyFieldsError);
    }
    else
    {
        Token token = std::stoi(item);
        LabelOperation operation = labelOperationFromString[qobject_cast<QComboBox*>(nhlfeTab->cellWidget(row, 2))->currentText().toStdString()];
        std::string labelsString = nhlfeTab->item(row, 1)->text().toStdString();
        if(operation == LabelOperation::pop&&!labelsString.empty())
        {
            showMessageBox(labelsDefWhenPopOpError);
            return;
        }
        if((operation == LabelOperation::swap || operation == LabelOperation::push) && labelsString.empty())
        {
            showMessageBox(provideLabelsError);
            return;
        }
        std::vector<Label> labels = labelsStackFromString(labelsString);
        int selectedPortIndex = qobject_cast<QComboBox*>(nhlfeTab->cellWidget(row, 3))->currentIndex();
        auto it = outputPorts.begin();
        std::advance(it, selectedPortIndex);
        PortId outPort = it->first;
        std::string error = nodesEditor->addNhlfeEntry(id, NhlfeEntry(outPort, token, operation, labels));
        if(!error.empty())
        {
            showMessageBox(error);
        }
    }
}

void TableDialog::addRowInIlmTable()
{
    int row = (int)ilmTable->size();
    std::string labelsString = ilmTab->item(row, 0)->text().toStdString();
    int selectedPortIndex = qobject_cast<QComboBox*>(ilmTab->cellWidget(row, 1))->currentIndex();
    auto it = inputPorts.begin();
    std::advance(it, selectedPortIndex);
    PortId inPort = it->first;
    std::string item3 = ilmTab->item(row, 2)->text().toStdString();
    if(labelsString.empty()||item3.empty())
    {
        showMessageBox(correctEmptyFieldsError);
    }
    else
    {
        Token token = std::stoi(item3);
        std::vector<Label> labels = labelsStackFromString(labelsString);
        std::string error = nodesEditor->addIlmEntry(id, IlmEntry(labels, inPort, token));
        if(!error.empty())
        {
            showMessageBox(error);
        }
    }
}

void TableDialog::addRowInFtnTable()
{
    int row = (int)ftnTable->size();
    std::string fec = ftnTab->item(row, 0)->text().toStdString();
    std::string item2 = ftnTab->item(row, 1)->text().toStdString();
    if(fec.empty()||item2.empty())
    {
        showMessageBox(correctEmptyFieldsError);
    }
    else
    {
        Token token = std::stoi(item2);
        std::string error = nodesEditor->addFtnEntry(id, FtnEntry(fec, token));
        if(!error.empty())
        {
            showMessageBox(error);
        }
    }
}

void TableDialog::showMessageBox(std::string message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(QString::fromUtf8(message.c_str()));
    msgBox.setStandardButtons(QMessageBox::Ok);
    if(msgBox.exec() == QMessageBox::Ok)
    {
      msgBox.close();
    }
}

void TableDialog::setTableSizePolicies(QTableWidget *table)
{
    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    table->horizontalHeader()->setStretchLastSection(true);
}

void TableDialog::fillPortsComboBox(QComboBox *combo, std::map<PortId, std::shared_ptr<Port> > ports)
{
    combo->clear();
    for(auto port: ports)
    {
        combo->addItem(tr(port.second->getName().c_str()));
    }
}

void TableDialog::subscribeForPortsUpdate()
{
    for(auto p : router->getPorts())
    {
        p.second->dettach(this);
        p.second->attach(this);
    }
}

void TableDialog::subscribeForUpdate()
{
    router->attach(this);
    for(auto p : router->getPorts())
    {
        p.second->attach(this);
    }
}

void TableDialog::unsubsribeForUpdate()
{
    router->dettach(this);
    for(auto p : router->getPorts())
    {
        p.second->dettach(this);
    }
}

void TableDialog::repopulateInputPortsList()
{
    inputPorts.clear();
    inputPorts = router->getInputPorts();
}

void TableDialog::repopulateOutputPortsList()
{
    outputPorts.clear();
    outputPorts = router->getOutputPorts();
}

std::vector<Label> TableDialog::labelsStackFromString(std::string labelsString)
{
    char* labelsChar = const_cast<char*>(labelsString.c_str());
    std::vector<Label> labels;
    const char * pch = strtok(labelsChar,"/");
    while (pch != NULL)
    {
        labels.push_back(std::stoi(pch));
        pch = strtok(NULL, "/");
    }
    return labels;
}
