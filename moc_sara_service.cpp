/****************************************************************************
** Meta object code from reading C++ file 'sara_service.h'
**
** Created: Mon Sep 16 20:23:09 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sara_service.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sara_service.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Sara__Service[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      22,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   36,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Sara__Service[] = {
    "Sara::Service\0\0done()\0doneManager()\0"
    "reply\0requestReceived(QNetworkReply*)\0"
};

void Sara::Service::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Service *_t = static_cast<Service *>(_o);
        switch (_id) {
        case 0: _t->done(); break;
        case 1: _t->doneManager(); break;
        case 2: _t->requestReceived((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Sara::Service::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Sara::Service::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Sara__Service,
      qt_meta_data_Sara__Service, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Sara::Service::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Sara::Service::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Sara::Service::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Sara__Service))
        return static_cast<void*>(const_cast< Service*>(this));
    return QObject::qt_metacast(_clname);
}

int Sara::Service::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Sara::Service::done()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Sara::Service::doneManager()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
