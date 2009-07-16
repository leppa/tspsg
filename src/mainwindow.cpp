/*
 *  TSPSG - TSP Solver and Generator
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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"TSPSG","tspsg");
	loadLanguage();
	setupUi(this);
#ifndef Q_OS_WINCE
	printer = new QPrinter();
#endif // Q_OS_WINCE
	groupSettingsLanguageList = new QActionGroup(this);
	actionSettingsLanguageEnglish->setData("en");
	actionSettingsLanguageEnglish->setActionGroup(groupSettingsLanguageList);
	loadLangList();
	spinCities->setValue(settings->value("NumCities",5).toInt());
	actionSettingsLanguageAutodetect->setChecked(settings->value("Language","").toString().isEmpty());
	connect(actionFileNew,SIGNAL(triggered()),this,SLOT(actionFileNewTriggered()));
	connect(actionFileOpen,SIGNAL(triggered()),this,SLOT(actionFileOpenTriggered()));
	connect(actionFileSaveTask,SIGNAL(triggered()),this,SLOT(actionFileSaveTaskTriggered()));
	connect(actionSettingsPreferences,SIGNAL(triggered()),this,SLOT(actionSettingsPreferencesTriggered()));
	connect(actionSettingsLanguageAutodetect,SIGNAL(triggered(bool)),this,SLOT(actionSettingsLanguageAutodetectTriggered(bool)));
	connect(groupSettingsLanguageList,SIGNAL(triggered(QAction *)),this,SLOT(groupSettingsLanguageListTriggered(QAction *)));
	connect(actionHelpAboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(actionHelpAbout,SIGNAL(triggered()),this,SLOT(actionHelpAboutTriggered()));
#ifndef Q_OS_WINCE
	connect(actionFilePrintSetup,SIGNAL(triggered()),this,SLOT(actionFilePrintSetupTriggered()));
#endif // Q_OS_WINCE
	connect(buttonSolve,SIGNAL(clicked()),this,SLOT(buttonSolveClicked()));
	connect(buttonRandom,SIGNAL(clicked()),this,SLOT(buttonRandomClicked()));
	connect(spinCities,SIGNAL(valueChanged(int)),this,SLOT(spinCitiesValueChanged(int)));
QRect rect = geometry();
#ifdef Q_OS_WINCE
	// HACK: Fix for all tabWidget elements becoming "unclickable" if making it central widget.
	rect.setSize(QApplication::desktop()->availableGeometry().size());
	rect.setHeight(rect.height() - (QApplication::desktop()->screenGeometry().height() - QApplication::desktop()->availableGeometry().height()));
	tabWidget->resize(rect.width(),rect.height() - toolBar->size().height());
#else
	if (settings->value("SavePos",false).toBool()) {
		// Loading of saved window state
		settings->beginGroup("MainWindow");
		resize(settings->value("Size",size()).toSize());
		move(settings->value("Position",pos()).toPoint());
		if (settings->value("Maximized",false).toBool())
			setWindowState(windowState() | Qt::WindowMaximized);
		settings->endGroup();
	} else {
		// Centering main window
		rect.moveCenter(QApplication::desktop()->availableGeometry(this).center());
		setGeometry(rect);
	}
#endif // Q_OS_WINCE
	qsrand(QDateTime().currentDateTime().toTime_t());
	tspmodel = new CTSPModel();
	tspmodel->setNumCities(spinCities->value());
	taskView->setModel(tspmodel);
	connect(tspmodel,SIGNAL(numCitiesChanged(int)),this,SLOT(numCitiesChanged(int)));
	connect(tspmodel,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),this,SLOT(dataChanged()));
	connect(tspmodel,SIGNAL(layoutChanged()),this,SLOT(dataChanged()));
#ifdef Q_OS_WINCE
	taskView->resizeColumnsToContents();
	taskView->resizeRowsToContents();
#endif // Q_OS_WINCE
}

bool MainWindow::loadLanguage(QString lang)
{
// i18n
bool ad = false;
	if (lang.isEmpty()) {
		ad = settings->value("Language","").toString().isEmpty();
		lang = settings->value("Language",QLocale::system().name()).toString();
	}
static QTranslator *qtTranslator;
	if (qtTranslator) {
		qApp->removeTranslator(qtTranslator);
		delete qtTranslator;
		qtTranslator = NULL;
	}
	qtTranslator = new QTranslator();
static QTranslator *translator;
	if (translator) {
		qApp->removeTranslator(translator);
		delete translator;
	}
	translator = new QTranslator();
	if (lang.compare("en") && !lang.startsWith("en_")) {
		// Trying to load system Qt library translation...
		if (qtTranslator->load("qt_" + lang,QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
			qApp->installTranslator(qtTranslator);
		else
			// No luck. Let's try to load bundled one.
			if (qtTranslator->load("qt_" + lang,"i18n"))
				qApp->installTranslator(qtTranslator);
			else {
				delete qtTranslator;
				qtTranslator = NULL;
			}
		// Now let's load application translation.
		if (translator->load(lang,"i18n"))
			qApp->installTranslator(translator);
		else {
			if (!ad)
				QMessageBox(QMessageBox::Warning,trUtf8("Language change"),trUtf8("Unable to load translation language."),QMessageBox::Ok,this).exec();
			delete translator;
			translator = NULL;
			return false;
		}
	}
	return true;
}

void MainWindow::spinCitiesValueChanged(int n)
{
#ifdef Q_OS_WINCE
int count = tspmodel->numCities();
#endif // Q_OS_WINCE
	tspmodel->setNumCities(n);
#ifdef Q_OS_WINCE
	if (n > count)
		for (int k = count; k < n; k++) {
			taskView->resizeColumnToContents(k);
			taskView->resizeRowToContents(k);
		}
#endif // Q_OS_WINCE
}


void MainWindow::actionFileNewTriggered()
{
	if (isWindowModified()) {
int res = QMessageBox(QMessageBox::Warning,trUtf8("New Task"),trUtf8("Would you like to save changes in current task?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,this).exec();
		if ((res == QMessageBox::Cancel) || ((res == QMessageBox::Yes) && !saveTask()))
			return;
	}
	tspmodel->clear();
	setWindowModified(false);
}

void MainWindow::actionFileOpenTriggered()
{
	if (isWindowModified()) {
int res = QMessageBox(QMessageBox::Warning,trUtf8("Task Open"),trUtf8("Would you like to save changes in current task?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,this).exec();
		if ((res == QMessageBox::Cancel) || ((res == QMessageBox::Yes) && !saveTask()))
			return;
	}
QFileDialog od(this);
	od.setAcceptMode(QFileDialog::AcceptOpen);
	od.setFileMode(QFileDialog::ExistingFile);
QStringList filters(trUtf8("All Supported Formats") + " (*.tspt *.zkt)");
	filters.append(QString(trUtf8("%1 Task Files")).arg("TSPSG") + " (*.tspt)");
	filters.append(QString(trUtf8("%1 Task Files")).arg("ZKomModRd") + " (*.zkt)");
	filters.append(trUtf8("All Files") + " (*)");
	od.setNameFilters(filters);
	if (od.exec() != QDialog::Accepted)
		return;
QStringList files = od.selectedFiles();
	if (files.size() < 1)
		return;
	tspmodel->loadTask(files.first());
	setWindowModified(false);
}

void MainWindow::actionFileSaveTaskTriggered()
{
	saveTask();
}

bool MainWindow::saveTask() {
QFileDialog sd(this);
	sd.setAcceptMode(QFileDialog::AcceptSave);
QStringList filters(QString(trUtf8("%1 Task File")).arg("TSPSG") + " (*.tspt)");
	filters.append(trUtf8("All Files") + " (*)");
	sd.setNameFilters(filters);
	sd.setDefaultSuffix("tspt");
	if (sd.exec() != QDialog::Accepted)
		return false;
QStringList files = sd.selectedFiles();
	if (files.size() < 1)
		return false;
	if (tspmodel->saveTask(files.first())) {
		setWindowModified(false);
		return true;
	} else
		return false;
}

void MainWindow::actionSettingsPreferencesTriggered()
{
SettingsDialog sd(this);
	sd.exec();
}

#ifndef Q_OS_WINCE
void MainWindow::actionFilePrintSetupTriggered()
{
QPrintDialog pd(printer,this);
#if QT_VERSION >= 0x040500
	// No such methods in Qt < 4.5
	pd.setOption(QAbstractPrintDialog::PrintSelection,false);
	pd.setOption(QAbstractPrintDialog::PrintPageRange,false);
#endif
	pd.exec();
}
#endif // Q_OS_WINCE

void MainWindow::buttonRandomClicked()
{
	tspmodel->randomize();
	setWindowModified(true);
#ifdef Q_OS_WINCE
	taskView->resizeColumnsToContents();
	taskView->resizeRowsToContents();
#endif // Q_OS_WINCE
}

void MainWindow::buttonSolveClicked()
{
	// TODO: Task solving goes here :-)
tMatrix matrix;
double *row;
int n = spinCities->value();
bool ok;
	for (int r = 0; r < n; r++) {
		row = new double[n];
		for (int c = 0; c < n; c++) {
			row[c] = tspmodel->index(r,c).data(Qt::UserRole).toDouble(&ok);
			if (!ok) {
				QMessageBox(QMessageBox::Critical,trUtf8("Data error"),QString(trUtf8("Error in cell [Row %1; Column %2]: Invalid data format.")).arg(r + 1).arg(c + 1),QMessageBox::Ok,this).exec();
				return;
			}
		}
		matrix.append(row);
	}
CTSPSolver solver;
sStep *root = solver.solve(spinCities->value(),matrix);
	if (!root)
		QMessageBox(QMessageBox::Critical,trUtf8("Solution error"),trUtf8("There was an error while solving the task."),QMessageBox::Ok,this).exec();
	// tabWidget->setCurrentIndex(1);
}

void MainWindow::actionHelpAboutTriggered()
{
	// TODO: Normal about window :-)
QString about = QString::fromUtf8("TSPSG - TSP Solver and Generator\n");
about += QString::fromUtf8("    Copyright (C) 2007-%1 Lёppa <contacts[at]oleksii[dot]name>\n").arg(QDate::currentDate().toString("yyyy"));
	about += QString::fromUtf8("Target OS: %1\n").arg(OS);
	about += "Qt library:\n";
	about += QString::fromUtf8("    Compile time: %1\n").arg(QT_VERSION_STR);
	about += QString::fromUtf8("    Runtime: %1\n").arg(qVersion());
	about += "\n";
	about += "TSPSG is licensed under the terms of the GNU General Public License. You should have received a copy of the GNU General Public License along with TSPSG.";
	QMessageBox(QMessageBox::Information,"About",about,QMessageBox::Ok,this).exec();
}

void MainWindow::loadLangList()
{
QSettings langinfo("i18n/languages.ini",QSettings::IniFormat);
#if QT_VERSION >= 0x040500
	// In Qt < 4.5 QSettings doesn't have method setIniCodec.
	langinfo.setIniCodec("UTF-8");
#endif
QDir dir("i18n","*.qm",QDir::Name | QDir::IgnoreCase,QDir::Files);
	if (!dir.exists())
		return;
QFileInfoList langs = dir.entryInfoList();
	if (langs.size() <= 0)
		return;
QAction *a;
	for (int k = 0; k < langs.size(); k++) {
		QFileInfo lang = langs.at(k);
		if (!lang.completeBaseName().startsWith("qt_") && lang.completeBaseName().compare("en")) {
#if QT_VERSION >= 0x040500
			a = menuSettingsLanguage->addAction(langinfo.value(lang.completeBaseName() + "/NativeName",lang.completeBaseName()).toString());
#else
			// We use Name if Qt < 4.5 because NativeName is in UTF-8, QSettings
			// reads .ini file as ASCII and there is no way to set file encoding.
			a = menuSettingsLanguage->addAction(langinfo.value(lang.completeBaseName() + "/Name",lang.completeBaseName()).toString());
#endif
			a->setData(lang.completeBaseName());
			a->setCheckable(true);
			a->setActionGroup(groupSettingsLanguageList);
			if (settings->value("Language",QLocale::system().name()).toString().startsWith(lang.completeBaseName()))
				a->setChecked(true);
		}
	}
}

void MainWindow::actionSettingsLanguageAutodetectTriggered(bool checked)
{
	if (checked) {
		settings->remove("Language");
		QMessageBox(QMessageBox::Information,trUtf8("Language change"),trUtf8("Language will be autodetected on next application start."),QMessageBox::Ok,this).exec();
	} else
		settings->setValue("Language",groupSettingsLanguageList->checkedAction()->data().toString());
}

void MainWindow::groupSettingsLanguageListTriggered(QAction *action)
{
	if (actionSettingsLanguageAutodetect->isChecked()) {
		// We have language autodetection. It needs to be disabled to change language.
		if (QMessageBox(QMessageBox::Question,trUtf8("Language change"),trUtf8("You have language autodetection turned on.\nIt needs to be off.\nDo you wish to turn it off?"),QMessageBox::Yes | QMessageBox::No,this).exec() == QMessageBox::Yes) {
			actionSettingsLanguageAutodetect->trigger();
		} else
			return;
	}
	if (loadLanguage(action->data().toString())) {
		settings->setValue("Language",action->data().toString());
		retranslateUi(this);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (isWindowModified()) {
int res = QMessageBox(QMessageBox::Warning,trUtf8("Application Close"),trUtf8("Would you like to save changes in current task?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,this).exec();
		if ((res == QMessageBox::Cancel) || ((res == QMessageBox::Yes) && !saveTask())) {
			event->ignore();
			return;
		}
	}
	settings->setValue("NumCities",spinCities->value());
#ifndef Q_OS_WINCE
	// Saving windows state
	if (settings->value("SavePos",false).toBool()) {
		settings->beginGroup("MainWindow");
		settings->setValue("Maximized",isMaximized());
		if (!isMaximized()) {
			settings->setValue("Size",size());
			settings->setValue("Position",pos());
		}
		settings->endGroup();
	}
#endif // Q_OS_WINCE
	QMainWindow::closeEvent(event);
}

void MainWindow::dataChanged()
{
	setWindowModified(true);
}

void MainWindow::numCitiesChanged(int nCities)
{
	spinCities->setValue(nCities);
}
