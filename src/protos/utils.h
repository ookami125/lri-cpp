#pragma once

template<typename T>
std::optional<T> ToOptional(std::vector<T> in) {
    if(in.size() > 1) throw "To many values for conversion!";
    if(in.size() == 1) return in[0];
    return std::nullopt;
}

template<typename T>
std::optional<T> ToOptional(std::vector<picoproto::Message*> in) {
    if(in.size() > 1) throw "To many values for conversion!";
    if(in.size() == 1) return (T)in[0];
    return std::nullopt;
}

template<typename T, typename U>
std::optional<T> ToOptional(std::vector<U> in) {
    if(in.size() > 1) throw "To many values for conversion!";
    if(in.size() == 1) return (T)in[0];
    return std::nullopt;
}

template<typename T>
std::vector<T> To(std::vector<picoproto::Message*> in) {
    std::vector<T> result;
    for(auto message : in) {
        result.push_back((T)message);
    }
    return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& obj)
{
    if(obj.has_value())
        os << obj.value();
    else
        os << "null";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& objs)
{
    os << "[";
    if(objs.size() > 0) os << std::endl;
    for(size_t i=0; i<objs.size(); ++i) {
        auto& obj = objs[i];
        os << obj;
        if(i<objs.size()-1) {
            os << ", ";
        }
        os << std::endl;
    }
    os << "]";
    return os;
}