#include "bb10proxystyle.h"

BB10ProxyStyle::BB10ProxyStyle(QStyle *style)
    : QProxyStyle(style) {}

int BB10ProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric) {
    case QStyle::PM_ButtonIconSize:
        return 42;
    case QStyle::PM_SmallIconSize:
        return 42;
    case QStyle::PM_LargeIconSize:
        return 64;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

int BB10ProxyStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    if (hint == QStyle::SH_UnderlineShortcut)
        return 0;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
