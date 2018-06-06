//#include "qgraphdesigner.h"
//#include <QtGui>
//#include <QtCore>


//const int		MARGIN				= 5;
//const int		RADIUS				= 30;
//const int		IS_ACTIVATED		= -1;
//const int		IS_NOT_ACTIVATED	= -2;
//const int		ERASED				= -99;
//const int		INFINITY_COUNT      = 100000000;

//QgraphDesigner::QgraphDesigner(QWidget *parent)
//    : QWidget(parent)
//{
//    setWindowTitle ("Graph Guide");

//    setAutoFillBackground(true);
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    setFocusPolicy(Qt::StrongFocus);

//    //initialise Managers
//    edgeManager.setX(IS_NOT_ACTIVATED);
//    edgeManager.setY(IS_NOT_ACTIVATED);
//    shortestPathManager.setX(IS_NOT_ACTIVATED);
//    shortestPathManager.setY(IS_NOT_ACTIVATED);

//    //create and configure buttons
//    drawTheShortestPath = new QToolButton(this);
//    drawTheShortestPath->setText("Shortest Path");
//    drawTheShortestPath->setCheckable(true);
//    drawTheShortestPath->adjustSize();
//    connect(drawTheShortestPath,SIGNAL(toggled(bool)),this,SLOT(resetShortestPathManager()));
//    reset = new QToolButton(this);
//    reset->setText("Reset");
//    reset->adjustSize();
//    connect(reset,SIGNAL(clicked(bool)),this,SLOT(resetAll()));


//    //create and configure the reader object
//    reader = new QLineEdit(this);
//    reader->hide();
//    reader->setFixedWidth(30);
//    connect(reader,SIGNAL(returnPressed()),this,SLOT(inputManager()));

//}

//QSize QgraphDesigner::minimumSizeHint() const
//{
//    return QSize(300,200);
//}

//QSize QgraphDesigner::sizeHint() const
//{
//    return QSize(6000,4000);
//}

//void QgraphDesigner::resizeEvent(QResizeEvent*)
//{
//    int y = height() - drawTheShortestPath->height() - 10 ;
//    reset->move(10,y);
//    drawTheShortestPath->move(10+reset->width()+5,y);
//    update();
//}

//void QgraphDesigner::paintEvent(QPaintEvent*)
//{
//    QPainter Painter(this);
//    Painter.setRenderHint(QPainter::Antialiasing);
//    QPen pen( Qt::black );
//    pen.setWidth(3);
//    pen.setStyle( Qt::SolidLine );
//    Painter.setPen( pen );
//    QFont font;
//    font.setPixelSize( 20 );
//    font.setWeight(3);
//    Painter.setFont(font);
//    QColor* darkGreen = new QColor(0,180,00,255);

//    //paint the outer cercle of all vertecies
//    for(int i = 0 ; i<positions.size() ; i++)
//    {
//        if(positions[i].x()==ERASED){continue;}
//        Painter.drawEllipse(positions[i],RADIUS,RADIUS);
//    }

//    Painter.setBrush(palette().dark());

//    //paint the inner disc and id number of all verticies
//    bool vertexFromThePath=false ;

//    for(int i = 0 ; i<positions.size() ; i++)
//    {
//        if(positions[i].x()==ERASED){continue;}
//        if(drawTheShortestPath->isChecked())
//        {
//            for(int j=0 ; j<path.size() ; j++)
//            {
//                if(path[j]==i){vertexFromThePath=true;break;}
//            }
//            if(vertexFromThePath)
//            {
//                Painter.setBrush(*darkGreen);
//            }
//        }

//        Painter.drawEllipse(positions[i],RADIUS - RADIUS/5,RADIUS - RADIUS/5);
//        Painter.drawText(positions[i].x()-RADIUS, positions[i].y()-RADIUS, RADIUS * 2, RADIUS * 2, Qt::AlignCenter, QString::number(i));
//        if(vertexFromThePath)
//        {
//            Painter.setBrush(palette().dark());
//            vertexFromThePath=false ;
//        }

//    }

//    //paint activated vertex if there is one
//    if(edgeManager.x()==IS_ACTIVATED)
//    {
//        Painter.setBrush(palette().window());
//        Painter.drawEllipse(positions[edgeManager.y()],RADIUS,RADIUS);
//        Painter.setBrush(Qt::yellow);
//        Painter.drawEllipse(positions[edgeManager.y()],RADIUS - RADIUS/5,RADIUS - RADIUS/5);
//        Painter.drawText(positions[edgeManager.y()].x()-RADIUS, positions[edgeManager.y()].y()-RADIUS, 2*RADIUS,2*RADIUS, Qt::AlignCenter, QString::number(edgeManager.y()));
//    }

//    //paint source vertex for a shortest path search if there is one
//    if((shortestPathManager.x()!=IS_NOT_ACTIVATED)&&(shortestPathManager.y()==IS_NOT_ACTIVATED))
//    {
//        Painter.setBrush(*darkGreen);
//        Painter.drawEllipse(positions[shortestPathManager.x()],RADIUS - RADIUS/5,RADIUS - RADIUS/5);
//        Painter.drawText(positions[shortestPathManager.x()].x()-RADIUS, positions[shortestPathManager.x()].y()-RADIUS, 2*RADIUS,2*RADIUS, Qt::AlignCenter, QString::number(shortestPathManager.x()));
//    }

//    //paint edges
//    QLineF one,tow,three,neoLine,arrowBase;
//    QPointF prime;
//    bool bothVerticiesAreIn=false,oneVertexIsIn=false;
//    int vertex1positionInPath,vertex2PositionInPath;

//    for(int i = 0 ; i < positions.size() ; i++)
//    {
//        if(positions[i].x()==ERASED){continue;}

//        for(int j=0 ; j<path.size() ; j++)
//        {
//            if(path[j]==i){oneVertexIsIn=true;vertex1positionInPath=j;break;}
//        }

//        for(int j=0 ; j< matrix[i].size() ; j++)
//        {

//            if(positions[matrix[i][j]].x()==ERASED){continue;}

//            for(int k=0 ; k<path.size() ; k++)
//            {
//                if((path[k]==matrix[i][j])&&(oneVertexIsIn)){bothVerticiesAreIn=true;vertex2PositionInPath=k;break;}
//            }
//            if((bothVerticiesAreIn)&&((vertex1positionInPath==vertex2PositionInPath+1)||(vertex2PositionInPath==vertex1positionInPath+1)))
//            {
//                Painter.setPen(*darkGreen);
//            }
//            one.setP1(positions[i]);
//            one.setP2(positions[matrix[i][j]]);
//            one.setLength(one.length() - RADIUS);
//            tow.setP2(positions[i]);
//            tow.setP1(positions[matrix[i][j]]);
//            tow.setLength(tow.length() - RADIUS);

//            prime.setX(one.p2().x());
//            prime.setY(one.p2().y());
//            neoLine.setP1(tow.p2());
//            neoLine.setP2(prime);
//            neoLine.setLength(neoLine.length()-10);


//            arrowBase = neoLine.normalVector();
//            arrowBase.translate(neoLine.dx(),neoLine.dy());
//            arrowBase.setLength(5);
//            three.setP1(arrowBase.p2());
//            three.setP2(neoLine.p2());
//            three.setLength(10);

//            Painter.drawLine(neoLine);
//            Painter.drawLine(three.p1(), three.p2());
//            Painter.drawLine(three.p1(), prime);
//            Painter.drawLine(three.p2(), prime);
//            if(bothVerticiesAreIn)
//            {
//                Painter.setPen(Qt::black);
//                bothVerticiesAreIn=false ;
//            }
//        }
//        oneVertexIsIn=false ;
//    }
//    for(int i = 0 ; i < positions.size() ; i++)
//    {
//        if(positions[i].x()==ERASED){continue;}
//        for(int j=0 ; j< weight[i].size() ; j++)
//        {
//            if(positions[matrix[i][j]].x()==ERASED){continue;}
//                Painter.save();
//                Painter.translate(positions[i]);
//                Painter.rotate(-angles[i][j]);
//                Painter.drawText(RADIUS + 20, 10, QString::number(weight[i][j]));
//                Painter.restore();

//        }

//    }
//}

//void QgraphDesigner::mousePressEvent(QMouseEvent* event)
//{

//    if(reader->isVisible()){reader->setFocus();return;}

//    QRect rect(MARGIN+RADIUS, MARGIN+RADIUS, width() - 2 * MARGIN - 2 * RADIUS, height() - 2 * MARGIN - 2 * RADIUS);


//    if (event->button() == Qt::LeftButton) //add a vertex || select a vertex
//    {
//        if((shortestPathManager.x()!=IS_NOT_ACTIVATED)&&(shortestPathManager.y()!=IS_NOT_ACTIVATED)){drawTheShortestPath->setFocus(); return ;}

//        if (rect.contains(event->pos())) //valid click
//        {
//            if(positions.isEmpty())
//            {
//                positions.append(event->pos()); //first vertex
//            }
//            else
//            {
//                bool AVertexIsClicked = false ;
//                int theClickedVertex=0;
//                QRect vertexSafeArea ;

//                while(!AVertexIsClicked) //check if the user clicked a vertex
//                {
//                    vertexSafeArea.setX( positions[theClickedVertex].x()- 2 * RADIUS );
//                    vertexSafeArea.setY( positions[theClickedVertex].y()- 2 * RADIUS );
//                    vertexSafeArea.setWidth(4 * RADIUS);
//                    vertexSafeArea.setHeight(4 * RADIUS);
//                    if(vertexSafeArea.contains(event->pos())){AVertexIsClicked=true;break;}
//                    theClickedVertex++;
//                    if(theClickedVertex==positions.size()){break;}
//                }
//                if(AVertexIsClicked==false) //No vertex clicked -> add a new vertex by saving the click's postion and adding a new empty edge vector in the main edge matrix
//                {
//                    if(drawTheShortestPath->isChecked()){return;}
//                    if(edgeManager.x()==IS_ACTIVATED){return;}
//                    positions.append(event->pos());
//                    QVector<int> vect ;
//                    QVector<qreal> vect2 ;
//                    matrix.insert(positions.size(),vect);
//                    weight.insert(positions.size(),vect);
//                    angles.insert(positions.size(),vect2);

//                }
//                else // Some vertex was clicked -> activate it if no one else is activated or add an new adge end desactive it if an other vertex is all ready activated
//                {
//                    vertexSafeArea.setX( positions[theClickedVertex].x()- RADIUS ); //make the area smaller for optimisation purposes
//                    vertexSafeArea.setY( positions[theClickedVertex].y()- RADIUS );
//                    vertexSafeArea.setWidth(2 * RADIUS);
//                    vertexSafeArea.setHeight(2 * RADIUS);
//                    if(vertexSafeArea.contains(event->pos()))
//                    {
//                        if(drawTheShortestPath->isChecked()) //if we are looking for the shortest path handle the shortest path manager
//                        {
//                            if(shortestPathManager.x()==IS_NOT_ACTIVATED)
//                            {
//                                shortestPathManager.setX(theClickedVertex);
//                            }
//                            else if(shortestPathManager.x()==theClickedVertex)
//                            {
//                                shortestPathManager.setX(IS_NOT_ACTIVATED);
//                            }
//                            else
//                            {
//                                shortestPathManager.setY(theClickedVertex);
//                                shortestPath();
//                            }

//                        }

//                        else //else activate the vertex or complete the addition of an edge
//                        {
//                            if(edgeManager.x()==IS_NOT_ACTIVATED) //activate the edge
//                            {
//                                edgeManager.setX(IS_ACTIVATED);
//                                edgeManager.setY(theClickedVertex);
//                            }
//                            else if(edgeManager.x()==IS_ACTIVATED)
//                            {
//                                if(edgeManager.y()==theClickedVertex) //if user click the same vetex twice then deactivate it
//                                {
//                                    edgeManager.setX(IS_NOT_ACTIVATED);
//                                }
//                                else //add the edge, its angle and actiate the reader only if it doesn't already existe.
//                                {
//                                    int k = 0 ; bool theEdgeAllreadyExists =false ;
//                                    while(k<matrix[edgeManager.y()].size())
//                                    {
//                                        if(matrix[edgeManager.y()][k]==theClickedVertex){theEdgeAllreadyExists=true ; break ;}
//                                        k++;
//                                    }

//                                    if(!theEdgeAllreadyExists)
//                                    {
//                                        QLineF li(positions[edgeManager.y()],positions[theClickedVertex]);
//                                        matrix[edgeManager.y()].append(theClickedVertex);
//                                        angles[edgeManager.y()].append(li.angle());
//                                        reader->move( (positions[edgeManager.y()].x()+positions[theClickedVertex].x())/2, (positions[edgeManager.y()].y()+positions[theClickedVertex].y())/2 );
//                                        reader->setEnabled(true);
//                                        reader->setVisible(true);
//                                        reader->setFocus();
//                                        edgeManager.setX(IS_NOT_ACTIVATED);
//                                    }
//                                    else
//                                    {
//                                        edgeManager.setX(IS_NOT_ACTIVATED);
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    else if(event->button() == Qt::RightButton) //erase a vertex
//    {
//        bool AVertexIsClicked = false ;
//        int theClickedVertex=0;
//        QRect vertexSafeArea ;
//        while(!AVertexIsClicked) //check if some vertex is clicked
//        {
//            vertexSafeArea.setX( positions[theClickedVertex].x()- RADIUS );
//            vertexSafeArea.setY( positions[theClickedVertex].y()- RADIUS );
//            vertexSafeArea.setWidth(2 * RADIUS);
//            vertexSafeArea.setHeight(2 * RADIUS);
//            if(vertexSafeArea.contains(event->pos())){AVertexIsClicked=true;break;}
//            theClickedVertex++;
//            if(theClickedVertex==positions.size()){break;}
//        }
//        if(AVertexIsClicked==true)
//        {
//            positions[theClickedVertex].setX(ERASED);
//        }
//        if(drawTheShortestPath->isChecked())
//        {
//            shortestPath();
//        }
//    }

//    update();

//}

//void QgraphDesigner::mouseMoveEvent(QMouseEvent * event)
//{
//    if(reader->isVisible()){reader->setFocus();return;}

//    QRect rect(RADIUS + MARGIN/2, RADIUS + MARGIN/2, width() - 2 * RADIUS - MARGIN, height() - 2 * RADIUS - MARGIN);

//    if (rect.contains(event->pos())) //valid click
//    {
//        bool AVertexIsClicked = false ;
//        int theClickedVertex=0;
//        QRect vertexSafeArea ;

//        while(!AVertexIsClicked) //check if the user clicked a vertex
//        {
//            vertexSafeArea.setX( positions[theClickedVertex].x()- RADIUS );
//            vertexSafeArea.setY( positions[theClickedVertex].y()- RADIUS );
//            vertexSafeArea.setWidth(2 * RADIUS);
//            vertexSafeArea.setHeight(2 * RADIUS);
//            if(vertexSafeArea.contains(event->pos())){AVertexIsClicked=true;break;}
//            theClickedVertex++;
//            if(theClickedVertex==positions.size()){break;}
//        }

//        if(AVertexIsClicked)
//        {
//            positions[theClickedVertex]=event->pos();
//            for(int i =0 ; i<angles[theClickedVertex].size() ; i++)
//            {
//                QLineF li(positions[theClickedVertex],positions[matrix[theClickedVertex][i]]);
//                angles[theClickedVertex][i]=li.angle();
//            }
//            edgeManager.setX(IS_NOT_ACTIVATED);
//            update();
//        }
//    }


//}

//void QgraphDesigner::inputManager()
//{
//    weight[edgeManager.y()].append(reader->text().toInt());
//    reader->clear();
//    reader->setDisabled(true);
//    reader->hide();
//    update();
//}

//void QgraphDesigner::shortestPath()
//{
//    int AdjMat[100][100];
//    int p[100][100];
//    path.clear();

//    //initialise AdjMat
//    for (int i = 0; i < positions.size(); i++)
//        for (int j = 0; j < positions.size(); j++)
//            AdjMat[i][j] = INFINITY_COUNT;


//    for(int i = 0 ; i < positions.size() ; i++)
//    {
//        if(positions[i].x()==ERASED){continue;}

//        for(int j=0 ; j< matrix[i].size() ; j++)
//        {
//            if(positions[matrix[i][j]].x()==ERASED){continue;}
//            AdjMat[i][matrix[i][j]]= weight[i][j];
//        }
//    }

//    //initialise Parent matrix
//    for (int i = 0; i < positions.size(); i++)
//        for (int j = 0; j < positions.size(); j++)
//            p[i][j] = i;

//    //Floyd_Warshall Algorithm for all pairs shortest paths
//    for (int k = 0; k < positions.size(); k++)
//    {
//        for (int i = 0; i < positions.size(); i++)
//        {
//            for (int j = 0; j < positions.size(); j++)
//            {
//                if (AdjMat[i][k] + AdjMat[k][j] < AdjMat[i][j])
//                {
//                    AdjMat[i][j] = AdjMat[i][k] + AdjMat[k][j];
//                    p[i][j] = p[k][j]; // update the parent matrix
//                }
//            }
//        }
//    }
//    if(AdjMat[shortestPathManager.x()][shortestPathManager.y()]!=INFINITY_COUNT)
//    {
//        int i=shortestPathManager.y();
//        while(i!=shortestPathManager.x())
//        {
//            path.append(i);
//            i=p[shortestPathManager.x()][i];
//        }
//        path.append(shortestPathManager.x());
//    }
//    else
//    {
//        path.append(INFINITY_COUNT);
//    }

//    update();

//}

//void QgraphDesigner:: resetShortestPathManager()
//{
//    shortestPathManager.setX(IS_NOT_ACTIVATED);
//    shortestPathManager.setY(IS_NOT_ACTIVATED);
//    path.clear();

//    update();

//}

//void QgraphDesigner:: resetAll()
//{
//    matrix.clear();
//    weight.clear();
//    angles.clear();
//    path.clear();
//    positions.clear();
//    edgeManager.setX(IS_NOT_ACTIVATED);
//    edgeManager.setY(IS_NOT_ACTIVATED);
//    shortestPathManager.setX(IS_NOT_ACTIVATED);
//    shortestPathManager.setY(IS_NOT_ACTIVATED);
//    if(drawTheShortestPath->isChecked()){drawTheShortestPath->toggle();}

//    update();
//}

//QgraphDesigner::~QgraphDesigner()
//{

//}
