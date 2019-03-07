#include "viewstyleproperties.h"

StyleProperties ViewStyleProperties::styleProperties()
{
    StyleProperties properties;
    properties.riseColor = riseColor();
    properties.plainColor=plainColor();
    properties.fallColor=fallColor();
    properties.normalColor = normalColor();

    properties.filePixmap = filePixmap();
    properties.fileHoverPixmap = fileHoverPixmap();

    return properties;
}
