#ifndef IVFDCHART_H
#define IVFDCHART_H

#include <QWidget>
#include <QObject>
#include <QToolTip>
#include <QProcess>
#include <QWhatsThis>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QColorDialog>
#include <QScrollBar>
#include <QFontDialog>
#include <QVBoxLayout>
#include <QDebug>
#include <QGridlayout>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>
#include <QSlider>
#include <QtGui>
#include <QMainWindow>
#include <QCheckBox>
#include <QRadioButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextEdit>
#include <QScrollBar>
#include <QProgressDialog>
#include <QErrorMessage>
#include <QTabWidget>
#include <QTextBrowser>
#include <QFontDialog>
#include <QScrollBar>
#include <QRect>
#include <QSplitter>
#include <QRadioButton>
#include <QCheckBox>
#include <QIcon>
#include <QSettings>
// here goes event handler headers
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
// here goes common c++ headers
// here goes Qt Containers and generic algorithms
#include <QtAlgorithms>
#include <QVector>
#include <QLinkedList>
#include <QList>
#include <QMap>
#include <QHash>
#include <QStack>
// here goes OpenGL headers 
// here goes standard c headers



#include <math.h>


#define TEXT_BOX_SPACE 5
#define X_INCREMENT 5
#define POINT_RADIUS 3
class IVFDChart : public QWidget
{
	Q_OBJECT

public:
	explicit IVFDChart(QWidget *parent = 0);
	~IVFDChart();

public slots:
    void addData(qreal data);

    void setUseAntialiasing(bool use)
    {
        m_bUseAntialiasing=use;
        update();
    }

    bool getUseAntialiasing() const
    {
        return m_bUseAntialiasing;
    }

    void setHorizontalLineColor(const QColor& clr)
    {
        m_hLineClr=clr;
        update();
    }

    void setShowPoint(bool show)
    {
        m_bShowPoint=show;
        update();
    }

    void setTitle(const QString& str)
    {
        m_strTitle=str;
        update();
    }

protected:
    void paintEvent(QPaintEvent *);

    void resizeEvent(QResizeEvent *)
    {
        updateVector();
    }

    QSize sizeHint() const
    {
        return QSize(350,350);

    }


    QSize minimumSizeHint() const
    {
        return QSize(280,280);
    }

    /*!

      painting used private functions
      */
private:
    void resetVariables(QPainter* painter);
    void drawBackground(QPainter* painter);
    void drawBox(QPainter* painter);
    void drawText(QPainter* painter);
    void drawGraph(QPainter* painter);
    void drawTitle(QPainter* painter);



private:
    qreal            m_leftSpace;
    qreal            m_topSpace;
    QVector<qreal>   m_dataVec;
    QVector<QPointF> m_potVec;
    bool             m_bUseAntialiasing;
    bool             m_bShowHLine;
    bool             m_bShowPoint;
    QRectF           m_topRect;
    QRectF           m_bottomRect;
    QRectF           m_boxRect;
    QColor           m_hLineClr;
    QString          m_strTitle;

    /*!
      private utility functions
      */
    void initVariables();

    void updateVector();
	
};

#endif // IVFDCHART_H
