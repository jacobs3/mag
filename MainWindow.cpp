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
#include "TableDialog.hpp"
#include "RouterManagementDialog.hpp"
#include "SimulateTrafficDialog.hpp"

#include <QGraphicsScene>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mplsPacketItem(nullptr)
{
    scene = new QGraphicsScene();
    setWindowTitle(tr("MPLS manager"));
    this->setAttribute(Qt::WA_AlwaysShowToolTips);
}

void MainWindow::setController(std::shared_ptr<IController> c)
{
    nodesEditor = c;
}

void MainWindow::start()
{

    dock = new QDockWidget(tr("View"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    view = new QGraphicsView(dock);
    view->setScene(scene);

    view->setRenderHint(QPainter::Antialiasing, true);

    dock->setWidget(view);
    addDockWidget(Qt::LeftDockWidgetArea, dock);


    nodesEditor->install(scene);


    QAction *addAct = new QAction(tr("&Add"), this);
    addAct->setStatusTip(tr("Add a router"));
    IController* tmp = nodesEditor.get();

    connect(addAct, SIGNAL(triggered()), tmp, SLOT(addRouter()));

    QAction *quitAct = new QAction(tr("&Exit"), this);
    quitAct->setStatusTip(tr("Quit application"));
    connect(quitAct, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(addAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
}

MainWindow::~MainWindow()
{

}



void MainWindow::addRouter(RouterId routerId)
{
    RouterView *v = new RouterView(routerId);
    scene->addItem(v);
    routerViews[routerId]=v;
    site->getRouter(routerId)->attach(this);
}

void MainWindow::addPort(RouterId routerId, PortId portId, std::string name, bool isInput)
{
    std::cout<<"addport"<<std::endl;
    PortView* portView = routerViews[routerId]->addPort(QString::fromUtf8(name.c_str()), isInput);
    portView->setId(portId);
    portViews[portId] = portView;
    scene->addItem(portView);
    site->getPort(portId)->attach(this);
}

void MainWindow::deletePort(PortId portId)
{
    delete portViews[portId];
    portViews.erase(portId);
}

void MainWindow::refreshPortName(PortId portId, std::string name)
{
    portViews[portId]->setName(QString::fromUtf8(name.c_str()));
}

void MainWindow::showContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1("Remove Data Point", this);
   connect(&action1, SIGNAL(triggered()), this, SLOT(quit()));
   contextMenu.addAction(&action1);

   contextMenu.exec(mapToGlobal(pos));
}


void MainWindow::showRouterDialog(std::shared_ptr<Router> router)
{
    RouterManagementDialog *window = new RouterManagementDialog(router,nodesEditor, this);
    window->show();
}

void MainWindow::showTableDialog(std::shared_ptr<Router> router)
{
    TableDialog *window = new TableDialog(router,nodesEditor, this);
    window->show();
}

void MainWindow::showSimulateTrafficDialog(std::shared_ptr<Router> router)
{
    SimulateTrafficDialog *window = new SimulateTrafficDialog(router,nodesEditor, this);
    window->show();
}

void MainWindow::update()
{
    for(auto p : portViews)
    {
        refreshPortName(p.first,site->getPort(p.first)->getName());
    };
}

void MainWindow::setModel(std::shared_ptr<Site> pSite)
{
    site = pSite;
}

void MainWindow::showMplsPacket(PortId startPortId, PortId endPortId, std::vector<Label> outLabels)
{
   if(mplsPacketItem)
   {
       delete mplsPacketItem;
       mplsPacketItem = nullptr;
   }

   mplsPacketItem = new MplsPacketView(portViews[startPortId]->scenePos(), portViews[endPortId]->scenePos(), outLabels);
   scene->addItem(mplsPacketItem);
}

void MainWindow::eraseMplsPacket()
{
    if(mplsPacketItem)
    {
        delete mplsPacketItem;
        mplsPacketItem = nullptr;
    }
}
