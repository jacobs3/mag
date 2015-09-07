/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */


#include "MainWindow.hpp"
#include "PortView.hpp"

#include "Editor.hpp"

#include <QGraphicsScene>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{


    scene = new QGraphicsScene();




    setWindowTitle(tr("MPLS manager"));



    dock = new QDockWidget(tr("View"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    view = new QGraphicsView(dock);
    view->setScene(scene);

    view->setRenderHint(QPainter::Antialiasing, true);

    dock->setWidget(view);
    addDockWidget(Qt::LeftDockWidgetArea, dock);







    nodesEditor = new Editor(this);
    nodesEditor->install(scene);


    QAction *quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    QAction *loadAct = new QAction(tr("&Load"), this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip(tr("Open a file"));
    connect(loadAct, SIGNAL(triggered()), nodesEditor, SLOT(loadFile()));

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save a file"));
    connect(saveAct, SIGNAL(triggered()), nodesEditor, SLOT(saveFile()));

    QAction *addAct = new QAction(tr("&Add"), this);
    addAct->setStatusTip(tr("Add a block"));
    connect(addAct, SIGNAL(triggered()), nodesEditor, SLOT(addRouter()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(addAct);
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);



  /*  RouterView *b = new RouterView(0);
    scene->addItem(b);
    b->addPort("R1", 0, PortView::NamePort);
  //  b->addPort("test", 0, QNEPort::TypePort);
    b->addInputPort("in");
   // b->addInputPort("in2");
   // b->addInputPort("in3");
    b->addOutputPort("out");
        b->addOutputPort("out2");
   // b->addOutputPort("out2");
    //b->addOutputPort("out3");

    b->setPos(0,0);
    b->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(b, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));
    //b = b->clone();
   // b->addPort("R2", 0, QNEPort::NamePort);
    RouterView *b2 = new RouterView(0);
    scene->addItem(b2);
    b2->addPort("R2", 0, PortView::NamePort);

    b2->addInputPort("in");
    b2->addOutputPort("out");
    b2->setPos(150, 0);

    RouterView *b3 = new RouterView(0);
        scene->addItem(b3);
    b3->addPort("R3", 0, PortView::NamePort);
    b3->addInputPort("in");
        b3->addInputPort("in2");
    b3->addOutputPort("out");
    b3->setPos(150, 150);


    RouterView *b4 = new RouterView(0);
        scene->addItem(b4);
    b4->addPort("R4", 0, PortView::NamePort);
    b4->addInputPort("in");
    b4->addOutputPort("out");
    b4->setPos(-150, -150);

    RouterView *b5 = new RouterView(0);
        scene->addItem(b5);
    b5->addPort("R5", 0, PortView::NamePort);
    b5->addInputPort("in");
    b5->addOutputPort("out");
    b5->setPos(-150, 150);

    RouterView *b6 = new RouterView(0);
        scene->addItem(b6);
    b6->addPort("R6", 0, PortView::NamePort);
    b6->addInputPort("in");
    b6->addOutputPort("out");
    b6->setPos(150, -150);

    RouterView *b7 = new RouterView(0);
        scene->addItem(b7);
    b7->addPort("R7", 0, PortView::NamePort);
    b7->addInputPort("in");
    b7->addOutputPort("out");
    b7->setPos(-150, 0);


   // b = b->clone();
   // b->addPort("R3", 0, QNEPort::NamePort);
  //  b->
   // b->setPos(150, 150);*/
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

MainWindow::~MainWindow()
{

}



void MainWindow::addRouter(RouterId id)
{
    RouterView *v = new RouterView(id);

    scene->addItem(v);
    /*for (int i = 0; i < 4 + rand() % 3; i++)
	{
        v->addPort("RX", rand() % 2, 0, 0);
        v->setPos(view->sceneRect().center().toPoint());
    }*/
    routers[id]=v;
}

void MainWindow::addPort(RouterId routerId, PortId portId, std::string name, bool isInput)
{
    PortView* port = routers[routerId]->addPort(QString::fromUtf8(name.c_str()), isInput);
    port->setId(portId);
    routers[routerId]->setPos(view->sceneRect().center().toPoint());
    ports[portId] = port;
    scene->addItem(port);
}

void MainWindow::deletePort(PortId portId)
{
    delete ports[portId];

}
/*void MainWindow::showContextMenu(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = b->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Menu Item 1");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
    }
    else
    {
        // nothing was chosen
    }
}*/
void MainWindow::showContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1("Remove Data Point", this);
   connect(&action1, SIGNAL(triggered()), this, SLOT(quit()));
   contextMenu.addAction(&action1);

   contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::showRouterMenu(RouterId id)
{
    RouterManagementDialog *window = new RouterManagementDialog(nodesEditor->getSite().getRouter(id),nodesEditor, this);
    window->show();

   /* std::shared_ptr<Router> r= ;
    std::cout<<"1";
    QWidget *wdg = new QWidget;
   // QHBoxLayout* layout = new QHBoxLayout;
    QMainWindow *routerMenuWindow = new QMainWindow;
    QHBoxLayout *layout = new QHBoxLayout( wdg );
     std::cout<<"2";
    QLabel * routerNameLabel = new QLabel(tr("router name"));
   QLineEdit *lineEdit = new QLineEdit (tr(r->getName().c_str(), wdg ));
   QPushButton *nameSubmitButton = new QPushButton(tr("Set name"),wdg);
   layout->addWidget(routerNameLabel);
   layout->addWidget(lineEdit);
   layout->addWidget(nameSubmitButton);

    routerMenuWindow->setCentralWidget(wdg);
        routerMenuWindow->show();
   // wdg->ad
   // dock->setWidget(abc) ;
    //layout->addWidget(wdg);
   // wdg->show();
  //  wdg->ad
   // this->
    QLineEdit *routerNameField = new QLineEdit("My first Text Edit box!");
    QComboBox *comboBox = new QComboBox;
        comboBox->addItem(tr("item 1"));
        comboBox->addItem(tr("item 2"));
        comboBox->addItem(tr("item 3"));*/


}
