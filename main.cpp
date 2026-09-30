#include <QCoreApplication>
#include <QCommandLineParser>
#include "mcpcommandregistry.h"
#include "testcmd.h"
#include "cancommands.h"
#include "doipcommands.h"
#include "hsfzcommands.h"
#include "mcpserver.h"

static quint16 resolvePort(const QCoreApplication &a)
{
    QCommandLineParser parser;
    QCommandLineOption portOption(QStringList{"p", "port"},
        "Port for the MCP HTTP transport.", "port");
    parser.addOption(portOption);
    parser.addHelpOption();
    parser.process(a);

    bool ok = false;
    if (parser.isSet(portOption)) {
        quint16 port = parser.value(portOption).toUShort(&ok);
        if (ok) return port;
        qWarning() << "Invalid --port value, ignoring:" << parser.value(portOption);
    }

    const QByteArray envPort = qgetenv("MCP_SERVER_PORT");
    if (!envPort.isEmpty()) {
        quint16 port = envPort.toUShort(&ok);
        if (ok) return port;
        qWarning() << "Invalid MCP_SERVER_PORT value, ignoring:" << envPort;
    }

    return 3768;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() <<QByteArray::fromBase64("ZlO0ngiLVG").toHex();
    return 0;
    // Set up code that uses the Qt event loop here.
    // Call QCoreApplication::quit() or QCoreApplication::exit() to quit the application.
    // A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to QCoreApplication::exec() or use the Non-Qt Plain C++ Application template.
    McpCommandRegistry registry;

    // 1. Register initial commands
    auto themeCmd = QSharedPointer<TestCmd>::create([](int type) {
        qDebug() << "Applying test type:" << type;
    });
    registry.addCommand(themeCmd);

    auto statusCmd = QSharedPointer<GetAppStatusCommand>::create();
    registry.addCommand(statusCmd);

    auto canManager = new CanManager(&a);
    registry.addCommand(QSharedPointer<CanListInterfacesCommand>::create(canManager));
    registry.addCommand(QSharedPointer<CanOpenCommand>::create(canManager));
    registry.addCommand(QSharedPointer<CanCloseCommand>::create(canManager));
    registry.addCommand(QSharedPointer<CanGetReceivedFramesCommand>::create(canManager));
    registry.addCommand(QSharedPointer<UdsSendRequestCommand>::create(canManager));
    registry.addCommand(QSharedPointer<UdsTesterPresentStartCommand>::create(canManager));
    registry.addCommand(QSharedPointer<UdsTesterPresentStopCommand>::create(canManager));

    auto doipManager = new DoipManager(&a);
    registry.addCommand(QSharedPointer<DoipDiscoverVehiclesCommand>::create(doipManager));
    registry.addCommand(QSharedPointer<DoipOpenCommand>::create(doipManager));
    registry.addCommand(QSharedPointer<DoipCloseCommand>::create(doipManager));
    registry.addCommand(QSharedPointer<DoipGetReceivedMessagesCommand>::create(doipManager));
    registry.addCommand(QSharedPointer<DoipSendRequestCommand>::create(doipManager));
    registry.addCommand(QSharedPointer<DoipTesterPresentStartCommand>::create(doipManager));
    registry.addCommand(QSharedPointer<DoipTesterPresentStopCommand>::create(doipManager));

    auto hsfzManager = new HsfzManager(&a);
    registry.addCommand(QSharedPointer<HsfzDiscoverVehiclesCommand>::create(hsfzManager));
    registry.addCommand(QSharedPointer<HsfzOpenCommand>::create(hsfzManager));
    registry.addCommand(QSharedPointer<HsfzCloseCommand>::create(hsfzManager));
    registry.addCommand(QSharedPointer<HsfzGetReceivedMessagesCommand>::create(hsfzManager));
    registry.addCommand(QSharedPointer<HsfzSendRequestCommand>::create(hsfzManager));
    registry.addCommand(QSharedPointer<HsfzTesterPresentStartCommand>::create(hsfzManager));
    registry.addCommand(QSharedPointer<HsfzTesterPresentStopCommand>::create(hsfzManager));

    // 2. Start MCP server (HTTP port configurable via --port or MCP_SERVER_PORT)
    const quint16 port = resolvePort(a);
    McpServer server(&registry, port);

    // 3. Example of dynamically removing a command runtime
    // registry.removeCommand("get_status");
    return QCoreApplication::exec();
}
