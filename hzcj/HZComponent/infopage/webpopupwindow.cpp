#include "webpopupwindow.h"
#include "infowebview.h"
#include <QIcon>
#include <QVBoxLayout>

WebPopupWindow::WebPopupWindow(QWebEngineProfile *profile)
    : m_view(new HZInfoWebView(this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    setLayout(layout);
    layout->addWidget(m_view);

    m_view->setPage(new HZInfoWebPage(PopWindow, profile, m_view));
    m_view->setFocus();

    connect(m_view->page(), &HZInfoWebPage::geometryChangeRequested, this, &WebPopupWindow::handleGeometryChangeRequested);
    connect(m_view->page(), &HZInfoWebPage::windowCloseRequested, this, &QWidget::close);
}

QWebEngineView *WebPopupWindow::view() const
{
    return m_view;
}

void WebPopupWindow::handleGeometryChangeRequested(const QRect &newGeometry)
{
    m_view->setMinimumSize(newGeometry.width(), newGeometry.height());
    move(newGeometry.topLeft() - m_view->pos());
    // let the layout do the magic
    resize(0, 0);
    show();
}
