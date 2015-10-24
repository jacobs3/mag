
#pragma once

#include "UiMainWindow.hpp"
#include "RouterView.hpp"
#include "IController.hpp"
#include "IObserver.hpp"

#include <QMainWindow>
#include <QtWidgets>
#include "Typedefs.hpp"
#include "Router.hpp"
#include <list>
#include <memory>
#include <map>

#define NHLFE_TABLE_ID "NHL"
#define ILM_TABLE_ID "ILM"
#define FTN_TABLE_ID "FTN"
#define CORRECT_EMPTY_FIELDS_ERROR "Please correct empty fields"
#define LABELS_DEFINED_WHEN_POP_ERROR "Please remove labels when pop operation chosen"
#define PROVIDE_LABELS_ERROR "Please provide labels for push or swap operation"

class TableDialog;
typedef void (TableDialog::*AddRowFunction)(void);

class TableDialog : public QDialog, IObserver
{
    Q_OBJECT

public:
    explicit TableDialog(std::shared_ptr<Router>, std::shared_ptr<IController>, QWidget*);
    ~TableDialog();

private slots:
    void removeRow();
    void addRow();

private:
    QTableWidget* createFtnTable();
    QTableWidget* createIlmTable();
    QTableWidget* createNhlfeTable();
    void addRemoveButton(QTableWidget *table, std::string objectName, int row, int column);
    void createAddRow(QTableWidget *table, int row, int columns, std::string objectName);
    std::string serializeLabels(std::vector<Label> labels);
    void update();
    void showMessageBox(std::string message);
    void setTableSizePolicies(QTableWidget *table);
    void fillPortsComboBox(QComboBox *combo, std::map<PortId, std::shared_ptr<Port> > ports);
    void subscribeForUpdate();
    void subscribeForPortsUpdate();
    void unsubsribeForUpdate();
    void addRowInNhlfeTable();
    void addRowInIlmTable();
    void addRowInFtnTable();
    void repopulateInputPortsList();
    void repopulateOutputPortsList();
    static std::map<LabelOperation, std::string> labelOperationToString;
    static std::map<std::string, LabelOperation> labelOperationFromString;
    static std::map<std::string, AddRowFunction> addRowFunctions;
    std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable;
    std::shared_ptr<std::vector<FtnEntry> > ftnTable;
    std::shared_ptr<std::vector<IlmEntry> > ilmTable;
    QTableWidget* nhlfeTab, *ilmTab, *ftnTab;
    QTabWidget *tabs;
    QVBoxLayout *layout;
 //   void subscribeForUpdate();
 //   void repopulatePortNamesList();
 //   void fillPortsComboBox(QComboBox *combo);
    QLineEdit *routerNameField;
    QLineEdit *portNameField;
    QLineEdit *addPortNameField;
    QCheckBox *isInputCheckBox;
    QVBoxLayout *mainLayout;
    std::shared_ptr<IController> nodesEditor;
    QComboBox *renamePortsComboBox;
    QComboBox *removePortsComboBox;
    QGraphicsView *view;
    std::shared_ptr<Router> router;
    RouterId id;
    std::map<PortId, std::shared_ptr<Port> > inputPorts;
    std::map<PortId, std::shared_ptr<Port> > outputPorts;

};
