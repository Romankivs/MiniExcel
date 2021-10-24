#include <gtest/gtest.h>
#include <QApplication>
#include <qtimer.h>

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    QTimer::singleShot(0, [&]()
    {
        ::testing::InitGoogleTest(&argc, argv);
        auto testResult = RUN_ALL_TESTS();
        app.exit(testResult);
    });

    return app.exec();
}
