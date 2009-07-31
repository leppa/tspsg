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
