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

#include "PortView.hpp"
#include "ConnectionView.hpp"

#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPen>
#include <iostream>

PortView::PortView(PortId portId, QGraphicsItem *parent):
    id(portId), QGraphicsPathItem(parent)
{
	label = new QGraphicsTextItem(this);

	radius_ = 5;
	margin = 2;

	QPainterPath p;
	p.addEllipse(-radius_, -radius_, 2*radius_, 2*radius_);
	setPath(p);

	setPen(QPen(Qt::darkRed));
	setBrush(Qt::red);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;
    _isConnected = false;
}

PortView::~PortView()
{
    foreach(ConnectionView *conn, m_connections)
        if(conn)
        {
            delete conn;
        }

}

void PortView::setNEBlock(RouterView* b)
{
	m_block = b;
}

void PortView::setName(const QString &n)
{
	name = n;
	label->setPlainText(n);
    setLabelPosition();
}

void PortView::setIsInput(bool isIn)
{
    isInput = isIn;

	QFontMetrics fm(scene()->font());
	QRect r = fm.boundingRect(name);
    setLabelPosition();
}

void PortView::setLabelPosition()
{
    if (isInput)
        label->setPos(-radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
    else
        label->setPos(radius_ + margin, -label->boundingRect().height()/2);
}

int PortView::radius()
{
	return radius_;
}

bool PortView::getIsInput()
{
    return isInput;
}

QVector<ConnectionView*>& PortView::connections()
{
	return m_connections;
}

void PortView::setPortFlags(int f)
{
	m_portFlags = f;

	if (m_portFlags & TypePort)
	{
		QFont font(scene()->font());
		font.setItalic(true);
		label->setFont(font);
		setPath(QPainterPath());
	} else if (m_portFlags & NamePort)
	{
		QFont font(scene()->font());
		font.setBold(true);
		label->setFont(font);
		setPath(QPainterPath());
	}
}

RouterView* PortView::block() const
{
	return m_block;
}

void PortView::setConnected(bool pisConnected)
{
    _isConnected = pisConnected;
}

bool PortView::isConnected()
{
    return _isConnected;
}

QVariant PortView::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
        foreach(ConnectionView *conn, m_connections)
		{
            std::cout<<"update port 1"<<std::endl;
			conn->updatePosFromPorts();
             std::cout<<"update port 2"<<std::endl;
			conn->updatePath();
             std::cout<<"update port 3"<<std::endl;
		}
	}
	return value;
}

PortId PortView::getId()
{
    return id;
}

void PortView::setId(PortId portId)
{
    id = portId;
}
