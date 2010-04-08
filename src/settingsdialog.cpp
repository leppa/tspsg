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

	buttonBox->button(QDialogButtonBox::Save)->setIcon(QIcon(":/images/icons/button_ok.png"));
	buttonBox->button(QDialogButtonBox::Save)->setStatusTip(tr("Save new preferences"));
	buttonBox->button(QDialogButtonBox::Save)->setCursor(QCursor(Qt::PointingHandCursor));
	buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/icons/button_cancel.png"));
	buttonBox->button(QDialogButtonBox::Cancel)->setStatusTip(tr("Close without saving preferences"));
	buttonBox->button(QDialogButtonBox::Cancel)->setCursor(QCursor(Qt::PointingHandCursor));

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
QVBoxLayout *vbox1, *vbox2;
QHBoxLayout *hbox1;

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
	imgIcon->setPixmap(QPixmap(":/images/icons/preferences_system.png"));
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
	hbox1 = new QHBoxLayout();
	hbox1->addWidget(imgIcon);
	hbox1->addWidget(lineVertical);
	hbox1->addWidget(bgWhite);

	vbox1 = static_cast<QVBoxLayout *>(tabGeneral->layout());
	vbox1->insertWidget(vbox1->indexOf(cbUseNativeDialogs) + 1, cbSaveState);
#ifdef Q_OS_WIN32
	if (QtWin::isCompositionEnabled())
		vbox1->insertWidget(vbox1->indexOf(cbUseNativeDialogs) + 1, cbUseTranslucency);
#endif // Q_OS_WIN32

	// Inserting label for hints to the bottom part (with grey bg)
	buttons->insertWidget(buttons->indexOf(buttonHelp) + 1, labelHint, 1);

	// Central layout
	vbox2 = new QVBoxLayout(this);
	vbox2->setMargin(0);
	vbox2->setSpacing(0);
	vbox2->addLayout(hbox1);
	vbox2->addWidget(bgGrey);
	setLayout(vbox2);
#endif // HANDHELD

#ifdef Q_OS_WINCE_WM
	// We need to react to SIP show/hide and resize the window appropriately
	connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), SLOT(desktopResized(int)));
#endif // Q_OS_WINCE_WM
	connect(spinRandMin,SIGNAL(valueChanged(int)),this,SLOT(spinRandMinValueChanged(int)));
	connect(buttonFont,SIGNAL(clicked()),this,SLOT(buttonFontClicked()));
	connect(buttonColor,SIGNAL(clicked()),this,SLOT(buttonColorClicked()));
	setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
#ifndef HANDHELD
	// Setting initial text of dialog hint label to own status tip text.
	labelHint->setText(labelHint->statusTip());
#endif // HANDHELD

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "TSPSG", "tspsg", this);
	settings->remove("SettingsReset");

	cbAutosize->setChecked(settings->value("Autosize", DEF_AUTOSIZE).toBool());
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
	cbShowMatrix->setChecked(settings->value("ShowMatrix", DEF_SHOW_MATRIX).toBool());
	cbCitiesLimit->setEnabled(cbShowMatrix->isChecked());
	cbCitiesLimit->setChecked(settings->value("UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT && cbShowMatrix->isChecked()).toBool());
	spinCitiesLimit->setEnabled(cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
	spinCitiesLimit->setValue(settings->value("ShowMatrixLimit", DEF_SHOW_MATRIX_LIMIT).toInt());
	spinCitiesLimit->setMaximum(MAX_NUM_CITIES);
	cbScrollToEnd->setChecked(settings->value("ScrollToEnd", DEF_SCROLL_TO_END).toBool());

	font = settings->value("Font",QFont(DEF_FONT_FAMILY,DEF_FONT_SIZE)).value<QFont>();
	color = settings->value("Color",DEF_FONT_COLOR).value<QColor>();
	settings->endGroup();

#ifndef Q_OS_WINCE_WM
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
			_newFont = (font != QFont(DEF_FONT_FAMILY, DEF_FONT_SIZE));
			_newColor = (color != DEF_FONT_COLOR);
			settings->remove("");
			settings->setValue("SettingsReset", true);
			QDialog::accept();
			QMessageBox::information(this, tr("Settings Reset"), tr("All settings where successfully reset to their defaults.\nIt is recommended to restart the application now."));
			return;
		} else
			return;
	}
#ifndef HANDHELD
	settings->setValue("SavePos", cbSaveState->isChecked());
#endif // HANDHELD
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
	settings->setValue("UseNativeDialogs", cbUseNativeDialogs->isChecked());

	settings->beginGroup("Task");
	settings->setValue("SymmetricMode", cbSymmetricMode->isChecked());
	settings->setValue("FractionalAccuracy", spinFractionalAccuracy->value());
	settings->setValue("RandMin", spinRandMin->value());
	settings->setValue("RandMax", spinRandMax->value());
	settings->setValue("FractionalRandom", cbFractionalRandom->isChecked());
	settings->endGroup();

	settings->beginGroup("Output");
	settings->setValue("ShowMatrix", cbShowMatrix->isChecked());
	settings->setValue("UseShowMatrixLimit", cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
	if (cbCitiesLimit->isChecked())
		settings->setValue("ShowMatrixLimit", spinCitiesLimit->value());
	settings->setValue("ScrollToEnd", cbScrollToEnd->isChecked());
	if (_newFont)
		settings->setValue("Font", font);
	if (_newColor)
		settings->setValue("Color", color);
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
