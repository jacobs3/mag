
#pragma once

#include <QObject>
#include <QMainWindow>
#include <QGraphicsScene>
#include "Site.hpp"

class IController : public QMainWindow
{
    Q_OBJECT
public:
    IController(QMainWindow *parent):QMainWindow(parent){}
    virtual void install(QGraphicsScene *scene) = 0;
    virtual bool setRouterName(RouterId id, std::string name) = 0;
    virtual void setPortName(PortId id, std::string name) = 0;
    virtual void removePort(RouterId routerId, PortId portId) = 0;
    virtual void addPort(RouterId routerId, std::string name, bool isInput) = 0;
    virtual void setModel(std::shared_ptr<Site> site) = 0;
    virtual std::string addNhlfeEntry(RouterId id, NhlfeEntry entry) = 0;
    virtual void removeNhlfeEntry(RouterId id, NhlfeEntry entry) = 0;
    virtual std::string addIlmEntry(RouterId id, IlmEntry entry) = 0;
    virtual void removeIlmEntry(RouterId id, IlmEntry entry) = 0;
    virtual std::string addFtnEntry(RouterId id, FtnEntry entry) = 0;
    virtual void removeFtnEntry(RouterId id, FtnEntry entry) = 0;
    virtual std::string displayNextMplsPacket() = 0;
    virtual std::string displayNextMplsPacket(RouterId id, FEC fec) = 0;
    virtual std::string displayPrevMplsPacket() = 0;
    virtual void eraseMplsPacket() = 0;
public slots:
    virtual void addRouter() = 0;

};
