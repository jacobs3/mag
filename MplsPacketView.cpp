
#include "MplsPacketView.hpp"

static const int packetLength = 100;
static const int packetHeight = 20;
static const int labelWidth = 20;
static const int heightAboveLink =  20;


MplsPacketView::MplsPacketView(QPointF p1, QPointF p2, std::vector<Label> outLabels, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    QPointF connectionCenterPoint((p1.x() + p2.x())/2, (p1.y() + p2.y())/2);
    int yCoordinate = connectionCenterPoint.y()-packetHeight/2;

    QPainterPath p;
    int currentXCoordinate = connectionCenterPoint.x() - 100;
    std::reverse(outLabels.begin(),outLabels.end());
    for(auto label :outLabels)
    {
        setPen(QPen(Qt::red));
        setBrush(Qt::red);
        p.addRect(currentXCoordinate, yCoordinate, labelWidth, packetHeight);
        QGraphicsTextItem* textLabel = new QGraphicsTextItem(this);
        textLabel->setPos(currentXCoordinate, yCoordinate);
        textLabel->setPlainText(QString::fromUtf8(std::to_string(label).c_str()));
        currentXCoordinate += labelWidth;
        setPath(p);
    }

    p.addRect(currentXCoordinate, yCoordinate, packetLength, packetHeight);
    setPath(p);
    setPen(QPen(Qt::darkGreen));
    setBrush(Qt::green);
}
