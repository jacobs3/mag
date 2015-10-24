
#include "MplsPacketView.hpp"

//#include <math.h>
#include <iostream>

static const int packetLength = 100;
static const int packetHeight = 20;
static const int labelWidth = 20;
static const int heightAboveLink =  20;


/*std::vector<Qt::GlobalColor> MplsPacketView::colors =
{
    Qt::darkGray,
    Qt::gray,
    Qt::lightGray,
    Qt::red,
    Qt::blue,
    Qt::cyan,
    Qt::magenta,
    Qt::yellow,
    Qt::darkRed,
    Qt::darkGreen,
    Qt::darkBlue,
    Qt::darkCyan,
    Qt::darkMagenta,
    Qt::darkYellow,
};*/

/*MplsPacketView::MplsPacketView(QPointF p1, QPointF p2, std::vector<Label> outLabels, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    std::cout<<"packet create"<<std::endl;

    int labelStackSize = outLabels.size();

    int mplsPacketSize = packetLength + labelStackSize*labelHeight;

    QPointF connectionCenterPoint((p1.x() + p2.x())/2, (p1.y() + p2.y())/2);
    connectionCenterPoint.setY(connectionCenterPoint.y() - heightAboveLink);

    double slopeCoefficient = (p1.y() - p2.y())/(p1.x() - p2.x());
    double rectangleXPos = slopeCoefficient/sqrt(mplsPacketSize) - 1;
    double rectangleYPos = (slopeCoefficient*sqrt(mplsPacketSize))/(1+slopeCoefficient);
  //  rectangleXPos = 20;
  //  rectangleYPos = 8;
    QPainterPath p;
    p.rotate(45);
    p.addRect(QRect(connectionCenterPoint.x() - abs(rectangleXPos) , rectangleYPos, abs(rectangleXPos*2), labelHeight));
    setPath(p);
  //  p.set
    setPen(QPen(Qt::darkGreen));
    setBrush(Qt::green);

    std::cout<<"p1 "<<p1.x()<<" "<<p1.y()<<" p2 "<<p2.x()<<" "<<p2.y()<<std::endl;
    std::cout<<"slope coefficient "<<slopeCoefficient<<" center point "<<connectionCenterPoint.x()<<" "<<connectionCenterPoint.y()<<std::endl;
     std::cout<<"rectangle pos "<<rectangleXPos<<" "<<rectangleYPos<<std::endl;

}*/

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
