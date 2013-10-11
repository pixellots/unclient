/****************************************************************************
** Meta object code from reading C++ file 'commander.h'
**
** Created: Fri Oct 11 19:53:29 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "commander.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commander.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateNode__Commander[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      38,   22,   22,   22, 0x05,
      76,   54,   22,   22, 0x05,
     125,  113,   22,   22, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_UpdateNode__Commander[] = {
    "UpdateNode::Commander\0\0processError()\0"
    "processOutput()\0aExitCode,aExitStatus\0"
    "updateExit(int,QProcess::ExitStatus)\0"
    "aStatusText\0progressText(QString)\0"
};

void UpdateNode::Commander::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Commander *_t = static_cast<Commander *>(_o);
        switch (_id) {
        case 0: _t->processError(); break;
        case 1: _t->processOutput(); break;
        case 2: _t->updateExit((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 3: _t->progressText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateNode::Commander::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateNode::Commander::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UpdateNode__Commander,
      qt_meta_data_UpdateNode__Commander, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateNode::Commander::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateNode::Commander::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateNode::Commander::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateNode__Commander))
        return static_cast<void*>(const_cast< Commander*>(this));
    return QObject::qt_metacast(_clname);
}

int UpdateNode::Commander::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UpdateNode::Commander::processError()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void UpdateNode::Commander::processOutput()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void UpdateNode::Commander::updateExit(int _t1, QProcess::ExitStatus _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UpdateNode::Commander::progressText(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
