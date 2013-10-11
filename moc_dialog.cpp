/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created: Fri Oct 11 19:53:27 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      22,    7,    7,    7, 0x0a,
      43,    7,    7,    7, 0x0a,
      53,    7,    7,    7, 0x0a,
      75,   70,    7,    7, 0x0a,
      90,    7,    7,    7, 0x0a,
     113,    7,    7,    7, 0x0a,
     137,    7,    7,    7, 0x0a,
     177,  152,    7,    7, 0x0a,
     237,  209,    7,    7, 0x0a,
     315,  306,    7,    7, 0x0a,
     342,  335,    7,    7, 0x0a,
     359,    7,    7,    7, 0x0a,
     376,    7,    7,    7, 0x0a,
     391,    7,    7,    7, 0x0a,
     429,  407,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dialog[] = {
    "Dialog\0\0serviceDone()\0serviceDoneManager()\0"
    "refresh()\0cancelProgress()\0aUrl\0"
    "openLink(QUrl)\0updateSelectedUpdate()\0"
    "updateSelectedMessage()\0startInstall()\0"
    "bytesReceived,bytesTotal\0"
    "downloadProgress(qint64,qint64)\0"
    "aUpdate,aError,aErrorString\0"
    "downloadDone(UpdateNode::Update,QNetworkReply::NetworkError,QString)\0"
    "aSuccess\0messageLoaded(bool)\0aIndex\0"
    "tabSelected(int)\0checkSelection()\0"
    "processError()\0processOutput()\0"
    "aExitCode,aExitStatus\0"
    "updateExit(int,QProcess::ExitStatus)\0"
};

void Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dialog *_t = static_cast<Dialog *>(_o);
        switch (_id) {
        case 0: _t->serviceDone(); break;
        case 1: _t->serviceDoneManager(); break;
        case 2: _t->refresh(); break;
        case 3: _t->cancelProgress(); break;
        case 4: _t->openLink((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 5: _t->updateSelectedUpdate(); break;
        case 6: _t->updateSelectedMessage(); break;
        case 7: _t->startInstall(); break;
        case 8: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 9: _t->downloadDone((*reinterpret_cast< const UpdateNode::Update(*)>(_a[1])),(*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->messageLoaded((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->tabSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->checkSelection(); break;
        case 13: _t->processError(); break;
        case 14: _t->processOutput(); break;
        case 15: _t->updateExit((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog,
      qt_meta_data_Dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
