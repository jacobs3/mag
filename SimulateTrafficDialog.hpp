#pragma once

#include "UiMainWindow.hpp"
#include "RouterView.hpp"
#include "IController.hpp"
#include "IObserver.hpp"

#include <QMainWindow>
#include <QtWidgets>
#include "Typedefs.hpp"
#include "Router.hpp"
#include <vector>
#include <memory>


class SimulateTrafficDialog : public QDialog, IObserver
{
    Q_OBJECT

public:
    explicit SimulateTrafficDialog(std::shared_ptr<Router>, std::shared_ptr<IController>, QMainWindow*);
    ~SimulateTrafficDialog();

private slots:
    void handleStartButton();
    void handleStopButton();
    void handlePrevButton();
    void handleNextButton();
    void update();

private:
    void repopulateFecList();
    void fillFecComboBox(QComboBox *combo);
    void lockControl();
    void unlockControl();
    void showMessageBox(std::string message);
    QPushButton* startButton;
    QPushButton* stopButton;
    QPushButton* prevButton;
    QPushButton* nextButton;
    QVBoxLayout *mainLayout;
    std::shared_ptr<IController> nodesEditor;
    QComboBox *fecComboBox;
    QMainWindow *view;
    std::shared_ptr<Router> router;
    RouterId id;
    std::vector<FEC> fecs;

};
