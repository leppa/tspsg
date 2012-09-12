/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#include "qtwin.h"
#include <QLibrary>
#include <QApplication>
#include <QWidget>
#include <QList>
#include <QPointer>

//! \todo TODO: Detect X11 in Qt 5 more reliably
// No Q_WS_X11 in Qt 5. Check if we're using X11 and define it.
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0)) && defined(Q_OS_UNIX) && !defined(Q_WS_X11)
#   if !defined(Q_OS_MAC) && !defined(Q_OS_SYMBIAN) && !defined(Q_WS_QPA)
#       define Q_WS_X11
#   endif
#endif

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif // Q_WS_X11

#ifdef Q_OS_WIN32

#include <qt_windows.h>

// Blur behind data structures
#define DWM_BB_ENABLE                 0x00000001  // fEnable has been specified
#define DWM_BB_BLURREGION             0x00000002  // hRgnBlur has been specified
#define DWM_BB_TRANSITIONONMAXIMIZED  0x00000004  // fTransitionOnMaximized has been specified
#define WM_DWMCOMPOSITIONCHANGED        0x031E    // Composition changed window message

typedef struct _DWM_BLURBEHIND
{
    DWORD dwFlags;
    BOOL fEnable;
    HRGN hRgnBlur;
    BOOL fTransitionOnMaximized;
} DWM_BLURBEHIND, *PDWM_BLURBEHIND;

typedef struct _MARGINS
{
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} MARGINS, *PMARGINS;

typedef HRESULT (WINAPI *PtrDwmIsCompositionEnabled)(BOOL* pfEnabled);
typedef HRESULT (WINAPI *PtrDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS* pMarInset);
typedef HRESULT (WINAPI *PtrDwmEnableBlurBehindWindow)(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
typedef HRESULT (WINAPI *PtrDwmGetColorizationColor)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);

static PtrDwmIsCompositionEnabled pDwmIsCompositionEnabled= 0;
static PtrDwmEnableBlurBehindWindow pDwmEnableBlurBehindWindow = 0;
static PtrDwmExtendFrameIntoClientArea pDwmExtendFrameIntoClientArea = 0;
static PtrDwmGetColorizationColor pDwmGetColorizationColor = 0;


/*
 * Internal helper class that notifies windows if the
 * DWM compositing state changes and updates the widget
 * flags correspondingly.
 */
class WindowNotifier : public QWidget
{
public:
    WindowNotifier() { winId(); }
    void addWidget(QWidget *widget) { widgets.append(widget); }
    void removeWidget(QWidget *widget) { widgets.removeAll(widget); }
    bool winEvent(MSG *message, long *result);

private:
    QWidgetList widgets;
};

static bool resolveLibs()
{
    if (!pDwmIsCompositionEnabled) {
        QLibrary dwmLib(QString::fromAscii("dwmapi"));
        pDwmIsCompositionEnabled =(PtrDwmIsCompositionEnabled)dwmLib.resolve("DwmIsCompositionEnabled");
        pDwmExtendFrameIntoClientArea = (PtrDwmExtendFrameIntoClientArea)dwmLib.resolve("DwmExtendFrameIntoClientArea");
        pDwmEnableBlurBehindWindow = (PtrDwmEnableBlurBehindWindow)dwmLib.resolve("DwmEnableBlurBehindWindow");
        pDwmGetColorizationColor = (PtrDwmGetColorizationColor)dwmLib.resolve("DwmGetColorizationColor");
    }
    return pDwmIsCompositionEnabled != 0;
}

#endif

/*!
  * Chekcs and returns true if Windows DWM composition
  * is currently enabled on the system.
  *
  * To get live notification on the availability of
  * this feature, you will currently have to
  * reimplement winEvent() on your widget and listen
  * for the WM_DWMCOMPOSITIONCHANGED event to occur.
  *
  */
bool QtWin::isCompositionEnabled()
{
#ifdef Q_OS_WIN32
    if (resolveLibs()) {
        HRESULT hr = S_OK;
        BOOL isEnabled = false;
        hr = pDwmIsCompositionEnabled(&isEnabled);
        if (SUCCEEDED(hr))
            return isEnabled;
    }
    return false;
#elif defined(Q_WS_X11)
    Display *d = XOpenDisplay(NULL);
    Atom _NET_WM_CM_S0 = XInternAtom(d, "_NET_WM_CM_S0", true);
    bool result = (_NET_WM_CM_S0 != None);
    if (result)
        result = (XGetSelectionOwner(d, _NET_WM_CM_S0) != None);
    XCloseDisplay(d);
    return result;
#else
    //! \todo TODO: Check for trsnsparency support in other OSes.
    return false;
#endif
}

/*!
  * Enables Blur behind on a Widget.
  *
  * \a enable tells if the blur should be enabled or not
  */
bool QtWin::enableBlurBehindWindow(QWidget *widget, bool enable)
{
    Q_ASSERT(widget);
    bool result = false;
#ifdef Q_OS_WIN32
    if (resolveLibs()) {
        DWM_BLURBEHIND bb = {0};
        HRESULT hr = S_OK;
        bb.fEnable = enable;
        bb.dwFlags = DWM_BB_ENABLE;
        bb.hRgnBlur = NULL;
#endif
        widget->setAttribute(Qt::WA_TranslucentBackground, enable);
#ifdef Q_WS_X11
        widget->setAttribute(Qt::WA_NoSystemBackground, false);
        QPalette p = widget->palette();
        QColor c = p.color(QPalette::Window);
        if (enable)
            c.setAlpha(160);
        else
            c.setAlpha(255);
        p.setColor(QPalette::Window, c);
        widget->setPalette(p);
        result = true;
#endif
#ifdef Q_OS_WIN32
        hr = pDwmEnableBlurBehindWindow(HWND(widget->winId()), &bb);
        if (SUCCEEDED(hr)) {
            result = true;
            windowNotifier()->addWidget(widget);
        }
    }
#endif
    return result;
}

/*!
  * ExtendFrameIntoClientArea.
  *
  * This controls the rendering of the frame inside the window.
  * Note that passing margins of -1 (the default value) will completely
  * remove the frame from the window.
  *
  * \note you should not call enableBlurBehindWindow before calling
  *       this functions
  *
  * \a enable tells if the blur should be enabled or not
  */
bool QtWin::extendFrameIntoClientArea(QWidget *widget, int left, int top, int right, int bottom)
{

    Q_ASSERT(widget);
    Q_UNUSED(widget);
    Q_UNUSED(left);
    Q_UNUSED(top);
    Q_UNUSED(right);
    Q_UNUSED(bottom);

    bool result = false;
#ifdef Q_OS_WIN32
    if (resolveLibs()) {
        QLibrary dwmLib(QString::fromAscii("dwmapi"));
        HRESULT hr = S_OK;
        MARGINS m = {left, top, right, bottom};
        hr = pDwmExtendFrameIntoClientArea(HWND(widget->winId()), &m);
        if (SUCCEEDED(hr)) {
            result = true;
            windowNotifier()->addWidget(widget);
        }
        widget->setAttribute(Qt::WA_TranslucentBackground, result);
    }
#endif
    return result;
}

/*!
  * Returns the current colorizationColor for the window.
  *
  * \a enable tells if the blur should be enabled or not
  */
QColor QtWin::colorizatinColor()
{
    QColor resultColor = QApplication::palette().window().color();

#ifdef Q_OS_WIN32
    if (resolveLibs()) {
        DWORD color = 0;
        BOOL opaque = FALSE;
        QLibrary dwmLib(QString::fromAscii("dwmapi"));
        HRESULT hr = S_OK;
        hr = pDwmGetColorizationColor(&color, &opaque);
        if (SUCCEEDED(hr))
            resultColor = QColor(color);
    }
#endif
    return resultColor;
}

#ifdef Q_OS_WIN32
WindowNotifier *QtWin::windowNotifier()
{
    static WindowNotifier *windowNotifierInstance = 0;
    if (!windowNotifierInstance)
        windowNotifierInstance = new WindowNotifier;
    return windowNotifierInstance;
}


/* Notify all enabled windows that the DWM state changed */
bool WindowNotifier::winEvent(MSG *message, long *result)
{
    if (message && message->message == WM_DWMCOMPOSITIONCHANGED) {
        bool compositionEnabled = QtWin::isCompositionEnabled();
        foreach(QWidget * widget, widgets) {
            if (widget) {
                widget->setAttribute(Qt::WA_NoSystemBackground, compositionEnabled);
            }
            widget->update();
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    return QWidget::winEvent(message, result);
#else
    return QWidget::nativeEvent("windows_generic_MSG", message, result);
#endif
}
#endif
