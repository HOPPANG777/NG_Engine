#pragma once

#define NOMINMAX

#include <string>
#include <vector>
#include <cstdint>

namespace NG_Engine
{
    class FileSystem
    {
    public:
        using ByteArray = std::vector<std::uint8_t>;

    public:
        static bool FileExists(const std::wstring& path);
        static bool DirectoryExists(const std::wstring& path);
        static bool IsDirectory(const std::wstring& path);
        static bool MyCreateDirectory(const std::wstring& path, bool recursive = false);
        static bool RemoveFile(const std::wstring& path);
        static bool MyRemoveDirectory(const std::wstring& path, bool recursive = false);

    public:
        static std::wstring Combine(const std::wstring& left, const std::wstring& right);
        static std::wstring GetDirectoryPath(const std::wstring& path);
        static std::wstring GetFileName(const std::wstring& path);
        static std::wstring GetFileNameWithoutExtension(const std::wstring& path);
        static std::wstring GetFileExtension(const std::wstring& path);
        static void NormalizePath(std::wstring& path);

    public:
        static bool ReadAllBytes(const std::wstring& path, ByteArray& outData);
        static bool WriteAllBytes(const std::wstring& path, const ByteArray& data, bool append = false);
        static bool ReadAllText(const std::wstring& path, std::string& outText);
        static bool WriteAllText(const std::wstring& path, const std::string& text, bool append = false);
        static bool MyCopyFile(const std::wstring& from, const std::wstring& to, bool overwrite = true);

    private:
        static bool EnsureParentDirectory(const std::wstring& path);
    };
} // namespace NG_Engine

