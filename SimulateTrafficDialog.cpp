
#include "SimulateTrafficDialog.hpp"
#include <iterator>

SimulateTrafficDialog::SimulateTrafficDialog(std::shared_ptr<Router> r, std::shared_ptr<IController> editor, QMainWindow *widget)
: QDialog(widget)
{
    nodesEditor = editor;
    id = r->getId();
    router = r;
    router->attach(this);
    view = widget;
    repopulateFecList();
    this->setAttribute(Qt::WA_DeleteOnClose);
    mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    QHBoxLayout *firstLayout = new QHBoxLayout;
    QHBoxLayout *secondLayout = new QHBoxLayout;
    QHBoxLayout *thirdLayout = new QHBoxLayout;
    setLayout(mainLayout);
    mainLayout->addLayout(firstLayout);
    mainLayout->addLayout(secondLayout);
    mainLayout->addLayout(thirdLayout);

    fecComboBox = new QComboBox;
    fillFecComboBox(fecComboBox);
    firstLayout->addWidget(fecComboBox);

    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    secondLayout->addWidget(startButton);
    secondLayout->addWidget(stopButton);

    prevButton = new QPushButton(tr("Prev"));
    nextButton = new QPushButton(tr("Next"));
    thirdLayout->addWidget(prevButton);
    thirdLayout->addWidget(nextButton);

    connect(startButton, SIGNAL(pressed()),
                         this, SLOT(handleStartButton()));
    connect(stopButton, SIGNAL(pressed()),
                         this, SLOT(handleStopButton()));
    connect(prevButton, SIGNAL(pressed()),
                         this, SLOT(handlePrevButton()));
    connect(nextButton, SIGNAL(pressed()),
                         this, SLOT(handleNextButton()));

    stopButton->setEnabled(false);
    prevButton->setEnabled(false);
    nextButton->setEnabled(false);
    this->setWindowTitle("Trafiic simulation");
}

SimulateTrafficDialog::~SimulateTrafficDialog()
{
    nodesEditor->eraseMplsPacket();
    router->dettach(this);
    unlockControl();
}

void SimulateTrafficDialog::handleStartButton()
{
    if(fecComboBox->count() == 0)
    {
        showMessageBox("No available FEC. Please define new FEC for this router");
    }
    else
    {
        std::cout<<"handleStartButton 1"<<std::endl;
        std::string result = nodesEditor->displayNextMplsPacket(id, fecComboBox->currentText().toStdString());
        std::cout<<"handleStartButton 1"<<std::endl;
        if(!result.empty())
        {
            showMessageBox(result);
        }
        else
        {
            lockControl();
        }
    }
}

void SimulateTrafficDialog::handleStopButton()
{
    nodesEditor->eraseMplsPacket();
    unlockControl();
}

void SimulateTrafficDialog::handlePrevButton()
{
    std::string result = nodesEditor->displayPrevMplsPacket();
    if(!result.empty())
    {
        showMessageBox(result);
    }
}

void SimulateTrafficDialog::handleNextButton()
{
    std::string result = nodesEditor->displayNextMplsPacket();
    if(!result.empty())
    {
        showMessageBox(result);
    }
    else
    {
        lockControl();
    }
}

void SimulateTrafficDialog::lockControl()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    prevButton->setEnabled(true);
    nextButton->setEnabled(true);
    fecComboBox->setEnabled(false);
    view->setDisabled(true);
    setDisabled(false);
}

void SimulateTrafficDialog::unlockControl()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    prevButton->setEnabled(false);
    nextButton->setEnabled(false);
    fecComboBox->setEnabled(true);
    view->setDisabled(false);
}

void SimulateTrafficDialog::repopulateFecList()
{
    fecs.clear();
    std::shared_ptr<std::vector<FtnEntry> > ftnTable = router->getFtnTable();
    for(auto entry : *ftnTable)
    {
        fecs.push_back(entry.fec);
    }
}


void SimulateTrafficDialog::fillFecComboBox(QComboBox *combo)
{
    combo->clear();
    for(auto fec: fecs)
    {
        combo->addItem(tr(fec.c_str()));
    }
}

void SimulateTrafficDialog::update()
{
    repopulateFecList();
    fillFecComboBox(fecComboBox);
}

void SimulateTrafficDialog::showMessageBox(std::string message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText(QString::fromUtf8(message.c_str()));
    msgBox.setStandardButtons(QMessageBox::Ok);
    if(msgBox.exec() == QMessageBox::Ok)
    {
      msgBox.close();
    }
}
