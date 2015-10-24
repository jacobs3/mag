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

#include "RouterView.hpp"
#include "PortView.hpp"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

RouterView::RouterView(RouterId routerId, QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
      id(routerId)
{
	QPainterPath p;
    p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
	horzMargin = 20;
	vertMargin = 5;
	width = horzMargin;
	height = vertMargin;

     m_pmnu = new QMenu(this);
     m_pmnu->addAction("&Red");
     m_pmnu->addAction("&Green");
     m_pmnu->addAction("&Blue");
}
void RouterView::onDelete()
{
}

int RouterView::getId() const
{
    return id;
}

PortView* RouterView::addPort(const QString &name, bool isInput)
{
    PortView *port = new PortView(0, this);
	port->setName(name);
    port->setIsInput(isInput);
	port->setNEBlock(this);

	QFontMetrics fm(scene()->font());
	int w = fm.width(name);
	int h = fm.height();
	if (w > width - horzMargin)
		width = w + horzMargin;
	height += h;

	QPainterPath p;
	p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
	setPath(p);

    int y = -height / 2 + vertMargin + port->getRadius();
    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != PortView::Type)
			continue;

        PortView *port = (PortView*) port_;
        if (!port->getIsInput())
            port->setPos(width/2 + port->getRadius(), y);
		else
            port->setPos(-width/2 - port->getRadius(), y);
		y += h;
	}

	return port;
}

void RouterView::addInputPort(const QString &name)
{
    addPort(name, true);
}

void RouterView::addOutputPort(const QString &name)
{
    addPort(name, false);
}

void RouterView::addInputPorts(const QStringList &names)
{
	foreach(QString n, names)
		addInputPort(n);
}

void RouterView::addOutputPorts(const QStringList &names)
{
	foreach(QString n, names)
		addOutputPort(n);
}

void RouterView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

    if (isSelected())
    {
        painter->setPen(QPen(Qt::darkBlue));
        painter->setBrush(Qt::darkBlue);
    }
    else
    {
        painter->setPen(QPen(Qt::darkBlue));
        painter->setBrush(Qt::blue);
	}

	painter->drawPath(path());
}

QVector<PortView*> RouterView::getPorts() const
{
    QVector<PortView*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
        if (port_->type() == PortView::Type)
            res.append((PortView*) port_);
	}
	return res;
}

QVariant RouterView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_UNUSED(change);
	return value;
}

