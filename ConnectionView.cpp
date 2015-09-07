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

#include "ConnectionView.hpp"
#include "PortView.hpp"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

ConnectionView::ConnectionView(ConnectionId connId, QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
    id = connId;
	setPen(QPen(Qt::black, 2));
	setBrush(Qt::NoBrush);
	setZValue(-1);
	m_port1 = 0;
	m_port2 = 0;
}
ConnectionView::~ConnectionView()
{
	if (m_port1)
		m_port1->connections().remove(m_port1->connections().indexOf(this));
	if (m_port2)
		m_port2->connections().remove(m_port2->connections().indexOf(this));
}

void ConnectionView::setPos1(const QPointF &p)
{
	pos1 = p;
}

void ConnectionView::setPos2(const QPointF &p)
{
	pos2 = p;
}

void ConnectionView::setPort1(PortView *p)
{
	m_port1 = p;

	m_port1->connections().append(this);
}

void ConnectionView::setPort2(PortView *p)
{
	m_port2 = p;

	m_port2->connections().append(this);
}

void ConnectionView::updatePosFromPorts()
{
    pos1 = m_port1->scenePos();
    pos2 = m_port2->scenePos();
}

void ConnectionView::updatePath()
{
	QPainterPath p;

	//QPointF pos1(m_port1->scenePos());
	//QPointF pos2(m_port2->scenePos());

	p.moveTo(pos1);

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

	QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
	QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

	p.cubicTo(ctr1, ctr2, pos2);

	setPath(p);
}

PortView* ConnectionView::port1() const
{
	return m_port1;
}

PortView* ConnectionView::port2() const
{
	return m_port2;
}

void ConnectionView::save(QDataStream &ds)
{
	ds << (quint64) m_port1;
	ds << (quint64) m_port2;
}

void ConnectionView::load(QDataStream &ds, const QMap<quint64, PortView*> &portMap)
{
	quint64 ptr1;
	quint64 ptr2;
	ds >> ptr1;
	ds >> ptr2;

	setPort1(portMap[ptr1]);
	setPort2(portMap[ptr2]);
	updatePosFromPorts();
	updatePath();
}

ConnectionId ConnectionView::getId()
{
    return id;
}
