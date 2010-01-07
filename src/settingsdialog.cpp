/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
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
	: QDialog(parent), newFont(false), newColor(false)
{
	setupUi(this);
	// Laying out elements
	layoutCitiesLimit = new QHBoxLayout();
	layoutCitiesLimit->setMargin(0);
	layoutCitiesLimit->setSpacing(0);
	layoutCitiesLimit->addSpacing(10);
	layoutCitiesLimit->addWidget(cbCitiesLimit);
	layoutCitiesLimit->addWidget(spinCitiesLimit);
	layoutCitiesLimit->addStretch();

	buttonBox->button(QDialogButtonBox::Save)->setIcon(QIcon(":/images/icons/button_ok.png"));
	buttonBox->button(QDialogButtonBox::Save)->setStatusTip(trUtf8("Save new preferences"));
	buttonBox->button(QDialogButtonBox::Save)->setCursor(QCursor(Qt::PointingHandCursor));
	buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon(":/images/icons/button_cancel.png"));
	buttonBox->button(QDialogButtonBox::Cancel)->setStatusTip(trUtf8("Close without saving preferences"));
	buttonBox->button(QDialogButtonBox::Cancel)->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_WINCE
	// Layout helper elements
QVBoxLayout *vbox1, *vbox2;
QHBoxLayout *hbox1, *hbox2;

	labelRandMin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	labelRandMax->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

	// Top part (with white bg)
	vbox2 = new QVBoxLayout(bgWhite);
	vbox2->addWidget(groupRandomSettings);
	vbox2->addWidget(groupOutputSettings);
	vbox2->addStretch();
	vbox2->addWidget(cbFractionalRandom);
	vbox2->addWidget(cbShowMatrix);
	vbox2->addLayout(layoutCitiesLimit);
	vbox2->addWidget(cbScrollToEnd);
	vbox2->addWidget(cbAutosize);

	// Output settings group
	hbox1 = new QHBoxLayout(groupOutputSettings);
	hbox1->addWidget(buttonFont);
	hbox1->addWidget(buttonColor);

	// Bottom part (with grey bg)
	hbox2 = new QHBoxLayout(bgGrey);
	hbox2->setMargin(6);
	hbox2->setSpacing(6);
	hbox2->addWidget(buttonHelp);
	hbox2->addStretch();
	hbox2->addWidget(buttonBox);

	// Central layout
	vbox1 = new QVBoxLayout(this);
	vbox1->setMargin(0);
	vbox1->setSpacing(0);
	vbox1->addWidget(bgWhite);
	vbox1->addWidget(lineHorizontal);
	vbox1->addWidget(bgGrey);
#else
	// Layout helper elements
QVBoxLayout *vbox1, *vbox2, *vbox3;
QHBoxLayout *hbox1, *hbox2, *hbox3;

	cbSaveState = new QCheckBox(bgWhite);
	cbSaveState->setObjectName("cbSaveState");
#ifndef QT_NO_STATUSTIP
	cbSaveState->setStatusTip(trUtf8("Restore main window state and position on application restart"));
#endif // QT_NO_STATUSTIP
	cbSaveState->setText(trUtf8("Save main window state and position"));
	cbSaveState->setCursor(QCursor(Qt::PointingHandCursor));

	imgIcon = new QLabel(this);
	imgIcon->setObjectName("imgIcon");
	imgIcon->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	imgIcon->setFrameShape(QFrame::Panel);
	imgIcon->setLineWidth(0);
	imgIcon->setPixmap(QPixmap(":/images/icons/preferences_system.png"));
	imgIcon->setStyleSheet("background-color: #0080C0;");
	imgIcon->setAlignment(Qt::AlignCenter);
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
	labelHint->setStatusTip(trUtf8("Hover mouse pointer over dialog elements to get additional help"));
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

	// Output settings group
	vbox3 = new QVBoxLayout(groupOutputSettings);
	vbox3->addWidget(buttonFont);
	vbox3->addWidget(buttonColor);

	// Random and Output settings groups
	hbox2 = new QHBoxLayout();
	hbox2->addWidget(groupRandomSettings);
	hbox2->addWidget(groupOutputSettings);
	hbox2->addStretch();

	// Top right part (with white bg)
	vbox2 = new QVBoxLayout(bgWhite);
	vbox2->addStretch();
	vbox2->addLayout(hbox2);
	vbox2->addWidget(cbFractionalRandom);
	vbox2->addWidget(cbShowMatrix);
	vbox2->addLayout(layoutCitiesLimit);
	vbox2->addWidget(cbScrollToEnd);
	vbox2->addWidget(cbAutosize);
	vbox2->addWidget(cbSaveState);
	vbox2->addStretch();

	// Bottom part (with grey bg)
	hbox3 = new QHBoxLayout(bgGrey);
	hbox3->setMargin(6);
	hbox3->setSpacing(6);
	hbox3->addWidget(buttonHelp);
	hbox3->addWidget(labelHint);
	hbox3->addWidget(buttonBox);

	// Central layout
	vbox1 = new QVBoxLayout(this);
	vbox1->setMargin(0);
	vbox1->setSpacing(0);
	vbox1->addLayout(hbox1);
	vbox1->addWidget(lineHorizontal);
	vbox1->addWidget(bgGrey);
#endif // Q_OS_WINCE
	connect(spinRandMin,SIGNAL(valueChanged(int)),this,SLOT(spinRandMinValueChanged(int)));
	connect(buttonFont,SIGNAL(clicked()),this,SLOT(buttonFontClicked()));
	connect(buttonColor,SIGNAL(clicked()),this,SLOT(buttonColorClicked()));
//	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
	setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
#ifndef Q_OS_WINCE
	// Setting initial text of dialog hint label to own status tip text.
	labelHint->setText(labelHint->statusTip());
#endif // Q_OS_WINCE

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "TSPSG", "tspsg", this);

	spinRandMin->setMaximum(MAX_RAND_VALUE);
	spinRandMin->setValue(settings->value("MinCost",DEF_RAND_MIN).toInt());
	spinRandMax->setMaximum(MAX_RAND_VALUE);
	spinRandMax->setValue(settings->value("MaxCost",DEF_RAND_MAX).toInt());
	cbFractionalRandom->setChecked(settings->value("FractionalRandom", DEF_FRACTIONAL_RANDOM).toBool());
	cbAutosize->setChecked(settings->value("Autosize",true).toBool());
#ifndef Q_OS_WINCE
	cbSaveState->setChecked(settings->value("SavePos",false).toBool());
#endif // Q_OS_WINCE

	settings->beginGroup("Output");
	cbShowMatrix->setChecked(settings->value("ShowMatrix", DEF_SHOW_MATRIX).toBool());
	cbCitiesLimit->setEnabled(cbShowMatrix->isChecked());
	cbCitiesLimit->setChecked(settings->value("UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT && cbShowMatrix->isChecked()).toBool());
	spinCitiesLimit->setEnabled(cbShowMatrix->isChecked());
	spinCitiesLimit->setValue(settings->value("ShowMatrixCitiesLimit", DEF_SHOW_MATRIX_CITY_LIMIT).toInt());
	spinCitiesLimit->setMaximum(MAX_NUM_CITIES);
	cbScrollToEnd->setChecked(settings->value("ScrollToEnd", DEF_SCROLL_TO_END).toBool());

	font = settings->value("Font",QFont(DEF_FONT_FAMILY,DEF_FONT_SIZE)).value<QFont>();
	color = settings->value("Color",DEF_FONT_COLOR).value<QColor>();
	settings->endGroup();

	adjustSize();
}

/*!
 * \brief Indicates whether or not the font color has been changed.
 * \return \c true if font color changed, otherwise \c false.
 */
bool SettingsDialog::colorChanged() const
{
	return newColor;
}

/*!
 * \brief Indicates whether or not the font properties have been changed.
 * \return \c true if font properties changed, otherwise \c false.
 */
bool SettingsDialog::fontChanged() const
{
	return newFont;
}

/* Privates **********************************************************/

void SettingsDialog::accept()
{
#ifndef Q_OS_WINCE
	settings->setValue("SavePos", cbSaveState->isChecked());
#endif // Q_OS_WINCE
	settings->setValue("Autosize", cbAutosize->isChecked());
	settings->setValue("MinCost", spinRandMin->value());
	settings->setValue("MaxCost", spinRandMax->value());
	settings->setValue("FractionalRandom", cbFractionalRandom->isChecked());

	settings->beginGroup("Output");
	settings->setValue("ShowMatrix", cbShowMatrix->isChecked());
	settings->setValue("UseShowMatrixLimit", cbShowMatrix->isChecked() && cbCitiesLimit->isChecked());
	if (cbCitiesLimit->isChecked())
		settings->setValue("ShowMatrixCitiesLimit", spinCitiesLimit->value());
	settings->setValue("ScrollToEnd", cbScrollToEnd->isChecked());
	if (newFont)
		settings->setValue("Font", font);
	if (newColor)
		settings->setValue("Color", color);
	settings->endGroup();
	QDialog::accept();
}

void SettingsDialog::buttonColorClicked()
{
QColor color = QColorDialog::getColor(this->color,this);
	if (color.isValid() && (this->color != color)) {
		this->color = color;
		newColor = true;
	}
}

void SettingsDialog::buttonFontClicked()
{
bool ok;
QFont font = QFontDialog::getFont(&ok,this->font,this);
	if (ok && (this->font != font)) {
		this->font = font;
		newFont = true;
	}
}

void SettingsDialog::spinRandMinValueChanged(int val) {
	spinRandMax->setMinimum(val);
}

#ifndef Q_OS_WINCE
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
#endif // Q_OS_WINCE
