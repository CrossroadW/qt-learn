#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QKeyEvent>
#include <QMovie>
#include <QObject>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
#include <QMediaPlayer>
class Ball : public QGraphicsRectItem
{
public:
    Ball()
    {
        setRect(0, 0, 100, 50);
        timer = new QTimer();
        timer->start(1000 / 60);
        QObject::connect(timer, &QTimer::timeout, [this] { move(); });

        movie = new QMovie(":/red.png");
        movie->start();
        QObject::connect(movie, &QMovie::frameChanged, [this]() { this->update(); });
        play();
        qDebug() << __FUNCTION__;
    }

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, movie->currentPixmap().width(), movie->currentPixmap().height());
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->drawPixmap(0, 0, movie->currentPixmap());
    }
    ~Ball()
    {
        delete timer;
        delete movie;
        delete player;
        qDebug() << __FUNCTION__;
    }
    void move()
    {
        setPos(x() + speed, y());
        if (x() > 1500 || y() > 1000 || x() < -1500 || y() < -1000) {
            scene()->removeItem(this);
            delete this;
        }
    }
    void play()
    {
        player = new QMediaPlayer();
        player->setMedia(QUrl("qrc:/fire.mp3"));
        player->setVolume(100);
        player->play();
    }
    QTimer *timer;
    int speed = 10;
    QMovie *movie;
    QMediaPlayer *player;
};
class GifItem : public QGraphicsItem
{

public:
    bool flag = true;
    GifItem(QGraphicsScene *scene) : QGraphicsItem(), scene(scene)
    {
        auto vx = QRandomGenerator::global()->bounded(-500, 400);
        auto vy = QRandomGenerator::global()->bounded(-400, 300);
        setPos(vx, vy);
        movie = new QMovie(":/fire.gif");
        movie->start();
        QObject::connect(movie, &QMovie::frameChanged, [this]() { this->update(); });
        throwTimer = new QTimer();
        QObject::connect(throwTimer, &QTimer::timeout, [this]() { this->throwBall(); });
        throwTimer->start(1000 / 33);
        duration = new QTimer();
        QObject::connect(throwTimer, &QTimer::timeout, [this]() { this->flag = !this->flag; });
        duration->start(5000);
    }

    ~GifItem()
    {
        delete movie;
        delete throwTimer;
        delete duration;
    }

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, movie->currentPixmap().width(), movie->currentPixmap().height());
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->drawPixmap(0, 0, movie->currentPixmap());
    }
    void throwBall()
    {
        if (!flag)
            return;
        Ball *ball = new Ball();
        auto newx = pos().x() + 200;
        auto newy = pos().y();

        ball->setPos(newx, newy);
        scene->addItem(ball);
    }

private:
    QMovie *movie;
    QGraphicsScene *scene;
    QTimer *throwTimer;
    QTimer *duration;
};

class CustomGraphicsView : public QGraphicsView
{
public:
    CustomGraphicsView(QGraphicsScene *scene) : QGraphicsView(scene)
    {
        setMouseTracking(true);
        moveTimer = new QTimer(this);
        moveTimer->setInterval(1000 / 33);
        connect(moveTimer, &QTimer::timeout, this, &CustomGraphicsView::moveItem);
    }

    void setRectItem(QGraphicsItem *item) { innerItem = item; }

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            movingItem = innerItem;
            mousePressPos = event->pos();
            itemStartPos = movingItem->pos();
            lastMoveDirection = QPointF(0, 0);
            moveTimer->start();
            qDebug() << __FUNCTION__;
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (movingItem) {
            QPointF diff = mapToScene(event->pos()) - mapToScene(mousePressPos);
            lastMoveDirection = diff;
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            movingItem = nullptr;
            moveTimer->stop();
        }
        qDebug() << __FUNCTION__;
        QGraphicsView::mouseReleaseEvent(event);
    }

private:
    QGraphicsItem *movingItem = nullptr;
    QPoint mousePressPos;
    QPointF itemStartPos;
    QPointF lastMoveDirection;
    QGraphicsItem *innerItem;
    QTimer *moveTimer;

    void moveItem()
    {
        if (!movingItem)
            return;

        QPointF newPos = movingItem->pos()
                + QPointF(lastMoveDirection.x() * 0.06, lastMoveDirection.y() * 0.06);

        qreal leftBoundary = -1500;
        qreal rightBoundary = 1500;
        qreal topBoundary = -1000;
        qreal bottomBoundary = 1000;

        if (newPos.x() >= leftBoundary && newPos.x() <= rightBoundary && newPos.y() >= topBoundary
            && newPos.y() <= bottomBoundary) {
            movingItem->setPos(newPos);
        } else {

            if (newPos.x() < leftBoundary || newPos.x() > rightBoundary) {
                lastMoveDirection.setX(-lastMoveDirection.x());
            }
            if (newPos.y() < topBoundary || newPos.y() > bottomBoundary) {
                lastMoveDirection.setY(-lastMoveDirection.y());
            }
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QGraphicsScene scene;
    // CustomGraphicsView view(&scene);
    // GifItem *rectItem = new GifItem(&scene);
    // scene.setSceneRect(-600, -600, 1200, 1200);
    // scene.addItem(rectItem);
    // view.setRectItem(rectItem);
    // view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // view.resize(1000, 800);

    // view.show();
    // view.move(250, 100);
    // view.activateWindow();

    return a.exec();
}
