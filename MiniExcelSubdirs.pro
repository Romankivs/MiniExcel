TEMPLATE = subdirs

SUBDIRS += \
    MiniExcel/testing.pro \
    MiniExcelUnit

MiniExcelUnit.depends = MiniExcel/testing.pro
