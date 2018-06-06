#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QToolButton>
#include <QMap>
#include <QVector>
#include <QLineEdit>


namespace Ui
{
class MainWindow;
}
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow( QWidget* parent = 0 );
	~MainWindow();

protected:
	void setStyle();
	void paintEvent( QPaintEvent* );
	void mousePressEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void keyPressEvent( QKeyEvent* event );

private slots:
	void inputManager() ;
	void resetShortestPathManager() ;

	void on_m_reset_clicked();
	void on_m_search_clicked();
	void on_m_add_clicked();

	void on_m_close_clicked();
	void on_m_minimize_clicked();

public slots:
	void shortestPath() ;

private:
	Ui::MainWindow*                  m_ui;
	QPoint                           m_edgeManager ;
	QPoint                           m_shortestPathManager ;
	QLineEdit*                       m_reader ;
	QMap< int, QVector<int> >        m_matrix ;
	QMap< int, QVector<int> >        m_weight ;
	QMap< int, QVector<qreal> >      m_angles ;
	QVector<QPointF>                 m_positions;
	QVector<int>                     m_path ;
	bool                             m_checked   = false;

};


#endif // MAINWINDOW_H
