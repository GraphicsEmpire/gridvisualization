#include "BeforeRenderingItem.h"
#define DEFAULT_AVG_WINDOW 128

//Reset
BeforeRenderGlobal* BeforeRenderGlobal::m_lpInstance = NULL;

BeforeRenderGlobal::~BeforeRenderGlobal(){
    if(m_lpInstance)
    {
        delete m_lpInstance;
        m_lpInstance = NULL;
    }
}

BeforeRenderGlobal* BeforeRenderGlobal::Instance(){
    if(m_lpInstance == NULL)
        m_lpInstance = new BeforeRenderGlobal(NULL);
    return m_lpInstance;
}

void BeforeRenderGlobal::handlerBefore()
{
    emit beforeRender();
}

void BeforeRenderGlobal::handlerAfter()
{
    emit afterRender();
}


BeforeRenderItem::BeforeRenderItem(QQuickItem *parent):QQuickItem(parent)
{
    connect(BeforeRenderGlobal::Instance(), SIGNAL(beforeRender()), this, SIGNAL(beforeRender()));
    connect(BeforeRenderGlobal::Instance(), SIGNAL(afterRender()), this, SIGNAL(afterRender()));
}

////////////////////////////////////////////////////////////////
PerfCounterItem::PerfCounterItem(QQuickItem *parent):QQuickItem(parent) {
    m_idxCurrent = 0;
    m_szAvgWindow = DEFAULT_AVG_WINDOW;
    m_vAverage.resize(DEFAULT_AVG_WINDOW);
    m_vAverage.fill(0);
}

void PerfCounterItem::start() {
    m_timer.start();
}

qint64 PerfCounterItem::stop() {
    qint64 elapsed = m_timer.elapsed();
    m_vAverage.push_back(elapsed);

    m_vAverage[m_idxCurrent] = elapsed;
    m_idxCurrent = (m_idxCurrent + 1) % m_szAvgWindow;
    emit averageChanged();

    return elapsed;
}

qint64 PerfCounterItem::average() const {
    qint64 sum = 0;
    for(quint32 i=0; i < m_szAvgWindow; i++)
        sum += m_vAverage[i];
    return sum / m_szAvgWindow;
}

qreal PerfCounterItem::fps() const {
    qint64 avg = average();
    return (avg != 0) ? 1000.0 / static_cast<qreal>(avg) : 1000.0;
}






