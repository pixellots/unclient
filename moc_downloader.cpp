/****************************************************************************
** Meta object code from reading C++ file 'downloader.h'
**
** Created: Fri Oct 11 19:53:29 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "downloader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'downloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateNode__Downloader[] = {

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
      52,   24,   23,   23, 0x05,
     138,  113,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     176,  170,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UpdateNode__Downloader[] = {
    "UpdateNode::Downloader\0\0"
    "aUpdate,aError,aErrorString\0"
    "done(UpdateNode::Update,QNetworkReply::NetworkError,QString)\0"
    "bytesReceived,bytesTotal\0"
    "downloadProgress(qint64,qint64)\0reply\0"
    "downloadFinished(QNetworkReply*)\0"
};

void UpdateNode::Downloader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Downloader *_t = static_cast<Downloader *>(_o);
        switch (_id) {
        case 0: _t->done((*reinterpret_cast< const UpdateNode::Update(*)>(_a[1])),(*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 2: _t->downloadFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateNode::Downloader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateNode::Downloader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UpdateNode__Downloader,
      qt_meta_data_UpdateNode__Downloader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateNode::Downloader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateNode::Downloader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateNode::Downloader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateNode__Downloader))
        return static_cast<void*>(const_cast< Downloader*>(this));
    return QObject::qt_metacast(_clname);
}

int UpdateNode::Downloader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void UpdateNode::Downloader::done(const UpdateNode::Update & _t1, QNetworkReply::NetworkError _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpdateNode::Downloader::downloadProgress(qint64 _t1, qint64 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE