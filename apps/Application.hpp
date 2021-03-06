/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#ifndef INCLUDED_APPLICATION_HPP
#define INCLUDED_APPLICATION_HPP

#include <pdal/pdal_error.hpp>
#include <boost/cstdint.hpp>



#ifdef PDAL_COMPILER_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4512)  // assignment operator could not be generated
#endif
#include <boost/program_options.hpp>
#ifdef PDAL_COMPILER_MSVC
#  pragma warning(pop)
#endif


class app_usage_error : public pdal::pdal_error
{
public:
    app_usage_error(std::string const& msg)
        : pdal_error(msg)
    {}
};


class app_runtime_error : public pdal::pdal_error
{
public:
    app_runtime_error(std::string const& msg)
        : pdal_error(msg)
    {}
};


//
// The application base class gives us these common options:
//    --help / -h
//    --verbose / -v
//    --version
//    --timer
//
class Application
{
public:
    // call this, to start the machine
    int run();

    bool isDebug() const;
    boost::uint32_t getVerboseLevel() const;
    void printError(const std::string&) const;

protected:
    // this is protected; your derived class ctor will be the public entry point
    Application(int argc, char* argv[], const std::string& appName);

    // implement this, with calls to addOptionSet()
    virtual void addSwitches() {}

    // implement this, to do sanity checking of cmd line
    // will throw if the user gave us bad options
    virtual void validateSwitches() {}

    // implement this, to do your actual work
    // it will be wrapped in a global catch try/block for you
    virtual int execute() = 0;

    void addSwitchSet(boost::program_options::options_description* options);
    void addPositionalSwitch(const char* name, int max_count);

    void setProgressShellCommand(std::vector<std::string> const& command) { m_heartbeat_shell_command = command; }
    std::vector<std::string> getProgressShellCommand() { return m_heartbeat_shell_command; }
    
private:
    int innerRun();
    void parseSwitches();
    void outputHelp();
    void outputVersion();
    void addBasicSwitchSet();

    int do_switches();
    int do_startup();
    int do_execution();
    int do_shutdown();

    bool m_isDebug;
    boost::uint32_t m_verboseLevel;
    bool m_showHelp;
    bool m_showVersion;
    bool m_showTime;
    const int m_argc;
    char** m_argv;
    const std::string m_appName;
    bool m_hardCoreDebug;
    std::vector<std::string> m_heartbeat_shell_command;
    
    std::vector<boost::program_options::options_description*> m_options;
    boost::program_options::positional_options_description m_positionalOptions;
    boost::program_options::variables_map m_variablesMap;

    Application& operator=(const Application&); // not implemented
    Application(const Application&); // not implemented

protected:
    bool m_usestdin;
    boost::uint32_t m_chunkSize;
    
};

#endif
