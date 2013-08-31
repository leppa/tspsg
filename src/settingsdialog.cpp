/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2013 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  TSPSG is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TSPSG.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "settingsdialog.h"

#include <QColorDialog>
#include <QFontDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QSettings>

#ifdef HANDHELD
#   include <QScrollArea>
#endif

#if !defined(QT_NO_STATUSTIP)
#   include <QStatusTipEvent>
#endif

#ifndef HANDHELD
    // Eyecandy
#   include "qtwin.h"
#endif // HANDHELD

/*!
 * \brief Class constructor.
 * \param parent Settings Dialog parent widget.
 *
 * Initializes Settings Dialog and creates its layout based on target OS.
 */
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), _fontChanged(false), _colorChanged(false), _translucency(0)
{
    setupUi(this);

    setWindowIcon(GET_ICON("preferences-system"));

    buttonBox->button(QDialogButtonBox::Ok)->setIcon(GET_ICON("dialog-ok"));
#ifndef QT_NO_STATUSTIP
    buttonBox->button(QDialogButtonBox::Ok)->setStatusTip(tr("Save new preferences"));
#endif
    buttonBox->button(QDialogButtonBox::Ok)->setCursor(QCursor(Qt::PointingHandCursor));
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon(GET_ICON("dialog-cancel"));
#ifndef QT_NO_STATUSTIP
    buttonBox->button(QDialogButtonBox::Cancel)->setStatusTip(tr("Close without saving preferences"));
#endif
    buttonBox->button(QDialogButtonBox::Cancel)->setCursor(QCursor(Qt::PointingHandCursor));

    buttonTextColor->setIcon(GET_ICON("format-text-color"));
    buttonSelectedColor->setIcon(GET_ICON("format-text-color"));
    buttonAlternateColor->setIcon(GET_ICON("format-text-color"));
    buttonBorderColor->setIcon(GET_ICON("format-stroke-color"));
//    buttonBgColor->setIcon(GET_ICON("format-fill-color"));
    buttonFont->setIcon(GET_ICON("preferences-desktop-font"));
    buttonHelp->setIcon(GET_ICON("help-hint"));

QPalette p = bgWhite->palette();
    p.setColor(QPalette::Window, p.color(QPalette::Base));
    bgWhite->setPalette(p);

    p = lineHorizontal->palette();
    p.setColor(QPalette::Window, p.color(QPalette::Text));
    lineHorizontal->setPalette(p);

// Layout helpers
QBoxLayout *box;
QHBoxLayout *hbox;
#ifndef QT_NO_PRINTER
    cbHQGraph = new QCheckBox(bgWhite);
    cbHQGraph->setObjectName("cbHQGraph");
#ifndef QT_NO_STATUSTIP
    cbHQGraph->setStatusTip(tr("Higher quality graph looks much better when printing but uglier on the screen"));
#endif // QT_NO_STATUSTIP
    cbHQGraph->setText(tr("Draw solution graph in higher quality"));
    cbHQGraph->setCursor(QCursor(Qt::PointingHandCursor));

    box = static_cast<QBoxLayout *>(tabOutputGeneral->layout());
    hbox = new QHBoxLayout();
    hbox->addSpacing(10);
    hbox->addWidget(cbHQGraph);
    box->insertLayout(box->indexOf(cbGenerateGraph) + 3, hbox);
    connect(cbGenerateGraph, SIGNAL(toggled(bool)), cbHQGraph, SLOT(setEnabled(bool)));
#endif

    if (hasUpdater()) {
        cbCheck4Updates = new QCheckBox(bgWhite);
        cbCheck4Updates->setObjectName("cbCheck4Updates");
#ifndef QT_NO_STATUSTIP
        cbCheck4Updates->setStatusTip(tr("Automatically check for updates at the given interval"));
#endif // QT_NO_STATUSTIP
        cbCheck4Updates->setText(tr("Check for updates every"));
        cbCheck4Updates->setCursor(QCursor(Qt::PointingHandCursor));

        spinUpdateCheckInterval = new QSpinBox(bgWhite);
        spinUpdateCheckInterval->setObjectName("spinUpdateCheckInterval");
#ifndef QT_NO_STATUSTIP
        spinUpdateCheckInterval->setStatusTip(tr("Minimal interval at which to check for updates"));
#endif // QT_NO_STATUSTIP
        spinUpdateCheckInterval->setSuffix(tr(" days", "Don't forget a space at the beginning!"));
        spinUpdateCheckInterval->setRange(1, 365);
        spinUpdateCheckInterval->setCursor(QCursor(Qt::PointingHandCursor));

        connect(cbCheck4Updates, SIGNAL(toggled(bool)), spinUpdateCheckInterval, SLOT(setEnabled(bool)));

        box = static_cast<QBoxLayout *>(tabGeneral->layout());
        hbox = new QHBoxLayout();
        hbox->setSpacing(0);
        hbox->addWidget(cbCheck4Updates);
        hbox->addWidget(spinUpdateCheckInterval);
        hbox->addStretch();
        box->insertLayout(box->indexOf(cbUseNativeDialogs) + 1, hbox);
    } else
        cbCheck4Updates = NULL;

#ifdef HANDHELD
QVBoxLayout *vbox1; // Layout helper

    // On screens with small height when SIP is shown and the window is resized
    // there is not enought space for all elements.
    // So we show the scrollbars to be able to access them.
QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(bgWhite);

    bgWhite->layout()->setMargin(0);

#ifdef Q_OS_SYMBIAN
    // On Symbian buttons are moved to a native bar so Help button is
    // left alone. This causes it to be more to the middle instead of
    // to the left. Adding a stretch fixes this issue.
    buttons->insertStretch(buttons->indexOf(buttonHelp) + 1);
#endif

    // Central layout
    vbox1 = new QVBoxLayout(this);
    vbox1->setMargin(0);
    vbox1->setSpacing(0);
    vbox1->addWidget(scrollArea);
    vbox1->addWidget(bgGrey);
    setLayout(vbox1);
#else // HANDHELD
QVBoxLayout *vbox; // Layout helper

#ifndef HANDHELD
    if (QtWin::isCompositionEnabled()) {
        cbUseTranslucency = new QCheckBox(bgWhite);
        cbUseTranslucency->setObjectName("cbUseTranslucency");
#ifndef QT_NO_STATUSTIP
        cbUseTranslucency->setStatusTip(tr("Make Main Window background translucent"));
#endif // QT_NO_STATUSTIP
        cbUseTranslucency->setText(tr("Use translucency effects"));
        cbUseTranslucency->setCursor(QCursor(Qt::PointingHandCursor));
    }
#endif // HANDHELD

    cbSaveState = new QCheckBox(bgWhite);
    cbSaveState->setObjectName("cbSaveState");
#ifndef QT_NO_STATUSTIP
    cbSaveState->setStatusTip(tr("Restore main window state and position on application restart"));
#endif // QT_NO_STATUSTIP
    cbSaveState->setText(tr("Save main window state and position"));
    cbSaveState->setCursor(QCursor(Qt::PointingHandCursor));

    imgIcon = new QLabel(this);
    imgIcon->setObjectName("imgIcon");
    imgIcon->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    imgIcon->setFrameShape(QFrame::NoFrame);
    imgIcon->setPixmap(GET_ICON("preferences-system").pixmap(128, 128));
    imgIcon->setStyleSheet("background-color: #0080C0; padding-top: 11px;");
    imgIcon->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    imgIcon->setMinimumWidth(150);

    labelHint = new QLabel(bgGrey);
    labelHint->setObjectName("labelHint");
    labelHint->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    labelHint->setMinimumSize(QSize(250, 28));
    labelHint->setMaximumSize(QSize(QWIDGETSIZE_MAX, 28));
    labelHint->setTextFormat(Qt::PlainText);
//	labelHint->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    labelHint->setWordWrap(true);
#ifndef QT_NO_STATUSTIP
    labelHint->setStatusTip(tr("Hover mouse pointer over dialog elements to get additional help"));
#endif // QT_NO_STATUSTIP

    lineVertical = new QFrame(this);
    lineVertical->setObjectName("lineVertical");
    lineVertical->setFrameShadow(QFrame::Plain);
    lineVertical->setFrameShape(QFrame::VLine);
    lineVertical->setLineWidth(2);

    // Top line
    hbox = new QHBoxLayout();
    hbox->addWidget(imgIcon);
    hbox->addWidget(lineVertical);
    hbox->addWidget(bgWhite);

    box = static_cast<QBoxLayout *>(tabGeneral->layout());
    box->insertWidget(box->indexOf(cbUseNativeDialogs) + 1, cbSaveState);
#ifndef HANDHELD
    if (QtWin::isCompositionEnabled())
        box->insertWidget(box->indexOf(cbUseNativeDialogs) + 1, cbUseTranslucency);
#endif // HANDHELD

    // Inserting label for hints to the bottom part (with grey bg)
    buttons->insertWidget(buttons->indexOf(buttonHelp) + 1, labelHint, 1);

    // Central layout
    vbox = new QVBoxLayout(this);
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addLayout(hbox);
    vbox->addWidget(bgGrey);
    setLayout(vbox);
#endif // HANDHELD

#ifdef Q_OS_WINCE_WM
    // We need to react to SIP show/hide and resize the window appropriately
    connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), SLOT(desktopResized(int)));
#endif // Q_OS_WINCE_WM
    connect(spinRandMin, SIGNAL(valueChanged(int)), SLOT(spinRandMinValueChanged(int)));
    connect(buttonFont, SIGNAL(clicked()), SLOT(buttonFontClicked()));
    connect(buttonTextColor, SIGNAL(clicked()), SLOT(buttonTextColorClicked()));
    connect(buttonSelectedColor, SIGNAL(clicked()), SLOT(buttonSelectedColorClicked()));
    connect(buttonAlternateColor, SIGNAL(clicked()), SLOT(buttonAlternateColorClicked()));
    connect(buttonBorderColor, SIGNAL(clicked()), SLOT(buttonBorderColorClicked()));
//    connect(buttonBgColor, SIGNAL(clicked()), SLOT(buttonBgColorClicked()));
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
#if !defined(QT_NO_STATUSTIP) && !defined(HANDHELD)
    // Setting initial text of dialog hint label to own status tip text.
    labelHint->setText(labelHint->statusTip());
#endif // HANDHELD

    settings = initSettings(this);
    settings->remove("SettingsReset");

    cbAutosize->setChecked(settings->value("Autosize", DEF_AUTOSIZE).toBool());
    cbSaveLastUsed->setChecked(settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool());
    cbUseNativeDialogs->setChecked(settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool());
#ifndef HANDHELD
    if (QtWin::isCompositionEnabled())
        cbUseTranslucency->setChecked(settings->value("UseTranslucency", DEF_USE_TRANSLUCENCY).toBool());
#endif // HANDHELD
#ifndef HANDHELD
    cbSaveState->setChecked(settings->value("SavePos", DEF_SAVEPOS).toBool());
#endif // HANDHELD
    if (cbCheck4Updates != NULL) {
        settings->beginGroup("Check4Updates");
        cbCheck4Updates->setChecked(settings->value("Enabled", DEF_CHECK_FOR_UPDATES).toBool());
        spinUpdateCheckInterval->setValue(settings->value("Interval", DEF_UPDATE_CHECK_INTERVAL).toInt());
        settings->endGroup();
        spinUpdateCheckInterval->setEnabled(cbCheck4Updates->isChecked());
    }

    settings->beginGroup("Task");
    cbSymmetricMode->setChecked(settings->value("SymmetricMode", DEF_SYMMETRIC_MODE).toBool());
    spinFractionalAccuracy->setValue(settings->value("FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt());
    spinRandMin->setMaximum(MAX_RAND_VALUE);
    spinRandMin->setValue(settings->value("RandMin",DEF_RAND_MIN).toInt());
    spinRandMax->setMaximum(MAX_RAND_VALUE);
    spinRandMax->setValue(settings->value("RandMax",DEF_RAND_MAX).toInt());
    cbFractionalRandom->setChecked(settings->value("FractionalRandom", DEF_FRACTIONAL_RANDOM).toBool());
    settings->endGroup();

    settings->beginGroup("Output");
    cbGenerateGraph->setChecked(settings->value("GenerateGraph", DEF_GENERATE_GRAPH).toBool());

    spinGraphWidth->setEnabled(cbGenerateGraph->isChecked());
    spinGraphWidth->setValue(settings->value("GraphWidth", DEF_GRAPH_WIDTH).toDouble());

#if !defined(NOSVG) && (QT_VERSION >= QT_VERSION_CHECK(4,5,0))
    comboGraphImageFormat->addItem("svg");
#endif
    // We create whitelist of formats, supported by the most popular web browsers according to
    //  http://en.wikipedia.org/wiki/Comparison_of_web_browsers#Image_format_support
    //  + TIFF format (there are plugins to support it).
QStringList whitelist;
    whitelist << "bmp" << "jpeg" << "png" << "tiff" << "xbm";
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        if (whitelist.contains(format))
            comboGraphImageFormat->addItem(format);
    }
    comboGraphImageFormat->model()->sort(0);
    comboGraphImageFormat->setCurrentIndex(comboGraphImageFormat->findText(settings->value("GraphImageFormat", DEF_GRAPH_IMAGE_FORMAT).toString(), Qt::MatchFixedString));
    if (comboGraphImageFormat->currentIndex() < 0)
        comboGraphImageFormat->setCurrentIndex(comboGraphImageFormat->findText(DEF_GRAPH_IMAGE_FORMAT, Qt::MatchFixedString));
    labelGraphImageFormat->setEnabled(cbGenerateGraph->isChecked());
    comboGraphImageFormat->setEnabled(cbGenerateGraph->isChecked());

#ifndef QT_NO_PRINTER
    cbHQGraph->setEnabled(cbGenerateGraph->isChecked());
    cbHQGraph->setChecked(settings->value("HQGraph", DEF_HQ_GRAPH).toBool());
#endif

    labelGraphWidth->setEnabled(cbGenerateGraph->isChecked());
    cbEmbedGraphIntoHTML->setEnabled(cbGenerateGraph->isChecked());
    cbEmbedGraphIntoHTML->setChecked(settings->value("EmbedGraphIntoHTML", DEF_EMBED_GRAPH_INTO_HTML).toBool());

    cbShowMatrix->setChecked(settings->value("ShowMatrix", DEF_SHOW_MATRIX).toBool());
    cbCitiesLimit->setEnabled(cbShowMatrix->isChecked());
    cbCitiesLimit->setChecked(settings->value("UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT && cbShowMatrix->isChecked()).toBool());
    spinCitiesLimit->setEnabled(cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
    spinCitiesLimit->setValue(settings->value("ShowMatrixLimit", DEF_SHOW_MATRIX_LIMIT).toInt());
    settings->endGroup();
    spinCitiesLimit->setMaximum(settings->value("Tweaks/MaxNumCities", MAX_NUM_CITIES).toInt());
    settings->beginGroup("Output");
    cbScrollToEnd->setChecked(settings->value("ScrollToEnd", DEF_SCROLL_TO_END).toBool());

    font = qvariant_cast<QFont>(settings->value("Font", QFont(DEF_FONT_FACE, DEF_FONT_SIZE)));
    textColor = QColor(settings->value("Colors/Text", DEF_TEXT_COLOR).toString());
    selColor = QColor(settings->value("Colors/Selected", DEF_SELECTED_COLOR).toString());
    altColor = QColor(settings->value("Colors/Alternate", DEF_ALTERNATE_COLOR).toString());
    borderColor = QColor(settings->value("Colors/TableBorder", DEF_TABLE_COLOR).toString());
//    bgColor = QColor(settings->value("Colors/Background", DEF_BACKGROUND_COLOR).toString());
    settings->endGroup();

    QFont f = font;
    f.setPointSize(labelFontExample->font().pointSize());
    labelFontExample->setFont(f);
    labelFontExample->setText(font.family());

    setBgColor(boxTextColor, textColor);
    setBgColor(boxSelectedColor, selColor);
    setBgColor(boxAlternateColor, altColor);
    setBgColor(boxBorderColor, borderColor);
//    setBgColor(boxBgColor, bgColor);

#ifdef HANDHELD
    setWindowState(Qt::WindowMaximized);
#else
    adjustSize();
#endif // Q_OS_WINCE_WM
}

/*!
 * \brief Indicates whether or not the font color has been changed.
 * \return \c true if font color changed, otherwise \c false.
 */
bool SettingsDialog::colorChanged() const
{
    return _colorChanged;
}

/*!
 * \brief Indicates whether or not the font properties have been changed.
 * \return \c true if font properties changed, otherwise \c false.
 */
bool SettingsDialog::fontChanged() const
{
    return _fontChanged;
}

/*!
 * \brief Indicates whether and how the translucency setting was changed
 * \retval -1 the translucency was \em disabled.
 * \retval  0 the translucency <em>didn't change</em>.
 * \retval  1 the translucency was \em enabled.
 */
qint8 SettingsDialog::translucencyChanged() const
{
    return _translucency;
}

/* Privates **********************************************************/

void SettingsDialog::accept()
{
    if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
        if (QMessageBox::question(this, tr("Settings Reset"),
                                  tr("Do you really want to <b>reset all application settings"
                                     " to their defaults</b>?<br>"
                                     "Please, note that \"Tweaks\" section won't be reset."),
                                  QMessageBox::RestoreDefaults | QMessageBox::Cancel)
                == QMessageBox::RestoreDefaults) {
            _fontChanged = (font != QFont(DEF_FONT_FACE, DEF_FONT_SIZE));
            _colorChanged = (textColor != DEF_TEXT_COLOR);
            // Saving Tweaks section as we don't reset it
            settings->beginGroup("Tweaks");
            QHash<QString, QVariant> values;
            foreach (const QString &key, settings->childKeys()) {
                values.insert(key, settings->value(key));
            }
            settings->endGroup();
            settings->remove("");
            settings->setValue("SettingsReset", true);
            if (!values.empty()) {
                settings->beginGroup("Tweaks");
                foreach (const QString &key, values.keys()) {
                    settings->setValue(key, values.value(key));
                }
                settings->endGroup();
            }
            QDialog::accept();
            QMessageBox::information(this->parentWidget(), tr("Settings Reset"), tr("All settings where successfully reset to their defaults.\nIt is recommended to restart the application now."));
            return;
        } else
            return;
    }
    settings->setValue("Autosize", cbAutosize->isChecked());
    settings->setValue("SaveLastUsed", cbSaveLastUsed->isChecked());
    settings->setValue("UseNativeDialogs", cbUseNativeDialogs->isChecked());
#ifndef HANDHELD
    if (QtWin::isCompositionEnabled()) {
bool old = settings->value("UseTranslucency", DEF_USE_TRANSLUCENCY).toBool();
        if ((!old && cbUseTranslucency->isChecked()) || (old && !cbUseTranslucency->isChecked())) {
            _translucency = old ? -1 : 1;
        } else
            _translucency = 0;
        settings->setValue("UseTranslucency", cbUseTranslucency->isChecked());
    }
    settings->setValue("SavePos", cbSaveState->isChecked());
#endif // HANDHELD
    if (cbCheck4Updates != NULL) {
        settings->beginGroup("Check4Updates");
        settings->setValue("Enabled", cbCheck4Updates->isChecked());
        if (cbCheck4Updates->isChecked())
            settings->setValue("Interval", spinUpdateCheckInterval->value());
        settings->endGroup();
    }

    settings->beginGroup("Task");
    settings->setValue("SymmetricMode", cbSymmetricMode->isChecked());
    settings->setValue("FractionalAccuracy", spinFractionalAccuracy->value());
    settings->setValue("RandMin", spinRandMin->value());
    settings->setValue("RandMax", spinRandMax->value());
    settings->setValue("FractionalRandom", cbFractionalRandom->isChecked());
    settings->endGroup();

    settings->beginGroup("Output");
    settings->setValue("GenerateGraph", cbGenerateGraph->isChecked());
    if (cbGenerateGraph->isChecked()) {
        settings->setValue("GraphWidth", spinGraphWidth->value());
        if (cbGenerateGraph->isChecked()) {
            if (comboGraphImageFormat->currentIndex() >= 0)
                settings->setValue("GraphImageFormat", comboGraphImageFormat->currentText());
            else
                settings->setValue("GraphImageFormat", DEF_GRAPH_IMAGE_FORMAT);
        }
#ifndef QT_NO_PRINTER
        settings->setValue("HQGraph", cbHQGraph->isChecked());
#endif
        settings->setValue("EmbedGraphIntoHTML", cbEmbedGraphIntoHTML->isChecked());
    }
    settings->setValue("ShowMatrix", cbShowMatrix->isChecked());
    settings->setValue("UseShowMatrixLimit", cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
    if (cbCitiesLimit->isChecked())
        settings->setValue("ShowMatrixLimit", spinCitiesLimit->value());
    settings->setValue("ScrollToEnd", cbScrollToEnd->isChecked());
    if (_fontChanged)
        settings->setValue("Font", font);
    if (_colorChanged) {
        settings->beginGroup("Colors");
        if (textColor != DEF_TEXT_COLOR)
            settings->setValue("Text", textColor.name());
        else
            settings->remove("Text");
        if (selColor != QColor(DEF_SELECTED_COLOR))
            settings->setValue("Selected", selColor.name());
        else
            settings->remove("Selected");
        if (altColor != DEF_ALTERNATE_COLOR)
            settings->setValue("Alternate",altColor.name());
        else
            settings->remove("Alternate");
        if (borderColor != DEF_TABLE_COLOR)
            settings->setValue("TableBorder", borderColor.name());
        else
            settings->remove("TableColor");
//        if (bgColor != DEF_BACKGROUND_COLOR)
//            settings->setValue("Background", bgColor.name());
//        else
//            settings->remove("Background");
        settings->endGroup();
    }
    settings->endGroup();
    QDialog::accept();
}

void SettingsDialog::pickColor(QColor &where)
{
    QColor color = QColorDialog::getColor(where, this);
    if (color.isValid() && (where != color)) {
        where = color;
        _colorChanged = true;
    }
}

void SettingsDialog::setBgColor(QWidget *widget, const QColor &color)
{
    QPalette p = widget->palette();
    p.setColor(QPalette::Window, color);
    widget->setPalette(p);
}

void SettingsDialog::buttonTextColorClicked()
{
    pickColor(textColor);
    setBgColor(boxTextColor, textColor);
}

void SettingsDialog::buttonSelectedColorClicked()
{
    pickColor(selColor);
    setBgColor(boxSelectedColor, selColor);
}

void SettingsDialog::buttonAlternateColorClicked()
{
    pickColor(altColor);
    setBgColor(boxAlternateColor, altColor);
}

void SettingsDialog::buttonBorderColorClicked()
{
    pickColor(borderColor);
    setBgColor(boxBorderColor, borderColor);
}

//void SettingsDialog::buttonBgColorClicked()
//{
//    pickColor(bgColor);
//    setBgColor(boxBgColor, bgColor);
//}

void SettingsDialog::buttonFontClicked()
{
    QFont newFont;
#ifdef Q_OS_BLACKBERRY
    QFontDialog fd(font, this);
    fd.setWindowState(Qt::WindowMaximized);
    if ((fd.exec() != QDialog::Accepted) || (fd.selectedFont() == font))
        return;
    newFont = fd.selectedFont();
#else
    bool ok;
    newFont = QFontDialog::getFont(&ok, font, this);
    if (!ok || (font == newFont))
        return;
#endif
    font = newFont;
    QFont f = font;
    f.setPointSize(labelFontExample->font().pointSize());
    labelFontExample->setFont(f);
    labelFontExample->setText(font.family());
    _fontChanged = true;
}

#ifdef Q_OS_WINCE_WM
void SettingsDialog::desktopResized(int screen)
{
    if (screen != 0)
        return;

QRect availableGeometry = QApplication::desktop()->availableGeometry(0);
    if (currentGeometry != availableGeometry) {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        /*!
         * \hack HACK: This hack checks whether \link QDesktopWidget::availableGeometry() availableGeometry()\endlink's \c top + \c hegiht = \link QDesktopWidget::screenGeometry() screenGeometry()\endlink's \c height.
         *  If \c true, the window gets maximized. If we used \c setGeometry() in this case, the bottom of the
         *  window would end up being behind the soft buttons. Is this a bug in Qt or Windows Mobile?
         */
        if ((availableGeometry.top() + availableGeometry.height()) == QApplication::desktop()->screenGeometry().height()) {
            setWindowState(windowState() | Qt::WindowMaximized);
        } else {
            if (windowState() & Qt::WindowMaximized)
                setWindowState(windowState() ^ Qt::WindowMaximized);
            setGeometry(availableGeometry);
        }
        currentGeometry = availableGeometry;
        QApplication::restoreOverrideCursor();
    }
}

void SettingsDialog::showEvent(QShowEvent *ev)
{
    desktopResized(0);

    QWidget::showEvent(ev);
}
#endif // Q_OS_WINCE_WM

void SettingsDialog::spinRandMinValueChanged(int val) {
    spinRandMax->setMinimum(val);
}

#if !defined(QT_NO_STATUSTIP) && !defined(HANDHELD)
bool SettingsDialog::event(QEvent *ev)
{
    // Checking for StatusTip event and if tip text is not empty string
    // setting it as text of the dialog hint label. Otherwise, setting
    // dialog hint label text to own status tip text.
    if (ev->type() == QEvent::StatusTip) {
QString tip = static_cast<QStatusTipEvent *>(ev)->tip();
        if (tip.length() != 0)
            labelHint->setText(tip);
        else
            labelHint->setText(labelHint->statusTip());
        return true;
    } else
        return QDialog::event(ev);
}
#endif // HANDHELD
