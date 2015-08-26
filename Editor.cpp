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
#include "Connection.hpp"
#include "Router.hpp"

#include <QGraphicsScene>
#include <QEvent>

//#include <qsignalmapper.h>
//#include <functional>

Editor::Editor(MainWindow *mainWindow, QMainWindow *parent)
   : QMainWindow(parent), view(mainWindow)
{
    std::cout<<"aaa"<<std::endl;
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
   // connect(action1, SIGNAL(triggered()), this, SLOT(cout1()));
    contextMenu.addAction(action1);
    QAction *action2=new QAction("Label management", this);
    contextMenu.addAction(action2);
    QAction *action3=new QAction("Remove router", this);
   contextMenu.addAction(action3);
    QAction* action4=new QAction("Simulate traffic", this);
    contextMenu.addAction(action4);

   QAction* calledItem=  contextMenu.exec(mapToGlobal(me->scenePos().toPoint()));
item->getId();
   if(calledItem == action1)
   {
       cout1();
   }
   else if(calledItem == action2)
   {
       cout2();
   }
   else if(calledItem == action3)
   {
       delete item;
   }
   else if(calledItem == action4)
   {
       cout4();
   }
}

void Editor::handleLinkMenu(QGraphicsSceneMouseEvent *me)
{
    ConnectionView *item = (ConnectionView*)itemAt(me->scenePos());
    QMenu contextMenu(tr("Link menu"), this);

    QAction *action1 = new QAction("Remove link", this);
    contextMenu.addAction(action1);
    QAction *action2 = new QAction("Define link parameters", this);
    contextMenu.addAction(action2);
    QAction *action3 = new QAction("Trigger link outage", this);
    contextMenu.addAction(action3);

    QAction* calledItem = contextMenu.exec(mapToGlobal(me->scenePos().toPoint()));


    if(calledItem == action1)
    {
       // item->onDelete();
         delete item;
    }
    else if(calledItem == action2)
    {
        cout2();
    }
    else if(calledItem == action3)
    {
       cout3();
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
                conn = new ConnectionView(0);
                scene->addItem(conn);
                conn->setPort1((PortView*) item);
				conn->setPos1(item->scenePos());
				conn->setPos2(me->scenePos());
				conn->updatePath();

				return true;
            } else if (item && item->type() == RouterView::Type)
			{
               //  if (selBlock)
                 //   selBlock->setSelected();
				// selBlock = (QNEBlock*) item;
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
                //delete item;




               /*   m_1 = m_pContextMenu->addAction("Set Size 100x100");
                  m_2 = m_pContextMenu->addAction("Set Size 200x200");



= m_pContextMenu->addAction("Set Size 300x300");
                  m_4 = m_pContextMenu->addAction("Set Size 400x400");
                     //m_pContextMenu->exec(mapToGlobal(pos));

                  connect(this,	       SIGNAL(customContextMenuRequested(const QPoint)),this,
                      SLOT(contextMenuRequested(QPoint)));
                  connect(m_1,SIGNAL(triggered()),this,SLOT(cout1()));
                  connect(m_2,SIGNAL(triggered()),this,SLOT(cout2()));
                  connect(m_3,SIGNAL(triggered()),this,SLOT(cout3()));
                  connect(m_4,SIGNAL(triggered()),this,SLOT(cout4()));*/


//std::cout<<"ABCx "<<abc.x()<<std::endl;
//std::cout<<"ABCy "<<abc.y()<<std::endl;
//QPointF par;
//par = item->boundingRect().center();
//QPointF par = mapToParent(abc.toPoint());
//std::cout<<"parx "<<par.x()<<std::endl;
//std::cout<<"pary "<<par.y()<<std::endl;
/*QPointF par2 = mapFromParent(abc.toPoint());
std::cout<<"par2x "<<par2.x()<<std::endl;
std::cout<<"par2y "<<par2.y()<<std::endl;
QPointF g = mapFromGlobal(abc.toPoint());
std::cout<<"gx "<<g.x()<<std::endl;
std::cout<<"gy "<<g.y()<<std::endl;*/
//std::cout<<"x "<<abc.toPoint().x()<<std::endl;
//std::cout<<"y "<<abc.toPoint().y()<<std::endl;




			// if (selBlock == (QNEBlock*) item)
				// selBlock = 0;
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
            if (item && item->type() == PortView::Type)
			{
                PortView *port1 = conn->port1();
                PortView *port2 = (PortView*) item;

				if (port1->block() != port2->block() && port1->isOutput() != port2->isOutput() && !port1->isConnected(port2))
				{
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = 0;
					return true;
				}
			}

			delete conn;
			conn = 0;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}

void Editor::addRouter()
{
    //fixme routerId
    RouterId id= site.addRouter();
    view->addRouter(id);

}

void Editor::saveFile()
{
    QString fname = QFileDialog::getSaveFileName();
    if (fname.isEmpty())
        return;

    QFile f(fname);
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    save(ds);
}

void Editor::loadFile()
{
    QString fname = QFileDialog::getOpenFileName();
    if (fname.isEmpty())
        return;

    QFile f(fname);
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    load(ds);
}

void Editor::save(QDataStream &ds)
{
	foreach(QGraphicsItem *item, scene->items())
        if (item->type() == RouterView::Type)
		{
			ds << item->type();
            ((RouterView*) item)->save(ds);
		}

	foreach(QGraphicsItem *item, scene->items())
        if (item->type() == ConnectionView::Type)
		{
			ds << item->type();
            ((ConnectionView*) item)->save(ds);
		}
}

void Editor::load(QDataStream &ds)
{
	scene->clear();

    QMap<quint64, PortView*> portMap;

	while (!ds.atEnd())
	{
		int type;
		ds >> type;
        if (type == RouterView::Type)
		{
            RouterView *block = new RouterView(0);
            scene->addItem(block);
			block->load(ds, portMap);
        } else if (type == ConnectionView::Type)
		{
            ConnectionView *conn = new ConnectionView(0);
            scene->addItem(conn);
			conn->load(ds, portMap);
		}
	}
}
/*
void Editor::contextMenuRequested(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1("Remove Data Point", this);
   connect(&action1, SIGNAL(triggered()), this, SLOT(cout1()));
   contextMenu.addAction(&action1);

   contextMenu.exec(mapToGlobal(pos));
}

/*void Editor::contextMenuRequested(const QPoint &pos)
{

   contextMenu.exec(mapToGlobal(pos));
}*/

void Editor::deleteItem(QWidget *item)
{

    delete item;
}
