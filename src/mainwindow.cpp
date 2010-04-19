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
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "TSPSG", "tspsg", this);

	loadLanguage();
	setupUi();

	initDocStyleSheet();

#ifndef QT_NO_PRINTER
	printer = new QPrinter(QPrinter::HighResolution);
#endif // QT_NO_PRINTER

#ifdef Q_OS_WINCE_WM
	currentGeometry = QApplication::desktop()->availableGeometry(0);
	// We need to react to SIP show/hide and resize the window appropriately
	connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), SLOT(desktopResized(int)));
#endif // Q_OS_WINCE_WM
	connect(actionFileNew,SIGNAL(triggered()),this,SLOT(actionFileNewTriggered()));
	connect(actionFileOpen,SIGNAL(triggered()),this,SLOT(actionFileOpenTriggered()));
	connect(actionFileSave,SIGNAL(triggered()),this,SLOT(actionFileSaveTriggered()));
	connect(actionFileSaveAsTask,SIGNAL(triggered()),this,SLOT(actionFileSaveAsTaskTriggered()));
	connect(actionFileSaveAsSolution,SIGNAL(triggered()),this,SLOT(actionFileSaveAsSolutionTriggered()));
#ifndef QT_NO_PRINTER
	connect(actionFilePrintPreview,SIGNAL(triggered()),this,SLOT(actionFilePrintPreviewTriggered()));
	connect(actionFilePrint,SIGNAL(triggered()),this,SLOT(actionFilePrintTriggered()));
#endif // QT_NO_PRINTER
	connect(actionSettingsPreferences,SIGNAL(triggered()),this,SLOT(actionSettingsPreferencesTriggered()));
#ifdef Q_OS_WIN32
	connect(actionHelpCheck4Updates, SIGNAL(triggered()), SLOT(actionHelpCheck4UpdatesTriggered()));
#endif // Q_OS_WIN32
	connect(actionSettingsLanguageAutodetect,SIGNAL(triggered(bool)),this,SLOT(actionSettingsLanguageAutodetectTriggered(bool)));
	connect(groupSettingsLanguageList,SIGNAL(triggered(QAction *)),this,SLOT(groupSettingsLanguageListTriggered(QAction *)));
	connect(actionHelpAboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
	connect(actionHelpAbout,SIGNAL(triggered()),this,SLOT(actionHelpAboutTriggered()));

	connect(buttonSolve,SIGNAL(clicked()),this,SLOT(buttonSolveClicked()));
	connect(buttonRandom,SIGNAL(clicked()),this,SLOT(buttonRandomClicked()));
	connect(buttonBackToTask,SIGNAL(clicked()),this,SLOT(buttonBackToTaskClicked()));
	connect(spinCities,SIGNAL(valueChanged(int)),this,SLOT(spinCitiesValueChanged(int)));

#ifndef HANDHELD
	// Centering main window
QRect rect = geometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry(this).center());
	setGeometry(rect);
	if (settings->value("SavePos", DEF_SAVEPOS).toBool()) {
		// Loading of saved window state
		settings->beginGroup("MainWindow");
		restoreGeometry(settings->value("Geometry").toByteArray());
		restoreState(settings->value("State").toByteArray());
		settings->endGroup();
	}
#else
	setWindowState(Qt::WindowMaximized);
#endif // HANDHELD

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

MainWindow::~MainWindow()
{
#ifndef QT_NO_PRINTER
	delete printer;
#endif
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
	toggleSolutionActions(false);
	QApplication::restoreOverrideCursor();
}

void MainWindow::actionFileOpenTriggered()
{
	if (!maybeSave())
		return;

QStringList filters(tr("All Supported Formats") + " (*.tspt *.zkt)");
	filters.append(tr("%1 Task Files").arg("TSPSG") + " (*.tspt)");
	filters.append(tr("%1 Task Files").arg("ZKomModRd") + " (*.zkt)");
	filters.append(tr("All Files") + " (*)");

QString file = QFileInfo(fileName).canonicalPath();
QFileDialog::Options opts = settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool() ? QFileDialog::Options() : QFileDialog::DontUseNativeDialog;
	file = QFileDialog::getOpenFileName(this, tr("Task Load"), file, filters.join(";;"), NULL, opts);
	if (file.isEmpty() || !QFileInfo(file).isFile())
		return;
	if (!tspmodel->loadTask(file))
		return;
	setFileName(file);
	tabWidget->setCurrentIndex(0);
	setWindowModified(false);
	solutionText->clear();
	toggleSolutionActions(false);
}

bool MainWindow::actionFileSaveTriggered()
{
	if ((fileName == tr("Untitled") + ".tspt") || (!fileName.endsWith(".tspt", Qt::CaseInsensitive)))
		return saveTask();
	else
		if (tspmodel->saveTask(fileName)) {
			setWindowModified(false);
			return true;
		} else
			return false;
}

void MainWindow::actionFileSaveAsTaskTriggered()
{
	saveTask();
}

void MainWindow::actionFileSaveAsSolutionTriggered()
{
static QString selectedFile;
	if (selectedFile.isEmpty())
		selectedFile = QFileInfo(fileName).canonicalPath();
	else
		selectedFile = QFileInfo(selectedFile).canonicalPath();
	if (!selectedFile.isEmpty())
		selectedFile += "/";
	if (fileName == tr("Untitled") + ".tspt") {
#ifndef QT_NO_PRINTER
		selectedFile += "solution.pdf";
#else
		selectedFile += "solution.html";
#endif // QT_NO_PRINTER
	} else {
#ifndef QT_NO_PRINTER
		selectedFile += QFileInfo(fileName).completeBaseName() + ".pdf";
#else
		selectedFile += QFileInfo(fileName).completeBaseName() + ".html";
#endif // QT_NO_PRINTER
	}

QStringList filters;
#ifndef QT_NO_PRINTER
	filters.append(tr("PDF Files") + " (*.pdf)");
#endif
	filters.append(tr("HTML Files") + " (*.html *.htm)");
#if QT_VERSION >= 0x040500
	filters.append(tr("OpenDocument Files") + " (*.odt)");
#endif // QT_VERSION >= 0x040500
	filters.append(tr("All Files") + " (*)");

QFileDialog::Options opts(settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool() ? QFileDialog::Options() : QFileDialog::DontUseNativeDialog);
QString file = QFileDialog::getSaveFileName(this, QString(), selectedFile, filters.join(";;"), NULL, opts);
	if (file.isEmpty())
		return;
	selectedFile = file;
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
	if (!dw.write(solutionText->document()))
		QMessageBox::critical(this, tr("Solution Save"), tr("Unable to save the solution.\nError: %1").arg(dw.device()->errorString()));
#else // QT_VERSION >= 0x040500
	// Qt < 4.5 has no QTextDocumentWriter class
QFile file(selectedFile);
	if (!file.open(QFile::WriteOnly)) {
		QApplication::restoreOverrideCursor();
		QMessageBox::critical(this, tr("Solution Save"), tr("Unable to save the solution.\nError: %1").arg(file->errorString()));
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
		if (!solutionText->document()->isEmpty() && sd.colorChanged())
			QMessageBox::information(this, tr("Settings Changed"), tr("You have changed color settings.\nThey will be applied to the next solution output."));
		initDocStyleSheet();
	}
	if (sd.translucencyChanged() != 0)
		toggleTranclucency(sd.translucencyChanged() == 1);
}

void MainWindow::actionSettingsLanguageAutodetectTriggered(bool checked)
{
	if (checked) {
		settings->remove("Language");
		QMessageBox::information(this, tr("Language change"), tr("Language will be autodetected on the next application start."));
	} else
		settings->setValue("Language", groupSettingsLanguageList->checkedAction()->data().toString());
}

void MainWindow::groupSettingsLanguageListTriggered(QAction *action)
{
	if (actionSettingsLanguageAutodetect->isChecked()) {
		// We have language autodetection. It needs to be disabled to change language.
		if (QMessageBox::question(this, tr("Language change"), tr("You have language autodetection turned on.\nIt needs to be off.\nDo you wish to turn it off?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
			actionSettingsLanguageAutodetect->trigger();
		} else
			return;
	}
bool untitled = (fileName == tr("Untitled") + ".tspt");
	if (loadLanguage(action->data().toString())) {
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		settings->setValue("Language",action->data().toString());
		retranslateUi();
		if (untitled)
			setFileName();
#ifdef Q_OS_WIN32
		if (QtWin::isCompositionEnabled() && settings->value("UseTranslucency", DEF_USE_TRANSLUCENCY).toBool())  {
			toggleStyle(labelVariant, true);
			toggleStyle(labelCities, true);
		}
#endif
		QApplication::restoreOverrideCursor();
		if (!solutionText->document()->isEmpty())
			QMessageBox::information(this, tr("Settings Changed"), tr("You have changed the application language.\nTo get current solution output in the new language\nyou need to re-run the solution process."));
	}
}

#ifdef Q_OS_WIN32
void MainWindow::actionHelpCheck4UpdatesTriggered()
{
	if (!hasUpdater()) {
		QMessageBox::warning(this, tr("Unsupported Feature"), tr("Sorry, but this feature is not supported on your platform\nor support for this feature was not installed."));
		return;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QProcess::execute("updater/Update.exe -name=\"TSPSG: TSP Solver and Generator\" -check=\"freeupdate\"");
	QApplication::restoreOverrideCursor();
}
#endif // Q_OS_WIN32

void MainWindow::actionHelpAboutTriggered()
{
QString title;
#ifdef HANDHELD
	title += QString("<b>TSPSG<br>TSP Solver and Generator</b><br>");
#else
	title += QString("<b>TSPSG: TSP Solver and Generator</b><br>");
#endif // HANDHELD
	title += QString("%1: <b>%2</b><br>").arg(tr("Version"), QApplication::applicationVersion());
#ifndef HANDHELD
	title += QString("<b>&copy; 2007-%1 <a href=\"http://%2/\">%3</a></b><br>").arg(QDate::currentDate().toString("yyyy"), QApplication::organizationDomain(), QApplication::organizationName());
	title += QString("<b><a href=\"http://tspsg.sourceforge.net/\">http://tspsg.sourceforge.net/</a></b>");
#else
	title += QString("<b><a href=\"http://tspsg.sourceforge.net/\">http://tspsg.sf.net/</a></b>");
#endif // Q_OS_WINCE_WM && Q_OS_SYMBIAN

QString about;
	about += QString("%1: <b>%2</b><br>").arg(tr("Target OS (ARCH)"), OS);
#ifndef STATIC_BUILD
	about += QString("%1 (%2):<br>").arg(tr("Qt library"), tr("shared"));
	about += QString("&nbsp;&nbsp;&nbsp;&nbsp;%1: <b>%2</b><br>").arg(tr("Build time"), QT_VERSION_STR);
	about += QString("&nbsp;&nbsp;&nbsp;&nbsp;%1: <b>%2</b><br>").arg(tr("Runtime"), qVersion());
#else
	about += QString("%1: <b>%2</b> (%3)<br>").arg(tr("Qt library"), QT_VERSION_STR, tr("static"));
#endif // STATIC_BUILD
	about += tr("Buid <b>%1</b>, built on <b>%2</b> at <b>%3</b>").arg(BUILD_NUMBER).arg(__DATE__).arg(__TIME__) + "<br>";
	about += QString("%1: <b>%2</b><br>").arg(tr("Algorithm"), CTSPSolver::getVersionId());
	about += "<br>";
	about += tr("TSPSG is free software: you can redistribute it and/or modify it<br>"
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
		"along with TSPSG.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.");

QDialog *dlg = new QDialog(this);
QLabel *lblIcon = new QLabel(dlg),
	*lblTitle = new QLabel(dlg),
	*lblTranslated = new QLabel(dlg);
#ifdef HANDHELD
QLabel *lblSubTitle = new QLabel(QString("<b>&copy; 2007-%1 <a href=\"http://%2/\">%3</a></b>").arg(QDate::currentDate().toString("yyyy"), QApplication::organizationDomain(), QApplication::organizationName()), dlg);
#endif // HANDHELD
QTextBrowser *txtAbout = new QTextBrowser(dlg);
QVBoxLayout *vb = new QVBoxLayout();
QHBoxLayout *hb1 = new QHBoxLayout(),
	*hb2 = new QHBoxLayout();
QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, dlg);

	lblTitle->setOpenExternalLinks(true);
	lblTitle->setText(title);
	lblTitle->setAlignment(Qt::AlignTop);
	lblTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
#ifndef HANDHELD
	lblTitle->setStyleSheet(QString("QLabel {background-color: %1; border-color: %2; border-width: 1px; border-style: solid; border-radius: 3px;}").arg(palette().window().color().name(), palette().shadow().color().name()));
#endif // HANDHELD

	lblIcon->setPixmap(QPixmap(":/images/tspsg.png").scaledToHeight(lblTitle->sizeHint().height(), Qt::SmoothTransformation));
	lblIcon->setAlignment(Qt::AlignVCenter);
#ifndef HANDHELD
	lblIcon->setStyleSheet(QString("QLabel {background-color: %1; border-color: %2; border-width: 1px; border-style: solid; border-radius: 3px;}").arg(palette().window().color().name(), palette().windowText().color().name()));
#endif // HANDHELD

	hb1->addWidget(lblIcon);
	hb1->addWidget(lblTitle);

	txtAbout->setWordWrapMode(QTextOption::NoWrap);
	txtAbout->setOpenExternalLinks(true);
	txtAbout->setHtml(about);
	txtAbout->moveCursor(QTextCursor::Start);
#ifndef HANDHELD
	txtAbout->setStyleSheet(QString("QTextBrowser {border-color: %1; border-width: 1px; border-style: solid; border-radius: 3px;}").arg(palette().shadow().color().name()));
#endif // HANDHELD

	bb->button(QDialogButtonBox::Ok)->setCursor(QCursor(Qt::PointingHandCursor));

	lblTranslated->setText(QApplication::translate("--------", "TRANSLATION", "Please, provide translator credits here."));
	if (lblTranslated->text() == "TRANSLATION")
		lblTranslated->hide();
	else {
		lblTranslated->setOpenExternalLinks(true);
#ifndef HANDHELD
		lblTranslated->setStyleSheet(QString("QLabel {background-color: %1; border-color: %2; border-width: 1px; border-style: solid; border-radius: 3px;}").arg(palette().window().color().name(), palette().shadow().color().name()));
#endif // HANDHELD
		hb2->addWidget(lblTranslated);
	}

	hb2->addWidget(bb);

#ifdef Q_OS_WINCE_WM
	vb->setMargin(3);
#endif // Q_OS_WINCE_WM
	vb->addLayout(hb1);
#ifdef HANDHELD
	vb->addWidget(lblSubTitle);
#endif // HANDHELD
	vb->addWidget(txtAbout);
	vb->addLayout(hb2);

	dlg->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	dlg->setWindowTitle(tr("About TSPSG"));
	dlg->setLayout(vb);

	connect(bb, SIGNAL(accepted()), dlg, SLOT(accept()));

#ifdef Q_OS_WIN32
	// Adding some eyecandy in Vista and 7 :-)
	if (QtWin::isCompositionEnabled())  {
		QtWin::enableBlurBehindWindow(dlg, true);
	}
#endif // Q_OS_WIN32

	dlg->resize(450, 350);

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
				QMessageBox::critical(this, tr("Data error"), tr("Error in cell [Row %1; Column %2]: Invalid data format.").arg(r + 1).arg(c + 1));
				return;
			}
		}
		matrix.append(row);
	}

QProgressDialog pd(this);
QProgressBar *pb = new QProgressBar(&pd);
	pb->setAlignment(Qt::AlignCenter);
	pb->setFormat(tr("%v of %1 parts found").arg(n));
	pd.setBar(pb);
	pd.setMaximum(n);
	pd.setAutoReset(false);
	pd.setLabelText(tr("Calculating optimal route..."));
	pd.setWindowTitle(tr("Solution Progress"));
	pd.setWindowModality(Qt::ApplicationModal);
	pd.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	pd.show();

CTSPSolver solver;
	connect(&solver, SIGNAL(routePartFound(int)), &pd, SLOT(setValue(int)));
	connect(&pd, SIGNAL(canceled()), &solver, SLOT(cancel()));
SStep *root = solver.solve(n, matrix);
	disconnect(&solver, SIGNAL(routePartFound(int)), &pd, SLOT(setValue(int)));
	disconnect(&pd, SIGNAL(canceled()), &solver, SLOT(cancel()));
	if (!root) {
		pd.reset();
		if (!solver.wasCanceled())
			QMessageBox::warning(this, tr("Solution Result"), tr("Unable to find a solution.\nMaybe, this task has no solution."));
		return;
	}
	pb->setFormat(tr("Generating header"));
	pd.setLabelText(tr("Generating solution output..."));
	pd.setMaximum(n);
	pd.setValue(0);

#ifdef DEBUG
QTime t;
	t.start();
#endif
	solutionText->clear();
#ifdef DEBUG
	qDebug() << "Clear:" << t.elapsed();
	t.restart();
#endif
	solutionText->setDocumentTitle(tr("Solution of Variant #%1 Task").arg(spinVariant->value()));

QTextDocument *doc = solutionText->document();
QTextCursor cur(doc);

	cur.beginEditBlock();
	cur.setBlockFormat(fmt_paragraph);
	cur.insertText(tr("Variant #%1").arg(spinVariant->value()), fmt_default);
	cur.insertBlock(fmt_paragraph);
	cur.insertText(tr("Task:"));
	outputMatrix(cur, matrix);
	cur.insertHtml("<hr>");
	cur.insertBlock(fmt_paragraph);
	cur.insertText(tr("Solution of Variant #%1 Task").arg(spinVariant->value()), fmt_default);
	cur.endEditBlock();

SStep *step = root;
	n = 1;
	pb->setFormat(tr("Generating step %v"));
	while (n < spinCities->value()) {
		if (pd.wasCanceled()) {
			pd.setLabelText(tr("Cleaning up..."));
			pd.setMaximum(0);
			pd.setCancelButton(NULL);
			pd.show();
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
			solver.cleanup(true);
			solutionText->clear();
			toggleSolutionActions(false);
			return;
		}
		pd.setValue(n);

		if (step->prNode->prNode != NULL || ((step->prNode->prNode == NULL) && (step->plNode->prNode == NULL))) {
			if (n != spinCities->value()) {
				cur.beginEditBlock();
				cur.insertBlock(fmt_paragraph);
				cur.insertText(tr("Step #%1").arg(n++));
				if (settings->value("Output/ShowMatrix", DEF_SHOW_MATRIX).toBool() && (!settings->value("Output/UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT).toBool() || (settings->value("Output/UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT).toBool() && (spinCities->value() <= settings->value("Output/ShowMatrixLimit", DEF_SHOW_MATRIX_LIMIT).toInt())))) {
					outputMatrix(cur, *step);
				}
				cur.insertBlock(fmt_paragraph);
				cur.insertText(tr("Selected candidate for branching: %1.").arg(tr("(%1;%2)").arg(step->candidate.nRow + 1).arg(step->candidate.nCol + 1)), fmt_default);
				if (!step->alts.empty()) {
SCandidate cand;
QString alts;
					foreach(cand, step->alts) {
						if (!alts.isEmpty())
							alts += ", ";
						alts += tr("(%1;%2)").arg(cand.nRow + 1).arg(cand.nCol + 1);
					}
					cur.insertBlock(fmt_paragraph);
					cur.insertText(tr("%n alternate candidate(s) for branching: %1.", "", step->alts.count()).arg(alts), fmt_altlist);
				}
				cur.insertBlock(fmt_paragraph);
				cur.insertText(" ", fmt_default);
				cur.endEditBlock();
			}
		}
		if (step->prNode->prNode != NULL)
			step = step->prNode;
		else if (step->plNode->prNode != NULL)
			step = step->plNode;
		else
			break;
	}
	pb->setFormat(tr("Generating footer"));
	pd.setValue(n);

	cur.beginEditBlock();
	cur.insertBlock(fmt_paragraph);
	if (solver.isOptimal())
		cur.insertText(tr("Optimal path:"));
	else
		cur.insertText(tr("Resulting path:"));

	cur.insertBlock(fmt_paragraph);
	cur.insertText("  " + solver.getSortedPath());

	cur.insertBlock(fmt_paragraph);
	if (isInteger(step->price))
		cur.insertHtml("<p>" + tr("The price is <b>%n</b> unit(s).", "", qRound(step->price)) + "</p>");
	else
		cur.insertHtml("<p>" + tr("The price is <b>%1</b> units.").arg(step->price, 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()) + "</p>");
	if (!solver.isOptimal()) {
		cur.insertBlock(fmt_paragraph);
		cur.insertText(" ");
		cur.insertBlock(fmt_paragraph);
		cur.insertHtml("<p>" + tr("<b>WARNING!!!</b><br>This result is a record, but it may not be optimal.<br>Iterations need to be continued to check whether this result is optimal or get an optimal one.") + "</p>");
	}
	cur.endEditBlock();
#ifdef DEBUG
	qDebug() << "Generate:" << t.elapsed();
#endif

	if (settings->value("Output/ScrollToEnd", DEF_SCROLL_TO_END).toBool()) {
		// Scrolling to the end of the text.
		solutionText->moveCursor(QTextCursor::End);
	} else
		solutionText->moveCursor(QTextCursor::Start);

	pd.setLabelText(tr("Cleaning up..."));
	pd.setMaximum(0);
	pd.setCancelButton(NULL);
	QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	solver.cleanup(true);
	toggleSolutionActions();
	tabWidget->setCurrentIndex(1);
}

void MainWindow::dataChanged()
{
	setWindowModified(true);
}

void MainWindow::dataChanged(const QModelIndex &tl, const QModelIndex &br)
{
	setWindowModified(true);
	if (settings->value("Autosize", DEF_AUTOSIZE).toBool()) {
		for (int k = tl.row(); k <= br.row(); k++)
			taskView->resizeRowToContents(k);
		for (int k = tl.column(); k <= br.column(); k++)
			taskView->resizeColumnToContents(k);
	}
}

#ifdef Q_OS_WINCE_WM
void MainWindow::changeEvent(QEvent *ev)
{
	if ((ev->type() == QEvent::ActivationChange) && isActiveWindow())
		desktopResized(0);

	QWidget::changeEvent(ev);
}

void MainWindow::desktopResized(int screen)
{
	if ((screen != 0) || !isActiveWindow())
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
#endif // Q_OS_WINCE_WM

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
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
int count = tspmodel->numCities();
	tspmodel->setNumCities(n);
	if ((n > count) && settings->value("Autosize", DEF_AUTOSIZE).toBool())
		for (int k = count; k < n; k++) {
			taskView->resizeColumnToContents(k);
			taskView->resizeRowToContents(k);
		}
	QApplication::restoreOverrideCursor();
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
	if (!maybeSave()) {
		ev->ignore();
		return;
	}
	if (!settings->value("SettingsReset", false).toBool()) {
		settings->setValue("NumCities", spinCities->value());

		// Saving Main Window state
		if (settings->value("SavePos", DEF_SAVEPOS).toBool()) {
			settings->beginGroup("MainWindow");
#ifndef HANDHELD
			settings->setValue("Geometry", saveGeometry());
#endif // HANDHELD
			settings->setValue("State", saveState());
			settings->endGroup();
		}
	} else {
		settings->remove("SettingsReset");
	}

	QMainWindow::closeEvent(ev);
}

bool MainWindow::hasUpdater() const
{
#ifdef Q_OS_WIN32
	return QFile::exists("updater/Update.exe");
#else // Q_OS_WIN32
	return false;
#endif // Q_OS_WIN32
}

void MainWindow::initDocStyleSheet()
{
	solutionText->document()->setDefaultFont(settings->value("Output/Font", QFont(DEF_FONT_FAMILY, DEF_FONT_SIZE)).value<QFont>());

	fmt_paragraph.setTopMargin(0);
	fmt_paragraph.setRightMargin(10);
	fmt_paragraph.setBottomMargin(0);
	fmt_paragraph.setLeftMargin(10);

	fmt_table.setTopMargin(5);
	fmt_table.setRightMargin(10);
	fmt_table.setBottomMargin(5);
	fmt_table.setLeftMargin(10);
	fmt_table.setBorder(0);
	fmt_table.setBorderStyle(QTextFrameFormat::BorderStyle_None);
	fmt_table.setCellSpacing(5);

	fmt_center.setAlignment(Qt::AlignHCenter);

QColor color = settings->value("Output/Colors/Text", DEF_TEXT_COLOR).value<QColor>();
QColor hilight;
	if (color.value() < 192)
		hilight.setHsv(color.hue(), color.saturation(), 127 + qRound(color.value() / 2));
	else
		hilight.setHsv(color.hue(), color.saturation(), color.value() / 2);

	solutionText->document()->setDefaultStyleSheet(QString("* {color: %1;}").arg(color.name()));
	fmt_default.setForeground(QBrush(color));

	fmt_selected.setForeground(QBrush(settings->value("Output/Colors/Selected", DEF_SELECTED_COLOR).value<QColor>()));
	fmt_selected.setFontWeight(QFont::Bold);

	fmt_alternate.setForeground(QBrush(settings->value("Output/Colors/Alternate", DEF_ALTERNATE_COLOR).value<QColor>()));
	fmt_alternate.setFontWeight(QFont::Bold);
	fmt_altlist.setForeground(QBrush(hilight));

	solutionText->setTextColor(color);
}

void MainWindow::loadLangList()
{
QDir dir(PATH_L10N, "tspsg_*.qm", QDir::Name | QDir::IgnoreCase, QDir::Files);
	if (!dir.exists())
		return;
QFileInfoList langs = dir.entryInfoList();
	if (langs.size() <= 0)
		return;
QAction *a;
QTranslator t;
QString name;
	for (int k = 0; k < langs.size(); k++) {
		QFileInfo lang = langs.at(k);
		if (lang.completeBaseName().compare("tspsg_en", Qt::CaseInsensitive) && t.load(lang.completeBaseName(), PATH_L10N)) {
			name = t.translate("--------", "LANGNAME", "Please, provide a native name of your translation language here.");
			a = menuSettingsLanguage->addAction(name);
			a->setStatusTip(QString("Set application language to %1").arg(name));
			a->setData(lang.completeBaseName().mid(6));
			a->setCheckable(true);
			a->setActionGroup(groupSettingsLanguageList);
			if (settings->value("Language", QLocale::system().name()).toString().startsWith(lang.completeBaseName().mid(6)))
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
		ad = settings->value("Language", "").toString().isEmpty();
		lng = settings->value("Language", QLocale::system().name()).toString();
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
		translator = NULL;
	}

	if (lng == "en")
		return true;

	// Trying to load system Qt library translation...
	qtTranslator = new QTranslator(this);
	if (qtTranslator->load("qt_" + lng, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
		qApp->installTranslator(qtTranslator);
	else {
		// No luck. Let's try to load a bundled one.
		if (qtTranslator->load("qt_" + lng, PATH_L10N))
			qApp->installTranslator(qtTranslator);
		else {
			// Qt library translation unavailable
			delete qtTranslator;
			qtTranslator = NULL;
		}
	}

	// Now let's load application translation.
	translator = new QTranslator(this);
	if (translator->load("tspsg_" + lng, PATH_L10N))
		qApp->installTranslator(translator);
	else {
		delete translator;
		translator = NULL;
		if (!ad) {
			settings->remove("Language");
			QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
			if (isVisible())
				QMessageBox::warning(this, tr("Language Change"), tr("Unable to load the translation language.\nFalling back to autodetection."));
			else
				QMessageBox::warning(NULL, tr("Language Change"), tr("Unable to load the translation language.\nFalling back to autodetection."));
			QApplication::restoreOverrideCursor();
		}
		return false;
	}
	return true;
}

bool MainWindow::maybeSave()
{
	if (!isWindowModified())
		return true;
int res = QMessageBox::warning(this, tr("Unsaved Changes"), tr("Would you like to save changes in the current task?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	if (res == QMessageBox::Save)
		return actionFileSaveTriggered();
	else if (res == QMessageBox::Cancel)
		return false;
	else
		return true;
}

void MainWindow::outputMatrix(QTextCursor &cur, const TMatrix &matrix)
{
int n = spinCities->value();
QTextTable *table = cur.insertTable(n, n, fmt_table);

	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			cur = table->cellAt(r, c).firstCursorPosition();
			cur.setBlockFormat(fmt_center);
			cur.setBlockCharFormat(fmt_default);
			if (matrix.at(r).at(c) == INFINITY)
				cur.insertText(INFSTR);
			else
				cur.insertText(isInteger(matrix.at(r).at(c)) ? QString("%1").arg(matrix.at(r).at(c)) : QString("%1").arg(matrix.at(r).at(c), 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()));
		}
		QApplication::processEvents();
	}
	cur.movePosition(QTextCursor::End);
}

void MainWindow::outputMatrix(QTextCursor &cur, const SStep &step)
{
int n = spinCities->value();
QTextTable *table = cur.insertTable(n, n, fmt_table);

	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			cur = table->cellAt(r, c).firstCursorPosition();
			cur.setBlockFormat(fmt_center);
			if (step.matrix.at(r).at(c) == INFINITY)
				cur.insertText(INFSTR, fmt_default);
			else if ((r == step.candidate.nRow) && (c == step.candidate.nCol))
				cur.insertText(isInteger(step.matrix.at(r).at(c)) ? QString("%1").arg(step.matrix.at(r).at(c)) : QString("%1").arg(step.matrix.at(r).at(c), 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()), fmt_selected);
			else {
SCandidate cand;
				cand.nRow = r;
				cand.nCol = c;
				if (step.alts.contains(cand))
					cur.insertText(isInteger(step.matrix.at(r).at(c)) ? QString("%1").arg(step.matrix.at(r).at(c)) : QString("%1").arg(step.matrix.at(r).at(c), 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()), fmt_alternate);
				else
					cur.insertText(isInteger(step.matrix.at(r).at(c)) ? QString("%1").arg(step.matrix.at(r).at(c)) : QString("%1").arg(step.matrix.at(r).at(c), 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()), fmt_default);
			}
		}
		QApplication::processEvents();
	}

	cur.movePosition(QTextCursor::End);
}

void MainWindow::retranslateUi(bool all)
{
	if (all)
		Ui::MainWindow::retranslateUi(this);

	actionSettingsLanguageEnglish->setStatusTip(tr("Set application language to %1").arg("English"));

#ifndef QT_NO_PRINTER
	actionFilePrintPreview->setText(QApplication::translate("MainWindow", "P&rint Preview...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
	actionFilePrintPreview->setToolTip(QApplication::translate("MainWindow", "Preview solution results", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
	actionFilePrintPreview->setStatusTip(QApplication::translate("MainWindow", "Preview current solution results before printing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

	actionFilePrint->setText(QApplication::translate("MainWindow", "&Print...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
	actionFilePrint->setToolTip(QApplication::translate("MainWindow", "Print solution", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
	actionFilePrint->setStatusTip(QApplication::translate("MainWindow", "Print current solution results", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
	actionFilePrint->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_PRINTER
#ifdef Q_OS_WIN32
	actionHelpCheck4Updates->setText(tr("Check for &Updates..."));
#ifndef QT_NO_TOOLTIP
	actionHelpCheck4Updates->setToolTip(tr("Check for %1 updates").arg(QApplication::applicationName()));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
	actionHelpCheck4Updates->setStatusTip(tr("Check for %1 updates").arg(QApplication::applicationName()));
#endif // QT_NO_STATUSTIP
#endif // Q_OS_WIN32
}

bool MainWindow::saveTask() {
QStringList filters(tr("%1 Task File").arg("TSPSG") + " (*.tspt)");
	filters.append(tr("All Files") + " (*)");
QString file;
	if (fileName.endsWith(".tspt", Qt::CaseInsensitive))
		file = fileName;
	else
		file = QFileInfo(fileName).canonicalPath() + "/" + QFileInfo(fileName).completeBaseName() + ".tspt";

QFileDialog::Options opts = settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool() ? QFileDialog::Options() : QFileDialog::DontUseNativeDialog;
	file = QFileDialog::getSaveFileName(this, tr("Task Save"), file, filters.join(";;"), NULL, opts);

	if (file.isEmpty())
		return false;
	if (tspmodel->saveTask(file)) {
		setFileName(file);
		setWindowModified(false);
		return true;
	}
	return false;
}

void MainWindow::setFileName(const QString &fileName)
{
	this->fileName = fileName;
	setWindowTitle(QString("%1[*] - %2").arg(QFileInfo(fileName).completeBaseName()).arg(tr("Travelling Salesman Problem")));
}

void MainWindow::setupUi()
{
	Ui::MainWindow::setupUi(this);

#if QT_VERSION >= 0x040600
	setToolButtonStyle(Qt::ToolButtonFollowStyle);
#endif

#ifndef HANDHELD
QStatusBar *statusbar = new QStatusBar(this);
	statusbar->setObjectName("statusbar");
	setStatusBar(statusbar);
#endif // HANDHELD

#ifdef Q_OS_WINCE_WM
	menuBar()->setDefaultAction(menuFile->menuAction());

QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(tabWidget);
	setCentralWidget(scrollArea);
#else
	setCentralWidget(tabWidget);
#endif // Q_OS_WINCE_WM

	//! \hack HACK: A little hack for toolbar icons to have a sane size.
#ifdef Q_OS_WINCE_WM
	toolBar->setIconSize(QSize(logicalDpiX() / 4, logicalDpiY() / 4));
#elif defined(Q_OS_SYMBIAN)
	toolBar->setIconSize(QSize(logicalDpiX() / 5, logicalDpiY() / 5));
#endif // Q_OS_WINCE_WM

	solutionText->document()->setDefaultFont(settings->value("Output/Font", QFont(DEF_FONT_FAMILY, DEF_FONT_SIZE)).value<QFont>());
	solutionText->setWordWrapMode(QTextOption::WordWrap);

#ifndef QT_NO_PRINTER
	actionFilePrintPreview = new QAction(this);
	actionFilePrintPreview->setObjectName("actionFilePrintPreview");
	actionFilePrintPreview->setEnabled(false);
	actionFilePrintPreview->setIcon(QIcon(":/images/icons/document_preview.png"));

	actionFilePrint = new QAction(this);
	actionFilePrint->setObjectName("actionFilePrint");
	actionFilePrint->setEnabled(false);
	actionFilePrint->setIcon(QIcon(":/images/icons/fileprint.png"));

	menuFile->insertAction(actionFileExit,actionFilePrintPreview);
	menuFile->insertAction(actionFileExit,actionFilePrint);
	menuFile->insertSeparator(actionFileExit);

	toolBar->insertAction(actionSettingsPreferences,actionFilePrint);
#endif // QT_NO_PRINTER
#ifdef Q_OS_WIN32
	actionHelpCheck4Updates = new QAction(this);
	actionHelpCheck4Updates->setEnabled(hasUpdater());
	menuHelp->insertAction(actionHelpAboutQt, actionHelpCheck4Updates);
	menuHelp->insertSeparator(actionHelpAboutQt);
#endif // Q_OS_WIN32

	groupSettingsLanguageList = new QActionGroup(this);
	actionSettingsLanguageEnglish->setData("en");
	actionSettingsLanguageEnglish->setActionGroup(groupSettingsLanguageList);
	loadLangList();
	actionSettingsLanguageAutodetect->setChecked(settings->value("Language", "").toString().isEmpty());

	spinCities->setMaximum(MAX_NUM_CITIES);

	retranslateUi(false);

#ifdef Q_OS_WIN32
	// Adding some eyecandy in Vista and 7 :-)
	if (QtWin::isCompositionEnabled() && settings->value("UseTranslucency", DEF_USE_TRANSLUCENCY).toBool())  {
		toggleTranclucency(true);
	}
#endif // Q_OS_WIN32
}

void MainWindow::toggleSolutionActions(bool enable)
{
	buttonSaveSolution->setEnabled(enable);
	actionFileSaveAsSolution->setEnabled(enable);
	solutionText->setEnabled(enable);
#ifndef QT_NO_PRINTER
	actionFilePrint->setEnabled(enable);
	actionFilePrintPreview->setEnabled(enable);
#endif // QT_NO_PRINTER
}

void MainWindow::toggleTranclucency(bool enable)
{
#ifdef Q_OS_WIN32
	toggleStyle(labelVariant, enable);
	toggleStyle(labelCities, enable);
	toggleStyle(statusBar(), enable);
	tabWidget->setDocumentMode(enable);
	QtWin::enableBlurBehindWindow(this, enable);
#else
	Q_UNUSED(enable);
#endif // Q_OS_WIN32
}
