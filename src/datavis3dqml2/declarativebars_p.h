/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDataVis3D module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVis3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef DECLARATIVEBARS_P_H
#define DECLARATIVEBARS_P_H

#include "datavis3dglobal_p.h"
#include "bars3dcontroller_p.h"
#include "declarativebars_p.h"
#include "qitemmodelbardatamapping.h"

#include <QAbstractItemModel>
#include <QQuickItem>
#include <QObject>

QT_DATAVIS3D_BEGIN_NAMESPACE

class DeclarativeBars : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *data READ data WRITE setData)
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode WRITE setSelectionMode)
    Q_PROPERTY(LabelTransparency labelTransparency READ labelTransparency WRITE setLabelTransparency)
    Q_PROPERTY(ShadowQuality shadowQuality READ shadowQuality WRITE setShadowQuality)
    Q_PROPERTY(QFont font READ font WRITE setFont)
    Q_PROPERTY(float fontSize READ fontSize WRITE setFontSize)
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible)
    Q_PROPERTY(bool backgroundVisible READ isBackgroundVisible WRITE setBackgroundVisible)
    Q_PROPERTY(int rows READ rows WRITE setRows)
    Q_PROPERTY(int columns READ columns WRITE setColumns)
    Q_PROPERTY(QItemModelBarDataMapping *mapping READ mapping WRITE setMapping)
    Q_ENUMS(SelectionMode)
    Q_ENUMS(ShadowQuality)
    Q_ENUMS(LabelTransparency)

public:
    // Duplicated here to be able to use the same enums
    enum SelectionMode {
        ModeNone = 0,
        ModeBar,
        ModeBarAndRow,
        ModeBarAndColumn,
        ModeBarRowAndColumn,
        ModeZoomRow,
        ModeZoomColumn
    };

    enum ShadowQuality {
        ShadowNone = 0,
        ShadowLow = 1,
        ShadowMedium = 3,
        ShadowHigh = 5
    };

    enum LabelTransparency {
        TransparencyNone = 0,       // Full solid, using colors from theme
        TransparencyFromTheme,      // Use colors and transparencies from theme
        TransparencyNoBackground    // Draw just text on transparent background
    };

public:
    explicit DeclarativeBars(QQuickItem *parent = 0);
    ~DeclarativeBars();

    void classBegin();
    void componentComplete();

    // Add whole data set.
    void setData(QAbstractItemModel *data);
    QAbstractItemModel *data();

    // bar thickness, spacing between bars, and is spacing relative to thickness or absolute
    // y -component sets the thickness/spacing of z -direction
    // With relative 0.0f means side-to-side, 1.0f = one thickness in between
    Q_INVOKABLE void setBarSpecs(QSizeF thickness = QSizeF(1.0f, 1.0f),
                                 QSizeF spacing = QSizeF(1.0f, 1.0f),
                                 bool relative = true);

    // bar type; bars (=cubes), pyramids, cones, cylinders, etc.
    Q_INVOKABLE void setBarType(BarStyle style, bool smooth = false);

    // override bar type with own mesh
    Q_INVOKABLE void setMeshFileName(const QString &objFileName);

    // how many samples per row and column, and names for axes
    Q_INVOKABLE void setupSampleSpace(int samplesRow, int samplesColumn);

    // Select preset camera placement
    Q_INVOKABLE void setCameraPreset(CameraPreset preset);

    // Set camera rotation if you don't want to use the presets (in horizontal (-180...180) and
    // vertical (0...90) (or (-90...90) if there are negative values) angles and distance in
    // percentage (10...500))
    Q_INVOKABLE void setCameraPosition(qreal horizontal, qreal vertical, int distance);

    // Set theme (bar colors, shaders, window color, background colors, light intensity and text
    // colors are affected)
    Q_INVOKABLE void setTheme(ColorTheme theme);

    // Set color if you don't want to use themes. Set uniform to false if you want the (height)
    // color to change from bottom to top
    Q_INVOKABLE void setBarColor(QColor baseColor, QColor heightColor, QColor depthColor,
                                 bool uniform = true);

    // Set segment count and step. Note; segmentCount * step should be the maximum possible value of data
    // set. Minimum is the absolute minimum possible value a bar can have. This is especially
    // important to set if values can be negative.
    Q_INVOKABLE void setSegmentCount(int segmentCount, qreal step, qreal minimum);

    // Change selection mode; single bar, bar and row, bar and column, or all
    void setSelectionMode(SelectionMode mode);
    SelectionMode selectionMode();

    // Font size adjustment
    void setFontSize(float fontsize);
    float fontSize();

    // Set font
    void setFont(const QFont &font);
    QFont font();

    // Label transparency adjustment
    void setLabelTransparency(LabelTransparency transparency);
    LabelTransparency labelTransparency();

    // Enable or disable background grid
    void setGridVisible(bool visible);
    bool isGridVisible();

    // Enable or disable background mesh
    void setBackgroundVisible(bool visible);
    bool isBackgroundVisible();

    // Adjust shadow quality
    void setShadowQuality(ShadowQuality quality);
    ShadowQuality shadowQuality();

    QItemModelBarDataMapping *mapping() const;
    void setMapping(QItemModelBarDataMapping *mapping);

    int rows() const;
    void setRows(int rows);

    int columns() const;
    void setColumns(int columns);

public slots:
    // Used to detect when shadow quality changes autonomously due to e.g. resizing.
    void handleShadowQualityUpdate(QtDataVis3D::ShadowQuality quality);

signals:
    // Signals shadow quality changes.
    void shadowQualityChanged(DeclarativeBars::ShadowQuality quality);

protected:
    Bars3dController *m_shared;

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QSize m_initialisedSize;
};

QT_DATAVIS3D_END_NAMESPACE

#endif
