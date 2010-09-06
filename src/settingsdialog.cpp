/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

/*!
 * \brief Class constructor.
 * \param parent Settings Dialog parent widget.
 *
 * Initializes Settings Dialog and creates its layout based on target OS.
 */
SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent), _newFont(false), _newColor(false), _translucency(0)
{
	setupUi(this);

	setWindowIcon(GET_ICON("preferences-system"));

	buttonBox->button(QDialogButtonBox::Ok)->setIcon(GET_ICON("dialog-ok"));
	buttonBox->button(QDialogButtonBox::Ok)->setStatusTip(tr("Save new preferences"));
	buttonBox->button(QDialogButtonBox::Ok)->setCursor(QCursor(Qt::PointingHandCursor));
	buttonBox->button(QDialogButtonBox::Cancel)->setIcon(GET_ICON("dialog-cancel"));
	buttonBox->button(QDialogButtonBox::Cancel)->setStatusTip(tr("Close without saving preferences"));
	buttonBox->button(QDialogButtonBox::Cancel)->setCursor(QCursor(Qt::PointingHandCursor));

	buttonColor->setIcon(GET_ICON("format-text-color"));
	buttonFont->setIcon(GET_ICON("preferences-desktop-font"));
	buttonHelp->setIcon(GET_ICON("help-hint"));

QPalette p = bgWhite->palette();
	p.setColor(QPalette::Window, p.color(QPalette::Base));
	bgWhite->setPalette(p);

	p = lineHorizontal->palette();
	p.setColor(QPalette::Window, p.color(QPalette::Text));
	lineHorizontal->setPalette(p);

#ifndef QT_NO_PRINTER
	cbHQGraph = new QCheckBox(bgWhite);
	cbHQGraph->setObjectName("cbHQGraph");
#ifndef QT_NO_STATUSTIP
	cbHQGraph->setStatusTip(tr("Higher quality graph looks much better when printing but uglier on the screen"));
#endif // QT_NO_STATUSTIP
	cbHQGraph->setText(tr("Draw solution graph in higher quality"));
	cbHQGraph->setCursor(QCursor(Qt::PointingHandCursor));

QBoxLayout *box = static_cast<QBoxLayout *>(tabOutput->layout());
QHBoxLayout *hbox1 = new QHBoxLayout();
	hbox1->addSpacing(10);
	hbox1->addWidget(cbHQGraph);
	box->insertLayout(box->indexOf(cbShowGraph) + 1, hbox1);
	connect(cbShowGraph, SIGNAL(toggled(bool)), cbHQGraph, SLOT(setEnabled(bool)));
#endif

#ifdef HANDHELD
QVBoxLayout *vbox1; // Layout helper

#ifdef Q_OS_WINCE_WM
	// On screens with small height when SIP is shown and the window is resized
	// there is not enought space for all elements.
	// So we show the scrollbars to be able to access them.
QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(bgWhite);
#else
	buttons->insertStretch(buttons->indexOf(buttonHelp) + 1);
#endif // Q_OS_WINCE_WM

	bgWhite->layout()->setMargin(0);

	// Central layout
	vbox1 = new QVBoxLayout(this);
	vbox1->setMargin(0);
	vbox1->setSpacing(0);
#ifdef Q_OS_WINCE_WM
	vbox1->addWidget(scrollArea);
#else
	vbox1->addWidget(bgWhite);
#endif // Q_OS_WINCE_WM
	vbox1->addWidget(bgGrey);
	setLayout(vbox1);
#else // HANDHELD
	// Layout helper elements
QVBoxLayout *vbox;
QHBoxLayout *hbox;

#ifdef Q_OS_WIN32
	if (QtWin::isCompositionEnabled()) {
		cbUseTranslucency = new QCheckBox(bgWhite);
		cbUseTranslucency->setObjectName("cbUseTranslucency");
#ifndef QT_NO_STATUSTIP
		cbUseTranslucency->setStatusTip(tr("Use translucent effect on the Main Window under Windows Vista and 7"));
#endif // QT_NO_STATUSTIP
		cbUseTranslucency->setText(tr("Use translucency effects"));
		cbUseTranslucency->setCursor(QCursor(Qt::PointingHandCursor));
	}
#endif // Q_OS_WIN32

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
//	labelHint->setMinimumSize(QSize(190,28));
	labelHint->setMinimumSize(QSize(0,28));
	labelHint->setMaximumSize(QSize(QWIDGETSIZE_MAX,28));
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

#ifdef QT_NO_PRINTER
QBoxLayout *box;
#endif
	box = static_cast<QBoxLayout *>(tabGeneral->layout());
	box->insertWidget(box->indexOf(cbUseNativeDialogs) + 1, cbSaveState);
#ifdef Q_OS_WIN32
	if (QtWin::isCompositionEnabled())
		box->insertWidget(box->indexOf(cbUseNativeDialogs) + 1, cbUseTranslucency);
#endif // Q_OS_WIN32

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
	connect(buttonColor, SIGNAL(clicked()), SLOT(buttonColorClicked()));
	setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
#ifndef HANDHELD
	// Setting initial text of dialog hint label to own status tip text.
	labelHint->setText(labelHint->statusTip());
#endif // HANDHELD

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "TSPSG", "tspsg", this);
	settings->remove("SettingsReset");

	cbAutosize->setChecked(settings->value("Autosize", DEF_AUTOSIZE).toBool());
	cbSaveLastUsed->setChecked(settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool());
	cbUseNativeDialogs->setChecked(settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool());
#ifdef Q_OS_WIN32
	if (QtWin::isCompositionEnabled())
		cbUseTranslucency->setChecked(settings->value("UseTranslucency", DEF_USE_TRANSLUCENCY).toBool());
#endif // Q_OS_WIN32
#ifndef HANDHELD
	cbSaveState->setChecked(settings->value("SavePos", DEF_SAVEPOS).toBool());
#endif // HANDHELD

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
	cbShowGraph->setChecked(settings->value("ShowGraph", DEF_SHOW_GRAPH).toBool());

#ifndef QT_NO_PRINTER
	cbHQGraph->setEnabled(cbShowGraph->isChecked());
	cbHQGraph->setChecked(settings->value("HQGraph", DEF_HQ_GRAPH && cbShowGraph->isChecked()).toBool());
#endif

#if !defined(NOSVG) && (QT_VERSION >= 0x040500)
	comboGraphImageFormat->addItem("svg");
#endif // NOSVG && QT_VERSION >= 0x040500
// We create a whitelist of formats, supported by the most popular web browsers according to
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
	labelGraphImageFormat->setEnabled(cbShowGraph->isChecked());
	comboGraphImageFormat->setEnabled(cbShowGraph->isChecked());

	cbShowMatrix->setChecked(settings->value("ShowMatrix", DEF_SHOW_MATRIX).toBool());
	cbCitiesLimit->setEnabled(cbShowMatrix->isChecked());
	cbCitiesLimit->setChecked(settings->value("UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT && cbShowMatrix->isChecked()).toBool());
	spinCitiesLimit->setEnabled(cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
	spinCitiesLimit->setValue(settings->value("ShowMatrixLimit", DEF_SHOW_MATRIX_LIMIT).toInt());
	spinCitiesLimit->setMaximum(MAX_NUM_CITIES);
	cbScrollToEnd->setChecked(settings->value("ScrollToEnd", DEF_SCROLL_TO_END).toBool());

	font = settings->value("Font", QFont(DEF_FONT_FACE, DEF_FONT_SIZE)).value<QFont>();
	color = settings->value("Colors/Text", DEF_TEXT_COLOR).value<QColor>();
	settings->endGroup();

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
	return _newColor;
}

/*!
 * \brief Indicates whether or not the font properties have been changed.
 * \return \c true if font properties changed, otherwise \c false.
 */
bool SettingsDialog::fontChanged() const
{
	return _newFont;
}

/*!
 * \brief Indicates whether and how the translucency setting was changed
 * \retval -1 the translucency was \em disabled.
 * \retval  0 the translucency was <em>not changed</em>.
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
		if (QMessageBox::question(this, tr("Settings Reset"), tr("Do you really want to <b>reset all application settings to their defaults</b>?"), QMessageBox::RestoreDefaults | QMessageBox::Cancel) == QMessageBox::RestoreDefaults) {
			_newFont = (font != QFont(DEF_FONT_FACE, DEF_FONT_SIZE));
			_newColor = (color != DEF_TEXT_COLOR);
			settings->remove("");
			settings->setValue("SettingsReset", true);
			QDialog::accept();
			QMessageBox::information(this->parentWidget(), tr("Settings Reset"), tr("All settings where successfully reset to their defaults.\nIt is recommended to restart the application now."));
			return;
		} else
			return;
	}
	settings->setValue("Autosize", cbAutosize->isChecked());
	settings->setValue("SaveLastUsed", cbSaveLastUsed->isChecked());
	settings->setValue("UseNativeDialogs", cbUseNativeDialogs->isChecked());
#ifdef Q_OS_WIN32
	if (QtWin::isCompositionEnabled()) {
bool old = settings->value("UseTranslucency", DEF_USE_TRANSLUCENCY).toBool();
		if ((!old && cbUseTranslucency->isChecked()) || (old && !cbUseTranslucency->isChecked())) {
			_translucency = old ? -1 : 1;
		} else
			_translucency = 0;
		settings->setValue("UseTranslucency", cbUseTranslucency->isChecked());
	}
#endif // Q_OS_WIN32
#ifndef HANDHELD
	settings->setValue("SavePos", cbSaveState->isChecked());
#endif // HANDHELD

	settings->beginGroup("Task");
	settings->setValue("SymmetricMode", cbSymmetricMode->isChecked());
	settings->setValue("FractionalAccuracy", spinFractionalAccuracy->value());
	settings->setValue("RandMin", spinRandMin->value());
	settings->setValue("RandMax", spinRandMax->value());
	settings->setValue("FractionalRandom", cbFractionalRandom->isChecked());
	settings->endGroup();

	settings->beginGroup("Output");
	settings->setValue("ShowGraph", cbShowGraph->isChecked());
#ifndef QT_NO_PRINTER
	settings->setValue("HQGraph", cbShowGraph->isChecked() && cbHQGraph->isChecked());
#endif
	if (cbShowGraph->isChecked()) {
		if (comboGraphImageFormat->currentIndex() >= 0)
			settings->setValue("GraphImageFormat", comboGraphImageFormat->currentText());
		else
			settings->setValue("GraphImageFormat", DEF_GRAPH_IMAGE_FORMAT);
	}
	settings->setValue("ShowMatrix", cbShowMatrix->isChecked());
	settings->setValue("UseShowMatrixLimit", cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
	if (cbCitiesLimit->isChecked())
		settings->setValue("ShowMatrixLimit", spinCitiesLimit->value());
	settings->setValue("ScrollToEnd", cbScrollToEnd->isChecked());
	if (_newFont)
		settings->setValue("Font", font);
	if (_newColor)
		settings->setValue("Colors/Text", color);
	settings->endGroup();
	QDialog::accept();
}

void SettingsDialog::buttonColorClicked()
{
QColor color = QColorDialog::getColor(this->color,this);
	if (color.isValid() && (this->color != color)) {
		this->color = color;
		_newColor = true;
	}
}

void SettingsDialog::buttonFontClicked()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,this->font,this);
	if (ok && (this->font != font)) {
		this->font = font;
		_newFont = true;
	}
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

#ifndef HANDHELD
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
