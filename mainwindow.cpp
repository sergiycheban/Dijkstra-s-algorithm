#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>
#include <QPixmap>
#include <QMessageBox>

////////////////////////////////////////////////////////////////////////////////

const int		RADIUS				= 20;
const int		IS_ACTIVATED		= -1;
const int		IS_NOT_ACTIVATED	= -2;
const int		INFINITY_COUNT      = 100000000;


QColor  darkGreen   = QColor( 50, 180, 60, 255 );
QColor  colorBlue   = QColor( 0, 200, 255, 255 );

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( QWidget* parent ) :
	QMainWindow( parent ),
	m_ui( new Ui::MainWindow )
{
	m_ui->setupUi( this );

	setStyle();

	m_edgeManager.setX( IS_NOT_ACTIVATED );
	m_edgeManager.setY( IS_NOT_ACTIVATED );
	m_shortestPathManager.setX( IS_NOT_ACTIVATED );
	m_shortestPathManager.setY( IS_NOT_ACTIVATED );

	m_reader = new QLineEdit( this );
	m_reader->hide();
	m_reader->setFixedWidth( 30 );
	m_reader->setStyleSheet( "QLineEdit { background: white;}" );
	connect( m_reader, SIGNAL( returnPressed() ), this, SLOT( inputManager() ) );


}
////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{

}

void MainWindow::setStyle()
{

	//this->setFixedSize(QSize(960, 600));

	this->setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint );
	this->statusBar()->setSizeGripEnabled( false );

	setStyleSheet( "background-color:rgba(255,255,255,0);" );

    QPixmap pixmapAdd( ":/new/resources/molecule.svg" );
	QIcon ButtonIconAdd( pixmapAdd );
	m_ui->m_add->setIcon( ButtonIconAdd );
	m_ui->m_add->setIconSize( QSize( 30, 30 ) );
	m_ui->m_add->setStyleSheet( "background-color:transparent;" );

    QPixmap pixmapReset( ":/new/resources/danger.svg" );
	QIcon ButtonIconReset( pixmapReset );
	m_ui->m_reset->setIcon( ButtonIconReset );
	m_ui->m_reset->setIconSize( QSize( 30, 30 ) );
	m_ui->m_reset->setStyleSheet( "background-color:transparent;" );

    QPixmap pixmapSearch( ":/new/resources/loupe.svg" );
	QIcon ButtonIconSearch( pixmapSearch );
	m_ui->m_search->setIcon( ButtonIconSearch );
	m_ui->m_search->setIconSize( QSize( 30, 30 ) );
	m_ui->m_search->setStyleSheet( "background-color:transparent;" );

    QPixmap pixmapClose( ":/new/resources/close.svg" );
	QIcon ButtonIconClose( pixmapClose );
	m_ui->m_close->setIcon( ButtonIconClose );
	m_ui->m_close->setIconSize( QSize( 30, 30 ) );
	m_ui->m_close->setStyleSheet( "background-color:transparent;" );


    QPixmap pixmapMinimize( ":/new/resources/stars.svg" );
	QIcon ButtonIconMinimize( pixmapMinimize );
	m_ui->m_minimize->setIcon( ButtonIconMinimize );
	m_ui->m_minimize->setIconSize( QSize( 30, 30 ) );
	m_ui->m_minimize->setStyleSheet( "background-color:transparent;" );

}
////////////////////////////////////////////////////////////////////////////////

void MainWindow::paintEvent( QPaintEvent* )
{
	QPainter Painter( this );
	Painter.setRenderHint( QPainter::Antialiasing );
	QPen pen( Qt::white );
	pen.setWidth( 3 );
	pen.setStyle( Qt::SolidLine );
	Painter.setPen( pen );
	QFont font;
	font.setPixelSize( 30 );
	font.setWeight( 10 );
	Painter.setFont( font );

	for ( int i = 0 ; i < m_positions.size() ; i++ )
	{
		Painter.drawEllipse( m_positions[i], RADIUS, RADIUS );
	}

	Painter.setBrush( palette().dark() );

	bool vertexFromThePath = false ;

	for ( int i = 0 ; i < m_positions.size() ; i++ )
	{
		if ( m_checked == true )
		{
			for ( int j = 0 ; j < m_path.size() ; j++ )
			{
				if ( m_path[j] == i )
				{
					vertexFromThePath = true;
					break;
				}
			}

			if ( vertexFromThePath )
			{
				Painter.setBrush( darkGreen );
			}
		}

		Painter.drawText( m_positions[i].x() - RADIUS, m_positions[i].y() - RADIUS, RADIUS * 2, RADIUS * 2,
						  Qt::AlignCenter, QString::number( i ) );

	}

	if ( m_edgeManager.x() == IS_ACTIVATED )
	{
		Painter.setBrush( Qt::red );
		Painter.drawEllipse( m_positions[m_edgeManager.y()], RADIUS - RADIUS / 5, RADIUS - RADIUS / 5 );
		Painter.drawText( m_positions[m_edgeManager.y()].x() - RADIUS,
						  m_positions[m_edgeManager.y()].y() - RADIUS,
						  2 * RADIUS, 2 * RADIUS, Qt::AlignCenter, QString::number( m_edgeManager.y() ) );
	}

	if ( ( m_shortestPathManager.x() != IS_NOT_ACTIVATED )
		 && ( m_shortestPathManager.y() == IS_NOT_ACTIVATED ) )
	{
		Painter.setBrush( darkGreen );
		Painter.drawEllipse( m_positions[m_shortestPathManager.x()], RADIUS - RADIUS / 5,
							 RADIUS - RADIUS / 5 );
		Painter.drawText( m_positions[m_shortestPathManager.x()].x() - RADIUS,
						  m_positions[m_shortestPathManager.x()].y() - RADIUS, 2 * RADIUS, 2 * RADIUS, Qt::AlignCenter,
						  QString::number( m_shortestPathManager.x() ) );
	}

	QLineF one, tow, three, neoLine, arrowBase;
	QPointF prime;
	bool bothVerticiesAreIn = false, oneVertexIsIn = false;
	int vertex1positionInPath, vertex2PositionInPath;

	for ( int i = 0 ; i < m_positions.size() ; i++ )
	{
		for ( int j = 0 ; j < m_path.size() ; j++ )
		{
			if ( m_path[j] == i )
			{
				oneVertexIsIn = true;
				vertex1positionInPath = j;
				break;
			}
		}

		for ( int j = 0 ; j < m_matrix[i].size() ; j++ )
		{
			for ( int k = 0 ; k < m_path.size() ; k++ )
			{
				if ( ( m_path[k] == m_matrix[i][j] ) && ( oneVertexIsIn ) )
				{
					bothVerticiesAreIn = true;
					vertex2PositionInPath = k;
					break;
				}
			}

			if ( ( bothVerticiesAreIn ) && ( ( vertex1positionInPath == vertex2PositionInPath + 1 )
											 || ( vertex2PositionInPath == vertex1positionInPath + 1 ) ) )
			{
				Painter.setPen( darkGreen );
			}

			one.setP1( m_positions[i] );
			one.setP2( m_positions[m_matrix[i][j]] );
			one.setLength( one.length() - RADIUS );
			tow.setP2( m_positions[i] );
			tow.setP1( m_positions[m_matrix[i][j]] );
			tow.setLength( tow.length() - RADIUS );

			prime.setX( one.p2().x() );
			prime.setY( one.p2().y() );
			neoLine.setP1( tow.p2() );
			neoLine.setP2( prime );
			neoLine.setLength( neoLine.length() - 10 );

			arrowBase = neoLine.normalVector();
			arrowBase.translate( neoLine.dx(), neoLine.dy() );
			arrowBase.setLength( 5 );
			three.setP1( arrowBase.p2() );
			three.setP2( neoLine.p2() );
			three.setLength( 10 );

			Painter.drawLine( neoLine );
			Painter.drawLine( three.p1(), three.p2() );
			Painter.drawLine( three.p1(), prime );
			Painter.drawLine( three.p2(), prime );

			if ( bothVerticiesAreIn )
			{
				Painter.setPen( Qt::white );
				bothVerticiesAreIn = false ;
			}
		}

		oneVertexIsIn = false ;
	}

	for ( int i = 0 ; i < m_positions.size() ; i++ )
	{
		for ( int j = 0 ; j < m_weight[i].size() ; j++ )
		{
			Painter.save();
			Painter.setPen( Qt::red );
			Painter.translate( m_positions[i] );
			Painter.rotate( -m_angles[i][j] );
			Painter.drawText( RADIUS + 20, 10, QString::number( m_weight[i][j] ) );
			Painter.restore();

		}

	}
}
////////////////////////////////////////////////////////////////////////////////

void MainWindow::mousePressEvent( QMouseEvent* event )
{

	if ( m_reader->isVisible() )
	{
		m_reader->setFocus();
		return;
	}

	QRect rect(RADIUS, RADIUS, width()- 2 * RADIUS,
				height() - 2 - 2 * RADIUS );


	if ( event->button() == Qt::LeftButton )
	{
		if ( ( m_shortestPathManager.x() != IS_NOT_ACTIVATED )
			 && ( m_shortestPathManager.y() != IS_NOT_ACTIVATED ) )
		{
			m_ui->m_search->setFocus();
			return ;
		}

		if ( rect.contains( event->pos() ) )
		{
			if ( m_positions.isEmpty() )
			{
				m_positions.append( event->pos() );
			}

			else
			{
				bool AVertexIsClicked = false ;
				int theClickedVertex = 0;
				QRect vertexSafeArea ;

				while ( !AVertexIsClicked )
				{
					vertexSafeArea.setX( m_positions[theClickedVertex].x() - 2 * RADIUS );
					vertexSafeArea.setY( m_positions[theClickedVertex].y() - 2 * RADIUS );
					vertexSafeArea.setWidth( 4 * RADIUS );
					vertexSafeArea.setHeight( 4 * RADIUS );

					if ( vertexSafeArea.contains( event->pos() ) )
					{
						AVertexIsClicked = true;
						break;
					}

					theClickedVertex++;

					if ( theClickedVertex == m_positions.size() )
					{
						break;
					}
				}

				if ( AVertexIsClicked == false )
				{
					if ( m_checked == true )
					{
						return;
					}

					if ( m_edgeManager.x() == IS_ACTIVATED )
					{
						return;
					}

					m_positions.append( event->pos() );
					QVector<int> vect ;
					QVector<qreal> vect2 ;
					m_matrix.insert( m_positions.size(), vect );
					m_weight.insert( m_positions.size(), vect );
					m_angles.insert( m_positions.size(), vect2 );

				}

				else
				{
					vertexSafeArea.setX( m_positions[theClickedVertex].x() - RADIUS );
					vertexSafeArea.setY( m_positions[theClickedVertex].y() - RADIUS );
					vertexSafeArea.setWidth( 2 * RADIUS );
					vertexSafeArea.setHeight( 2 * RADIUS );

					if ( vertexSafeArea.contains( event->pos() ) )
					{
						if ( m_checked == true )
						{
							if ( m_shortestPathManager.x() == IS_NOT_ACTIVATED )
							{
								m_shortestPathManager.setX( theClickedVertex );
							}

							else if ( m_shortestPathManager.x() == theClickedVertex )
							{
								m_shortestPathManager.setX( IS_NOT_ACTIVATED );
							}

							else
							{
								m_shortestPathManager.setY( theClickedVertex );
								shortestPath();
							}

						}

						else
						{
							if ( m_edgeManager.x() == IS_NOT_ACTIVATED )
							{
								m_edgeManager.setX( IS_ACTIVATED );
								m_edgeManager.setY( theClickedVertex );
							}

							else if ( m_edgeManager.x() == IS_ACTIVATED )
							{
								if ( m_edgeManager.y() == theClickedVertex )
								{
									m_edgeManager.setX( IS_NOT_ACTIVATED );
								}

								else
								{
									int k = 0 ; bool theEdgeAllreadyExists = false ;

									while ( k < m_matrix[m_edgeManager.y()].size() )
									{
										if ( m_matrix[m_edgeManager.y()][k] == theClickedVertex )
										{
											theEdgeAllreadyExists = true ;
											break ;
										}

										k++;
									}

									if ( !theEdgeAllreadyExists )
									{
										QLineF li( m_positions[m_edgeManager.y()], m_positions[theClickedVertex] );
										m_matrix[m_edgeManager.y()].append( theClickedVertex );
										m_angles[m_edgeManager.y()].append( li.angle() );
										m_reader->move( ( m_positions[m_edgeManager.y()].x() + m_positions[theClickedVertex].x() ) / 2,
														( m_positions[m_edgeManager.y()].y() + m_positions[theClickedVertex].y() ) / 2 );
										m_reader->setEnabled( true );
										m_reader->setVisible( true );
										m_reader->setFocus();
										m_edgeManager.setX( IS_NOT_ACTIVATED );
									}

									else
									{
										m_edgeManager.setX( IS_NOT_ACTIVATED );
									}
								}
							}
						}
					}
				}
			}
		}
	}

	else if ( event->button() == Qt::RightButton )
	{
		bool AVertexIsClicked = false ;
		int theClickedVertex = 0;
		QRect vertexSafeArea ;

		while ( !AVertexIsClicked )
		{
			vertexSafeArea.setX( m_positions[theClickedVertex].x() - RADIUS );
			vertexSafeArea.setY( m_positions[theClickedVertex].y() - RADIUS );
			vertexSafeArea.setWidth( 2 * RADIUS );
			vertexSafeArea.setHeight( 2 * RADIUS );

			if ( vertexSafeArea.contains( event->pos() ) )
			{
				AVertexIsClicked = true;
				break;
			}

			theClickedVertex++;

			if ( theClickedVertex == m_positions.size() )
			{
				break;
			}
		}

		if ( m_checked == true )
		{
			shortestPath();

		}
	}

	update();
}
////////////////////////////////////////////////////////////////////////////////

void MainWindow::mouseMoveEvent( QMouseEvent* event )
{
	if ( m_reader->isVisible() )
	{
		m_reader->setFocus();
		return;
	}

	QRect rect( RADIUS / 2, RADIUS / 2, width() - 2 * RADIUS,
				height() - 2 * RADIUS);

	if ( rect.contains( event->pos() ) )
	{
		bool AVertexIsClicked = false ;
		int theClickedVertex = 0;
		QRect vertexSafeArea ;

		while ( !AVertexIsClicked )
		{
			vertexSafeArea.setX( m_positions[theClickedVertex].x() - RADIUS );
			vertexSafeArea.setY( m_positions[theClickedVertex].y() - RADIUS );
			vertexSafeArea.setWidth( 2 * RADIUS );
			vertexSafeArea.setHeight( 2 * RADIUS );

			if ( vertexSafeArea.contains( event->pos() ) )
			{
				AVertexIsClicked = true;
				break;
			}

			theClickedVertex++;

			if ( theClickedVertex == m_positions.size() )
			{
				break;
			}
		}

		if ( AVertexIsClicked )
		{
			m_positions[theClickedVertex] = event->pos();

			for ( int i = 0 ; i < m_angles[theClickedVertex].size() ; i++ )
			{
				QLineF li( m_positions[theClickedVertex], m_positions[m_matrix[theClickedVertex][i]] );
				m_angles[theClickedVertex][i] = li.angle();
			}

			m_edgeManager.setX( IS_NOT_ACTIVATED );
			update();
		}
	}
}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_Delete )
	{
		if ( event->modifiers() & Qt::CTRL )
		{
			m_matrix.clear();
			m_weight.clear();
			m_angles.clear();
			m_path.clear();
			update();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::inputManager()
{
	m_weight[m_edgeManager.y()].append( m_reader->text().toInt() );
	m_reader->clear();
	m_reader->setDisabled( true );
	m_reader->hide();
	update();
}
////////////////////////////////////////////////////////////////////////////////

void MainWindow::shortestPath()
{
	int AdjMat[100][100];
	int p[100][100];
	m_path.clear();

	for ( int i = 0; i < m_positions.size(); i++ )
		for ( int j = 0; j < m_positions.size(); j++ )
		{
			AdjMat[i][j] = INFINITY_COUNT;
		}


	for ( int i = 0 ; i < m_positions.size() ; i++ )
	{
		for ( int j = 0 ; j < m_matrix[i].size() ; j++ )
		{
			AdjMat[i][m_matrix[i][j]] = m_weight[i][j];
		}
	}

	for ( int i = 0; i < m_positions.size(); i++ )
		for ( int j = 0; j < m_positions.size(); j++ )
		{
			p[i][j] = i;
		}

	for ( int k = 0; k < m_positions.size(); k++ )
	{
		for ( int i = 0; i < m_positions.size(); i++ )
		{
			for ( int j = 0; j < m_positions.size(); j++ )
			{
				if ( AdjMat[i][k] + AdjMat[k][j] < AdjMat[i][j] )
				{
					AdjMat[i][j] = AdjMat[i][k] + AdjMat[k][j];
					p[i][j] = p[k][j];
				}
			}
		}
	}

	if ( AdjMat[m_shortestPathManager.x()][m_shortestPathManager.y()] != INFINITY_COUNT )
	{
		int i = m_shortestPathManager.y();

		while ( i != m_shortestPathManager.x() )
		{
			m_path.append( i );
			i = p[m_shortestPathManager.x()][i];
		}

		m_path.append( m_shortestPathManager.x() );
	}

	else
	{
		m_path.append( INFINITY_COUNT );
	}

	update();

}
////////////////////////////////////////////////////////////////////////////////

void MainWindow:: resetShortestPathManager()
{
	m_shortestPathManager.setX( IS_NOT_ACTIVATED );
	m_shortestPathManager.setY( IS_NOT_ACTIVATED );
	m_path.clear();

	update();

}
////////////////////////////////////////////////////////////////////////////////


void MainWindow::on_m_reset_clicked()
{
	m_matrix.clear();
	m_weight.clear();
	m_angles.clear();
	m_path.clear();
	m_positions.clear();
	m_edgeManager.setX( IS_NOT_ACTIVATED );
	m_edgeManager.setY( IS_NOT_ACTIVATED );
	m_shortestPathManager.setX( IS_NOT_ACTIVATED );
	m_shortestPathManager.setY( IS_NOT_ACTIVATED );

	if ( m_checked == true )
	{
		m_ui->m_search->toggle();
	}

	update();
}
////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_m_search_clicked()
{

	m_checked = true;
	m_shortestPathManager.setX( IS_NOT_ACTIVATED );
	m_shortestPathManager.setY( IS_NOT_ACTIVATED );
	m_path.clear();

	update();
}
////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_m_add_clicked()
{
	m_checked = false;
}

void MainWindow::on_m_close_clicked()
{
	close();
}

void MainWindow::on_m_minimize_clicked()
{
	showMinimized();
}

