/*
    SPDX-FileCopyrightText: 2026 SonicDE contributors

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "klanguagebutton.h"

#include <QMenu>
#include <QSignalSpy>
#include <QTest>

class KLanguageButtonTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void insertAlphabetically_data();
    void insertAlphabetically();
};

void KLanguageButtonTest::insertAlphabetically_data()
{
    QTest::addColumn<bool>("openMenu");
    QTest::addColumn<bool>("showCodes");

    QTest::newRow("before-opening") << false << false;
    QTest::newRow("after-opening") << true << false;
    QTest::newRow("before-opening-with-codes") << false << true;
    QTest::newRow("after-opening-with-codes") << true << true;
}

void KLanguageButtonTest::insertAlphabetically()
{
    QFETCH(bool, openMenu);
    QFETCH(bool, showCodes);

    KLanguageButton button;
    button.showLanguageCodes(showCodes);
    button.insertLanguage(QStringLiteral("en"), QStringLiteral("English"));
    button.insertLanguage(QStringLiteral("de"), QStringLiteral("German"));
    button.insertLanguage(QStringLiteral("it"), QStringLiteral("Italian"));
    button.insertLanguage(QStringLiteral("es"), QStringLiteral("Spanish"));

    auto* menu = button.findChild<QMenu*>();
    QVERIFY(menu);
    if (openMenu) {
        QSignalSpy aboutToShow(menu, &QMenu::aboutToShow);
        button.show();
        menu->popup(QPoint(10, 10));
        QCOMPARE(aboutToShow.count(), 1);
        menu->close();
    }

    button.insertLanguage(QStringLiteral("fr"), QStringLiteral("French"));

    QStringList codes;
    for (const auto* action : menu->actions()) {
        codes.append(action->data().toString());
    }
    QCOMPARE(codes, (QStringList{QStringLiteral("en"), QStringLiteral("fr"), QStringLiteral("de"), QStringLiteral("it"), QStringLiteral("es")}));
}

QTEST_MAIN(KLanguageButtonTest)

#include "klanguagebutton_unittest.moc"
