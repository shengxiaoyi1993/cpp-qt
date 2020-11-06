#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
/*
 *this file is created for keeping all the constants value in one place.
 *so that they will be managed easliy and effectivly.
 * in future may can give a file to initialize these constants
 * if we need.
*/

#define _DEBUG_MODE_

namespace log_config {
    /**
     * @brief               defination of debug mode, default is WARN mode
     * @property ERROR      in this mode, will only print errors what will make program crash
     * @property WARN       in this mode, will print errors and warns what will mot make program crash
     *                      that may becauseof some proctect behaviors like using default value but
     *                      may make program did not run as exception and at same time may trigger some
     *                      other errors
     * @property LOG        in this mode, will print all the message containning errors, warns and the
     *                      infomation that developer want to see.
     */
    //enum log_type {ERROR, WARN, LOG};
    //enum log_mode {CMD, WINDOW, FILE};
    static const int TYPE_ERROR = 0;
    static const int TYPE_WARN = 1;
    static const int TYPE_LOG = 2;
    static const int MODE_CMD = 100;
    static const int MODE_WINDOW = 101;
    static const int MODE_FILE = 102;
    static const int LOG_TYPE = MODE_FILE;
    static const int LOG_MODE = TYPE_WARN;
    static const QString DEFAULT_LOG_FILE_PATH = "./log/log.txt";
}

namespace server_config {
#ifdef _DEBUG_MODE_
    static const QString CONFIG_FILE_PATH = "./resources/config/config.json";;
    static const QString DEFAULT_CONFIG_FILE_PATH = "./resources/config/config_default.json";
    static const QString DEFAULT_LANGUAGE_PKG_PATH = "./resources/language_package";
    static const QString DEFAULT_LANG_FILE_PATH = "./resources/language_package/language_list.json";
#else
    static const QString CONFIG_FILE_PATH = "./resources/config/config.json";
    static const QString DEFAULT_CONFIG_FILE_PATH = "./resources/config/config_default.json";
    static const QString DEFAULT_LANGUAGE_PKG_PATH = "./resources/language_package";
    static const QString DEFAULT_LANG_FILE_PATH = "./resources/language_package/language_list.json";
#endif

    namespace common_config_ns {
        static const QString DEFAULT_LANGUAGE = "en_US";
        static const QString DEFAULT_COMPANT_NAME = "unknown";
#ifdef _DEBUG_MODE_
        static const QString DEFAULT_LOGO_PATH = "./resources/image/logo.png";
        static const QString DEFAULT_LOGO_SELECT_PATH = "./resources/image/";
#else
        static const QString DEFAULT_LOGO_PATH = "./resources/image/logo.png";
#endif
    }

    namespace database_config_ns {
#ifdef _DEBUG_MODE_
        static const QString MONGODB_PATH = "./MongoDB";
        static const QString MONGODB_BIN_PATH = "./MongoDB/bin/mongod";
        static const QString MONGODB_LOG_PATH = "./MongoDB/log";
        static const QString MONGODB_LOG_FILE = "./MongoDB/log/mongod.log";
        static const QString MOMGODB_DATA_PATH = "./MongoDB/db";
#else
    static const QString MONGODB_PATH = "./MongoDB";
    static const QString MONGODB_BIN_PATH = "./MongoDB/bin/mongod";
    static const QString MONGODB_LOG_PATH = "./MongoDB/log";
    static const QString MONGODB_LOG_FILE = "./MongoDB/log/mongod.log";
    static const QString MOMGODB_DATA_PATH = "./MongoDB/db";
#endif
        static const QString DEFAULT_DATABASE_DIR = "";
        static const QString DEFAULT_TIME_PATH_FORMAT = "yy/MM/dd/hh.mm.ss";
        static const QString DEFAULT_MIN_RESERVE_SPACE = "1024";
        static const QString DEFAULT_SHORT_SPACE_OPERATEOR = "2";
        static const QString DEFAULT_SAVE_TIME= "90";
    }

    namespace network_config_ns {
        static QString DEFAULT_IP = "127.0.0.1";
    }

    namespace device_config_ns {

    }

}

#endif // CONSTANTS_H
