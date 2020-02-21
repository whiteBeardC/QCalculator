#include "mainwindow.h"
#include "StandardCalculator.h"
#include "ScientificCalculator.h"
#include "ProgrammerCalculator.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QEvent>
#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Calculator"));
	m_englishTrans = nullptr;
	m_chineseTrans = nullptr;

	// ²Ëµ¥À¸:Ä£Ê½
	QMenuBar* pMenuBar = menuBar();
	m_pMode = pMenuBar->addMenu(tr("Change Mode"));
	m_pStandard = m_pMode->addAction(tr("Standard"));
	m_pScientific = m_pMode->addAction(tr("Scientific"));
	m_pProgrammer = m_pMode->addAction(tr("Programmer"));

	// ²Ëµ¥À¸:ÓïÑÔ
	m_pLanguage = pMenuBar->addMenu(tr("Change Language"));
	m_pEnglish = m_pLanguage->addAction(tr("English"));
	m_pChinese = m_pLanguage->addAction(tr("Chinese"));

	connect(m_pEnglish, SIGNAL(triggered(bool)), this, SLOT(changeLanguageToEnglish()));
	connect(m_pChinese, SIGNAL(triggered(bool)), this, SLOT(changeLanguageToChinese()));

	// ²Ëµ¥À¸:»»·ô
	m_pStyle = pMenuBar->addMenu(tr("Change Style"));
	m_pDefault = m_pStyle->addAction(tr("Default"));
	m_pDark = m_pStyle->addAction(tr("Drak"));
	m_pLight = m_pStyle->addAction(tr("Light"));

	connect(m_pDefault, SIGNAL(triggered(bool)), this, SLOT(changeStyleToDefault()));
	connect(m_pDark, SIGNAL(triggered(bool)), this, SLOT(changeStyleToDark()));
	connect(m_pLight, SIGNAL(triggered(bool)), this, SLOT(changeStyleToLight()));

	// ¼ÆËãÆ÷
	StandardCalculator* pStandardCalculator = new StandardCalculator();
	ScientificCalculator* pScientificCalculator = new ScientificCalculator;
	ProgrammerCalculator* pProgrammerCalculator = new ProgrammerCalculator;

	pStackedLayout = new QStackedLayout;
	pStackedLayout->addWidget(pStandardCalculator);
	pStackedLayout->addWidget(pScientificCalculator);
	pStackedLayout->addWidget(pProgrammerCalculator);

	connect(m_pStandard, SIGNAL(triggered(bool)), this, SLOT(changeStyleToStandard()));
	connect(m_pScientific, SIGNAL(triggered(bool)), this, SLOT(changeStyleToScientific()));
	connect(m_pProgrammer, SIGNAL(triggered(bool)), this, SLOT(changeStyleToProgrammer()));

	// ²¼¾Ö
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(pStackedLayout);

	QWidget* pWidget = new QWidget;
	pWidget->setLayout(mainLayout);
	setCentralWidget(pWidget);
}

MainWindow::~MainWindow()
{

}

// ÇÐ»»¼ÆËãÆ÷Ä£Ê½
void MainWindow::changeStyleToStandard()
{
	pStackedLayout->setCurrentIndex(0);
}

void MainWindow::changeStyleToScientific()
{
	pStackedLayout->setCurrentIndex(1);
}

void MainWindow::changeStyleToProgrammer()
{
	pStackedLayout->setCurrentIndex(2);
}

// ÊÂ¼þ
void MainWindow::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		retranslate();
	}
	QMainWindow::changeEvent(event);
}


// ÇÐ»»ÓïÑÔ
void MainWindow::changeLanguageToEnglish()
{
	if (m_chineseTrans)
		qApp->removeTranslator(m_chineseTrans);
	if (nullptr == m_englishTrans)
	{
		m_englishTrans = new QTranslator;
		m_englishTrans->load(QApplication::applicationDirPath() + "/changeLanguage_en.qm");
	}
	qApp->installTranslator(m_englishTrans);
}

void MainWindow::changeLanguageToChinese()
{
	if (m_englishTrans)
		qApp->removeTranslator(m_englishTrans);
	if (nullptr == m_chineseTrans)
	{
		m_chineseTrans = new QTranslator;
		m_chineseTrans->load(QApplication::applicationDirPath() + "/changeLanguage_ch.qm");
	}
	qApp->installTranslator(m_chineseTrans);
}

void MainWindow::retranslate()
{
	setWindowTitle(tr("Calculator"));
	m_pLanguage->setTitle(tr("Change Language"));
	m_pEnglish->setText(tr("English"));
	m_pChinese->setText(tr("Chinese"));

	m_pMode->setTitle(tr("Change Mode"));
	m_pStandard->setText(tr("Standard"));
	m_pScientific->setText(tr("Scientific"));
	m_pProgrammer->setText(tr("Programmer"));

	m_pStyle->setTitle(tr("Change Style"));
	m_pDefault->setText(tr("Default"));
	m_pDark->setText(tr("Dark"));
	m_pLight->setText(tr("Light"));
}

void MainWindow::changeStyleToDefault()
{
	qApp->setStyle(QStyleFactory::create("Default"));
}

void MainWindow::changeStyleToDark()
{
	qApp->setStyle(QStyleFactory::create("Dark"));
}

void MainWindow::changeStyleToLight()
{
	qApp->setStyle(QStyleFactory::create("Light"));
}
