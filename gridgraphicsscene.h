#ifndef GRIDGRAPHICSSCENE_H
#define GRIDGRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

class GridGraphicsScene : public QGraphicsScene
{
public:
    GridGraphicsScene(QObject *parent = nullptr) : QGraphicsScene(parent) { }

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override
    {

        QPen pen(Qt::lightGray, 0);
        painter->setPen(pen);

        const QRectF &sceneRect = rect;
        const int gridSize = 200;
        for (qreal x = sceneRect.left(); x <= sceneRect.right(); x += gridSize) {
            painter->drawLine(QLineF(x, sceneRect.top(), x, sceneRect.bottom()));
        }

        for (qreal y = sceneRect.top(); y <= sceneRect.bottom(); y += gridSize) {
            painter->drawLine(QLineF(sceneRect.left(), y, sceneRect.right(), y));
        }
        painter->setPen(Qt::green);
        painter->drawLine(
                QLineF(sceneRect.left(), sceneRect.top(), sceneRect.left(), sceneRect.bottom()));
        painter->drawLine(
                QLineF(sceneRect.right(), sceneRect.top(), sceneRect.right(), sceneRect.bottom()));
        painter->drawLine(
                QLineF(sceneRect.left(), sceneRect.top(), sceneRect.right(), sceneRect.top()));
        painter->drawLine(QLineF(sceneRect.left(), sceneRect.bottom(), sceneRect.right(),
                                 sceneRect.bottom()));
    }
};

#endif // GRIDGRAPHICSSCENE_H
