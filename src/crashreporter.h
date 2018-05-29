#ifndef STACKTRACE_CRASH_REPORTER_H
#define STACKTRACE_CRASH_REPORTER_H

#include <memory>
#include <string>
#include <utility>

#include <boost/stacktrace.hpp>

/**
 * @class CrashReporter
 * @brief Глобальный обработчик ошибочных ситуаций.
 *        Позволяет сформировать отчёт по полученной информации о стеке вызовов.
 *
 * @note Перехватывает сигналы SIGABRT, SIGSEGV, SIGILL, SIGFPE
 *       и перед выходом формирует отчёт, содержащий текущий стек вызовов.
 */
class CrashReporter
{
private:
    CrashReporter(const std::string& appName, int appPid);

public:
    ~CrashReporter();

    /**
     * @brief initialize - инициализирует единственный глобальный объект-обработчик (singleton).
     * @note Для автоматического формирования имён файлов отчётов:
     * @param appName - имя приложения.
     * @param appPid - pid процесса.
     * @return указатель на созданный объект.
     */
    static CrashReporter* initialize(const std::string& appName, int appPid);

    /**
     * @brief instance - предоставляет точку доступа к объекту.
     * @return указатель на единственный объект.
     */
    static CrashReporter* instance();

    /**
     * @brief crashReportFileName - возвращает текущее установленное значение имени файла отчёта.
     * @return имя файла отчёта.
     */
    const std::string& crashReportFileName() const;

    /**
     * @brief setCrashReportFileName - устанавливает новое значение для имени файла отчёта.
     * @param outputFileName - устанавливаемое значение.
     */
    void setCrashReportFileName(const std::string& outputFileName);

    /**
     * @brief makeCrashReportCustomFileName - генерирует автоматическое значение имени файла отчета.
     * @return автоматически сгенерированное имя файла отчёта в формате 'yyyyMMdd_hhmmss_appname_apppid.dump'.
     */
    std::string makeCrashReportCustomFileName() const;

    /**
     * @brief makeCrashReport - создаёт отчёт по переданному стеку вызовов st.
     * @param st - стек вызовов для формирования отчёта.
     * @return - отчёт (стек вызовов в human-readable виде).
     */
    std::string makeCrashReport(const boost::stacktrace::stacktrace& st) const;

    /**
     * @brief saveCrashReport - сохраняет переданный отчёт report в файл.
     *        Имя файла определяется crashReportFileName().
     * @param report - сохраняемый отчёт.
     * @return результат сохранения: first - флаг успешности выполнения операции,
     *         second - сообщение об ошибке при неуспешном завершении (при first == true).
     */
    std::pair<bool, std::string> saveCrashReport(const std::string& report) const;

    /**
     * @brief saveCrashReport - сохраняет отчёт по переданному стеку вызовов st в файл.
     *        Имя файла определяется crashReportFileName().
     * @param st - стек вызовов для формирования отчёта.
     * @return результат сохранения: first - флаг успешности выполнения операции,
     *         second - сообщение об ошибке при неуспешном завершении (при first == true).
     */
    std::pair<bool, std::string> saveCrashReport(const boost::stacktrace::stacktrace& st) const;

private:
    static std::unique_ptr<CrashReporter> m_instance;

    const std::string m_appName;
    const int m_appPid = 0;

    std::string m_filename;

};

#endif // STACKTRACE_CRASH_REPORTER_H
