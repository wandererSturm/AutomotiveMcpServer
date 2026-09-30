#include "testcmd.h"


TestCmd::TestCmd(TestCallback callback)
    : m_callback(std::move(callback)) {}

QJsonObject TestCmd::definition() const {
    return QJsonObject{
        {"name", "test_cmd"},
        {"description", "Test MCP state"},
        {"inputSchema", QJsonObject{
                            {"type", "object"},
                            {"properties", QJsonObject{
                                               {"test", QJsonObject{
                                                             {"type", "integer"},
                                                             {"enum", QJsonArray{1, 2, 3}}
                                                         }}
                                           }},
                            {"required", QJsonArray{"test"}}
                        }}
    };
}

QJsonObject TestCmd::execute(const QJsonObject &args) {
    int theme = args["test"].toInt();
    if (m_callback) {
        m_callback(theme);
    }
    return QJsonObject{
        {"content", QJsonArray{
                        QJsonObject{{"type", "text"}, {"text", QString("Test is ok  %1").arg(theme)}}
                    }}
    };
}

QJsonObject GetAppStatusCommand::definition() const {
    return QJsonObject{
        {"name", "get_status"},
        {"description", "Retrieve core application diagnostics and state"},
        {"inputSchema", QJsonObject{{"type", "object"}}}
    };
}

QJsonObject GetAppStatusCommand::execute(const QJsonObject &args) {
    Q_UNUSED(args);
    return QJsonObject{
        {"content", QJsonArray{
                        QJsonObject{{"type", "text"}, {"text", "App Status: OK. Engine running."}}
                    }}
    };
}
