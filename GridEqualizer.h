#ifndef BUILDING_H
#define BUILDING_H

#include <Qt3DQuick/QQuickItem3D>
#include <Qt3D/QGLBuilder>

//Extrudes a series of rectangles to 3D cubes of buildings
//Will add texturing and more details in the future.
class GridEqualizer : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(int rows READ getRows WRITE setRows NOTIFY gridChanged)
    Q_PROPERTY(int cols READ getCols WRITE setCols NOTIFY gridChanged)
    Q_PROPERTY(QImage texture READ getTexture WRITE setTexture NOTIFY textureChanged)
public:
    explicit GridEqualizer(QObject* parent = 0);
    virtual ~GridEqualizer();

    //Dims
    int getRows() const { return m_rows;}
    void setRows(int r);
    int getCols() const { return m_cols;}
    void setCols(int c);


    //Texture file path
    QImage getTexture() const;
    void setTexture(const QImage& img);

    //Advance Animation
    Q_INVOKABLE void advanceAnimation();
    Q_INVOKABLE void setTargetHeightsAtRandom();
    Q_INVOKABLE void setTargetHeightsBellCurve(bool bInverseBell = false);
    Q_INVOKABLE void setTargetHeights(const QVector<float>& heights);


Q_SIGNALS:
    void gridChanged();
    void textureChanged();

protected:
    //Main Draw Call
    void drawItem(QGLPainter* lpPainter);

private:
    void createMaterials();
    void createGrid();
    void addBlock(int iRow, int iCol, int idxMaterial = 0);

private:
    //Scene node geometry
    QGLBuilder* m_lpBuilder;
    QGLSceneNode* m_lpSceneNode;
    QGLMaterialCollection* m_lpMtrlCollection;

    //Grid
    int m_cols;
    int m_rows;

    //Heights
    QVector<float> m_vTargetHeights;
    QVector3DArray m_blockVertices;
    QVector3DArray m_blockNormals;
    QVector2DArray m_blockTexCoords;

    //Texture
    QImage m_texture;
};



/*
QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class Q_QT3D_QUICK_EXPORT Line : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QVariant vertices READ vertices WRITE setVertices NOTIFY verticesChanged)

public:
    explicit Line(QObject *parent = 0);
    ~Line();

    QVariant vertices() const;
    void setVertices(const QVariant &value);

    qreal width() const {return m_width;}
    void setWidth(qreal width);

Q_SIGNALS:
    void verticesChanged();
    void widthChanged();

protected:
    void drawItem(QGLPainter *painter);

private:
    qreal m_width;
    QVariant m_vertices;
    QVector3DArray m_vertexArray;
    QGLSceneNode * m_geometry;
    bool m_changeFlag;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(Line)

QT_END_HEADER
*/

#endif // BUILDING_H
