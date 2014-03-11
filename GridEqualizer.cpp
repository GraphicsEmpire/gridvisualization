#include "GridEqualizer.h"
#include "PS_MathBase.h"
#include <QGraphicsScale3D>

#define BLOCKS_ACROSS_X 16
#define BLOCKS_ACROSS_Z 16

GridEqualizer::GridEqualizer(QObject *parent):QQuickItem3D(parent)
{
    //Init
    m_lpSceneNode = NULL;
    m_lpBuilder = NULL;

    m_cols = BLOCKS_ACROSS_X;
    m_rows = BLOCKS_ACROSS_Z;


    createGrid();
}

GridEqualizer::~GridEqualizer()
{   
   SAFE_DELETE(m_lpSceneNode);
   SAFE_DELETE(m_lpBuilder);
  // SAFE_DELETE(m_lpMtrlCollection);
}

void GridEqualizer::setCols(int c)
{
    m_cols = c;
    createGrid();
    emit gridChanged();
}

void GridEqualizer::setRows(int r)
{
    m_rows = r;
    createGrid();
    emit gridChanged();
}

QImage GridEqualizer::getTexture() const {
    return m_texture;
}

void GridEqualizer::setTexture(const QImage& img) {
    m_texture = img;
    createGrid();
    emit textureChanged();
    emit gridChanged();
}


void GridEqualizer::advanceAnimation() {
    if(m_lpSceneNode == NULL)
        return;
    if(m_lpSceneNode->children().count() != m_vTargetHeights.size())
        return;

    for(int i=0; i < m_cols; i++) {
        for(int j=0; j < m_rows; j++) {
            int index = i * m_rows + j;
            QGLSceneNode* lpNode = m_lpSceneNode->children()[index];
            QGraphicsScale3D* lpScale = dynamic_cast<QGraphicsScale3D*>(lpNode->transforms()[0]);

            float currentH = lpScale->scale().y();
            currentH += (m_vTargetHeights[index] - currentH) * .2;
            lpScale->setScale(QVector3D(1, currentH, 1));
        }
    }
    update();
}

void GridEqualizer::setTargetHeightsAtRandom() {
    for(int i=0; i < m_vTargetHeights.size(); i++)
        m_vTargetHeights[i] = RandRangeT<float>(1.0f, 4.0f);
}

void GridEqualizer::setTargetHeightsBellCurve(bool bInverseBell) {

    int hw = m_cols >> 1;
    int hh = m_rows >> 1;

    float t = RandRangeT<float>(1.0f, 8.0f);
    for(int i=0; i < m_cols; i++) {
        for(int j=0; j < m_rows; j++) {
            int index = i * m_rows + j;
            int ii = i - hw;
            int jj = j - hh;
            if(bInverseBell)
                m_vTargetHeights[index] = t - t * exp(-(0.005* ii*ii + 0.005*jj*jj));
            else
                m_vTargetHeights[index] = t * exp(-(0.005* ii*ii + 0.005*jj*jj));
        }
    }
}

void GridEqualizer::setTargetHeights(const QVector<float>& heights) {
    if(heights.size() == m_cols * m_rows)
        m_vTargetHeights = heights;
}

//Create all materials
void GridEqualizer::createMaterials() {
    QStringList lstMaterialNames;
    lstMaterialNames << "red" << "green" << "blue" << "orange" << "pink" << "gray";
    m_lpMtrlCollection = new QGLMaterialCollection(this);
    for(int i=0; i<lstMaterialNames.size(); i++)
    {
        QColor clMtrl(lstMaterialNames[i].toLatin1().data());
        QGLMaterial* mtrl = new QGLMaterial();
        mtrl->setDiffuseColor(clMtrl);
        m_lpMtrlCollection->addMaterial(mtrl);
    }
}

/*!
  * Create the map structure with buildings and streets
  */
void GridEqualizer::createGrid()
{
    if(m_cols == 0 || m_rows == 0)
        return;
    SAFE_DELETE(m_lpSceneNode);
    SAFE_DELETE(m_lpBuilder);

    //Materials
    createMaterials();

    //Geometries
    m_lpBuilder = new QGLBuilder(m_lpMtrlCollection);
    m_vTargetHeights.resize(m_cols * m_rows);
    m_vTargetHeights.fill(1.0f);

    //Vertices
    m_blockVertices.resize(0);
    m_blockVertices.append(0, 0, 0);
    m_blockVertices.append(0, 0, 1);
    m_blockVertices.append(1, 0, 1);
    m_blockVertices.append(1, 0, 0);
    m_blockVertices.append(0, 0, 0);

    //TexCoords
    m_blockTexCoords.resize(0);
    m_blockTexCoords.append(QPointF(0, 0));
    m_blockTexCoords.append(QPointF(0, 1));
    m_blockTexCoords.append(QPointF(1, 1));
    m_blockTexCoords.append(QPointF(1, 0));
    m_blockTexCoords.append(QPointF(0, 0));

    //Normals
    m_blockNormals = m_blockVertices;
    for(int i=0; i<m_blockNormals.size(); i++) {
        m_blockNormals[i] = m_blockNormals[i].normalized();
    }


    //Texture Coords
//    m_blockTexCoords.resize(0);
//    m_blockTexCoords.append(0, 0);
//    m_blockTexCoords.append(0, 1);
//    m_blockTexCoords.append(1, 1);
//    m_blockTexCoords.append(1, 0);
//    m_blockTexCoords.append(0, 0);



    //1.Streets
    /*
    int xMaxHalf = m_cols/2;
    int zMaxHalf = m_rows/2;

    QGeometryData street;
    for(int i=0; i <= m_gridWidth; i++)
    {
        if(i % 8 == 0)
        {
            street.appendVertex(QVector3D(i, 0, 0));
            street.appendVertex(QVector3D(i, 0, m_gridHeight));
            street.appendVertex(QVector3D(i + 2, 0, m_gridHeight));
            street.appendVertex(QVector3D(i + 2, 0, 0));
        }
    }

    for(int j=0; j <= m_gridHeight; j++)
    {
        if(j % 8 == 0)
        {
            street.appendVertex(QVector3D(0, 0.01, j));
            street.appendVertex(QVector3D(0, 0.01, j+2));
            street.appendVertex(QVector3D(m_gridWidth, 0.01, j+2));
            street.appendVertex(QVector3D(m_gridWidth, 0.01, j));

        }
    }
    m_lpBuilder->newSection();
    m_lpBuilder->addQuads(street);
    m_lpBuilder->currentNode()->setEffect(QGL::LitMaterial);
    m_lpBuilder->currentNode()->setMaterialIndex(5);
    QMatrix4x4 mat;
    mat.translate(-xMaxHalf, 0, -zMaxHalf);
    m_lpBuilder->currentNode()->setLocalTransform(mat);
    */


    //2.Blocks
    for(int i=0; i<m_cols; i++)
    {
        for(int j=0; j<m_rows; j++)
        {
            //if((abs(i - xMaxHalf) < 16)&&(abs(j - zMaxHalf) < 16))
              //  continue;
            //if((i % 8 > 1)&&(j % 8 > 1))
            {
                int idxMtrl = (int)RandRangeT<float>(0, m_lpMtrlCollection->size());
                this->addBlock(i, j, idxMtrl);
            }

        }
    }


    //Create final scene graph root node
    m_lpSceneNode = m_lpBuilder->finalizedSceneNode();
    {
        QMatrix4x4 mat;
//        QQuaternion q1 = QQuaternion::fromAxisAndAngle(0, 1, 0, 45.0);
//        QQuaternion q2 = QQuaternion::fromAxisAndAngle(1, 0, 1, 10.0);
        //QQuaternion q1 = QQuaternion::fromAxisAndAngle(0, 1, 0, 45.0);
        QQuaternion q2 = QQuaternion::fromAxisAndAngle(1, 0, 0, 2.0);

        mat.scale(QVector3D(0.05, 0.05, 0.05));
        mat.translate(0, -4, 0);
        //mat.rotate(q1*q2);
        mat.rotate(q2);
        m_lpSceneNode->setLocalTransform(mat);
    }
}

/*!
  * Adds a single Building to the map
  */
void GridEqualizer::addBlock(int iRow, int iCol, int idxMaterial)
{
    int xMaxHalf = m_cols/2;
    int zMaxHalf = m_rows/2;
    QVector3D pos = QVector3D(iRow - xMaxHalf, 0, iCol - zMaxHalf);

    //Create Building Base and close its polygon
    QGeometryData base;
    base.appendVertexArray(m_blockVertices);
    base.appendNormalArray(m_blockNormals);

    //Roof
    QGeometryData roof = base.translated(QVector3D(0, 1, 0));
    QGeometryData roofTextured;
    roofTextured.appendVertexArray(m_blockVertices);
    roofTextured.appendNormalArray(m_blockNormals);
    roofTextured.appendTexCoordArray(m_blockTexCoords);


    //Build SceneNode Roof first and then the rest
    m_lpBuilder->newSection();
    m_lpBuilder->addQuadsInterleaved(roof, base);
    m_lpBuilder->addTriangulatedFace(roof);
    m_lpBuilder->addTriangulatedFace(base.reversed());
    m_lpBuilder->currentNode()->setMaterialIndex(idxMaterial);
    m_lpBuilder->currentNode()->setEffect(QGL::LitMaterial);

    //Transformation Node
    QGraphicsScale3D* lpScale = new QGraphicsScale3D();
    lpScale->setScale(QVector3D(1, 1, 1));
    m_lpBuilder->currentNode()->addTransform(lpScale);

    //Local Transform
    QMatrix4x4 mat;
    mat.translate(pos);

    m_lpBuilder->currentNode()->setLocalTransform(mat);
}

void GridEqualizer::drawItem(QGLPainter* lpPainter)
{
    //Draw the item with OpenGL
    if(m_lpSceneNode)
        m_lpSceneNode->draw(lpPainter);
}

/*


void Line::drawItem(QGLPainter *painter)
{
    if (m_changeFlag || !m_geometry) {
        if (m_geometry) delete m_geometry;

        QGLBuilder builder;

        QGeometryData lineCollection;
        builder.newSection();

        lineCollection.appendVertexArray(m_vertexArray);

        builder.addTriangles(lineCollection);
        builder.currentNode()->setDrawingMode(QGL::LineStrip);
        builder.currentNode()->setDrawingWidth(m_width);
        m_geometry = builder.finalizedSceneNode();

        m_changeFlag = false;
    }

    // Draw the geometry.
    m_geometry->draw(painter);
}

QT_END_NAMESPACE

*/
