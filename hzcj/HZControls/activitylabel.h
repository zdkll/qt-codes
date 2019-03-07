#ifndef ACTIVITYLABEL_H
#define ACTIVITYLABEL_H
#include <QLabel>
#include "hzcontrols_global.h"

class HZCONTROLSSHARED_EXPORT ActivityLabel : public QLabel
{
    Q_OBJECT
public:
    ActivityLabel(const QString StrText = "", QWidget * parent = 0);
    ~ActivityLabel(void);

protected:
    void mouseReleaseEvent(QMouseEvent * ev);

signals:
    void clicked();

};
#endif // ACTIVITYLABEL_H
