// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "precompiled.hpp"
#include "hrApplication.hpp"
#include "hrH3MReader.hpp"

#include "hrWindow.hpp"

#ifdef Q_WS_WIN32
#   include "client/windows/handler/exception_handler.h"
#elif defined(Q_WS_X11)
#   include "client/linux/handler/exception_handler.h"
#endif

void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stdout, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stdout, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stdout, "Fatal: %s\n", msg);
        abort();
    }
}

void checkPlugins()
{
    QList<QByteArray> formats = QImageReader::supportedImageFormats();

    if ( !formats.contains("def") || !formats.contains("pcx" )) {
        QMessageBox::critical(NULL, "Critical error!", "Plugins didn't load. Files maybe corrupted. Try to re-install application.");
        exit(1);
    }
}

bool callback(
#ifdef Q_WS_WIN32
        const wchar_t *dump_path, const wchar_t *id,
#else
        const char *dump_path, const char *id,
#endif
                     void *context,
#ifdef Q_WS_WIN32
                     EXCEPTION_POINTERS *exinfo,
                     MDRawAssertionInfo *assertion,
#endif
                     bool succeeded) {
    printf("true");
    qWarning("%s", dump_path);
  if (succeeded) {
    qWarning("Dump is successfull");
  } else {
    qWarning("Dump failed");
  }
  return succeeded;
}

int main(int argc, char** argv)
{
    QT_REQUIRE_VERSION(argc, argv, "4.5.0");
    qInstallMsgHandler(myMessageOutput);



#ifdef Q_WS_WIN32
    google_breakpad::ExceptionHandler eh(L".", NULL, callback, NULL,
        google_breakpad::ExceptionHandler::HANDLER_ALL );

#else
    google_breakpad::ExceptionHandler eh(".", NULL, callback, NULL, true);
#endif

    hrApplication app(argc, argv);

    checkPlugins();

    hrWindow w;
    w.show();

    return app.exec();
}
