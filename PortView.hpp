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
#include "Typedefs.hpp"

class RouterView;
class ConnectionView;

class PortView : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 1 };
	enum { NamePort = 1, TypePort = 2 };

    PortView(PortId id, QGraphicsItem *parent = 0);
    ~PortView();

    void setNEBlock(RouterView*);
	void setName(const QString &n);
    void setIsInput(bool isIn);
    int getRadius() const;
    bool getIsInput() const;
    QVector<ConnectionView*>& getConnections();
    const QString& getPortName() const;
	int type() const { return Type; }
    PortId getId() const;
    RouterView* getBlock() const;
    bool isConnected() const;
    void setConnected(bool pisConnected);
    void setId(PortId);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    void setLabelPosition();
    RouterView *m_block;
	QString name;
    bool isInput;
	QGraphicsTextItem *label;
	int radius_;
	int margin;
    QVector<ConnectionView*> m_connections;
	int m_portFlags;
    PortId id;
    bool _isConnected;
};

