#ifndef STATE_DATA_H
#define STATE_DATA_H

#include <string>
#include <map>
#include <any>

namespace Core {
    class StateData {
    private: 
        std::map<std::string, std::any> data;

    public:
        StateData() = default;

        template<typename T> 
        void set(const std::string& key, const T& value) {
            data[key] = value;
        }

        template<typename T> 
        T get(const std::string& key) const {
            try {
                return std::any_cast<T>(data.at(key));
            } catch(const std::exception& e) {
                throw std::runtime_error("Invalid type cast for the key: " + key);
            }
        }
    };
}

#endif