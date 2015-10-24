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

#include "Editor.hpp"
#include "PortView.hpp"
#include "ConnectionView.hpp"
#include "Router.hpp"
#include "MainWindow.hpp"
#include <QEvent>

static const std::string noPreviousPacketsError = "No previous packets to display";


Editor::Editor(MainWindow *mainWindow, QMainWindow *parent)
   : IController(parent), view(mainWindow)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    conn = 0;
}

void Editor::install(QGraphicsScene *s)
{
    s->installEventFilter(this);
    scene = s;
}

QGraphicsItem* Editor::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

void Editor::handleRouterMenu(QGraphicsSceneMouseEvent *me)
{
    RouterView *item = (RouterView*)itemAt(me->scenePos());
    QMenu contextMenu(tr("Router menu"), this);
    QAction *action1=new QAction("Router management", this);
    contextMenu.addAction(action1);
    QAction *action2=new QAction("Label management", this);
    contextMenu.addAction(action2);
    QAction *action3=new QAction("Remove router", this);
    contextMenu.addAction(action3);
    QAction* action4=new QAction("Simulate traffic", this);
    contextMenu.addAction(action4);

    QAction* calledItem=  contextMenu.exec(me->screenPos());
    if(calledItem == action1)
    {
        view->showRouterDialog(site->getRouter(item->getId()));
    }
    else if(calledItem == action2)
    {
        view->showTableDialog(site->getRouter(item->getId()));
    }
    else if(calledItem == action3)
    {
        site->deleteRouter(item->getId());
        delete item;
   }
     else if(calledItem == action4)
    {
        view->showSimulateTrafficDialog(site->getRouter(item->getId()));
    }
}

void Editor::handleLinkMenu(QGraphicsSceneMouseEvent *me)
{
    ConnectionView *item = (ConnectionView*)itemAt(me->scenePos());
    QMenu contextMenu(tr("Link menu"), this);

    QAction *action1 = new QAction("Remove link", this);
    contextMenu.addAction(action1);
    QAction* calledItem = contextMenu.exec(me->screenPos());

    if(calledItem == action1)
    {
         site->deleteConnection(item->getId());
         delete item;
    }
}

bool Editor::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;
	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == PortView::Type)
			{

                conn = new ConnectionView(site->initializeConnection());
                scene->addItem(conn);
                conn->setPort1((PortView*) item);
                ((PortView*) item)->setConnected(true);
				conn->setPos1(item->scenePos());
				conn->setPos2(me->scenePos());
				conn->updatePath();

				return true;
            }
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
            setWindowTitle("Menu");
            setFixedSize(400,200);
            setContextMenuPolicy(Qt::CustomContextMenu);
            if (item && item->type() == ConnectionView::Type)
            {
                handleLinkMenu(me);

            }
            else if(item && item->type() == RouterView::Type)
            {
                handleRouterMenu(me);
            }
            break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
        if (conn)
		{
            conn->setPos2(me->scenePos());
            conn->updatePath();
			return true;
        }
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (conn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
            PortView *port1 = conn->port1();
            PortView *port2 = (PortView*) item;
            if (item && item->type() == PortView::Type)
			{
                if (port1->getBlock() != port2->getBlock() && port1->getIsInput() != port2->getIsInput() && !port2->isConnected())
                {
                    port2->setConnected(true);
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
                    site->addConnection(conn->getId(), port1->getId(),port2->getId());
                    conn = nullptr;
					return true;
                }
			}
            else
            {
                port1->getConnections().clear();
            }
            delete conn;
            conn = nullptr;
			return true;
		}
		break;
	}
	}
    return QObject::eventFilter(o, e);
}

void Editor::addRouter()
{
    RouterId id= site->addRouter();
    PortId inputPortId = site->addPort(id, "in", true);
    PortId outputPortId = site->addPort(id, "out", false);
    site->getRouter(id)->addInputPort(inputPortId, site->getPort(inputPortId));
    site->getRouter(id)->addOutputPort(outputPortId, site->getPort(outputPortId));
    view->addRouter(id);
    view->addPort(id, inputPortId, "in", true);
    view->addPort(id, outputPortId, "out", false);
}

bool Editor::setRouterName(RouterId id, std::string name)
{
    return site->setRouterName(id, name);
}

void Editor::setPortName(PortId id, std::string name)
{
    site->getPort(id)->setName(name);
}

void Editor::removePort(RouterId routerId, PortId portId)
{
    view->deletePort(portId);
    site->deletePort(routerId, portId);
}

void Editor::addPort(RouterId routerId, std::string name, bool isInput)
{
    PortId portId = site->addPort(routerId, name, isInput);
    view->addPort(routerId, portId, name, isInput);
}

void Editor::setModel(std::shared_ptr<Site> pSite)
{
    site = pSite;
}

std::string Editor::addNhlfeEntry(RouterId id, NhlfeEntry entry)
{
    return (site->getRouter(id))->addNhlfeEntry(entry);
}

void Editor::removeNhlfeEntry(RouterId id, NhlfeEntry entry)
{
    (site->getRouter(id))->removeNhlfeEntry(entry);
}

std::string Editor::addIlmEntry(RouterId id, IlmEntry entry)
{
    return (site->getRouter(id))->addIlmEntry(entry);
}

void Editor::removeIlmEntry(RouterId id, IlmEntry entry)
{
    (site->getRouter(id))->removeIlmEntry(entry);
}

std::string Editor::addFtnEntry(RouterId id, FtnEntry entry)
{
    return (site->getRouter(id))->addFtnEntry(entry);
}

void Editor::removeFtnEntry(RouterId id, FtnEntry entry)
{
    (site->getRouter(id))->removeFtnEntry(entry);
}

std::string Editor::displayNextMplsPacket()
{
    try
    {
        lastMplsPackets.push(site->calculateNextHop(labelStack, nextHopPort));
        nextHopPort = std::get<2>(lastMplsPackets.top());
        labelStack = std::get<0>(lastMplsPackets.top());
        view->showMplsPacket(std::get<1>(lastMplsPackets.top()), std::get<2>(lastMplsPackets.top()),
                             std::get<0>(lastMplsPackets.top()));
    }
    catch(std::runtime_error e)
    {
        return std::string(e.what());
    }
    return "";
}

std::string Editor::displayNextMplsPacket(RouterId id, FEC fec)
{
    try
    {
        lastMplsPackets.push(site->calculateNextHop(id, fec));
        nextHopPort = std::get<2>(lastMplsPackets.top());
        labelStack = std::get<0>(lastMplsPackets.top());
        view->showMplsPacket(std::get<1>(lastMplsPackets.top()), std::get<2>(lastMplsPackets.top()),
                             std::get<0>(lastMplsPackets.top()));
    }
    catch(std::runtime_error e)
    {
        return std::string(e.what());
    }
    return "";
}

std::string Editor::displayPrevMplsPacket()
{
    auto tmp = lastMplsPackets.top();
    lastMplsPackets.pop();
    if(lastMplsPackets.size() > 0 )
    {
        view->showMplsPacket(std::get<1>(lastMplsPackets.top()), std::get<2>(lastMplsPackets.top()),
                             std::get<0>(lastMplsPackets.top()));
        labelStack = std::get<0>(lastMplsPackets.top());
        nextHopPort = std::get<2>(lastMplsPackets.top());
    }
    else
    {
        lastMplsPackets.push(tmp);
        return noPreviousPacketsError;
    }
    return "";
}

void Editor::eraseMplsPacket()
{
    view->eraseMplsPacket();
}
