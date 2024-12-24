#ifndef ANYRAND_H
#define ANYRAND_H

#include <random>
#include <string>
#include <ctime>

/// @brief 随机数生成器管理类，提供线程安全的随机引擎实例
class AnyRandomEngine
{
public:
    static std::mt19937 &getEngine()
    {
        static thread_local std::mt19937 engine(std::random_device{}());
        return engine;
    }
};

/// @brief 包含各种随机生成工具的命名空间
namespace AnyRand
{
    /// @brief 生成指定范围内的随机整数
    inline int generateInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(AnyRandomEngine::getEngine());
    }

    /// @brief 生成指定范围内的随机浮点数
    inline double generateDouble(double min, double max)
    {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(AnyRandomEngine::getEngine());
    }

    /// @brief 随机生成多行字符串
    inline std::string generateText(size_t length, size_t lineCount)
    {
        std::string randStr = "abcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+<>?:{}|";
        size_t totalLength = lineCount * (length + 1); // 每行长度加上换行符
        std::string result;
        result.reserve(totalLength); // 预分配内存
        for (size_t i = 0; i < lineCount; ++i)
        {
            for (size_t j = 0; j < length; ++j)
            {
                result += randStr[generateInt(0, randStr.size() - 1)];
            }
            result += '\n';
        }
        return result;
    }

    inline std::string generateMd5Base(char base)
    {
        std::string result(32, ' '); // 预分配 32 个字符
        for (size_t i = 0; i < 32; ++i)
        {
            int tag = generateInt(0, 35);
            if (tag < 26)
            {
                result[i] = static_cast<char>(base + tag); // 'a' 或 'A'
            }
            else
            {
                result[i] = static_cast<char>('0' + tag - 26);
            }
        }
        return result;
    }

    /// @brief 生成Md5字符串(由32个a-z,0-9字符组成)
    inline std::string generateLowMd5()
    {
        return generateMd5Base('a');
    }

    /// @brief 生成Md5字符串(由32个A-Z,0-9字符组成)
    inline std::string generateUpMd5()
    {
        return generateMd5Base('A');
    }

    /// @brief 生成UUID字符串
    inline std::string generateUUID()
    {
        std::string md5Part = generateUpMd5();
        std::string uuid(36, ' '); // UUID 长度为 36（包括 4 个分隔符）

        size_t index = 0;
        for (size_t i = 0; i < 32; ++i)
        {
            if (i == 8 || i == 12 || i == 16 || i == 20)
            {
                uuid[index++] = '-'; // 插入分隔符
            }
            uuid[index++] = md5Part[i];
        }
        return uuid;
    }

    /// @brief 生成随机日期
    inline std::tm generateRandomDate(int startYear = 1970, int endYear = 2030)
    {
        std::tm result = {};
        result.tm_year = generateInt(startYear, endYear);
        result.tm_mon = generateInt(1, 12);  // 月份范围 [1, 12]
        result.tm_mday = generateInt(1, 28); // 简化为 [1, 28]，避免溢出
        result.tm_hour = generateInt(0, 23);
        result.tm_min = generateInt(0, 59);
        result.tm_sec = generateInt(0, 59);
        return result;
    }

    /// @brief 生成随机IPv4地址
    inline std::string generateIPv4()
    {
        return std::to_string(generateInt(0, 255)) + "." +
               std::to_string(generateInt(0, 255)) + "." +
               std::to_string(generateInt(0, 255)) + "." +
               std::to_string(generateInt(0, 255));
    }
}

#endif // ANYRAND_H