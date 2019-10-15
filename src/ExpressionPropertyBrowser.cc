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


#include "ExpressionLuaTable.h"
#include "ExpressionPropertyBrowser.h"
#include <QMap>
#include <QLabel>
#include <QPainter>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <vendor/QtPropertyBrowser/src/QtTreePropertyBrowser>

#include "qteditorfactory.h"
#include "luatables.h"


void ExpressionVector3DPropertyManagerPrivate::markPropertyRed(QtProperty *property) {
    for (auto &item : q_ptr->parent->items(property)) {
        q_ptr->parent->setBackgroundColor(item, QColor(255, 128, 128));
    }
}

void ExpressionVector3DPropertyManagerPrivate::unmarkPropertyRed(QtProperty *property) {
    for (auto &item : q_ptr->parent->items(property)) {
        q_ptr->parent->setBackgroundColor(item, QColor(255, 255, 255));
    }
}

void ExpressionVector3DPropertyManagerPrivate::slotStringChanged(QtProperty *property, QString value) {
    if (QtProperty *prop = m_xToProperty.value(property, 0)) {
        ExpressionVector3D v = m_values[prop];
        try {
            v.setX(parseExpression(value.toStdString(), q_ptr->model->expressionVariables));
            unmarkPropertyRed(property);
        } catch (LuaParseError &e) {
            markPropertyRed(property);
        }
        q_ptr->setValue(prop, v);
    } else if (QtProperty *prop = m_yToProperty.value(property, 0)) {
        ExpressionVector3D v = m_values[prop];
        try {
            v.setY(parseExpression(value.toStdString(), q_ptr->model->expressionVariables));
            unmarkPropertyRed(property);
        } catch (LuaParseError &e) {
            markPropertyRed(property);
        }
        q_ptr->setValue(prop, v);
    } else if (QtProperty *prop = m_zToProperty.value(property, 0)) {
        ExpressionVector3D v = m_values[prop];
        try {
            v.setZ(parseExpression(value.toStdString(), q_ptr->model->expressionVariables));
            unmarkPropertyRed(property);
        } catch (LuaParseError &e) {
            markPropertyRed(property);
        }
        q_ptr->setValue(prop, v);
    }
}

void ExpressionVector3DPropertyManagerPrivate::slotPropertyDestroyed(QtProperty *property) {
    if (QtProperty *pointProp = m_xToProperty.value(property, 0)) {
        m_propertyToX[pointProp] = 0;
        m_xToProperty.remove(property);
    } else if (QtProperty *pointProp = m_yToProperty.value(property, 0)) {
        m_propertyToY[pointProp] = 0;
        m_yToProperty.remove(property);
    } else if (QtProperty *pointProp = m_zToProperty.value(property, 0)) {
        m_propertyToZ[pointProp] = 0;
        m_zToProperty.remove(property);
    }
}

ExpressionVector3DPropertyManager::ExpressionVector3DPropertyManager(QtTreePropertyBrowser *_parent)
        : QtAbstractPropertyManager(_parent) {
    d_ptr = new ExpressionVector3DPropertyManagerPrivate;
    d_ptr->q_ptr = this;
    parent = _parent;

    propertyLabel[0] = "X";
    propertyLabel[1] = "Y";
    propertyLabel[2] = "Z";

    d_ptr->m_stringPropertyManager = new QtStringPropertyManager(this);
    connect(d_ptr->m_stringPropertyManager, SIGNAL(valueChanged(QtProperty * , QString)), this,
            SLOT(slotStringChanged(QtProperty * , QString)));

    connect(d_ptr->m_stringPropertyManager, SIGNAL(propertyDestroyed(QtProperty * )), this,
            SLOT(slotPropertyDestroyed(QtProperty * )));
}

ExpressionVector3DPropertyManager::~ExpressionVector3DPropertyManager() {
    clear();
    delete d_ptr;
}

QtStringPropertyManager *ExpressionVector3DPropertyManager::subStringPropertyManager() const {
    return d_ptr->m_stringPropertyManager;
}

ExpressionVector3D ExpressionVector3DPropertyManager::value(const QtProperty *property) const {
    return d_ptr->m_values.value(property, ExpressionVector3D());
}

void
ExpressionVector3DPropertyManager::setPropertyLabels(const QString &label_x, const QString &label_y,
                                                     const QString &label_z) {
    propertyLabel[0] = label_x;
    propertyLabel[1] = label_y;
    propertyLabel[2] = label_z;
}

QString ExpressionVector3DPropertyManager::valueText(const QtProperty *property) const {
    const ExpressionVector3DPropertyManagerPrivate::PropertyValueMap::const_iterator it = d_ptr->m_values.constFind(
            property);
    if (it == d_ptr->m_values.constEnd())
        return QString();

    return QCoreApplication::translate("QtPropertyBrowserUtils", "[%1, %2, %3]")
            .arg(QString::fromUtf8(it.value().x().serialize().c_str()))
            .arg(QString::fromUtf8(it.value().y().serialize().c_str()))
            .arg(QString::fromUtf8(it.value().z().serialize().c_str()));
}

void ExpressionVector3DPropertyManager::setValue(QtProperty *property, const ExpressionVector3D &val) {
    const ExpressionVector3DPropertyManagerPrivate::PropertyValueMap::iterator it = d_ptr->m_values.find(property);
    if (it == d_ptr->m_values.end())
        return;

    if (it.value() == val)
        return;

    it.value() = val;

    d_ptr->m_stringPropertyManager->setValue(d_ptr->m_propertyToX[property],
                                             QString::fromUtf8(val.x().serialize().c_str()));
    d_ptr->m_stringPropertyManager->setValue(d_ptr->m_propertyToY[property],
                                             QString::fromUtf8(val.y().serialize().c_str()));
    d_ptr->m_stringPropertyManager->setValue(d_ptr->m_propertyToZ[property],
                                             QString::fromUtf8(val.z().serialize().c_str()));

    emit propertyChanged(property);
    emit valueChanged(property, val);
}

void ExpressionVector3DPropertyManager::initializeProperty(QtProperty *property) {
    ExpressionVector3D val;
    d_ptr->m_values[property] = val;

    QtProperty *xProp = d_ptr->m_stringPropertyManager->addProperty();
    xProp->setPropertyName(tr(propertyLabel[0].toLatin1()));
    d_ptr->m_stringPropertyManager->setValue(xProp, QString::fromUtf8(val.x().serialize().c_str()));
    d_ptr->m_propertyToX[property] = xProp;
    d_ptr->m_xToProperty[xProp] = property;
    property->addSubProperty(xProp);

    QtProperty *yProp = d_ptr->m_stringPropertyManager->addProperty();
    yProp->setPropertyName(tr(propertyLabel[1].toLatin1()));
    d_ptr->m_stringPropertyManager->setValue(yProp, QString::fromUtf8(val.y().serialize().c_str()));
    d_ptr->m_propertyToY[property] = yProp;
    d_ptr->m_yToProperty[yProp] = property;
    property->addSubProperty(yProp);

    QtProperty *zProp = d_ptr->m_stringPropertyManager->addProperty();
    zProp->setPropertyName(tr(propertyLabel[2].toLatin1()));
    d_ptr->m_stringPropertyManager->setValue(zProp, QString::fromUtf8(val.z().serialize().c_str()));
    d_ptr->m_propertyToZ[property] = zProp;
    d_ptr->m_zToProperty[zProp] = property;
    property->addSubProperty(zProp);
}

void ExpressionVector3DPropertyManager::uninitializeProperty(QtProperty *property) {
    QtProperty *xProp = d_ptr->m_propertyToX[property];
    if (xProp) {
        d_ptr->m_xToProperty.remove(xProp);
        delete xProp;
    }
    d_ptr->m_propertyToX.remove(property);

    QtProperty *yProp = d_ptr->m_propertyToY[property];
    if (yProp) {
        d_ptr->m_yToProperty.remove(yProp);
        delete yProp;
    }
    d_ptr->m_propertyToY.remove(property);

    QtProperty *zProp = d_ptr->m_propertyToZ[property];
    if (zProp) {
        d_ptr->m_zToProperty.remove(zProp);
        delete zProp;
    }
    d_ptr->m_propertyToZ.remove(property);

    d_ptr->m_values.remove(property);
}

/*
void ExpressionVector3DPropertyManager::valueChanged(QtProperty * _t1, const ExpressionVector3D & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
*/

class ExpressionVector3DEditWidget : public QWidget {
Q_OBJECT

public:
    ExpressionVector3DEditWidget(QWidget *parent);
    virtual ~ExpressionVector3DEditWidget() {};   // somehow required for the linker to find the vtable

public Q_SLOTS:

    void setValue(const ExpressionVector3D &value);

Q_SIGNALS:
    void valueChanged(const ExpressionVector3D &value);

protected:
    void paintEvent(QPaintEvent *);

private:
    ExpressionVector3D m_vector3d;
    QLabel *m_label;
};

static inline void setupTreeViewEditorMargin(QLayout *lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}

ExpressionVector3DEditWidget::ExpressionVector3DEditWidget(QWidget *parent) : QWidget(parent), m_label(new QLabel) {
    QHBoxLayout *lt = new QHBoxLayout(this);
    setupTreeViewEditorMargin(lt);
    lt->setSpacing(0);
    lt->addWidget(m_label);
    lt->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));

    m_label->setText(
            QCoreApplication::translate("QtPropertyBrowserUtils", "[%1, %2, %3]")
                    .arg(QString::fromUtf8(m_vector3d.x().serialize().c_str()))
                    .arg(QString::fromUtf8(m_vector3d.y().serialize().c_str()))
                    .arg(QString::fromUtf8(m_vector3d.z().serialize().c_str()))
    );
}

void ExpressionVector3DEditWidget::setValue(const ExpressionVector3D &c) {
    if (!(m_vector3d == c)) {
        m_vector3d = c;
        m_label->setText(
                QCoreApplication::translate("QtPropertyBrowserUtils", "[%1, %2, %3]")
                        .arg(QString::fromUtf8(c.x().serialize().c_str()))
                        .arg(QString::fromUtf8(c.y().serialize().c_str()))
                        .arg(QString::fromUtf8(c.z().serialize().c_str()))
        );
    }
}

void ExpressionVector3DEditWidget::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ExpressionVector3DEditWidget *ExpressionVector3DEditorFactoryPrivate::createEditor(QtProperty *property, QWidget *parent)
{
    ExpressionVector3DEditWidget *editor = new ExpressionVector3DEditWidget(parent);
    initializeEditor(property, editor);
    return editor;
}

void ExpressionVector3DEditorFactoryPrivate::initializeEditor(QtProperty *property, ExpressionVector3DEditWidget *editor)
{
    typename PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        it = m_createdEditors.insert(property, EditorList());
    it.value().append(editor);
    m_editorToProperty.insert(editor, property);
}

void ExpressionVector3DEditorFactoryPrivate::slotEditorDestroyed(QObject *object)
{
    const typename EditorToPropertyMap::iterator ecend = m_editorToProperty.end();
    for (typename EditorToPropertyMap::iterator itEditor = m_editorToProperty.begin(); itEditor !=  ecend; ++itEditor) {
        if (itEditor.key() == object) {
            ExpressionVector3DEditWidget *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            const typename PropertyToEditorListMap::iterator pit = m_createdEditors.find(property);
            if (pit != m_createdEditors.end()) {
                pit.value().removeAll(editor);
                if (pit.value().empty())
                    m_createdEditors.erase(pit);
            }
            m_editorToProperty.erase(itEditor);
            return;
        }
    }
}

void
ExpressionVector3DEditorFactoryPrivate::slotPropertyChanged(QtProperty *property, const ExpressionVector3D &value) {
    const PropertyToEditorListMap::iterator it = m_createdEditors.find(property);
    if (it == m_createdEditors.end())
        return;
    QListIterator<ExpressionVector3DEditWidget *> itEditor(it.value());

    while (itEditor.hasNext())
        itEditor.next()->setValue(value);
}

void ExpressionVector3DEditorFactoryPrivate::slotSetValue(const ExpressionVector3D &value) {
    QObject *object = q_ptr->sender();
    const EditorToPropertyMap::ConstIterator ecend = m_editorToProperty.constEnd();
    for (EditorToPropertyMap::ConstIterator itEditor = m_editorToProperty.constBegin(); itEditor != ecend; ++itEditor)
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            ExpressionVector3DPropertyManager *manager = q_ptr->propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
}

ExpressionVector3DEditorFactory::ExpressionVector3DEditorFactory(QObject *parent)
        : QtAbstractEditorFactory<ExpressionVector3DPropertyManager>(parent),
          d_ptr(new ExpressionVector3DEditorFactoryPrivate()) {
    d_ptr->q_ptr = this;
}

/*!
	Destroys this factory, and all the widgets it has created.
*/
ExpressionVector3DEditorFactory::~ExpressionVector3DEditorFactory() {
    qDeleteAll(d_ptr->m_editorToProperty.keys());
    delete d_ptr;
}

/*!
	\internal
	Reimplemented from the QtAbstractEditorFactory class.
*/
void ExpressionVector3DEditorFactory::connectPropertyManager(ExpressionVector3DPropertyManager *manager) {
    connect(manager, SIGNAL(valueChanged(QtProperty * , ExpressionVector3D)), this,
            SLOT(slotPropertyChanged(QtProperty * , ExpressionVector3D)));
}

/*!
	\internal
	Reimplemented from the QtAbstractEditorFactory class.
*/
QWidget *
ExpressionVector3DEditorFactory::createEditor(ExpressionVector3DPropertyManager *manager, QtProperty *property, QWidget *parent) {
    ExpressionVector3DEditWidget *editor = d_ptr->createEditor(property, parent);
    editor->setValue(manager->value(property));
    connect(editor, SIGNAL(valueChanged(ExpressionVector3D)), this, SLOT(slotSetValue(ExpressionVector3D)));
    connect(editor, SIGNAL(destroyed(QObject * )), this, SLOT(slotEditorDestroyed(QObject * )));
    return editor;
}

/*!
	\internal

	Reimplemented from the QtAbstractEditorFactory class.
*/
void ExpressionVector3DEditorFactory::disconnectPropertyManager(ExpressionVector3DPropertyManager *manager) {
    disconnect(manager, SIGNAL(valueChanged(QtProperty * , ExpressionVector3D)), this,
               SLOT(slotPropertyChanged(QtProperty * , ExpressionVector3D)));
}

#include "ExpressionPropertyBrowser.moc"
