#ifndef BEFORERENDERINGITEM_H
#define BEFORERENDERINGITEM_H

#include <QDebug>
#include <QObject>
#include <QQuickItem>
#include <QElapsedTimer>

//Universal Object handling forwarding before render signal to all listeners
class BeforeRenderGlobal : public QObject{
    Q_OBJECT

public:
    explicit BeforeRenderGlobal(QObject* parent):QObject(parent) {}
    ~BeforeRenderGlobal();

    static BeforeRenderGlobal* Instance();
public Q_SLOTS:
    void handlerBefore();
    void handlerAfter();

Q_SIGNALS:
    void beforeRender();
    void afterRender();

public:
    static BeforeRenderGlobal* m_lpInstance;
};


//QML Component listens and emits beforeRender signal
class BeforeRenderItem : public QQuickItem{
    Q_OBJECT
public:
    explicit BeforeRenderItem(QQuickItem* parent = 0);
    ~BeforeRenderItem() {}


Q_SIGNALS:
    void beforeRender();
    void afterRender();
};

class PerfCounterItem : public QQuickItem {
    Q_OBJECT

    Q_PROPERTY(qint64 averageTime READ average NOTIFY averageChanged)
    Q_PROPERTY(qreal averageFPS READ fps NOTIFY averageChanged)

public:
    explicit PerfCounterItem(QQuickItem* parent = 0);
    ~PerfCounterItem() {}

    Q_INVOKABLE void start();
    Q_INVOKABLE qint64 stop();

    qint64 average() const;
    qreal fps() const;

Q_SIGNALS:
    void averageChanged();
private:
    QElapsedTimer m_timer;

    bool m_bComputeAvg;
    QVector<qint64> m_vAverage;
    quint32 m_szAvgWindow;
    quint32 m_idxCurrent;
};


#endif // BEFORERENDERINGITEM_H
