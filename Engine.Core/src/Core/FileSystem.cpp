#include "Core/FileSystem.h"

#include <Windows.h>

#include <algorithm>
#include <fstream>
#include <limits>

using namespace NG_Engine;

namespace
{
    bool IsDotDirectory(const wchar_t* name)
    {
        return name[0] == L'.' && (name[1] == L'\0' || (name[1] == L'.' && name[2] == L'\0'));
    }
}

bool FileSystem::FileExists(const std::wstring& path)
{
    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    DWORD attributes = ::GetFileAttributesW(normalized.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool FileSystem::DirectoryExists(const std::wstring& path)
{
    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    DWORD attributes = ::GetFileAttributesW(normalized.c_str());
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool FileSystem::IsDirectory(const std::wstring& path)
{
    return DirectoryExists(path);
}

bool FileSystem::MyCreateDirectory(const std::wstring& path, bool recursive)
{
    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    FileSystem::NormalizePath(normalized);

    if (::CreateDirectoryW(normalized.c_str(), nullptr) != 0)
    {
        return true;
    }

    DWORD error = ::GetLastError();
    if (error == ERROR_ALREADY_EXISTS)
    {
        return FileSystem::DirectoryExists(normalized);
    }

    if (!recursive || error != ERROR_PATH_NOT_FOUND)
    {
        return false;
    }

    std::wstring parent = FileSystem::GetDirectoryPath(normalized);
    if (parent.empty() || parent == normalized)
    {
        return false;
    }

    if (!MyCreateDirectory(parent, true))
    {
        return false;
    }

    if (::CreateDirectoryW(normalized.c_str(), nullptr) != 0)
    {
        return true;
    }

    return ::GetLastError() == ERROR_ALREADY_EXISTS && FileSystem::DirectoryExists(normalized);
}

bool FileSystem::RemoveFile(const std::wstring& path)
{
    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    if (::DeleteFileW(normalized.c_str()) != 0)
    {
        return true;
    }

    DWORD error = ::GetLastError();
    return error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND;
}

bool FileSystem::MyRemoveDirectory(const std::wstring& path, bool recursive)
{
    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    FileSystem::NormalizePath(normalized);

    if (!FileSystem::DirectoryExists(normalized))
    {
        return true;
    }

    if (!recursive)
    {
        if (::RemoveDirectoryW(normalized.c_str()) != 0)
        {
            return true;
        }

        DWORD error = ::GetLastError();
        return error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND;
    }

    WIN32_FIND_DATAW findData;
    std::wstring search = FileSystem::Combine(normalized, L"*");
    HANDLE handle = ::FindFirstFileW(search.c_str(), &findData);
    if (handle == INVALID_HANDLE_VALUE)
    {
        DWORD error = ::GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
        {
            return ::RemoveDirectoryW(normalized.c_str()) != 0;
        }
        return false;
    }

    bool result = true;
    do
    {
        if (IsDotDirectory(findData.cFileName))
        {
            continue;
        }

        std::wstring child = FileSystem::Combine(normalized, findData.cFileName);
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            if (!MyRemoveDirectory(child, true))
            {
                result = false;
                break;
            }
        }
        else
        {
            if (!FileSystem::RemoveFile(child))
            {
                result = false;
                break;
            }
        }
    } while (::FindNextFileW(handle, &findData));

    DWORD loopError = ::GetLastError();
    ::FindClose(handle);

    if (!result)
    {
        return false;
    }

    if (loopError != ERROR_NO_MORE_FILES)
    {
        return false;
    }

    if (::RemoveDirectoryW(normalized.c_str()) != 0)
    {
        return true;
    }

    DWORD error = ::GetLastError();
    return error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND;
}

std::wstring FileSystem::Combine(const std::wstring& left, const std::wstring& right)
{
    if (left.empty())
    {
        std::wstring result = right;
        NormalizePath(result);
        return result;
    }

    if (right.empty())
    {
        std::wstring result = left;
        NormalizePath(result);
        return result;
    }

    std::wstring result = left;
    if (result.back() != L'\\' && result.back() != L'/')
    {
        result.push_back(L'\\');
    }

    if (right.front() == L'\\' || right.front() == L'/')
    {
        result.append(right.begin() + 1, right.end());
    }
    else
    {
        result.append(right);
    }

    NormalizePath(result);
    return result;
}

std::wstring FileSystem::GetDirectoryPath(const std::wstring& path)
{
    if (path.empty())
    {
        return L"";
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    while (normalized.size() > 1 && normalized.back() == L'\\')
    {
        if (normalized.size() == 2 && normalized[1] == L':')
        {
            break;
        }

        if (normalized.size() > 2 && normalized[0] == L'\\' && normalized[1] == L'\\')
        {
            break;
        }

        normalized.pop_back();
    }

    std::wstring::size_type pos = normalized.find_last_of(L'\\');
    if (pos == std::wstring::npos)
    {
        return L"";
    }

    if (pos == 0)
    {
        return L"\\";
    }

    if (pos == 2 && normalized[1] == L':')
    {
        return normalized.substr(0, pos + 1);
    }

    return normalized.substr(0, pos);
}

std::wstring FileSystem::GetFileName(const std::wstring& path)
{
    if (path.empty())
    {
        return L"";
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    while (normalized.size() > 1 && normalized.back() == L'\\')
    {
        if (normalized.size() == 2 && normalized[1] == L':')
        {
            break;
        }

        normalized.pop_back();
    }

    std::wstring::size_type pos = normalized.find_last_of(L'\\');
    if (pos == std::wstring::npos)
    {
        return normalized;
    }

    return normalized.substr(pos + 1);
}

std::wstring FileSystem::GetFileNameWithoutExtension(const std::wstring& path)
{
    std::wstring fileName = GetFileName(path);
    std::wstring::size_type pos = fileName.find_last_of(L'.');
    if (pos == std::wstring::npos)
    {
        return fileName;
    }

    return fileName.substr(0, pos);
}

std::wstring FileSystem::GetFileExtension(const std::wstring& path)
{
    std::wstring fileName = GetFileName(path);
    std::wstring::size_type pos = fileName.find_last_of(L'.');
    if (pos == std::wstring::npos)
    {
        return L"";
    }

    return fileName.substr(pos);
}

void FileSystem::NormalizePath(std::wstring& path)
{
    std::replace(path.begin(), path.end(), L'/', L'\\');
}

bool FileSystem::ReadAllBytes(const std::wstring& path, ByteArray& outData)
{
    outData.clear();

    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    std::ifstream file(normalized, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.seekg(0, std::ios::end);
    std::streamoff size = file.tellg();
    if (size < 0)
    {
        return false;
    }

    if (size > static_cast<std::streamoff>(std::numeric_limits<std::size_t>::max()))
    {
        return false;
    }

    file.seekg(0, std::ios::beg);
    outData.resize(static_cast<std::size_t>(size));

    if (size == 0)
    {
        return true;
    }

    file.read(reinterpret_cast<char*>(outData.data()), size);
    return file.good();
}

bool FileSystem::WriteAllBytes(const std::wstring& path, const ByteArray& data, bool append)
{
    if (path.empty())
    {
        return false;
    }

    if (!EnsureParentDirectory(path))
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    std::ios::openmode mode = std::ios::binary | std::ios::out;
    mode |= append ? std::ios::app : std::ios::trunc;

    std::ofstream file(normalized, mode);
    if (!file.is_open())
    {
        return false;
    }

    if (!data.empty())
    {
        if (data.size() > static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max()))
        {
            return false;
        }

        file.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        if (!file.good())
        {
            return false;
        }
    }

    return true;
}

bool FileSystem::ReadAllText(const std::wstring& path, std::string& outText)
{
    outText.clear();

    if (path.empty())
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    std::ifstream file(normalized, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.seekg(0, std::ios::end);
    std::streamoff size = file.tellg();
    if (size < 0)
    {
        return false;
    }

    if (size > static_cast<std::streamoff>(std::numeric_limits<std::size_t>::max()))
    {
        return false;
    }

    file.seekg(0, std::ios::beg);
    outText.resize(static_cast<std::size_t>(size));

    if (size == 0)
    {
        return true;
    }

    file.read(&outText[0], size);
    return file.good();
}

bool FileSystem::WriteAllText(const std::wstring& path, const std::string& text, bool append)
{
    if (path.empty())
    {
        return false;
    }

    if (!EnsureParentDirectory(path))
    {
        return false;
    }

    std::wstring normalized = path;
    NormalizePath(normalized);

    std::ios::openmode mode = std::ios::out;
    mode |= append ? std::ios::app : std::ios::trunc;

    std::ofstream file(normalized, mode | std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    if (!text.empty())
    {
        if (text.size() > static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max()))
        {
            return false;
        }

        file.write(text.data(), static_cast<std::streamsize>(text.size()));
        if (!file.good())
        {
            return false;
        }
    }

    return true;
}

bool FileSystem::MyCopyFile(const std::wstring& from, const std::wstring& to, bool overwrite)
{
    if (from.empty() || to.empty())
    {
        return false;
    }

    if (!FileSystem::FileExists(from))
    {
        return false;
    }

    if (!FileSystem::EnsureParentDirectory(to))
    {
        return false;
    }

    std::wstring normalizedFrom = from;
    std::wstring normalizedTo = to;
    FileSystem::NormalizePath(normalizedFrom);
    FileSystem::NormalizePath(normalizedTo);

    BOOL failIfExists = overwrite ? FALSE : TRUE;
    if (::CopyFileW(normalizedFrom.c_str(), normalizedTo.c_str(), failIfExists) != 0)
    {
        return true;
    }

    DWORD error = ::GetLastError();
    if (!overwrite && error == ERROR_FILE_EXISTS)
    {
        return true;
    }

    return false;
}

bool FileSystem::EnsureParentDirectory(const std::wstring& path)
{
    std::wstring directory = GetDirectoryPath(path);
    if (directory.empty())
    {
        return true;
    }

    return MyCreateDirectory(directory, true);
}