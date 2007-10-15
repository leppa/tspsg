/*
 *  TSPSG - TSP Solver and Generator
 *  Copyright (C) 2007 Lёppa <lacontacts[at]gmail[dot]com>
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

#include <QMessageBox>
#include <QStatusTipEvent>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	connect(buttonOK,SIGNAL(clicked()),this,SLOT(accept()));
	connect(buttonCancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(spinRandMin,SIGNAL(valueChanged(int)),this,SLOT(spinRandMinValueChanged(int)));
//	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
	setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
	layout()->setSizeConstraint(layout()->SetFixedSize);
	// Setting initial text of dialog hint label to own status tip text
	labelHint->setText(labelHint->statusTip());
}

bool SettingsDialog::event(QEvent *ev)
{
	// Checking for StatusTip event and if tip text is not empty string
	// setting it as text of the dialog hint label. Otherwise, setting
	// dialog hint label text to own status tip text
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

