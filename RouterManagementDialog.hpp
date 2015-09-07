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
#include "Typedefs.hpp"
#include "Router.hpp"
#include <list>
#include <memory>
class Editor;

class Editor;

class RouterManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RouterManagementDialog(std::shared_ptr<Router>, Editor*, QWidget*);
    ~RouterManagementDialog(){}

private slots:
    void setRouterName();
    void setPortName();
    void removePort();
    void addPort();

private:
    void repopulatePortNamesList();
    void fillPortsComboBox(QComboBox *combo);
    QLineEdit *routerNameField;
    QLineEdit *portNameField;
    QLineEdit *addPortNameField;
    QCheckBox *isInputCheckBox;
    QVBoxLayout *mainLayout;
    Editor *nodesEditor;
    QComboBox *renamePortsComboBox;
    QComboBox *removePortsComboBox;
    QGraphicsView *view;
    QGraphicsScene *scene;
    std::shared_ptr<Router> router;
    RouterId id;
    std::map<PortId, std::shared_ptr<Port> > ports;

};
