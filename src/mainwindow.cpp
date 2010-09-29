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

#ifdef Q_OS_WIN32
	#include "shobjidl.h"
#endif

#ifdef _T_T_L_
#include "_.h"
_C_ _R_ _Y_ _P_ _T_
#endif

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

	if (settings->contains("Style")) {
QStyle *s = QStyleFactory::create(settings->value("Style").toString());
		if (s != NULL)
			QApplication::setStyle(s);
		else
			settings->remove("Style");
	}

	loadLanguage();
	setupUi();
	setAcceptDrops(true);

	initDocStyleSheet();

#ifndef QT_NO_PRINTER
	printer = new QPrinter(QPrinter::HighResolution);
#endif // QT_NO_PRINTER

#ifdef Q_OS_WINCE_WM
	currentGeometry = QApplication::desktop()->availableGeometry(0);
	// We need to react to SIP show/hide and resize the window appropriately
	connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), SLOT(desktopResized(int)));
#endif // Q_OS_WINCE_WM
	connect(actionFileNew, SIGNAL(triggered()), SLOT(actionFileNewTriggered()));
	connect(actionFileOpen, SIGNAL(triggered()), SLOT(actionFileOpenTriggered()));
	connect(actionFileSave, SIGNAL(triggered()), SLOT(actionFileSaveTriggered()));
	connect(actionFileSaveAsTask, SIGNAL(triggered()), SLOT(actionFileSaveAsTaskTriggered()));
	connect(actionFileSaveAsSolution, SIGNAL(triggered()), SLOT(actionFileSaveAsSolutionTriggered()));
#ifndef QT_NO_PRINTER
	connect(actionFilePrintPreview, SIGNAL(triggered()), SLOT(actionFilePrintPreviewTriggered()));
	connect(actionFilePrint, SIGNAL(triggered()), SLOT(actionFilePrintTriggered()));
#endif // QT_NO_PRINTER
#ifndef HANDHELD
	connect(actionSettingsToolbarsConfigure, SIGNAL(triggered()), SLOT(actionSettingsToolbarsConfigureTriggered()));
#endif // HANDHELD
	connect(actionSettingsPreferences, SIGNAL(triggered()), SLOT(actionSettingsPreferencesTriggered()));
	if (actionHelpCheck4Updates != NULL)
		connect(actionHelpCheck4Updates, SIGNAL(triggered()), SLOT(actionHelpCheck4UpdatesTriggered()));
	connect(actionSettingsLanguageAutodetect, SIGNAL(triggered(bool)), SLOT(actionSettingsLanguageAutodetectTriggered(bool)));
	connect(groupSettingsLanguageList, SIGNAL(triggered(QAction *)), SLOT(groupSettingsLanguageListTriggered(QAction *)));
	connect(actionSettingsStyleSystem, SIGNAL(triggered(bool)), SLOT(actionSettingsStyleSystemTriggered(bool)));
	connect(groupSettingsStyleList, SIGNAL(triggered(QAction*)), SLOT(groupSettingsStyleListTriggered(QAction*)));
	connect(actionHelpAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(actionHelpAbout, SIGNAL(triggered()), SLOT(actionHelpAboutTriggered()));

	connect(buttonSolve, SIGNAL(clicked()), SLOT(buttonSolveClicked()));
	connect(buttonRandom, SIGNAL(clicked()), SLOT(buttonRandomClicked()));
	connect(buttonBackToTask, SIGNAL(clicked()), SLOT(buttonBackToTaskClicked()));
	connect(spinCities, SIGNAL(valueChanged(int)), SLOT(spinCitiesValueChanged(int)));

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
#endif // HANDHELD

	tspmodel = new CTSPModel(this);
	taskView->setModel(tspmodel);
	connect(tspmodel, SIGNAL(numCitiesChanged(int)), SLOT(numCitiesChanged(int)));
	connect(tspmodel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
	connect(tspmodel, SIGNAL(layoutChanged()), SLOT(dataChanged()));
	if ((QCoreApplication::arguments().count() > 1) && (tspmodel->loadTask(QCoreApplication::arguments().at(1))))
		setFileName(QCoreApplication::arguments().at(1));
	else {
		setFileName();
		spinCities->setValue(settings->value("NumCities",DEF_NUM_CITIES).toInt());
		spinCitiesValueChanged(spinCities->value());
	}
	setWindowModified(false);

	if ((actionHelpCheck4Updates != NULL)
		&& (settings->value("Check4Updates/Enabled", DEF_CHECK_FOR_UPDATES).toBool())
		&& (QDate(qvariant_cast<QDate>(settings->value("Check4Updates/LastAttempt"))).daysTo(QDate::currentDate()) >= settings->value("Check4Updates/Interval", DEF_UPDATE_CHECK_INTERVAL).toInt())) {
		check4Updates(true);
	}
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

QString file;
	if ((fileName == tr("Untitled") + ".tspt") && settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool())
		file = settings->value(OS"/LastUsed/TaskLoadPath").toString();
	else
		file = QFileInfo(fileName).path();
QFileDialog::Options opts = settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool() ? QFileDialog::Options() : QFileDialog::DontUseNativeDialog;
	file = QFileDialog::getOpenFileName(this, tr("Task Load"), file, filters.join(";;"), NULL, opts);
	if (file.isEmpty() || !QFileInfo(file).isFile())
		return;
	if (settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool())
		settings->setValue(OS"/LastUsed/TaskLoadPath", QFileInfo(file).path());

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
	if ((fileName == tr("Untitled") + ".tspt") || !fileName.endsWith(".tspt", Qt::CaseInsensitive))
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
	if (selectedFile.isEmpty()) {
		if (settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool()) {
			selectedFile = settings->value(OS"/LastUsed/SolutionSavePath").toString();
		}
	} else
		selectedFile = QFileInfo(selectedFile).path();
	if (!selectedFile.isEmpty())
		selectedFile.append("/");
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
	filters.append(tr("OpenDocument Files") + " (*.odt)");
	filters.append(tr("All Files") + " (*)");

QFileDialog::Options opts(settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool() ? QFileDialog::Options() : QFileDialog::DontUseNativeDialog);
QString file = QFileDialog::getSaveFileName(this, QString(), selectedFile, filters.join(";;"), NULL, opts);
	if (file.isEmpty())
		return;
	selectedFile = file;
	if (settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool())
		settings->setValue(OS"/LastUsed/SolutionSavePath", QFileInfo(selectedFile).path());
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
	if (selectedFile.endsWith(".htm", Qt::CaseInsensitive) || selectedFile.endsWith(".html", Qt::CaseInsensitive)) {
QFile file(selectedFile);
		if (!file.open(QFile::WriteOnly)) {
			QApplication::restoreOverrideCursor();
			QMessageBox::critical(this, tr("Solution Save"), tr("Unable to save the solution.\nError: %1").arg(file.errorString()));
			return;
		}
QFileInfo fi(selectedFile);
QString format = settings->value("Output/GraphImageFormat", DEF_GRAPH_IMAGE_FORMAT).toString();
#if !defined(NOSVG)
	if (!QImageWriter::supportedImageFormats().contains(format.toAscii()) && (format != "svg")) {
#else // NOSVG
	if (!QImageWriter::supportedImageFormats().contains(format.toAscii())) {
#endif // NOSVG
		format = DEF_GRAPH_IMAGE_FORMAT;
		settings->remove("Output/GraphImageFormat");
	}
QString html = solutionText->document()->toHtml("UTF-8"),
		img =  fi.completeBaseName() + "." + format;
		html.replace(QRegExp("font-family:([^;]*);"), "font-family:\\1, 'DejaVu Sans Mono', 'Courier New', Courier, monospace;");
		html.replace(QRegExp("<img\\s+src=\"tspsg://graph.pic\""), QString("<img src=\"%1\" alt=\"%2\"").arg(img, tr("Solution Graph")));

		// Saving solution text as HTML
QTextStream ts(&file);
		ts.setCodec(QTextCodec::codecForName("UTF-8"));
		ts << html;
		file.close();

		// Saving solution graph as SVG or PNG (depending on settings and SVG support)
#if !defined(NOSVG)
		if (format == "svg") {
QSvgGenerator svg;
			svg.setSize(QSize(graph.width() + 2, graph.height() + 2));
			svg.setResolution(graph.logicalDpiX());
			svg.setFileName(fi.path() + "/" + img);
			svg.setTitle(tr("Solution Graph"));
			svg.setDescription(tr("Generated with %1").arg(QApplication::applicationName()));
QPainter p;
			p.begin(&svg);
			p.drawPicture(1, 1, graph);
			p.end();
		} else {
#endif // NOSVG
QImage i(graph.width() + 2, graph.height() + 2, QImage::Format_ARGB32);
			i.fill(0x00FFFFFF);
QPainter p;
			p.begin(&i);
			p.drawPicture(1, 1, graph);
			p.end();
QImageWriter pic(fi.path() + "/" + img);
			if (pic.supportsOption(QImageIOHandler::Description)) {
				pic.setText("Title", "Solution Graph");
				pic.setText("Software", QApplication::applicationName());
			}
			if (format == "png")
				pic.setQuality(5);
			else if (format == "jpeg")
				pic.setQuality(80);
			if (!pic.write(i)) {
				QApplication::restoreOverrideCursor();
				QMessageBox::critical(this, tr("Solution Save"), tr("Unable to save the solution graph.\nError: %1").arg(pic.errorString()));
				return;
			}
#if !defined(NOSVG)
		}
#endif // NOSVG
	} else {
QTextDocumentWriter dw(selectedFile);
		if (!selectedFile.endsWith(".odt",Qt::CaseInsensitive))
			dw.setFormat("plaintext");
		if (!dw.write(solutionText->document()))
			QMessageBox::critical(this, tr("Solution Save"), tr("Unable to save the solution.\nError: %1").arg(dw.device()->errorString()));
	}
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
	if (pd.exec() != QDialog::Accepted)
		return;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	solutionText->print(printer);
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
		QMessageBox::information(this, tr("Language change"), tr("Language will be autodetected on the next %1 start.").arg(QApplication::applicationName()));
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

void MainWindow::actionSettingsStyleSystemTriggered(bool checked)
{
	if (checked) {
		settings->remove("Style");
		QMessageBox::information(this, tr("Style Change"), tr("To apply the default style you need to restart %1.").arg(QApplication::applicationName()));
	} else {
		settings->setValue("Style", groupSettingsStyleList->checkedAction()->text());
	}
}

void MainWindow::groupSettingsStyleListTriggered(QAction *action)
{
QStyle *s = QStyleFactory::create(action->text());
	if (s != NULL) {
		QApplication::setStyle(s);
		settings->setValue("Style", action->text());
		actionSettingsStyleSystem->setChecked(false);
	}
}

#ifndef HANDHELD
void MainWindow::actionSettingsToolbarsConfigureTriggered()
{
QtToolBarDialog dlg(this);
	dlg.setToolBarManager(toolBarManager);
	dlg.exec();
QToolButton *tb = static_cast<QToolButton *>(toolBarMain->widgetForAction(actionFileSave));
	if (tb != NULL) {
		tb->setMenu(menuFileSaveAs);
		tb->setPopupMode(QToolButton::MenuButtonPopup);
		tb->resize(tb->sizeHint());
	}

	loadToolbarList();
}
#endif // HANDHELD

void MainWindow::actionHelpCheck4UpdatesTriggered()
{
	if (!hasUpdater()) {
		QMessageBox::warning(this, tr("Unsupported Feature"), tr("Sorry, but this feature is not supported on your platform\nor support for this feature was not installed."));
		return;
	}

	check4Updates();
}

void MainWindow::actionHelpAboutTriggered()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

QString title;
	title += QString("<b>%1</b><br>").arg(QApplication::applicationName());
	title += QString("%1: <b>%2</b><br>").arg(tr("Version"), QApplication::applicationVersion());
#ifndef HANDHELD
	title += QString("<b>&copy; 2007-%1 <a href=\"http://%2/\">%3</a></b><br>").arg(QDate::currentDate().toString("yyyy"), QApplication::organizationDomain(), QApplication::organizationName());
#endif // HANDHELD
	title += QString("<b><a href=\"http://tspsg.info/\">http://tspsg.info/</a></b>");

QString about;
	about += QString("%1: <b>%2</b><br>").arg(tr("Target OS (ARCH)"), PLATFROM);
#ifndef STATIC_BUILD
	about += QString("%1 (%2):<br>").arg(tr("Qt library"), tr("shared"));
	about += QString("&nbsp;&nbsp;&nbsp;&nbsp;%1: <b>%2</b><br>").arg(tr("Build time"), QT_VERSION_STR);
	about += QString("&nbsp;&nbsp;&nbsp;&nbsp;%1: <b>%2</b><br>").arg(tr("Runtime"), qVersion());
#else
	about += QString("%1: <b>%2</b> (%3)<br>").arg(tr("Qt library"), QT_VERSION_STR, tr("static"));
#endif // STATIC_BUILD
	about += tr("Buid <b>%1</b>, built on <b>%2</b> at <b>%3</b> with <b>%4</b> compiler.").arg(BUILD_NUMBER).arg(__DATE__).arg(__TIME__).arg(COMPILER) + "<br>";
	about += QString("%1: <b>%2</b><br>").arg(tr("Algorithm"), CTSPSolver::getVersionId());
	about += "<br>";
	about += tr("This program is free software: you can redistribute it and/or modify<br>\n"
		"it under the terms of the GNU General Public License as published by<br>\n"
		"the Free Software Foundation, either version 3 of the License, or<br>\n"
		"(at your option) any later version.<br>\n"
		"<br>\n"
		"This program is distributed in the hope that it will be useful,<br>\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of<br>\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the<br>\n"
		"GNU General Public License for more details.<br>\n"
		"<br>\n"
		"You should have received a copy of the GNU General Public License<br>\n"
		"along with TSPSG.  If not, see <a href=\"http://www.gnu.org/licenses/\">www.gnu.org/licenses/</a>.");

QString credits;
	credits += tr("%1 was created using <b>Qt&nbsp;framework</b> licensed "
		"under the terms of the GNU Lesser General Public License,<br>\n"
		"see <a href=\"http://qt.nokia.com/\">qt.nokia.com</a><br>\n"
		"<br>\n"
		"Most icons used in %1 are part of <b>Oxygen&nbsp;Icons</b> project "
		"licensed according to the GNU Lesser General Public License,<br>\n"
		"see <a href=\"http://www.oxygen-icons.org/\">www.oxygen-icons.org</a><br>\n"
		"<br>\n"
		"Country flag icons used in %1 are part of the free "
		"<b>Flag&nbsp;Icons</b> collection created by <b>IconDrawer</b>,<br>\n"
		"see <a href=\"http://www.icondrawer.com/\">www.icondrawer.com</a><br>\n"
		"<br>\n"
		"%1 comes with the default \"embedded\" font <b>DejaVu&nbsp;LGC&nbsp;Sans&nbsp;"
		"Mono</b> from the <b>DejaVu fonts</b> licensed under a Free license</a>,<br>\n"
		"see <a href=\"http://dejavu-fonts.org/\">dejavu-fonts.org</a>")
			.arg("TSPSG");

QFile f(":/files/COPYING");
	f.open(QIODevice::ReadOnly);

QString translation = QApplication::translate("--------", "AUTHORS %1", "Please, provide translator credits here. %1 will be replaced with VERSION");
	if ((translation != "AUTHORS %1") && (translation.contains("%1"))) {
QString about = QApplication::translate("--------", "VERSION", "Please, provide your translation version here.");
		if (about != "VERSION")
			translation = translation.arg(about);
	}

QDialog *dlg = new QDialog(this);
QLabel *lblIcon = new QLabel(dlg),
	*lblTitle = new QLabel(dlg);
#ifdef HANDHELD
QLabel *lblSubTitle = new QLabel(QString("<b>&copy; 2007-%1 <a href=\"http://%2/\">%3</a></b>").arg(QDate::currentDate().toString("yyyy"), QApplication::organizationDomain(), QApplication::organizationName()), dlg);
#endif // HANDHELD
QTabWidget *tabs = new QTabWidget(dlg);
QTextBrowser *txtAbout = new QTextBrowser(dlg);
QTextBrowser *txtLicense = new QTextBrowser(dlg);
QTextBrowser *txtCredits = new QTextBrowser(dlg);
QVBoxLayout *vb = new QVBoxLayout();
QHBoxLayout *hb1 = new QHBoxLayout(),
	*hb2 = new QHBoxLayout();
QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, dlg);

	lblTitle->setOpenExternalLinks(true);
	lblTitle->setText(title);
	lblTitle->setAlignment(Qt::AlignTop);
	lblTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
#ifndef HANDHELD
	lblTitle->setStyleSheet(QString("QLabel {background-color: %1; border-color: %2; border-width: 1px; border-style: solid; border-radius: 4px; padding: 1px;}").arg(palette().alternateBase().color().name(), palette().shadow().color().name()));
#endif // HANDHELD

	lblIcon->setPixmap(QPixmap(":/images/tspsg.png").scaledToHeight(lblTitle->sizeHint().height(), Qt::SmoothTransformation));
	lblIcon->setAlignment(Qt::AlignVCenter);
#ifndef HANDHELD
	lblIcon->setStyleSheet(QString("QLabel {background-color: white; border-color: %1; border-width: 1px; border-style: solid; border-radius: 4px; padding: 1px;}").arg(palette().windowText().color().name()));
#endif // HANDHELD

	hb1->addWidget(lblIcon);
	hb1->addWidget(lblTitle);

	txtAbout->setWordWrapMode(QTextOption::NoWrap);
	txtAbout->setOpenExternalLinks(true);
	txtAbout->setHtml(about);
	txtAbout->moveCursor(QTextCursor::Start);
	txtAbout->setFrameShape(QFrame::NoFrame);

//	txtCredits->setWordWrapMode(QTextOption::NoWrap);
	txtCredits->setOpenExternalLinks(true);
	txtCredits->setHtml(credits);
	txtCredits->moveCursor(QTextCursor::Start);
	txtCredits->setFrameShape(QFrame::NoFrame);

	txtLicense->setWordWrapMode(QTextOption::NoWrap);
	txtLicense->setOpenExternalLinks(true);
	txtLicense->setText(f.readAll());
	txtLicense->moveCursor(QTextCursor::Start);
	txtLicense->setFrameShape(QFrame::NoFrame);

	bb->button(QDialogButtonBox::Ok)->setCursor(QCursor(Qt::PointingHandCursor));
	bb->button(QDialogButtonBox::Ok)->setIcon(GET_ICON("dialog-ok"));

	hb2->addWidget(bb);

#ifdef Q_OS_WINCE_WM
	vb->setMargin(3);
#endif // Q_OS_WINCE_WM
	vb->addLayout(hb1);
#ifdef HANDHELD
	vb->addWidget(lblSubTitle);
#endif // HANDHELD

	tabs->addTab(txtAbout, tr("About"));
	tabs->addTab(txtLicense, tr("License"));
	tabs->addTab(txtCredits, tr("Credits"));
	if (translation != "AUTHORS %1") {
QTextBrowser *txtTranslation = new QTextBrowser(dlg);
//		txtTranslation->setWordWrapMode(QTextOption::NoWrap);
		txtTranslation->setOpenExternalLinks(true);
		txtTranslation->setText(translation);
		txtTranslation->moveCursor(QTextCursor::Start);
		txtTranslation->setFrameShape(QFrame::NoFrame);

		tabs->addTab(txtTranslation, tr("Translation"));
	}
#ifndef HANDHELD
	tabs->setStyleSheet(QString("QTabWidget::pane {background-color: %1; border-color: %3; border-width: 1px; border-style: solid; border-bottom-left-radius: 4px; border-bottom-right-radius: 4px; padding: 1px;} QTabBar::tab {background-color: %2; border-color: %3; border-width: 1px; border-style: solid; border-bottom: none; border-top-left-radius: 4px; border-top-right-radius: 4px; padding: 2px 6px;} QTabBar::tab:selected {background-color: %4;} QTabBar::tab:!selected {margin-top: 1px;}").arg(palette().base().color().name(), palette().button().color().name(), palette().shadow().color().name(), palette().light().color().name()));
#endif // HANDHELD

	vb->addWidget(tabs);
	vb->addLayout(hb2);

	dlg->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	dlg->setWindowTitle(tr("About %1").arg(QApplication::applicationName()));
	dlg->setWindowIcon(GET_ICON("help-about"));
	dlg->setLayout(vb);

	connect(bb, SIGNAL(accepted()), dlg, SLOT(accept()));

#ifdef Q_OS_WIN32
	// Adding some eyecandy in Vista and 7 :-)
	if (QtWin::isCompositionEnabled())  {
		QtWin::enableBlurBehindWindow(dlg, true);
	}
#endif // Q_OS_WIN32

	dlg->resize(450, 350);
	QApplication::restoreOverrideCursor();

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
QPushButton *cancel = new QPushButton(&pd);
	cancel->setIcon(GET_ICON("dialog-cancel"));
	cancel->setText(QApplication::translate("QProgressDialog", "Cancel", "No need to translate this. This translation will be taken from Qt translation files."));
	pd.setCancelButton(cancel);
	pd.setMaximum(n);
	pd.setAutoReset(false);
	pd.setLabelText(tr("Calculating optimal route..."));
	pd.setWindowTitle(tr("Solution Progress"));
	pd.setWindowModality(Qt::ApplicationModal);
	pd.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	pd.show();

#ifdef Q_OS_WIN32
HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (LPVOID*)&tl);
	if (SUCCEEDED(hr)) {
		hr = tl->HrInit();
		if (FAILED(hr)) {
			tl->Release();
			tl = NULL;
		} else {
//			tl->SetProgressState(winId(), TBPF_INDETERMINATE);
			tl->SetProgressValue(winId(), 0, n * 2);
		}
	}
#endif

CTSPSolver solver;
	solver.setCleanupOnCancel(false);
	connect(&solver, SIGNAL(routePartFound(int)), &pd, SLOT(setValue(int)));
	connect(&pd, SIGNAL(canceled()), &solver, SLOT(cancel()));
#ifdef Q_OS_WIN32
	if (tl != NULL)
		connect(&solver, SIGNAL(routePartFound(int)), SLOT(solverRoutePartFound(int)));
#endif
SStep *root = solver.solve(n, matrix);
#ifdef Q_OS_WIN32
	if (tl != NULL)
		disconnect(&solver, SIGNAL(routePartFound(int)), this, SLOT(solverRoutePartFound(int)));
#endif
	disconnect(&solver, SIGNAL(routePartFound(int)), &pd, SLOT(setValue(int)));
	disconnect(&pd, SIGNAL(canceled()), &solver, SLOT(cancel()));
	if (!root) {
		pd.reset();
		if (!solver.wasCanceled()) {
#ifdef Q_OS_WIN32
			if (tl != NULL) {
//				tl->SetProgressValue(winId(), n, n * 2);
				tl->SetProgressState(winId(), TBPF_ERROR);
			}
#endif
			QApplication::alert(this);
			QMessageBox::warning(this, tr("Solution Result"), tr("Unable to find a solution.\nMaybe, this task has no solution."));
		}
		pd.setLabelText(tr("Cleaning up..."));
		pd.setMaximum(0);
		pd.setCancelButton(NULL);
		pd.show();
#ifdef Q_OS_WIN32
		if (tl != NULL)
			tl->SetProgressState(winId(), TBPF_INDETERMINATE);
#endif
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

#ifndef QT_NO_CONCURRENT
QFuture<void> f = QtConcurrent::run(&solver, &CTSPSolver::cleanup, false);
		while (!f.isFinished()) {
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		}
#else
		solver.cleanup(true);
#endif
		pd.reset();
#ifdef Q_OS_WIN32
		if (tl != NULL) {
			tl->SetProgressState(winId(), TBPF_NOPROGRESS);
			tl->Release();
			tl = NULL;
		}
#endif
		return;
	}
	pb->setFormat(tr("Generating header"));
	pd.setLabelText(tr("Generating solution output..."));
	pd.setMaximum(solver.getTotalSteps() + 1);
	pd.setValue(0);

#ifdef Q_OS_WIN32
	if (tl != NULL)
		tl->SetProgressValue(winId(), spinCities->value(), spinCities->value() + solver.getTotalSteps() + 1);
#endif

	solutionText->clear();
	solutionText->setDocumentTitle(tr("Solution of Variant #%1 Task").arg(spinVariant->value()));

QPainter pic;
	if (settings->value("Output/ShowGraph", DEF_SHOW_GRAPH).toBool()) {
		pic.begin(&graph);
		pic.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
QFont font = qvariant_cast<QFont>(settings->value("Output/Font", QFont(DEF_FONT_FACE, 9)));
		if (settings->value("Output/HQGraph", DEF_HQ_GRAPH).toBool()) {
			font.setWeight(QFont::DemiBold);
			font.setPointSizeF(font.pointSizeF() * 2);
		}
		pic.setFont(font);
		pic.setBrush(QBrush(QColor(Qt::white)));
		if (settings->value("Output/HQGraph", DEF_HQ_GRAPH).toBool()) {
QPen pen = pic.pen();
			pen.setWidth(2);
			pic.setPen(pen);
		}
		pic.setBackgroundMode(Qt::OpaqueMode);
	}

QTextDocument *doc = solutionText->document();
QTextCursor cur(doc);

	cur.beginEditBlock();
	cur.setBlockFormat(fmt_paragraph);
	cur.insertText(tr("Variant #%1 Task").arg(spinVariant->value()), fmt_default);
	cur.insertBlock(fmt_paragraph);
	cur.insertText(tr("Task:"));
	outputMatrix(cur, matrix);
	if (settings->value("Output/ShowGraph", DEF_SHOW_GRAPH).toBool()) {
#ifdef _T_T_L_
		_b_ _i_ _z_ _a_ _r_ _r_ _e_
#endif
		drawNode(pic, 0);
	}
	cur.insertHtml("<hr>");
	cur.insertBlock(fmt_paragraph);
int imgpos = cur.position();
	cur.insertText(tr("Variant #%1 Solution").arg(spinVariant->value()), fmt_default);
	cur.endEditBlock();

SStep *step = root;
int c = n = 1;
	pb->setFormat(tr("Generating step %v"));
	while ((step->next != SStep::NoNextStep) && (c < spinCities->value())) {
		if (pd.wasCanceled()) {
			pd.setLabelText(tr("Cleaning up..."));
			pd.setMaximum(0);
			pd.setCancelButton(NULL);
			pd.show();
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
#ifdef Q_OS_WIN32
			if (tl != NULL)
				tl->SetProgressState(winId(), TBPF_INDETERMINATE);
#endif
#ifndef QT_NO_CONCURRENT
QFuture<void> f = QtConcurrent::run(&solver, &CTSPSolver::cleanup, false);
			while (!f.isFinished()) {
				QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
			}
#else
			solver.cleanup(true);
#endif
			solutionText->clear();
			toggleSolutionActions(false);
#ifdef Q_OS_WIN32
			if (tl != NULL) {
				tl->SetProgressState(winId(), TBPF_NOPROGRESS);
				tl->Release();
				tl = NULL;
			}
#endif
			return;
		}
		pd.setValue(n);
#ifdef Q_OS_WIN32
		if (tl != NULL)
			tl->SetProgressValue(winId(), spinCities->value() + n, spinCities->value() + solver.getTotalSteps() + 1);
#endif

		cur.beginEditBlock();
		cur.insertBlock(fmt_paragraph);
		cur.insertText(tr("Step #%1").arg(n));
		if (settings->value("Output/ShowMatrix", DEF_SHOW_MATRIX).toBool() && (!settings->value("Output/UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT).toBool() || (settings->value("Output/UseShowMatrixLimit", DEF_USE_SHOW_MATRIX_LIMIT).toBool() && (spinCities->value() <= settings->value("Output/ShowMatrixLimit", DEF_SHOW_MATRIX_LIMIT).toInt())))) {
			outputMatrix(cur, *step);
		}
		cur.insertBlock(fmt_paragraph);
		cur.insertText(tr("Selected route %1 %2 part.").arg((step->next == SStep::RightBranch) ? tr("with") : tr("without")).arg(tr("(%1;%2)").arg(step->candidate.nRow + 1).arg(step->candidate.nCol + 1)), fmt_default);
		if (!step->alts.empty()) {
			SStep::SCandidate cand;
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

		if (settings->value("Output/ShowGraph", DEF_SHOW_GRAPH).toBool()) {
			if (step->prNode != NULL)
				drawNode(pic, n, false, step->prNode);
			if (step->plNode != NULL)
				drawNode(pic, n, true, step->plNode);
		}
		n++;

		if (step->next == SStep::RightBranch) {
			c++;
			step = step->prNode;
		} else if (step->next == SStep::LeftBranch) {
			step = step->plNode;
		} else
			break;
	}
	pb->setFormat(tr("Generating footer"));
	pd.setValue(n);
#ifdef Q_OS_WIN32
	if (tl != NULL)
		tl->SetProgressValue(winId(), spinCities->value() + n, spinCities->value() + solver.getTotalSteps() + 1);
#endif

	cur.beginEditBlock();
	cur.insertBlock(fmt_paragraph);
	if (solver.isOptimal())
		cur.insertText(tr("Optimal path:"));
	else
		cur.insertText(tr("Resulting path:"));

	cur.insertBlock(fmt_paragraph);
	cur.insertText("  " + solver.getSortedPath(tr("City %1")));

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

	if (settings->value("Output/ShowGraph", DEF_SHOW_GRAPH).toBool()) {
		pic.end();

QImage i(graph.width() + 2, graph.height() + 2, QImage::Format_RGB32);
		i.fill(0xFFFFFF);
		pic.begin(&i);
		pic.drawPicture(1, 1, graph);
		pic.end();
		doc->addResource(QTextDocument::ImageResource, QUrl("tspsg://graph.pic"), i);

QTextImageFormat img;
		img.setName("tspsg://graph.pic");
		if (settings->value("Output/HQGraph", DEF_HQ_GRAPH).toBool()) {
			img.setWidth(i.width() / 2);
			img.setHeight(i.height() / 2);
		} else {
			img.setWidth(i.width());
			img.setHeight(i.height());
		}

		cur.setPosition(imgpos);
		cur.insertImage(img, QTextFrameFormat::FloatRight);
	}

	if (settings->value("Output/ScrollToEnd", DEF_SCROLL_TO_END).toBool()) {
		// Scrolling to the end of the text.
		solutionText->moveCursor(QTextCursor::End);
	} else
		solutionText->moveCursor(QTextCursor::Start);

	pd.setLabelText(tr("Cleaning up..."));
	pd.setMaximum(0);
	pd.setCancelButton(NULL);
	QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
#ifdef Q_OS_WIN32
	if (tl != NULL)
		tl->SetProgressState(winId(), TBPF_INDETERMINATE);
#endif
#ifndef QT_NO_CONCURRENT
QFuture<void> f = QtConcurrent::run(&solver, &CTSPSolver::cleanup, false);
	while (!f.isFinished()) {
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}
#else
	solver.cleanup(true);
#endif
	toggleSolutionActions();
	tabWidget->setCurrentIndex(1);
#ifdef Q_OS_WIN32
	if (tl != NULL) {
		tl->SetProgressState(winId(), TBPF_NOPROGRESS);
		tl->Release();
		tl = NULL;
	}
#endif

	pd.reset();
	QApplication::alert(this, 3000);
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

#ifdef Q_OS_WIN32
void MainWindow::solverRoutePartFound(int n)
{
#ifdef Q_OS_WIN32
	tl->SetProgressValue(winId(), n, spinCities->value() * 2);
#else
	Q_UNUSED(n);
#endif // Q_OS_WIN32
}
#endif // Q_OS_WIN32

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

void MainWindow::check4Updates(bool silent)
{
#ifdef Q_OS_WIN32
	if (silent)
		QProcess::startDetached("updater/Update.exe -name=\"TSPSG: TSP Solver and Generator\" -check=\"freeupdate\" -silentcheck");
	else {
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		QProcess::execute("updater/Update.exe -name=\"TSPSG: TSP Solver and Generator\" -check=\"freeupdate\"");
		QApplication::restoreOverrideCursor();
	}
#else
	Q_UNUSED(silent)
#endif
	settings->setValue("Check4Updates/LastAttempt", QDate::currentDate().toString(Qt::ISODate));
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
#ifndef HANDHELD
		if (settings->value("SavePos", DEF_SAVEPOS).toBool()) {
			settings->beginGroup("MainWindow");
			settings->setValue("Geometry", saveGeometry());
			settings->setValue("State", saveState());
			settings->setValue("Toolbars", toolBarManager->saveState());
			settings->endGroup();
		}
#else
		settings->setValue("MainWindow/ToolbarVisible", toolBarMain->isVisible());
#endif // HANDHELD
	} else {
		settings->remove("SettingsReset");
	}

	QMainWindow::closeEvent(ev);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *ev)
{
	if (ev->mimeData()->hasUrls() && (ev->mimeData()->urls().count() == 1)) {
QFileInfo fi(ev->mimeData()->urls().first().toLocalFile());
		if ((fi.suffix() == "tspt") || (fi.suffix() == "zkt"))
			ev->acceptProposedAction();
	}
}

void MainWindow::drawNode(QPainter &pic, int nstep, bool left, SStep *step)
{
int r;
	if (settings->value("Output/HQGraph", DEF_HQ_GRAPH).toBool())
		r = 70;
	else
		r = 35;
qreal x, y;
	if (step != NULL)
		x = left ? r : r * 3.5;
	else
		x = r * 2.25;
	y = r * (3 * nstep + 1);

#ifdef _T_T_L_
	if (nstep == -481124) {
		_t_t_l_(pic, r, x);
		return;
	}
#endif

	pic.drawEllipse(QPointF(x, y), r, r);

	if (step != NULL) {
QFont font;
		if (left) {
			font = pic.font();
			font.setStrikeOut(true);
			pic.setFont(font);
		}
		pic.drawText(QRectF(x - r, y - r, r * 2, r * 2), Qt::AlignCenter, tr("(%1;%2)").arg(step->pNode->candidate.nRow + 1).arg(step->pNode->candidate.nCol + 1) + "\n");
		if (left) {
			font.setStrikeOut(false);
			pic.setFont(font);
		}
		if (step->price != INFINITY) {
			pic.drawText(QRectF(x - r, y - r, r * 2, r * 2), Qt::AlignCenter, isInteger(step->price) ?  QString("\n%1").arg(step->price) : QString("\n%1").arg(step->price, 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()));
		} else {
			pic.drawText(QRectF(x - r, y - r, r * 2, r * 2), Qt::AlignCenter, "\n"INFSTR);
		}
	} else {
		pic.drawText(QRectF(x - r, y - r, r * 2, r * 2), Qt::AlignCenter, tr("Root"));
	}

	if (nstep == 1) {
		pic.drawLine(QPointF(x, y - r), QPointF(r * 2.25, y - 2 * r));
	} else if (nstep > 1) {
		pic.drawLine(QPointF(x, y - r), QPointF((step->pNode->pNode->next == SStep::RightBranch) ? r * 3.5 : r, y - 2 * r));
	}

}

void MainWindow::dropEvent(QDropEvent *ev)
{
	if (maybeSave() && tspmodel->loadTask(ev->mimeData()->urls().first().toLocalFile())) {
		setFileName(ev->mimeData()->urls().first().toLocalFile());
		tabWidget->setCurrentIndex(0);
		setWindowModified(false);
		solutionText->clear();
		toggleSolutionActions(false);

		ev->setDropAction(Qt::CopyAction);
		ev->accept();
	}
}

void MainWindow::initDocStyleSheet()
{
	solutionText->document()->setDefaultFont(qvariant_cast<QFont>(settings->value("Output/Font", QFont(DEF_FONT_FACE, DEF_FONT_SIZE))));

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

	fmt_cell.setAlignment(Qt::AlignHCenter);

	settings->beginGroup("Output/Colors");

QColor color = qvariant_cast<QColor>(settings->value("Text", DEF_TEXT_COLOR));
QColor hilight;
	if (color.value() < 192)
		hilight.setHsv(color.hue(), color.saturation(), 127 + qRound(color.value() / 2));
	else
		hilight.setHsv(color.hue(), color.saturation(), color.value() / 2);

	solutionText->document()->setDefaultStyleSheet(QString("* {color: %1;}").arg(color.name()));
	fmt_default.setForeground(QBrush(color));

	fmt_selected.setForeground(QBrush(qvariant_cast<QColor>(settings->value("Selected", DEF_SELECTED_COLOR))));
	fmt_selected.setFontWeight(QFont::Bold);

	fmt_alternate.setForeground(QBrush(qvariant_cast<QColor>(settings->value("Alternate", DEF_ALTERNATE_COLOR))));
	fmt_alternate.setFontWeight(QFont::Bold);
	fmt_altlist.setForeground(QBrush(hilight));

	settings->endGroup();

	solutionText->setTextColor(color);
}

void MainWindow::loadLangList()
{
QMap<QString, QStringList> langlist;
QFileInfoList langs;
QFileInfo lang;
QString name;
QStringList language, dirs;
QTranslator t;
QDir dir;
	dir.setFilter(QDir::Files);
	dir.setNameFilters(QStringList("tspsg_*.qm"));
	dir.setSorting(QDir::NoSort);

	dirs << PATH_L10N << ":/l10n";
	foreach (QString dirname, dirs) {
		dir.setPath(dirname);
		if (dir.exists()) {
			langs = dir.entryInfoList();
			for (int k = 0; k < langs.size(); k++) {
				lang = langs.at(k);
				if (lang.completeBaseName().compare("tspsg_en", Qt::CaseInsensitive) && !langlist.contains(lang.completeBaseName().mid(6)) && t.load(lang.completeBaseName(), dirname)) {

					language.clear();
					language.append(lang.completeBaseName().mid(6));
					language.append(t.translate("--------", "COUNTRY", "Please, provide an ISO 3166-1 alpha-2 country code for this translation language here (eg., UA).").toLower());
					language.append(t.translate("--------", "LANGNAME", "Please, provide a native name of your translation language here."));
					language.append(t.translate("MainWindow", "Set application language to %1", "").arg(name));

					langlist.insert(language.at(0), language);
				}
			}
		}
	}

QAction *a;
	foreach (language, langlist) {
		a = menuSettingsLanguage->addAction(language.at(2));
		a->setStatusTip(language.at(3));
#if QT_VERSION >= 0x040600
		a->setIcon(QIcon::fromTheme(QString("flag-%1").arg(language.at(1)), QIcon(QString(":/images/icons/l10n/flag-%1.png").arg(language.at(1)))));
#else
		a->setIcon(QIcon(QString(":/images/icons/l10n/flag-%1.png").arg(language.at(1))));
#endif
		a->setData(language.at(0));
		a->setCheckable(true);
		a->setActionGroup(groupSettingsLanguageList);
		if (settings->value("Language", QLocale::system().name()).toString().startsWith(language.at(0)))
			a->setChecked(true);
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
		if (qtTranslator->load("qt_" + lng, PATH_L10N)) {
			// We have a translation in the localization direcotry.
			qApp->installTranslator(qtTranslator);
		} else if (qtTranslator->load("qt_" + lng, ":/l10n")) {
			// We have a translation "built-in" into application resources.
			qApp->installTranslator(qtTranslator);
		} else {
			// Qt library translation unavailable for this language.
			delete qtTranslator;
			qtTranslator = NULL;
		}
	}

	// Now let's load application translation.
	translator = new QTranslator(this);
	if (translator->load("tspsg_" + lng, PATH_L10N)) {
		// We have a translation in the localization directory.
		qApp->installTranslator(translator);
	} else if (translator->load("tspsg_" + lng, ":/l10n")) {
		// We have a translation "built-in" into application resources.
		qApp->installTranslator(translator);
	} else {
		delete translator;
		translator = NULL;
		if (!ad) {
			settings->remove("Language");
			QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
			QMessageBox::warning(isVisible() ? this : NULL, tr("Language Change"), tr("Unable to load the translation language.\nFalling back to autodetection."));
			QApplication::restoreOverrideCursor();
		}
		return false;
	}
	return true;
}

void MainWindow::loadStyleList()
{
	menuSettingsStyle->clear();
QStringList styles = QStyleFactory::keys();
	menuSettingsStyle->insertAction(NULL, actionSettingsStyleSystem);
	actionSettingsStyleSystem->setChecked(!settings->contains("Style"));
	menuSettingsStyle->addSeparator();
QAction *a;
	foreach (QString style, styles) {
		a = menuSettingsStyle->addAction(style);
		a->setData(false);
		a->setStatusTip(tr("Set application style to %1").arg(style));
		a->setCheckable(true);
		a->setActionGroup(groupSettingsStyleList);
		if ((style == settings->value("Style").toString())
			|| QString(QApplication::style()->metaObject()->className()).contains(QRegExp(QString("^Q?%1(Style)?$").arg(QRegExp::escape(style)), Qt::CaseInsensitive))) {
			a->setChecked(true);
		}
	}
}

void MainWindow::loadToolbarList()
{
	menuSettingsToolbars->clear();
#ifndef HANDHELD
	menuSettingsToolbars->insertAction(NULL, actionSettingsToolbarsConfigure);
	menuSettingsToolbars->addSeparator();
QList<QToolBar *> list = toolBarManager->toolBars();
	foreach (QToolBar *t, list) {
		menuSettingsToolbars->insertAction(NULL, t->toggleViewAction());
	}
#else // HANDHELD
	menuSettingsToolbars->insertAction(NULL, toolBarMain->toggleViewAction());
#endif // HANDHELD
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
			cur.setBlockFormat(fmt_cell);
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
			cur.setBlockFormat(fmt_cell);
			if (step.matrix.at(r).at(c) == INFINITY)
				cur.insertText(INFSTR, fmt_default);
			else if ((r == step.candidate.nRow) && (c == step.candidate.nCol))
				cur.insertText(isInteger(step.matrix.at(r).at(c)) ? QString("%1").arg(step.matrix.at(r).at(c)) : QString("%1").arg(step.matrix.at(r).at(c), 0, 'f', settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt()), fmt_selected);
			else {
SStep::SCandidate cand;
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
		Ui_MainWindow::retranslateUi(this);

	loadStyleList();
	loadToolbarList();

#ifndef QT_NO_PRINTER
	actionFilePrintPreview->setText(tr("P&rint Preview..."));
#ifndef QT_NO_TOOLTIP
	actionFilePrintPreview->setToolTip(tr("Preview solution results"));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
	actionFilePrintPreview->setStatusTip(tr("Preview current solution results before printing"));
#endif // QT_NO_STATUSTIP

	actionFilePrint->setText(tr("&Print..."));
#ifndef QT_NO_TOOLTIP
	actionFilePrint->setToolTip(tr("Print solution"));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
	actionFilePrint->setStatusTip(tr("Print current solution results"));
#endif // QT_NO_STATUSTIP
	actionFilePrint->setShortcut(tr("Ctrl+P"));
#endif // QT_NO_PRINTER

#ifndef HANDHELD
	actionSettingsToolbarsConfigure->setText(tr("Configure..."));
#ifndef QT_NO_STATUSTIP
	actionSettingsToolbarsConfigure->setStatusTip(tr("Customize toolbars"));
#endif // QT_NO_STATUSTIP
#endif // HANDHELD

	if (actionHelpCheck4Updates != NULL) {
		actionHelpCheck4Updates->setText(tr("Check for &Updates..."));
#ifndef QT_NO_STATUSTIP
		actionHelpCheck4Updates->setStatusTip(tr("Check for %1 updates").arg(QApplication::applicationName()));
#endif // QT_NO_STATUSTIP
	}
}

bool MainWindow::saveTask() {
QStringList filters(tr("%1 Task File").arg("TSPSG") + " (*.tspt)");
	filters.append(tr("All Files") + " (*)");
QString file;
	if ((fileName == tr("Untitled") + ".tspt") && settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool()) {
		file = settings->value(OS"/LastUsed/TaskSavePath").toString();
		if (!file.isEmpty())
			file.append("/");
		file.append(fileName);
	} else if (fileName.endsWith(".tspt", Qt::CaseInsensitive))
		file = fileName;
	else
		file = QFileInfo(fileName).path() + "/" + QFileInfo(fileName).completeBaseName() + ".tspt";

QFileDialog::Options opts = settings->value("UseNativeDialogs", DEF_USE_NATIVE_DIALOGS).toBool() ? QFileDialog::Options() : QFileDialog::DontUseNativeDialog;
	file = QFileDialog::getSaveFileName(this, tr("Task Save"), file, filters.join(";;"), NULL, opts);
	if (file.isEmpty())
		return false;
	else if (settings->value("SaveLastUsed", DEF_SAVE_LAST_USED).toBool())
		settings->setValue(OS"/LastUsed/TaskSavePath", QFileInfo(file).path());

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
	setWindowTitle(QString("%1[*] - %2").arg(QFileInfo(fileName).completeBaseName()).arg(QApplication::applicationName()));
}

void MainWindow::setupUi()
{
	Ui_MainWindow::setupUi(this);

	// File Menu
	actionFileNew->setIcon(GET_ICON("document-new"));
	actionFileOpen->setIcon(GET_ICON("document-open"));
	actionFileSave->setIcon(GET_ICON("document-save"));
#ifndef HANDHELD
	menuFileSaveAs->setIcon(GET_ICON("document-save-as"));
#endif
	actionFileExit->setIcon(GET_ICON("application-exit"));
	// Settings Menu
#ifndef HANDHELD
	menuSettingsLanguage->setIcon(GET_ICON("preferences-desktop-locale"));
#if QT_VERSION >= 0x040600
	actionSettingsLanguageEnglish->setIcon(QIcon::fromTheme("flag-gb", QIcon(":/images/icons/l10n/flag-gb.png")));
#else // QT_VERSION >= 0x040600
	actionSettingsLanguageEnglish->setIcon(QIcon(":/images/icons/l10n/flag-gb.png"));
#endif // QT_VERSION >= 0x040600
	menuSettingsStyle->setIcon(GET_ICON("preferences-desktop-theme"));
#endif // HANDHELD
	actionSettingsPreferences->setIcon(GET_ICON("preferences-system"));
	// Help Menu
#ifndef HANDHELD
	actionHelpContents->setIcon(GET_ICON("help-contents"));
	actionHelpContextual->setIcon(GET_ICON("help-contextual"));
	actionHelpAbout->setIcon(GET_ICON("help-about"));
	actionHelpAboutQt->setIcon(QIcon(":/images/icons/"ICON_SIZE"/qtlogo."ICON_FORMAT));
#endif
	// Buttons
	buttonRandom->setIcon(GET_ICON("roll"));
	buttonSolve->setIcon(GET_ICON("dialog-ok"));
	buttonSaveSolution->setIcon(GET_ICON("document-save-as"));
	buttonBackToTask->setIcon(GET_ICON("go-previous"));

//	action->setIcon(GET_ICON(""));

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
#ifdef HANDHELD
	toolBarMain->setIconSize(QSize(logicalDpiX() / 4, logicalDpiY() / 4));
#endif // HANDHELD
QToolButton *tb = static_cast<QToolButton *>(toolBarMain->widgetForAction(actionFileSave));
	if (tb != NULL)	 {
		tb->setMenu(menuFileSaveAs);
		tb->setPopupMode(QToolButton::MenuButtonPopup);
	}

//	solutionText->document()->setDefaultFont(settings->value("Output/Font", QFont(DEF_FONT_FAMILY, DEF_FONT_SIZE)).value<QFont>());
	solutionText->setWordWrapMode(QTextOption::WordWrap);

#ifndef QT_NO_PRINTER
	actionFilePrintPreview = new QAction(this);
	actionFilePrintPreview->setObjectName("actionFilePrintPreview");
	actionFilePrintPreview->setEnabled(false);
	actionFilePrintPreview->setIcon(GET_ICON("document-print-preview"));

	actionFilePrint = new QAction(this);
	actionFilePrint->setObjectName("actionFilePrint");
	actionFilePrint->setEnabled(false);
	actionFilePrint->setIcon(GET_ICON("document-print"));

	menuFile->insertAction(actionFileExit,actionFilePrintPreview);
	menuFile->insertAction(actionFileExit,actionFilePrint);
	menuFile->insertSeparator(actionFileExit);

	toolBarMain->insertAction(actionSettingsPreferences, actionFilePrint);
#endif // QT_NO_PRINTER

	groupSettingsLanguageList = new QActionGroup(this);
	actionSettingsLanguageEnglish->setData("en");
	actionSettingsLanguageEnglish->setActionGroup(groupSettingsLanguageList);
	loadLangList();
	actionSettingsLanguageAutodetect->setChecked(settings->value("Language", "").toString().isEmpty());

	actionSettingsStyleSystem->setData(true);
	groupSettingsStyleList = new QActionGroup(this);

#ifndef HANDHELD
	actionSettingsToolbarsConfigure = new QAction(this);
	actionSettingsToolbarsConfigure->setIcon(GET_ICON("configure-toolbars"));
#endif // HANDHELD

	if (hasUpdater()) {
		actionHelpCheck4Updates = new QAction(this);
		actionHelpCheck4Updates->setIcon(GET_ICON("system-software-update"));
		actionHelpCheck4Updates->setEnabled(hasUpdater());
		menuHelp->insertAction(actionHelpAboutQt, actionHelpCheck4Updates);
		menuHelp->insertSeparator(actionHelpAboutQt);
	} else
		actionHelpCheck4Updates = NULL;

	spinCities->setMaximum(MAX_NUM_CITIES);

#ifndef HANDHELD
	toolBarManager = new QtToolBarManager;
	toolBarManager->setMainWindow(this);
QString cat = toolBarMain->windowTitle();
	toolBarManager->addToolBar(toolBarMain, cat);
#ifndef QT_NO_PRINTER
	toolBarManager->addAction(actionFilePrintPreview, cat);
#endif // QT_NO_PRINTER
	toolBarManager->addAction(actionHelpContents, cat);
	toolBarManager->addAction(actionHelpContextual, cat);
//	toolBarManager->addAction(action, cat);
	toolBarManager->restoreState(settings->value("MainWindow/Toolbars").toByteArray());
#else
	toolBarMain->setVisible(settings->value("MainWindow/ToolbarVisible", true).toBool());
#endif // HANDHELD

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
