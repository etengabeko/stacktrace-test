#include <signal.h>

#include <iostream>
#include <string>
#include <vector>

#include <qt5/QtCore/QCoreApplication>
#include <qt5/QtCore/QDebug>

#include <boost/filesystem.hpp>
#include <boost/stacktrace.hpp>

namespace
{

template<typename T>
void unused(T value) { (void)(value); }

const std::string dumpFileName() { return "backtrace.dump"; }

void safetyQuit(int signum)
{
    qDebug() << "Crashed!";
    ::signal(signum, SIG_DFL);
    boost::stacktrace::safe_dump_to(dumpFileName().c_str());
    ::raise(signum);
}

bool checkDump()
{
    boost::system::error_code ec;
    if (boost::filesystem::exists(boost::filesystem::path(dumpFileName()), ec))
    {
        std::ifstream in(dumpFileName());

        boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(in);
        std::cout << "Previous run crashed:\n";
        for (const boost::stacktrace::frame& each : st)
        {
            std::cout << each << " at "
                      << each.source_file() << ":"
                      << each.source_line() << "\n";
        }

        in.close();
        boost::filesystem::remove(dumpFileName());
    }

    return (ec.value() != boost::system::errc::success);
}

void unsafelyDivide()
{
    int a = 0,
        b = 0;

    std::cout << "Input a: ";
    std::cin >> a;
    std::cout << "Input b: ";
    std::cin >> b;

    int res = a / b;

    std::cout << "Divide a/b = " << res << std::endl;
}

void unsafelyAccess()
{
    std::vector<int> v;
    size_t s = 0,
           i = 0;

    std::cout << "Input size: ";
    std::cin >> s;
    std::cout << "Input index: ";
    std::cin >> i;

    v.resize(s, 1);
    int res = v[i];

    std::cout << "v[" << i << "] = " << res << std::endl;
}

}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    ::signal(SIGSEGV, &safetyQuit);
    ::signal(SIGFPE,  &safetyQuit);
    ::signal(SIGABRT, &safetyQuit);

    if (::checkDump())
    {
        ::unsafelyDivide();
        ::unsafelyAccess();
    }

    return EXIT_SUCCESS;
}
