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

#include "mainwindow.h"

/*!
 * \brief Class constructor.
 * \param parent Main Window parent widget.
 *
 *  Initializes Main Window and creates its layout based on target OS.
 *  Loads TSPSG settings and opens a task file if it was specified as a commandline parameter.
 */
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"TSPSG","tspsg");
	loadLanguage();
	setupUi(this);
#ifndef Q_OS_WINCE
QStatusBar *statusbar = new QStatusBar(this);
	statusbar->setObjectName("statusbar");
	setStatusBar(statusbar);
#endif // Q_OS_WINCE
	initDocStyleSheet();
	solutionText->document()->setDefaultFont(settings->value("Output/Font",QFont(DEF_FONT_FAMILY,DEF_FONT_SIZE)).value<QFont>());
	solutionText->setTextColor(settings->value("Output/Color",DEF_FONT_COLOR).value<QColor>());
	solutionText->setWordWrapMode(QTextOption::WordWrap);
#ifdef Q_OS_WINCE
	// A little hack for toolbar icons to have sane size.
int s = qMin(QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
	toolBar->setIconSize(QSize(s / 10,s / 10));
#endif
#ifndef QT_NO_PRINTER
	printer = new QPrinter(QPrinter::HighResolution);
#endif // QT_NO_PRINTER
	groupSettingsLanguageList = new QActionGroup(this);
	actionSettingsLanguageEnglish->setData("en");
	actionSettingsLanguageEnglish->setActionGroup(groupSettingsLanguageList);
	loadLangList();
	spinCities->setMaximum(MAX_NUM_CITIES);
	actionSettingsLanguageAutodetect->setChecked(settings->value("Language","").toString().isEmpty());
	connect(actionFileNew,SIGNAL(triggered()),this,SLOT(actionFileNewTriggered()));
	connect(actionFileOpen,SIGNAL(triggered()),this,SLOT(actionFileOpenTriggered()));
	connect(actionFileSave,SIGNAL(triggered()),this,SLOT(actionFileSaveTriggered()));
	connect(actionFileSaveAsTask,SIGNAL(triggered()),this,SLOT(actionFileSaveAsTaskTriggered()));
	connect(actionFileSaveAsSolution,SIGNAL(triggered()),this,SLOT(actionFileSaveAsSolutionTriggered()));
	connect(actionSettingsPreferences,SIGNAL(triggered()),this,SLOT(actionSettingsPreferencesTriggered()));
	connect(actionSettingsLanguageAutodetect,SIGNAL(triggered(bool)),this,SLOT(actionSettingsLanguageAutodetectTriggered(bool)));
	connect(groupSettingsLanguageList,SIGNAL(triggered(QAction *)),this,SLOT(groupSettingsLanguageListTriggered(QAction *)));
	connect(actionHelpAboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(actionHelpAbout,SIGNAL(triggered()),this,SLOT(actionHelpAboutTriggered()));
#ifndef QT_NO_PRINTER
	menuFile->insertAction(actionFileExit,actionFilePrintPreview);
	menuFile->insertAction(actionFileExit,actionFilePrint);
	menuFile->insertSeparator(actionFileExit);
	toolBar->insertAction(actionSettingsPreferences,actionFilePrint);
	connect(actionFilePrintPreview,SIGNAL(triggered()),this,SLOT(actionFilePrintPreviewTriggered()));
	connect(actionFilePrint,SIGNAL(triggered()),this,SLOT(actionFilePrintTriggered()));
#endif // QT_NO_PRINTER
	connect(buttonSolve,SIGNAL(clicked()),this,SLOT(buttonSolveClicked()));
	connect(buttonRandom,SIGNAL(clicked()),this,SLOT(buttonRandomClicked()));
	connect(buttonBackToTask,SIGNAL(clicked()),this,SLOT(buttonBackToTaskClicked()));
	connect(spinCities,SIGNAL(valueChanged(int)),this,SLOT(spinCitiesValueChanged(int)));
	setCentralWidget(tabWidget);

	if (settings->value("SavePos", false).toBool()) {
		// Loading of saved window state
		settings->beginGroup("MainWindow");
#ifndef Q_OS_WINCE
		restoreGeometry(settings->value("Geometry").toByteArray());
#endif // Q_OS_WINCE
		restoreState(settings->value("State").toByteArray());
		settings->endGroup();
#ifndef Q_OS_WINCE
	} else {
		// Centering main window
QRect rect = geometry();
		rect.moveCenter(QApplication::desktop()->availableGeometry(this).center());
		setGeometry(rect);
#endif // Q_OS_WINCE
	}

	qsrand(QDateTime().currentDateTime().toTime_t());
	tspmodel = new CTSPModel(this);
	taskView->setModel(tspmodel);
	connect(tspmodel,SIGNAL(numCitiesChanged(int)),this,SLOT(numCitiesChanged(int)));
	connect(tspmodel,SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),this,SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
	connect(tspmodel,SIGNAL(layoutChanged()),this,SLOT(dataChanged()));
	if ((QCoreApplication::arguments().count() > 1) && (tspmodel->loadTask(QCoreApplication::arguments().at(1))))
		setFileName(QCoreApplication::arguments().at(1));
	else {
		setFileName();
		spinCities->setValue(settings->value("NumCities",DEF_NUM_CITIES).toInt());
		spinCitiesValueChanged(spinCities->value());
	}
	setWindowModified(false);
}

/* Privates **********************************************************/

void MainWindow::actionFileNewTriggered()
{
	if (!maybeSave())
		return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	tspmodel->clear();
	setFileName();
	setWindowModified(false);
	tabWidget->setCurrentIndex(0);
	solutionText->clear();
	enableSolutionActions(false);
	QApplication::restoreOverrideCursor();
}

void MainWindow::actionFileOpenTriggered()
{
	if (!maybeSave())
		return;
QFileDialog od(this);
	od.setAcceptMode(QFileDialog::AcceptOpen);
	od.setFileMode(QFileDialog::ExistingFile);
QStringList filters(trUtf8("All Supported Formats") + " (*.tspt *.zkt)");
	filters.append(trUtf8("%1 Task Files").arg("TSPSG") + " (*.tspt)");
	filters.append(trUtf8("%1 Task Files").arg("ZKomModRd") + " (*.zkt)");
	filters.append(trUtf8("All Files") + " (*)");
	od.setNameFilters(filters);
	if (od.exec() != QDialog::Accepted)
		return;
QStringList files = od.selectedFiles();
	if (files.empty())
		return;
	if (!tspmodel->loadTask(files.first()))
		return;
	setFileName(files.first());
	tabWidget->setCurrentIndex(0);
	setWindowModified(false);
	solutionText->clear();
	enableSolutionActions(false);
}

void MainWindow::actionFileSaveTriggered()
{
	if ((fileName == trUtf8("Untitled") + ".tspt") || (!fileName.endsWith(".tspt",Qt::CaseInsensitive)))
		saveTask();
	else
		if (tspmodel->saveTask(fileName))
			setWindowModified(false);
}

void MainWindow::actionFileSaveAsTaskTriggered()
{
	saveTask();
}

void MainWindow::actionFileSaveAsSolutionTriggered()
{
static QString selectedFile;
	if (selectedFile.isEmpty())
#ifndef QT_NO_PRINTER
		selectedFile = "solution.pdf";
#else
		selectedFile = "solution.html";
#endif // QT_NO_PRINTER
QFileDialog sd(this);
	sd.setAcceptMode(QFileDialog::AcceptSave);
QStringList filters;
#ifndef QT_NO_PRINTER
	filters.append(trUtf8("PDF Files") + "(*.pdf)");
#endif
	filters.append(trUtf8("HTML Files") + " (*.html *.htm)");
#if QT_VERSION >= 0x040500
	filters.append(trUtf8("OpenDocument Files") + " (*.odt)");
#endif // QT_VERSION >= 0x040500
	filters.append(trUtf8("All Files") + " (*)");
	sd.setNameFilters(filters);
	sd.selectFile(selectedFile);
	if (sd.exec() != QDialog::Accepted)
		return;
QStringList files = sd.selectedFiles();
	if (files.empty())
		return;
	selectedFile = files.first();
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#ifndef QT_NO_PRINTER
	if (selectedFile.endsWith(".pdf",Qt::CaseInsensitive)) {
QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(selectedFile);
		solutionText->document()->print(&printer);
		QApplication::restoreOverrideCursor();
		return;
	}
#endif
#if QT_VERSION >= 0x040500
QTextDocumentWriter dw(selectedFile);
	if (!(selectedFile.endsWith(".htm",Qt::CaseInsensitive) || selectedFile.endsWith(".html",Qt::CaseInsensitive) || selectedFile.endsWith(".odt",Qt::CaseInsensitive) || selectedFile.endsWith(".txt",Qt::CaseInsensitive)))
		dw.setFormat("plaintext");
	dw.write(solutionText->document());
#else
	// Qt < 4.5 has no QTextDocumentWriter class
QFile file(selectedFile);
	if (!file.open(QFile::WriteOnly)) {
		QApplication::restoreOverrideCursor();
		return;
	}
QTextStream ts(&file);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	ts << solutionText->document()->toHtml("UTF-8");
	file.close();
#endif // QT_VERSION >= 0x040500
	QApplication::restoreOverrideCursor();
}

#ifndef QT_NO_PRINTER
void MainWindow::actionFilePrintPreviewTriggered()
{
QPrintPreviewDialog ppd(printer, this);
    connect(&ppd,SIGNAL(paintRequested(QPrinter *)),SLOT(printPreview(QPrinter *)));
    ppd.exec();
}

void MainWindow::actionFilePrintTriggered()
{
QPrintDialog pd(printer,this);
#if QT_VERSION >= 0x040500
	// No such methods in Qt < 4.5
	pd.setOption(QAbstractPrintDialog::PrintSelection,false);
	pd.setOption(QAbstractPrintDialog::PrintPageRange,false);
#endif
	if (pd.exec() != QDialog::Accepted)
		return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	solutionText->document()->print(printer);
	QApplication::restoreOverrideCursor();
}
#endif // QT_NO_PRINTER

void MainWindow::actionSettingsPreferencesTriggered()
{
SettingsDialog sd(this);
	if (sd.exec() != QDialog::Accepted)
		return;
	if (sd.colorChanged() || sd.fontChanged()) {
		initDocStyleSheet();
		if (!output.isEmpty() && sd.colorChanged() && (QMessageBox(QMessageBox::Question,trUtf8("Settings Changed"),trUtf8("You have changed color settings.\nDo you wish to apply them to current solution text?"),QMessageBox::Yes | QMessageBox::No,this).exec() == QMessageBox::Yes)) {
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			solutionText->clear();
			solutionText->setHtml(output.join(""));
			QApplication::restoreOverrideCursor();
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
bool untitled = (fileName == trUtf8("Untitled") + ".tspt");
	if (loadLanguage(action->data().toString())) {
		settings->setValue("Language",action->data().toString());
		retranslateUi(this);
		if (untitled)
			setFileName();
	}
}

void MainWindow::actionHelpAboutTriggered()
{
//! \todo TODO: Normal about window :-)
QString about = QString::fromUtf8("<b>TSPSG: TSP Solver and Generator</b><br>");
	about += QString::fromUtf8("&nbsp;&nbsp;&nbsp;&nbsp;Version: <b>"BUILD_VERSION"</b><br>");
	about += QString::fromUtf8("&nbsp;&nbsp;&nbsp;&nbsp;Copyright: <b>&copy; 2007-%1 Lёppa</b><br>").arg(QDate::currentDate().toString("yyyy"));
	about += QString::fromUtf8("&nbsp;&nbsp;&nbsp;&nbsp;<b><a href=\"http://tspsg.sourceforge.net/\">http://tspsg.sourceforge.net/</a></b><br>");
	about += "<br>";
	about += QString::fromUtf8("Target OS: <b>%1</b><br>").arg(OS);
	about += "Qt library:<br>";
	about += QString::fromUtf8("&nbsp;&nbsp;&nbsp;&nbsp;Build time: <b>%1</b><br>").arg(QT_VERSION_STR);
	about += QString::fromUtf8("&nbsp;&nbsp;&nbsp;&nbsp;Runtime: <b>%1</b><br>").arg(qVersion());
	about += QString::fromUtf8("Built on <b>%1</b> at <b>%2</b><br>").arg(__DATE__).arg(__TIME__);
	about += QString::fromUtf8("Id: <b>"VERSIONID"</b><br>");
	about += QString::fromUtf8("Algorithm: <b>%1</b><br>").arg(CTSPSolver::getVersionId());
	about += "<br>";
	about += "TSPSG is free software: you can redistribute it and/or modify it<br>"
		"under the terms of the GNU General Public License as published<br>"
		"by the Free Software Foundation, either version 3 of the License,<br>"
		"or (at your option) any later version.<br>"
		"<br>"
		"TSPSG is distributed in the hope that it will be useful, but<br>"
		"WITHOUT ANY WARRANTY; without even the implied warranty of<br>"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the<br>"
		"GNU General Public License for more details.<br>"
		"<br>"
		"You should have received a copy of the GNU General Public License<br>"
		"along with TSPSG.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.";

QDialog *dlg = new QDialog(this);
QLabel *lblIcon = new QLabel(dlg);
QTextBrowser *txtAbout = new QTextBrowser(dlg);
QVBoxLayout *vb1 = new QVBoxLayout(),
	*vb2 = new QVBoxLayout();
QHBoxLayout *hb = new QHBoxLayout();
QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, dlg);

	lblIcon->setPixmap(QPixmap(":/images/tspsg.png").scaledToWidth(64, Qt::SmoothTransformation));

	vb1->addWidget(lblIcon);
	vb1->addStretch();

//	txtAbout->setTextInteractionFlags(txtAbout->textInteractionFlags() ^ Qt::TextEditable);
	txtAbout->setWordWrapMode(QTextOption::NoWrap);
	txtAbout->setOpenExternalLinks(true);
	txtAbout->setHtml(about);
	txtAbout->moveCursor(QTextCursor::Start);

	hb->addLayout(vb1);
	hb->addWidget(txtAbout);

	vb2->addLayout(hb);
	vb2->addWidget(bb);

	dlg->setWindowTitle(trUtf8("About TSPSG"));
	dlg->setLayout(vb2);

	connect(bb, SIGNAL(accepted()), dlg, SLOT(accept()));

	dlg->resize(475, 350);
	dlg->exec();

	delete dlg;
}

void MainWindow::buttonBackToTaskClicked()
{
	tabWidget->setCurrentIndex(0);
}

void MainWindow::buttonRandomClicked()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	tspmodel->randomize();
	QApplication::restoreOverrideCursor();
}

void MainWindow::buttonSolveClicked()
{
TMatrix matrix;
QList<double> row;
int n = spinCities->value();
bool ok;
	for (int r = 0; r < n; r++) {
		row.clear();
		for (int c = 0; c < n; c++) {
			row.append(tspmodel->index(r,c).data(Qt::UserRole).toDouble(&ok));
			if (!ok) {
				QMessageBox(QMessageBox::Critical,trUtf8("Data error"),trUtf8("Error in cell [Row %1; Column %2]: Invalid data format.").arg(r + 1).arg(c + 1),QMessageBox::Ok,this).exec();
				return;
			}
		}
		matrix.append(row);
	}
CTSPSolver solver;
SStep *root = solver.solve(n,matrix,this);
	if (!root)
		return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
QColor color = settings->value("Output/Color",DEF_FONT_COLOR).value<QColor>();
	output.clear();
	output.append("<p>" + trUtf8("Variant #%1").arg(spinVariant->value()) + "</p>");
	output.append("<p>" + trUtf8("Task:") + "</p>");
	outputMatrix(matrix,output);
	output.append("<hr>");
	output.append("<p>" + trUtf8("Solution of Variant #%1 task").arg(spinVariant->value()) + "</p>");
SStep *step = root;
	n = 1;
	while (n <= spinCities->value()) {
		if (step->prNode->prNode != NULL || ((step->prNode->prNode == NULL) && (step->plNode->prNode == NULL))) {
			if (n != spinCities->value()) {
				output.append("<p>" + trUtf8("Step #%1").arg(n++) + "</p>");
				outputMatrix(*step, output);
				output.append("<p>" + trUtf8("Selected candidate for branching: %1.").arg(trUtf8("(%1;%2)").arg(step->candidate.nRow + 1).arg(step->candidate.nCol + 1)) + "</p>");
				if (!step->alts.empty()) {
TCandidate cand;
QString alts;
					foreach(cand, step->alts) {
						if (!alts.isEmpty())
							alts += ", ";
						alts += trUtf8("(%1;%2)").arg(cand.nRow + 1).arg(cand.nCol + 1);
					}
					output.append("<p class=\"hasalts\">" + trUtf8("%n alternate candidate(s) for branching: %1.","",step->alts.count()).arg(alts) + "</p>");
				}
				output.append("<p>&nbsp;</p>");
			}
		}
		if (step->prNode->prNode != NULL)
			step = step->prNode;
		else if (step->plNode->prNode != NULL)
			step = step->plNode;
		else
			break;
	}
	if (solver.isOptimal())
		output.append("<p>" + trUtf8("Optimal path:") + "</p>");
	else
		output.append("<p>" + trUtf8("Resulting path:") + "</p>");
	output.append("<p>&nbsp;&nbsp;" + solver.getSortedPath() + "</p>");
	output.append("<p>" + trUtf8("The price is <b>%1</b> units.").arg(step->price) + "</p>");
	if (!solver.isOptimal()) {
		output.append("<p>&nbsp;</p>");
		output.append("<p>" + trUtf8("<b>WARNING!!!</b><br>This result is a record, but it may not be optimal.<br>Iterations need to be continued to check whether this result is optimal or get an optimal one.") + "</p>");
	}
	output.append("<p></p>");
	solutionText->setHtml(output.join(""));
	solutionText->setDocumentTitle(trUtf8("Solution of Variant #%1 task").arg(spinVariant->value()));

	// Scrolling to the end of text.
	solutionText->moveCursor(QTextCursor::End);

	enableSolutionActions();
	tabWidget->setCurrentIndex(1);
	QApplication::restoreOverrideCursor();
}

void MainWindow::dataChanged()
{
	setWindowModified(true);
}

void MainWindow::dataChanged(const QModelIndex &tl, const QModelIndex &br)
{
	setWindowModified(true);
	if (settings->value("Autosize",true).toBool()) {
		for (int k = tl.row(); k <= br.row(); k++)
			taskView->resizeRowToContents(k);
		for (int k = tl.column(); k <= br.column(); k++)
			taskView->resizeColumnToContents(k);
	}
}

void MainWindow::numCitiesChanged(int nCities)
{
	blockSignals(true);
	spinCities->setValue(nCities);
	blockSignals(false);
}

#ifndef QT_NO_PRINTER
void MainWindow::printPreview(QPrinter *printer)
{
	solutionText->print(printer);
}
#endif // QT_NO_PRINTER

void MainWindow::spinCitiesValueChanged(int n)
{
int count = tspmodel->numCities();
	tspmodel->setNumCities(n);
	if ((n > count) && settings->value("Autosize",true).toBool())
		for (int k = count; k < n; k++) {
			taskView->resizeColumnToContents(k);
			taskView->resizeRowToContents(k);
		}
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	if (!maybeSave()) {
		ev->ignore();
		return;
	}
	settings->setValue("NumCities", spinCities->value());

	// Saving Main Window state
	if (settings->value("SavePos", false).toBool()) {
		settings->beginGroup("MainWindow");
#ifndef Q_OS_WINCE
		settings->setValue("Geometry", saveGeometry());
#endif // Q_OS_WINCE
		settings->setValue("State", saveState());
		settings->endGroup();
	}

	QMainWindow::closeEvent(ev);
}

void MainWindow::enableSolutionActions(bool enable)
{
	buttonSaveSolution->setEnabled(enable);
	actionFileSaveAsSolution->setEnabled(enable);
	solutionText->setEnabled(enable);
	if (!enable)
		output.clear();
#ifndef QT_NO_PRINTER
	actionFilePrint->setEnabled(enable);
	actionFilePrintPreview->setEnabled(enable);
#endif // QT_NO_PRINTER
}

void MainWindow::initDocStyleSheet()
{
QColor color = settings->value("Output/Color",DEF_FONT_COLOR).value<QColor>();
QColor hilight;
	if (color.value() < 192)
		hilight.setHsv(color.hue(),color.saturation(),127 + qRound(color.value() / 2));
	else
		hilight.setHsv(color.hue(),color.saturation(),color.value() / 2);
	solutionText->document()->setDefaultStyleSheet("* {color: " + color.name() +";} p {margin: 0px 10px;} table {margin: 5px;} td {padding: 1px 5px;} .hasalts {color: " + hilight.name() + ";} .selected {color: #A00000; font-weight: bold;} .alternate {color: #008000; font-weight: bold;}");
	solutionText->document()->setDefaultFont(settings->value("Output/Font",QFont(DEF_FONT_FAMILY,DEF_FONT_SIZE)).value<QFont>());
}

void MainWindow::loadLangList()
{
QSettings langinfo(PATH_I18N"/languages.ini",QSettings::IniFormat);
#if QT_VERSION >= 0x040500
	// In Qt < 4.5 QSettings doesn't have method setIniCodec.
	langinfo.setIniCodec("UTF-8");
#endif
QDir dir(PATH_I18N,"*.qm",QDir::Name | QDir::IgnoreCase,QDir::Files);
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

bool MainWindow::loadLanguage(const QString &lang)
{
// i18n
bool ad = false;
QString lng = lang;
	if (lng.isEmpty()) {
		ad = settings->value("Language","").toString().isEmpty();
		lng = settings->value("Language",QLocale::system().name()).toString();
	}
static QTranslator *qtTranslator; // Qt library translator
	if (qtTranslator) {
		qApp->removeTranslator(qtTranslator);
		delete qtTranslator;
		qtTranslator = NULL;
	}
static QTranslator *translator; // Application translator
	if (translator) {
		qApp->removeTranslator(translator);
		delete translator;
	}
	translator = new QTranslator();
	if ((lng.compare("en") != 0) && !lng.startsWith("en_")) {
		// Trying to load system Qt library translation...
		qtTranslator = new QTranslator();
		if (qtTranslator->load("qt_" + lng,QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
			qApp->installTranslator(qtTranslator);
		else {
			// No luck. Let's try to load a bundled one.
			if (qtTranslator->load("qt_" + lng,PATH_I18N))
				qApp->installTranslator(qtTranslator);
			else {
				// Qt library translation unavailable
				delete qtTranslator;
				qtTranslator = NULL;
			}
		}
	}
	// Now let's load application translation.
	if (translator->load(lng,PATH_I18N))
		qApp->installTranslator(translator);
	else {
		delete translator;
		translator = NULL;
		if ((lng.compare("en") != 0) && !lng.startsWith("en_")) {
			if (!ad)
				QMessageBox(QMessageBox::Warning,trUtf8("Language Change"),trUtf8("Unable to load translation language."),QMessageBox::Ok,this).exec();
			return false;
		}
	}
	return true;
}

bool MainWindow::maybeSave()
{
	if (!isWindowModified())
		return true;
int res = QMessageBox(QMessageBox::Warning,trUtf8("Unsaved Changes"),trUtf8("Would you like to save changes in current task?"),QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,this).exec();
	if (res == QMessageBox::Save)
		return saveTask();
	else if (res == QMessageBox::Cancel)
		return false;
	else
		return true;
}

void MainWindow::outputMatrix(const TMatrix &matrix, QStringList &output)
{
int n = spinCities->value();
QString line="";
	output.append("<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">");
	for (int r = 0; r < n; r++) {
		line = "<tr>";
		for (int c = 0; c < n; c++) {
			if (matrix.at(r).at(c) == INFINITY)
				line += "<td align=\"center\">"INFSTR"</td>";
			else
				line += "<td align=\"center\">" + QVariant(matrix.at(r).at(c)).toString() + "</td>";
		}
		line += "</tr>";
		output.append(line);
	}
	output.append("</table>");
}

void MainWindow::outputMatrix(const SStep &step, QStringList &output)
{
int n = spinCities->value();
QString line="";
	output.append("<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">");
	for (int r = 0; r < n; r++) {
		line = "<tr>";
		for (int c = 0; c < n; c++) {
			if (step.matrix.at(r).at(c) == INFINITY)
				line += "<td align=\"center\">"INFSTR"</td>";
			else if ((r == step.candidate.nRow) && (c == step.candidate.nCol))
				line += "<td align=\"center\" class=\"selected\">" + QVariant(step.matrix.at(r).at(c)).toString() + "</td>";
			else {
TCandidate cand;
				cand.nRow = r;
				cand.nCol = c;
				if (step.alts.contains(cand))
					line += "<td align=\"center\" class=\"alternate\">" + QVariant(step.matrix.at(r).at(c)).toString() + "</td>";
				else
					line += "<td align=\"center\">" + QVariant(step.matrix.at(r).at(c)).toString() + "</td>";
			}
		}
		line += "</tr>";
		output.append(line);
	}
	output.append("</table>");
}

bool MainWindow::saveTask() {
QFileDialog sd(this);
	sd.setAcceptMode(QFileDialog::AcceptSave);
QStringList filters(trUtf8("%1 Task File").arg("TSPSG") + " (*.tspt)");
	filters.append(trUtf8("All Files") + " (*)");
	sd.setNameFilters(filters);
	sd.setDefaultSuffix("tspt");
	if (fileName.endsWith(".tspt",Qt::CaseInsensitive))
		sd.selectFile(fileName);
	else
		sd.selectFile(QFileInfo(fileName).canonicalPath() + "/" + QFileInfo(fileName).completeBaseName() + ".tspt");
	if (sd.exec() != QDialog::Accepted)
		return false;
QStringList files = sd.selectedFiles();
	if (files.empty())
		return false;
	if (tspmodel->saveTask(files.first())) {
		setFileName(files.first());
		setWindowModified(false);
		return true;
	}
	return false;
}

void MainWindow::setFileName(const QString &fileName)
{
	this->fileName = fileName;
	setWindowTitle(QString("%1[*] - %2").arg(QFileInfo(fileName).completeBaseName()).arg(trUtf8("Travelling Salesman Problem")));
}
