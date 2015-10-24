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

#pragma once

#include "UiMainWindow.hpp"
#include "RouterView.hpp"

#include <QMainWindow>
#include <QtWidgets>
#include <list>
#include <vector>
#include <memory>
#include "Typedefs.hpp"
#include "IController.hpp"
#include "Router.hpp"
#include "Port.hpp"
#include "IObserver.hpp"
#include "Site.hpp"
#include "MplsPacketView.hpp"

class MainWindow : public QMainWindow, IObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(){}
    void showRouterDialog(std::shared_ptr<Router> router);
    void showTableDialog(std::shared_ptr<Router> router);
    void showSimulateTrafficDialog(std::shared_ptr<Router> router);
    void addRouter(RouterId routerId);
    void addPort(RouterId routerId, PortId portId, std::string name, bool isInput);
    void deletePort(PortId portId);
    void setController(std::shared_ptr<IController> c);
    void start();
    void setModel(std::shared_ptr<Site> site);
    void update();
    void showMplsPacket(PortId startPortId, PortId endPortId, std::vector<Label> outLabels);
    void eraseMplsPacket();

private slots:
    void showContextMenu(const QPoint& pos);

private:
    void refreshPortName(PortId portId, std::string name);
    std::shared_ptr<IController> nodesEditor;
    std::shared_ptr<Site> site;
    QMenu *fileMenu;
    QGraphicsView *view;
    QGraphicsScene *scene;
    std::map<RouterId, RouterView*> routerViews;
    std::map<PortId, PortView*> portViews;
    QDockWidget *dock;
    MplsPacketView* mplsPacketItem;

};
