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

#include <QGraphicsPathItem>
#include <QWidget>
#include <QMenu>
#include <QtGui>

#include "Typedefs.hpp"

class PortView;

class RouterView : public QWidget, public QGraphicsPathItem
{
    Q_OBJECT
public:
	enum { Type = QGraphicsItem::UserType + 3 };

    RouterView(RouterId id, QGraphicsItem *parent = 0);

    PortView* addPort(const QString &name, bool isInput);
	void addInputPort(const QString &name);
	void addOutputPort(const QString &name);
	void addInputPorts(const QStringList &names);
	void addOutputPorts(const QStringList &names);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int getId() const;
    QVector<PortView*> getPorts() const;
    void onDelete();
	int type() const { return Type; }

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void contextMenuEvent(QContextMenuEvent* pe)
       {
           m_pmnu->exec(pe->globalPos());
       }

private:
	int horzMargin;
	int vertMargin;
	int width;
	int height;
    RouterId id;
    QMenu* m_pmnu;
};

