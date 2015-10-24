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

#include <QObject>
#include <QMenu>
#include <QAction>
#include <iostream>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>
#include <memory>
#include <tuple>
#include <vector>
#include <stack>

#include "Typedefs.hpp"
#include "IController.hpp"
class RouterView;
class MainWindow;
class ConnectionView;



class Editor : public IController
{
	Q_OBJECT
public:
    explicit Editor(MainWindow *mainWindow, QMainWindow *parent = 0);

    void install(QGraphicsScene *scene);
    void handleRouterMenu(QGraphicsSceneMouseEvent *me);
    void handleLinkMenu(QGraphicsSceneMouseEvent *me);

    bool setRouterName(RouterId id, std::string name);
    void setPortName(PortId id, std::string name);
    void removePort(RouterId routerId, PortId portId);
    void addPort(RouterId routerId, std::string name, bool isInput);
    void setModel(std::shared_ptr<Site> site);

    std::string addNhlfeEntry(RouterId id, NhlfeEntry entry);
    void removeNhlfeEntry(RouterId id, NhlfeEntry entry);
    std::string addIlmEntry(RouterId id, IlmEntry entry);
    void removeIlmEntry(RouterId id, IlmEntry entry);
    std::string addFtnEntry(RouterId id, FtnEntry entry);
    void removeFtnEntry(RouterId id, FtnEntry entry);
    std::string displayNextMplsPacket();
    std::string displayNextMplsPacket(RouterId id, FEC fec);
    std::string displayPrevMplsPacket();
    void eraseMplsPacket();

public slots:
    void addRouter();

private:
    std::shared_ptr<Site> site;
    bool eventFilter(QObject *, QEvent *);
    QGraphicsItem *itemAt(const QPointF&);
    MainWindow *view;
    QGraphicsScene *scene;
    ConnectionView *conn;
    std::stack<std::tuple<std::vector<Label>, PortId, PortId> > lastMplsPackets;
    PortId nextHopPort;
    std::vector<Label> labelStack;

private slots:
void deleteItem(QWidget *item);
};

