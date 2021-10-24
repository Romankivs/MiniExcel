/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.1.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.1.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Window_t {
    const uint offsetsAndSize[50];
    char stringdata0[374];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Window_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Window_t qt_meta_stringdata_Window = {
    {
QT_MOC_LITERAL(0, 6), // "Window"
QT_MOC_LITERAL(7, 18), // "currentFontChanged"
QT_MOC_LITERAL(26, 0), // ""
QT_MOC_LITERAL(27, 4), // "font"
QT_MOC_LITERAL(32, 22), // "currentFontSizeChanged"
QT_MOC_LITERAL(55, 8), // "fontSize"
QT_MOC_LITERAL(64, 24), // "currentFontIsBoldChanged"
QT_MOC_LITERAL(89, 4), // "bold"
QT_MOC_LITERAL(94, 26), // "currentFontIsItalicChanged"
QT_MOC_LITERAL(121, 6), // "italic"
QT_MOC_LITERAL(128, 33), // "currentFontIsStrikethroughCha..."
QT_MOC_LITERAL(162, 13), // "strikethrough"
QT_MOC_LITERAL(176, 26), // "verHeaderCustomContextMenu"
QT_MOC_LITERAL(203, 5), // "point"
QT_MOC_LITERAL(209, 26), // "horHeaderCustomContextMenu"
QT_MOC_LITERAL(236, 16), // "selectionChanged"
QT_MOC_LITERAL(253, 14), // "QItemSelection"
QT_MOC_LITERAL(268, 8), // "selected"
QT_MOC_LITERAL(277, 10), // "deselected"
QT_MOC_LITERAL(288, 23), // "fontChangedFromComboBox"
QT_MOC_LITERAL(312, 15), // "fontSizeChanged"
QT_MOC_LITERAL(328, 16), // "fontStyleChanged"
QT_MOC_LITERAL(345, 16), // "FontStyleOptions"
QT_MOC_LITERAL(362, 5), // "style"
QT_MOC_LITERAL(368, 5) // "value"

    },
    "Window\0currentFontChanged\0\0font\0"
    "currentFontSizeChanged\0fontSize\0"
    "currentFontIsBoldChanged\0bold\0"
    "currentFontIsItalicChanged\0italic\0"
    "currentFontIsStrikethroughChanged\0"
    "strikethrough\0verHeaderCustomContextMenu\0"
    "point\0horHeaderCustomContextMenu\0"
    "selectionChanged\0QItemSelection\0"
    "selected\0deselected\0fontChangedFromComboBox\0"
    "fontSizeChanged\0fontStyleChanged\0"
    "FontStyleOptions\0style\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Window[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    0 /* Public */,
       4,    1,   83,    2, 0x06,    2 /* Public */,
       6,    1,   86,    2, 0x06,    4 /* Public */,
       8,    1,   89,    2, 0x06,    6 /* Public */,
      10,    1,   92,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    1,   95,    2, 0x08,   10 /* Private */,
      14,    1,   98,    2, 0x08,   12 /* Private */,
      15,    2,  101,    2, 0x08,   14 /* Private */,
      19,    1,  106,    2, 0x08,   17 /* Private */,
      20,    1,  109,    2, 0x08,   19 /* Private */,
      21,    2,  112,    2, 0x08,   21 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QFont,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,   13,
    QMetaType::Void, QMetaType::QPoint,   13,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 16,   17,   18,
    QMetaType::Void, QMetaType::QFont,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 22, QMetaType::Bool,   23,   24,

       0        // eod
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Window *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->currentFontChanged((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 1: _t->currentFontSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->currentFontIsBoldChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->currentFontIsItalicChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->currentFontIsStrikethroughChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->verHeaderCustomContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: _t->horHeaderCustomContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->selectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 8: _t->fontChangedFromComboBox((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 9: _t->fontSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->fontStyleChanged((*reinterpret_cast< FontStyleOptions(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QItemSelection >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Window::*)(const QFont & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Window::currentFontChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Window::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Window::currentFontSizeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Window::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Window::currentFontIsBoldChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Window::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Window::currentFontIsItalicChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Window::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Window::currentFontIsStrikethroughChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject Window::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_Window.offsetsAndSize,
    qt_meta_data_Window,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Window_t
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QFont &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QItemSelection &, std::false_type>, QtPrivate::TypeAndForceComplete<const QItemSelection &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QFont &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<FontStyleOptions, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Window.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Window::currentFontChanged(const QFont & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Window::currentFontSizeChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Window::currentFontIsBoldChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Window::currentFontIsItalicChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Window::currentFontIsStrikethroughChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
