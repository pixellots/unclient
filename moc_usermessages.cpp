/****************************************************************************
** Meta object code from reading C++ file 'usermessages.h'
**
** Created: Mon Sep 16 20:23:10 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "usermessages.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usermessages.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserMessages[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      28,   13,   13,   13, 0x0a,
      37,   13,   13,   13, 0x0a,
      47,   13,   13,   13, 0x0a,
      56,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UserMessages[] = {
    "UserMessages\0\0serviceDone()\0onLeft()\0"
    "onRight()\0onRead()\0openLink(QUrl)\0"
};

void UserMessages::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UserMessages *_t = static_cast<UserMessages *>(_o);
        switch (_id) {
        case 0: _t->serviceDone(); break;
        case 1: _t->onLeft(); break;
        case 2: _t->onRight(); break;
        case 3: _t->onRead(); break;
        case 4: _t->openLink((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UserMessages::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UserMessages::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_UserMessages,
      qt_meta_data_UserMessages, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UserMessages::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UserMessages::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UserMessages::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserMessages))
        return static_cast<void*>(const_cast< UserMessages*>(this));
    return QDialog::qt_metacast(_clname);
}

int UserMessages::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
