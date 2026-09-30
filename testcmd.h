#ifndef TESTCMD_H
#define TESTCMD_H
#include <QJsonArray>
#include <QObject>
#include "mcpcommand.h"

class TestCmd : public McpCommand
{

public:
    using TestCallback = std::function<void(const int&)>;

    explicit TestCmd(TestCallback callback);

    QJsonObject definition() const override;

    QJsonObject execute(const QJsonObject &args) override;

private:
    TestCallback m_callback;
};

// Command to query app state
class GetAppStatusCommand : public McpCommand {
public:
    QJsonObject definition() const override;

    QJsonObject execute(const QJsonObject &args) override;
};

#endif // TESTCMD_H
