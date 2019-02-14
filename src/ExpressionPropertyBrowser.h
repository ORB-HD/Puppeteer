/* 
 * Puppeteer - A Motion Capture Mapping Tool
 * Copyright (c) 2013-2016 Martin Felis <martin.felis@iwr.uni-heidelberg.de>.
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE* 
 */

#ifndef MARKER_EXPRESSION_PROPERTY_BROWSER
#define MARKER_EXPRESSION_PROPERTY_BROWSER

#include <QtGui/QVector3D>
#include <vendor/QtPropertyBrowser/src/QtDoublePropertyManager>
#include <QtCore/QMap>
#include "ExpressionLuaTable.h"

using namespace std;


class ExpressionVector3DPropertyManagerPrivate;

class ExpressionVector3DPropertyManager : public QtAbstractPropertyManager {
Q_OBJECT
public:
    ExpressionVector3DPropertyManager(QObject *parent = 0);
    ~ExpressionVector3DPropertyManager();

    QtStringPropertyManager *subStringPropertyManager() const;

    ExpressionVector3D value(const QtProperty *property) const;
    void setPropertyLabels (const QString &label_x, const QString &label_y, const QString &label_z);
public Q_SLOTS:
    void setValue(QtProperty *property, const ExpressionVector3D &val);
Q_SIGNALS:
    void valueChanged(QtProperty *property, const ExpressionVector3D &val);
protected:
    QString valueText(const QtProperty *property) const;
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
private:
    ExpressionVector3DPropertyManagerPrivate *d_ptr;
    QString propertyLabel[3];
    Q_DECLARE_PRIVATE(ExpressionVector3DPropertyManager)
    Q_DISABLE_COPY(ExpressionVector3DPropertyManager)
    Q_PRIVATE_SLOT(d_func(), void slotStringChanged(QtProperty *, QString))
    Q_PRIVATE_SLOT(d_func(), void slotPropertyDestroyed(QtProperty *))
};

class ExpressionVector3DPropertyManagerPrivate {
    ExpressionVector3DPropertyManager *q_ptr;

    Q_DECLARE_PUBLIC(ExpressionVector3DPropertyManager)

public:

    void slotStringChanged(QtProperty *property, QString value);

    void slotPropertyDestroyed(QtProperty *property);

    typedef QMap<const QtProperty *, ExpressionVector3D> PropertyValueMap;
    PropertyValueMap m_values;

    QtStringPropertyManager *m_stringPropertyManager;

    QMap<const QtProperty *, QtProperty *> m_propertyToX;
    QMap<const QtProperty *, QtProperty *> m_propertyToY;
    QMap<const QtProperty *, QtProperty *> m_propertyToZ;

    QMap<const QtProperty *, QtProperty *> m_xToProperty;
    QMap<const QtProperty *, QtProperty *> m_yToProperty;
    QMap<const QtProperty *, QtProperty *> m_zToProperty;
};

class ExpressionVector3DEditorFactoryPrivate;

class ExpressionVector3DEditWidget;

class ExpressionVector3DEditorFactory : public QtAbstractEditorFactory<ExpressionVector3DPropertyManager>
{
Q_OBJECT
public:
    ExpressionVector3DEditorFactory(QObject *parent = 0);
    ~ExpressionVector3DEditorFactory();
protected:
    void connectPropertyManager(ExpressionVector3DPropertyManager *manager);
    QWidget *createEditor(ExpressionVector3DPropertyManager *manager, QtProperty *property, QWidget *parent);
    void disconnectPropertyManager(ExpressionVector3DPropertyManager *manager);
private:
    ExpressionVector3DEditorFactoryPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ExpressionVector3DEditorFactory)
    Q_DISABLE_COPY(ExpressionVector3DEditorFactory)
    Q_PRIVATE_SLOT(d_func(), void slotPropertyChanged(QtProperty *, const ExpressionVector3D &))
    Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed(QObject *))
    Q_PRIVATE_SLOT(d_func(), void slotSetValue(const ExpressionVector3D &))
};

class ExpressionVector3DEditorFactoryPrivate {
    ExpressionVector3DEditorFactory *q_ptr;

    Q_DECLARE_PUBLIC(ExpressionVector3DEditorFactory)

    typedef QList<ExpressionVector3DEditWidget *> EditorList;
    typedef QMap<QtProperty *, EditorList> PropertyToEditorListMap;
    typedef QMap<ExpressionVector3DEditWidget *, QtProperty *> EditorToPropertyMap;

    ExpressionVector3DEditWidget *createEditor(QtProperty *property, QWidget *parent);
    void initializeEditor(QtProperty *property, ExpressionVector3DEditWidget *e);
    void slotEditorDestroyed(QObject *object);

    PropertyToEditorListMap  m_createdEditors;
    EditorToPropertyMap m_editorToProperty;

public:
    void slotPropertyChanged(QtProperty *property, const ExpressionVector3D &value);

    void slotSetValue(const ExpressionVector3D &value);
};


/* MARKER_EXPRESSION_PROPERTY_BROWSER */
#endif
