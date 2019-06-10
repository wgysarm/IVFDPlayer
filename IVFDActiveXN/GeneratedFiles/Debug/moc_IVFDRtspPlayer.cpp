/****************************************************************************
** Meta object code from reading C++ file 'IVFDRtspPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IVFDRtspPlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IVFDRtspPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IVFDRtspPlayer_t {
    QByteArrayData data[9];
    char stringdata[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IVFDRtspPlayer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IVFDRtspPlayer_t qt_meta_stringdata_IVFDRtspPlayer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "IVFDRtspPlayer"
QT_MOC_LITERAL(1, 15, 15), // "SIG_GetOneFrame"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "SIG_StartToPlay0"
QT_MOC_LITERAL(4, 49, 16), // "SIG_StartToPlay1"
QT_MOC_LITERAL(5, 66, 16), // "SIG_StartTryPlay"
QT_MOC_LITERAL(6, 83, 16), // "SLOT_StartToPlay"
QT_MOC_LITERAL(7, 100, 17), // "SLOT_StartToPlay1"
QT_MOC_LITERAL(8, 118, 17) // "SLOT_StartTryPlay"

    },
    "IVFDRtspPlayer\0SIG_GetOneFrame\0\0"
    "SIG_StartToPlay0\0SIG_StartToPlay1\0"
    "SIG_StartTryPlay\0SLOT_StartToPlay\0"
    "SLOT_StartToPlay1\0SLOT_StartTryPlay"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IVFDRtspPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    0,   52,    2, 0x06 /* Public */,
       4,    0,   53,    2, 0x06 /* Public */,
       5,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   55,    2, 0x0a /* Public */,
       7,    0,   56,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IVFDRtspPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IVFDRtspPlayer *_t = static_cast<IVFDRtspPlayer *>(_o);
        switch (_id) {
        case 0: _t->SIG_GetOneFrame((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->SIG_StartToPlay0(); break;
        case 2: _t->SIG_StartToPlay1(); break;
        case 3: _t->SIG_StartTryPlay(); break;
        case 4: _t->SLOT_StartToPlay(); break;
        case 5: _t->SLOT_StartToPlay1(); break;
        case 6: _t->SLOT_StartTryPlay(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IVFDRtspPlayer::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IVFDRtspPlayer::SIG_GetOneFrame)) {
                *result = 0;
            }
        }
        {
            typedef void (IVFDRtspPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IVFDRtspPlayer::SIG_StartToPlay0)) {
                *result = 1;
            }
        }
        {
            typedef void (IVFDRtspPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IVFDRtspPlayer::SIG_StartToPlay1)) {
                *result = 2;
            }
        }
        {
            typedef void (IVFDRtspPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IVFDRtspPlayer::SIG_StartTryPlay)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject IVFDRtspPlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IVFDRtspPlayer.data,
      qt_meta_data_IVFDRtspPlayer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IVFDRtspPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IVFDRtspPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IVFDRtspPlayer.stringdata))
        return static_cast<void*>(const_cast< IVFDRtspPlayer*>(this));
    return QObject::qt_metacast(_clname);
}

int IVFDRtspPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void IVFDRtspPlayer::SIG_GetOneFrame(QImage _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IVFDRtspPlayer::SIG_StartToPlay0()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void IVFDRtspPlayer::SIG_StartToPlay1()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void IVFDRtspPlayer::SIG_StartTryPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
