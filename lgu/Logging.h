/*
 * Logging.h
 *
 *  Created on: 3 jun 2015
 *      Author: jonas
 */

#ifndef LGU_LOGGING_H_
#define LGU_LOGGING_H_

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>


struct Logging
{
    static void initLogging()
    {
        if(isInited)
        {
            return;
        }

        boost::log::add_file_log(
            boost::log::keywords::file_name = "lgu.log",
            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
            );
        boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
        );



        isInited = true;
    }

    Logging() = delete;
    Logging(const Logging& from) = delete;

private:
    static bool isInited;
};



#endif /* LGU_LOGGING_H_ */
