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

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent), newFont(false), newColor(false)
{
	setupUi(this);
	// Laying out elements
#ifdef Q_OS_WINCE
	// Layout helper elements
QVBoxLayout *vbox1, *vbox2;
QHBoxLayout *hbox1, *hbox2;
QSpacerItem *spacer;

	labelRandMin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	labelRandMax->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

	// Top part (with white bg)
	vbox2 = new QVBoxLayout(bgWhite);
	vbox2->addWidget(groupRandomSettings);
	vbox2->addWidget(groupOutputSettings);
	spacer = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding);
	vbox2->addItem(spacer);

	// Output settings group
	hbox1 = new QHBoxLayout(groupOutputSettings);
	hbox1->addWidget(buttonFont);
	hbox1->addWidget(buttonColor);

	// Bottom part (with grey bg)
	hbox2 = new QHBoxLayout(bgGrey);
	hbox2->setMargin(6);
	hbox2->setSpacing(6);
	hbox2->addWidget(buttonHelp);
	spacer = new QSpacerItem(0,0,QSizePolicy::Expanding);
	hbox2->addItem(spacer);
	hbox2->addWidget(buttonOK);
	hbox2->addWidget(buttonCancel);

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
QSpacerItem *spacer;

	cbSaveState = new QCheckBox(bgWhite);
	cbSaveState->setObjectName("cbSaveState");
#ifndef QT_NO_STATUSTIP
	cbSaveState->setStatusTip(trUtf8("Restore main window state and position on application restart"));
#endif // QT_NO_STATUSTIP
	cbSaveState->setText(trUtf8("Save main window state and position"));
	cbSaveState->setCursor(QCursor(Qt::PointingHandCursor));

	imgIcon = new QLabel(this);
	imgIcon->setObjectName("imgIcon");
	imgIcon->setFrameShape(QFrame::StyledPanel);
	imgIcon->setLineWidth(0);
	imgIcon->setPixmap(QPixmap(QString::fromUtf8(":/images/icons/preferences_system.png")));
	imgIcon->setStyleSheet("background-color: #0080C0;");
	imgIcon->setAlignment(Qt::AlignCenter);

	labelHint = new QLabel(bgGrey);
	labelHint->setObjectName("labelHint");
	labelHint->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	labelHint->setMinimumSize(QSize(175,28));
	labelHint->setMaximumSize(QSize(0xFFFFFF,28));
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
	spacer = new QSpacerItem(0,0,QSizePolicy::Expanding);
	hbox2->addItem(spacer);

	// Top right part (with white bg)
	vbox2 = new QVBoxLayout(bgWhite);
	spacer = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding);
	vbox2->addItem(spacer);
	vbox2->addLayout(hbox2);
	vbox2->addWidget(cbSaveState);
	spacer = new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding);
	vbox2->addItem(spacer);

	// Bottom part (with grey bg)
	hbox3 = new QHBoxLayout(bgGrey);
	hbox3->setMargin(6);
	hbox3->setSpacing(6);
	hbox3->addWidget(buttonHelp);
	hbox3->addWidget(labelHint);
	hbox3->addWidget(buttonOK);
	hbox3->addWidget(buttonCancel);

	// Central layout
	vbox1 = new QVBoxLayout(this);
	vbox1->setMargin(0);
	vbox1->setSpacing(0);
	vbox1->addLayout(hbox1);
	vbox1->addWidget(lineHorizontal);
	vbox1->addWidget(bgGrey);
#endif // Q_OS_WINCE
	connect(buttonOK,SIGNAL(clicked()),this,SLOT(accept()));
	connect(buttonCancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(spinRandMin,SIGNAL(valueChanged(int)),this,SLOT(spinRandMinValueChanged(int)));
	connect(buttonFont,SIGNAL(clicked()),this,SLOT(buttonFontClicked()));
	connect(buttonColor,SIGNAL(clicked()),this,SLOT(buttonColorClicked()));
//	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
	setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
#ifndef Q_OS_WINCE
	// Setting initial text of dialog hint label to own status tip
	// text.
	labelHint->setText(labelHint->statusTip());
#endif // Q_OS_WINCE
	settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"TSPSG","tspsg");
	spinRandMin->setValue(settings->value("MinCost",DEF_RAND_MIN).toInt());
	spinRandMax->setValue(settings->value("MaxCost",DEF_RAND_MAX).toInt());
#ifndef Q_OS_WINCE
	cbSaveState->setChecked(settings->value("SavePos",false).toBool());
#endif // Q_OS_WINCE
	settings->beginGroup("Output");
	font = settings->value("Font",QFont(DEF_FONT_FAMILY,DEF_FONT_SIZE)).value<QFont>();
	color = settings->value("Color",DEF_FONT_COLOR).value<QColor>();
	settings->endGroup();
}

void SettingsDialog::accept()
{
#ifndef Q_OS_WINCE
	settings->setValue("SavePos",cbSaveState->isChecked());
#endif // Q_OS_WINCE
	settings->setValue("MinCost",spinRandMin->value());
	settings->setValue("MaxCost",spinRandMax->value());
	settings->beginGroup("Output");
	if (newFont)
		settings->setValue("Font",font);
	if (newColor)
		settings->setValue("Color",color);
	settings->endGroup();
	QDialog::accept();
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

void SettingsDialog::buttonColorClicked()
{
QColor color = QColorDialog::getColor(this->color,this);
	if (color.isValid() && (this->color != color)) {
		this->color = color;
		newColor = true;
	}
}

bool SettingsDialog::colorChanged() const
{
	return newColor;
}

bool SettingsDialog::fontChanged() const
{
	return newFont;
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
