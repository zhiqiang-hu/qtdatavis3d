/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVisualization module.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

#include "q3dbars.h"
#include "q3dbars_p.h"
#include "bars3dcontroller_p.h"
#include "q3dvalueaxis.h"
#include "q3dcategoryaxis.h"
#include "qbardataproxy.h"

#include <QMouseEvent>

#include <QDebug>

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

/*!
 * \class Q3DBars
 * \inmodule QtDataVisualization
 * \brief The Q3DBars class provides methods for rendering 3D bar graphs.
 * \since 1.0.0
 *
 * This class enables developers to render bar graphs in 3D and to view them by rotating the scene
 * freely. Rotation is done by holding down the right mouse button and moving the mouse. Zooming
 * is done by mouse wheel. Selection, if enabled, is done by left mouse button. The scene can be
 * reset to default camera view by clicking mouse wheel. In touch devices rotation is done
 * by tap-and-move, selection by tap-and-hold and zoom by pinch.
 *
 * If no axes are explicitly set to Q3DBars, temporary default axes with no labels are created.
 * These default axes can be modified via axis accessors, but as soon any axis is explicitly
 * set for the orientation, the default axis for that orientation is destroyed.
 *
 * Data proxies work similarly: If no data proxy is explicitly set, Q3DBars creates a default
 * proxy. If any other proxy is set as active data proxy later, the default proxy and all data
 * added to it is destroyed.
 *
 * Methods are provided for changing bar types, themes, bar selection modes and so on. See the
 * methods for more detailed descriptions.
 *
 * \section1 How to construct a minimal Q3DBars chart
 *
 * After constructing Q3DBars, you can set the data window by changing the range on the row and
 * column axes. It is not mandatory, as data window will default to showing all of the data in
 * the data proxy. If the amount of data is large, it is usually preferable to show just a
 * portion of it. For the example, let's set the data window to show first five rows and columns:
 *
 * \snippet doc_src_q3dbars_construction.cpp 0
 *
 * Now Q3DBars is ready to receive data to be rendered. Add one row of 5 qreals into the data
 * set:
 *
 * \snippet doc_src_q3dbars_construction.cpp 1
 *
 * \note We set the sample space to 5 x 5, but we are inserting only one row of data. This is ok,
 * the rest of the rows will just be blank.
 *
 * Finally you will need to set it visible:
 *
 * \snippet doc_src_q3dbars_construction.cpp 2
 *
 * The complete code needed to create and display this chart is:
 *
 * \snippet doc_src_q3dbars_construction.cpp 3
 *
 * And this is what those few lines of code produce:
 *
 * \image q3dbars-minimal.png
 *
 * The scene can be rotated, zoomed into, and a bar can be selected to view it's value,
 * but no other interaction is included in this minimal code example. You can learn more by
 * familiarizing yourself with the examples provided, like the \l{Rainfall Example} or
 * the \l{Widget Example}.
 *
 * \sa Q3DScatter, Q3DSurface, {Qt Data Visualization C++ Classes}
 */

/*!
 * \qmltype Bars3D
 * \instantiates Q3DBars
 *
 * This type enables developers to render bar graphs in 3D with Qt Quick 2.
 *
 * You will need to import data visualization module to use this type:
 *
 * \snippet doc_src_qmldatavisualization.cpp 0
 *
 * After that you can use Bars3D in your qml files:
 *
 * \snippet doc_src_qmldatavisualization.cpp 1
 *
 * See \l{Qt Quick 2 Barchart Example} for more thorough usage example.
 *
 * \sa ItemModelBarDataProxy, Scatter3D, Surface3D, {Qt Data Visualization C++ Classes}
 */

/*!
 * \qmlproperty BarDataProxy Bars3D::dataProxy
 * The active data proxy.
 *
 * If a proxy is not given, a temporary default proxy is created and activated.
 * This temporary proxy is destroyed if another proxy is explicitly set active via this property.
 */

/*!
 * \qmlproperty CategoryAxis3D Bars3D::rowAxis
 * A user-defined row axis.
 *
 * If an axis is not given, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another axis is explicitly set to same orientation.
 */

/*!
 * \qmlproperty ValueAxis3D Bars3D::valueAxis
 * A user-defined value axis.
 *
 * If an axis is not given, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another axis is explicitly set to same orientation.
 */

/*!
 * \qmlproperty CategoryAxis3D Bars3D::columnAxis
 * A user-defined column axis.
 *
 * If an axis is not given, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another axis is explicitly set to same orientation.
 */

/*!
 * \qmlproperty Bars3D.SelectionMode Bars3D::selectionMode
 * Bar selection mode.
 */

/*!
 * \qmlproperty Bars3D.LabelTransparency Bars3D::labelTransparency
 * Label transparency.
 */

/*!
 * \qmlproperty Bars3D.ShadowQuality Bars3D::shadowQuality
 * Shadow quality.
 */

/*!
 * \qmlproperty Bars3D.MeshStyle Bars3D::barType
 * Bar object type.
 */

/*!
 * \qmlproperty Bars3D.CameraPreset Bars3D::cameraPreset
 * Camera preset.
 */

/*!
 * \qmlproperty Bars3D.ColorTheme Bars3D::theme
 * Theme of the graph. Theme affects bar colors, label colors, text color, background color, window
 * color and grid color. Lighting is also adjusted by themes.
 */

/*!
 * \qmlproperty real Bars3D::barThickness
 * Bar thickness ratio between X and Z dimensions. 1.0 means bars are as wide as they are deep, 0.5
 * makes them twice as deep as they are wide.
 */

/*!
 * \qmlproperty size Bars3D::barSpacing
 * Bar spacing in X and Z dimensions.
 */

/*!
 * \qmlproperty bool Bars3D::barSpacingRelative
 * Relative or absolute bar spacing.
 */

/*!
 * \qmlproperty bool Bars3D::barSmoothingEnabled
 * Bar smoothing. If false, bar shading is flat.
 */

/*!
 * \qmlproperty string Bars3D::meshFileName
 * Override bar type with a mesh object.
 * \note Object needs to be in Wavefront obj format and include vertices, normals and UVs.
 * It also needs to be in triangles.
 */

/*!
 * \qmlproperty font Bars3D::font
 * Font used for labels.
 */

/*!
 * \qmlproperty bool Bars3D::gridVisible
 * Grid visibility. If false, grid lines are not drawn.
 */

/*!
 * \qmlproperty bool Bars3D::backgroundVisible
 * Background visibility. If false, background is not drawn.
 */

/*!
 * \qmlproperty int Bars3D::rows
 * Row count of data window.
 */

/*!
 * \qmlproperty int Bars3D::columns
 * Column count of data window.
 */

/*!
 * \qmlproperty string Bars3D::itemLabelFormat
 * Label format of single item labels, e.g. a selected bar.
 */

/*!
 * \qmlproperty point Bars3D::selectedBarPos
 * Position of the selected bar in data window. Only one bar can be selected at a time.
 * To clear selection, specify an illegal position, e.g. Qt.point(-1.0, -1.0).
 */

/*!
 * Constructs a new 3D bar window.
 */
Q3DBars::Q3DBars()
    : d_ptr(new Q3DBarsPrivate(this, geometry()))
{
    d_ptr->m_shared->initializeOpenGL();
    QObject::connect(d_ptr->m_shared, &Bars3DController::selectedBarPosChanged, this,
                     &Q3DBars::selectedBarPosChanged);
    QObject::connect(d_ptr->m_shared, &Abstract3DController::needRender, this,
                     &Q3DWindow::renderLater);
}

/*!
 *  Destroys the 3D bar window.
 */
Q3DBars::~Q3DBars()
{
}

/*!
 * \internal
 */
void Q3DBars::render()
{
    d_ptr->m_shared->synchDataToRenderer();
    d_ptr->m_shared->render();
}

#if defined(Q_OS_ANDROID)
/*!
 * \internal
 */
void Q3DBars::mouseDoubleClickEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseDoubleClickEvent(event);
}

/*!
 * \internal
 */
void Q3DBars::touchEvent(QTouchEvent *event)
{
    d_ptr->m_shared->touchEvent(event);
}
#endif

/*!
 * \internal
 */
void Q3DBars::mousePressEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mousePressEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DBars::mouseReleaseEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseReleaseEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DBars::mouseMoveEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseMoveEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DBars::wheelEvent(QWheelEvent *event)
{
    d_ptr->m_shared->wheelEvent(event);
}

/*!
 * \internal
 */
void Q3DBars::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    d_ptr->m_shared->setSize(width(), height());
}

/*!
 * Sets window \a width.
 */
void Q3DBars::setWidth(const int width)
{
    d_ptr->m_shared->setWidth(width);
    QWindow::setWidth(width);
}

/*!
 * Sets window \a height.
 */
void Q3DBars::setHeight(const int height)
{
    d_ptr->m_shared->setHeight(height);
    QWindow::setHeight(height);
}

/*!
 * \property Q3DBars::barThickness
 *
 * Bar thickness ratio between X and Z dimensions. 1.0 means bars are as wide as they are deep, 0.5
 * makes them twice as deep as they are wide. It is preset to \c 1.0 by default.
 */
void Q3DBars::setBarThickness(qreal thicknessRatio)
{
    d_ptr->m_shared->setBarSpecs(GLfloat(thicknessRatio), barSpacing(), isBarSpacingRelative());
}

qreal Q3DBars::barThickness()
{
    return d_ptr->m_shared->barThickness();
}

/*!
 * \property Q3DBars::barSpacing
 *
 * Bar spacing, ie. the empty space between bars, in X and Z dimensions. It is preset to
 * \c {(1.0, 1.0)} by default. Spacing is affected by barSpacingRelative -property.
 *
 * \sa barSpacingRelative
 */
void Q3DBars::setBarSpacing(QSizeF spacing)
{
    d_ptr->m_shared->setBarSpecs(GLfloat(barThickness()), spacing, isBarSpacingRelative());
}

QSizeF Q3DBars::barSpacing()
{
    return d_ptr->m_shared->barSpacing();
}

/*!
 * \property Q3DBars::barSpacingRelative
 *
 * This is used to indicate if spacing is meant to be absolute or relative to bar thickness.
 * If it is true, value of 0.0 means the bars are side-to-side and for example 1.0 means
 * there is one thickness in between the bars. It is preset to \c true.
 */
void Q3DBars::setBarSpacingRelative(bool relative)
{
    d_ptr->m_shared->setBarSpecs(GLfloat(barThickness()), barSpacing(), relative);
}

bool Q3DBars::isBarSpacingRelative()
{
    return d_ptr->m_shared->isBarSpecRelative();
}

/*!
 * Sets the bar \a style to one of the values in \c QDataVis::MeshStyle. It is preset to
 * \c QDataVis::Bars by default. A \a smooth flag can be used to set shading to smooth.
 * It is \c false by default.
 *
 * \sa setMeshFileName()
 */
void Q3DBars::setBarType(QDataVis::MeshStyle style, bool smooth)
{
    d_ptr->m_shared->setBarType(style, smooth);
}

/*!
 * \property Q3DBars::cameraPreset
 *
 * The \a preset position of the camera. The position can be one of \c QDataVis::CameraPreset.
 */
void Q3DBars::setCameraPreset(QDataVis::CameraPreset preset)
{
    d_ptr->m_shared->setCameraPreset(preset);
}

QDataVis::CameraPreset Q3DBars::cameraPreset() const
{
    return d_ptr->m_shared->cameraPreset();
}

/*!
 * Move camera to a wanted position based on \a horizontal and \a vertical angles. Angles are limited
 * to -180...180 in horizontal direction and either -90...90 or 0...90 in vertical, depending
 * on data values. Negative vertical angles are allowed only if there are negative bar values.
 * \a distance is adjustable between 10 and 500, being \c 100 by default.
 */
void Q3DBars::setCameraPosition(qreal horizontal, qreal vertical, int distance)
{
    d_ptr->m_shared->setCameraPosition(GLfloat(horizontal), GLfloat(vertical), GLint(distance));
}

/*!
 * Sets a predefined \a theme from \c QDataVis::ColorTheme. It is preset to \c QDataVis::ThemeSystem by
 * default. Theme affects bar colors, label colors, text color, background color, window color and
 * grid color. Lighting is also adjusted by themes.
 *
 * \sa setBarColor()
 *
 * \warning This method is subject to change.
 */
void Q3DBars::setTheme(QDataVis::ColorTheme theme)
{
    d_ptr->m_shared->setColorTheme(theme);
}

/*!
 * Set bar color using your own color. \a baseColor sets the base color of a bar. The \a uniform
 * -flag is used to define if color needs to be uniform throughout bar's length, or will the colors
 * be applied by height, starting with dark at the bottom. It is \c true by default.
 *
 * Calling this method overrides colors from theme.
 *
 * \sa setTheme()
 *
 * \warning This method is subject to change.
 */
void Q3DBars::setBarColor(const QColor &baseColor, bool uniform)
{
    d_ptr->m_shared->setObjectColor(baseColor, uniform);
}

/*!
 * \return bar color in use.
 */
QColor Q3DBars::barColor() const
{
    return d_ptr->m_shared->objectColor();
}

/*!
 * \property Q3DBars::selectionMode
 *
 * Sets bar selection \a mode to one of \c QDataVis::SelectionMode. It is preset to
 * \c QDataVis::ModeItem by default.
 */
void Q3DBars::setSelectionMode(QDataVis::SelectionMode mode)
{
    d_ptr->m_shared->setSelectionMode(mode);
}

QDataVis::SelectionMode Q3DBars::selectionMode() const
{
    return d_ptr->m_shared->selectionMode();
}

/*!
 * \property Q3DBars::meshFileName
 *
 * Override bar type with a mesh object located in \a objFileName.
 * \note Object needs to be in Wavefront obj format and include vertices, normals and UVs.
 * It also needs to be in triangles.
 *
 * \sa setBarType()
 */
void Q3DBars::setMeshFileName(const QString &objFileName)
{
    d_ptr->m_shared->setMeshFileName(objFileName);
}

QString Q3DBars::meshFileName() const
{
    return d_ptr->m_shared->meshFileName();
}

/*!
 * \property Q3DBars::font
 *
 * Sets the \a font for labels. It is preset to \c Arial by default.
 */
void Q3DBars::setFont(const QFont &font)
{
    d_ptr->m_shared->setFont(font);
}

QFont Q3DBars::font() const
{
    return d_ptr->m_shared->font();
}

/*!
 * \property Q3DBars::labelTransparency
 *
 * Sets label \a transparency to one of \c QDataVis::LabelTransparency. It is preset to
 * \c QDataVis::TransparencyFromTheme by default.
 */
void Q3DBars::setLabelTransparency(QDataVis::LabelTransparency transparency)
{
    d_ptr->m_shared->setLabelTransparency(transparency);
}

QDataVis::LabelTransparency Q3DBars::labelTransparency() const
{
    return d_ptr->m_shared->labelTransparency();
}

/*!
 * \property Q3DBars::gridVisible
 *
 * Sets grid visibility to \a visible. It is preset to \c true by default.
 */
void Q3DBars::setGridVisible(bool visible)
{
    d_ptr->m_shared->setGridEnabled(visible);
}

bool Q3DBars::isGridVisible() const
{
    return d_ptr->m_shared->gridEnabled();
}

/*!
 * \property Q3DBars::backgroundVisible
 *
 * Sets background visibility to \a visible. It is preset to \c true by default.
 */
void Q3DBars::setBackgroundVisible(bool visible)
{
    d_ptr->m_shared->setBackgroundEnabled(visible);
}

bool Q3DBars::isBackgroundVisible() const
{
    return d_ptr->m_shared->backgroundEnabled();
}

/*!
 * \property Q3DBars::selectedBarPos
 *
 * Selects a bar in a \a position. The position is the position in data window.
 * Only one bar can be selected at a time.
 * To clear selection, specify an illegal \a position, e.g. (-1, -1).
 */
void Q3DBars::setSelectedBarPos(const QPoint &position)
{
    d_ptr->m_shared->setSelectedBarPos(position);
}

QPoint Q3DBars::selectedBarPos() const
{
    return d_ptr->m_shared->selectedBarPos();
}

/*!
 * \property Q3DBars::shadowQuality
 *
 * Sets shadow \a quality to one of \c QDataVis::ShadowQuality. It is preset to
 * \c QDataVis::ShadowMedium by default.
 *
 * \note If setting QDataVis::ShadowQuality of a certain level fails, a level is lowered
 * until it is successful and shadowQualityChanged signal is emitted for each time the change is
 * done.
 */
void Q3DBars::setShadowQuality(QDataVis::ShadowQuality quality)
{
    d_ptr->m_shared->setShadowQuality(quality);
}

QDataVis::ShadowQuality Q3DBars::shadowQuality() const
{
    return d_ptr->m_shared->shadowQuality();
}

/*!
 * Sets a user-defined row \a axis. Implicitly calls addAxis() to transfer ownership of
 * the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DBars::setRowAxis(Q3DCategoryAxis *axis)
{
    d_ptr->m_shared->setAxisX(axis);
}

/*!
 * \return category axis for rows.
 */
Q3DCategoryAxis *Q3DBars::rowAxis() const
{
    return static_cast<Q3DCategoryAxis *>(d_ptr->m_shared->axisX());
}

/*!
 * Sets a user-defined column \a axis. Implicitly calls addAxis() to transfer ownership of
 * the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DBars::setColumnAxis(Q3DCategoryAxis *axis)
{
    d_ptr->m_shared->setAxisZ(axis);
}

/*!
 * \return category axis for columns.
 */
Q3DCategoryAxis *Q3DBars::columnAxis() const
{
    return static_cast<Q3DCategoryAxis *>(d_ptr->m_shared->axisZ());
}

/*!
 * Sets a user-defined value \a axis (the Y-axis). Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DBars::setValueAxis(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisY(axis);
}

/*!
 * \return used value axis (Y-axis).
 */
Q3DValueAxis *Q3DBars::valueAxis() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisY());
}

/*!
 * Adds \a axis to the graph. The axes added via addAxis are not yet taken to use,
 * addAxis is simply used to give the ownership of the \a axis to the graph.
 * The \a axis must not be null or added to another graph.
 *
 * \sa releaseAxis(), setValueAxis(), setRowAxis(), setColumnAxis()
 */
void Q3DBars::addAxis(Q3DAbstractAxis *axis)
{
    d_ptr->m_shared->addAxis(axis);
}

/*!
 * Releases the ownership of the \a axis back to the caller, if it is added to this graph.
 * If the released \a axis is in use, a new default axis will be created and set active.
 *
 * If the default axis is released and added back later, it behaves as any other axis would.
 *
 * \sa addAxis(), setValueAxis(), setRowAxis(), setColumnAxis()
 */
void Q3DBars::releaseAxis(Q3DAbstractAxis *axis)
{
    d_ptr->m_shared->releaseAxis(axis);
}

/*!
 * \return list of all added axes.
 *
 * \sa addAxis()
 */
QList<Q3DAbstractAxis *> Q3DBars::axes() const
{
    return d_ptr->m_shared->axes();
}

/*!
 * Sets the active data \a proxy. Implicitly calls addDataProxy() to transfer ownership of
 * the \a proxy to this graph.
 *
 * If the \a proxy is null, a temporary default proxy is created and activated.
 * This temporary proxy is destroyed if another \a proxy is explicitly set active via this method.
 *
 * \sa addDataProxy(), releaseDataProxy()
 */
void Q3DBars::setActiveDataProxy(QBarDataProxy *proxy)
{
    d_ptr->m_shared->setActiveDataProxy(proxy);
}

/*!
 * \return active data proxy.
 */
QBarDataProxy *Q3DBars::activeDataProxy() const
{
    return static_cast<QBarDataProxy *>(d_ptr->m_shared->activeDataProxy());
}

/*!
 * Adds data \a proxy to the graph. The proxies added via addDataProxy are not yet taken to use,
 * addDataProxy is simply used to give the ownership of the data \a proxy to the graph.
 * The \a proxy must not be null or added to another graph.
 *
 * \sa releaseDataProxy(), setActiveDataProxy()
 */
void Q3DBars::addDataProxy(QBarDataProxy *proxy)
{
    d_ptr->m_shared->addDataProxy(proxy);
}

/*!
 * Releases the ownership of the data \a proxy back to the caller, if it is added to this graph.
 * If the released \a proxy is in use, a new empty default proxy is created and taken to use.
 *
 * If the default \a proxy is released and added back later, it behaves as any other proxy would.
 *
 * \sa addDataProxy(), setActiveDataProxy()
 */
void Q3DBars::releaseDataProxy(QBarDataProxy *proxy)
{
    d_ptr->m_shared->releaseDataProxy(proxy);
}

/*!
 * \return list of all added data proxies.
 *
 * \sa addDataProxy()
 */
QList<QBarDataProxy *> Q3DBars::dataProxies() const
{
    QList<QBarDataProxy *> retList;
    QList<QAbstractDataProxy *> abstractList = d_ptr->m_shared->dataProxies();
    foreach (QAbstractDataProxy *proxy, abstractList)
        retList.append(static_cast<QBarDataProxy *>(proxy));

    return retList;
}

Q3DBarsPrivate::Q3DBarsPrivate(Q3DBars *q, QRect rect)
    : q_ptr(q),
      m_shared(new Bars3DController(rect))
{
    QObject::connect(m_shared, &Abstract3DController::shadowQualityChanged, this,
                     &Q3DBarsPrivate::handleShadowQualityUpdate);
}

Q3DBarsPrivate::~Q3DBarsPrivate()
{
    qDebug() << "Destroying Q3DBarsPrivate";
    delete m_shared;
}

void Q3DBarsPrivate::handleShadowQualityUpdate(QDataVis::ShadowQuality quality)
{
    emit q_ptr->shadowQualityChanged(quality);
}

QT_DATAVISUALIZATION_END_NAMESPACE
