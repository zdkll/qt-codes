/********************************************************************
created:2019.1
author: jdxchina
purpose:
*********************************************************************/
#ifndef WEBPOPUPWINDOW_H
#define WEBPOPUPWINDOW_H

#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QWebEngineProfile;
class QWebEngineView;
QT_END_NAMESPACE

class HZInfoWebView;
class WebPopupWindow : public QWidget
{
    Q_OBJECT

public:
    WebPopupWindow(QWebEngineProfile *profile);
    QWebEngineView *view() const;

private slots:
    void handleGeometryChangeRequested(const QRect &newGeometry);

private:
    HZInfoWebView *m_view;
};
#endif // WEBPOPUPWINDOW_H
