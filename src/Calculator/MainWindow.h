// Author: yangle
// Date: 2020-02-21

#pragma once

#include <QtWidgets/QMainWindow>

class QAction;
class QMenu;
class QTranslator;
class QStackedLayout;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void changeEvent(QEvent* event);

private slots:
	void changeStyleToStandard();
	void changeStyleToScientific();
	void changeStyleToProgrammer();
	void changeLanguageToEnglish();
	void changeLanguageToChinese();
	void changeStyleToDefault();
	void changeStyleToDark();
	void changeStyleToLight();

private:
	QStackedLayout* pStackedLayout;
	QMenu* m_pLanguage;
	QAction* m_pEnglish;
	QAction* m_pChinese;
	QTranslator* m_englishTrans;
	QTranslator* m_chineseTrans;

	QMenu* m_pMode;
	QAction* m_pStandard;
	QAction* m_pScientific;
	QAction* m_pProgrammer;

	QMenu* m_pStyle;
	QAction* m_pDefault;
	QAction* m_pDark;
	QAction* m_pLight;

	void retranslate();
};