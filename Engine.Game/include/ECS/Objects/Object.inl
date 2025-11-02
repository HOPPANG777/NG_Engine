#pragma once

namespace NG_Engine
{
    template<typename T>
    std::shared_ptr<T> Object::GetComponent(const std::string& compName) const
    {
        auto it = m_components.find(compName);
        if (it == m_components.end())
        {
            return nullptr;
        }

        return std::dynamic_pointer_cast<T>(it->second);
    }

    template<typename T>
    T* Object::FindComponentByInterface() const
    {
        for (const auto& pair : m_components)
        {
            if (T* component = dynamic_cast<T*>(pair.second.get()))
            {
                return component;
            }
        }

        return nullptr;
    }

    template<typename T>
    std::vector<T*> Object::GetAllComponentsByInterface() const
    {
        std::vector<T*> result;
        result.reserve(m_components.size());

        for (const auto& pair : m_components)
        {
            if (T* component = dynamic_cast<T*>(pair.second.get()))
            {
                result.push_back(component);
            }
        }

        return result;
    }
}