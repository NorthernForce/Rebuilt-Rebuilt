#pragma once

#include "Tunable.h"
#include <type_traits>
#include <units/base.h>

namespace nfr
{

template<typename T>
Tunable<T>::Tunable(const std::string& key, const T& defaultValue, const std::string& tableName)
    : key_(key), currentValue_(defaultValue), defaultValue_(defaultValue),
      table_(nt::NetworkTableInstance::GetDefault().GetTable(tableName))
{
    InitializeNetworkTables();
    PublishValue();
    
    // Log the tunable creation - use double conversion for units types
    try {
        if constexpr (std::is_arithmetic_v<T>) {
            nfr::logger[tableName + "/" + key] << defaultValue;
        } else {
            // For units types, log the numeric value
            nfr::logger[tableName + "/" + key] << static_cast<double>(defaultValue.value());
        }
    } catch (...) {
        // Fallback: just log that tunable was created
        nfr::logger[tableName + "/" + key] << std::string("tunable_created");
    }
}

template<typename T>
Tunable<T>::Tunable(const Tunable& other)
    : key_(other.key_), currentValue_(other.currentValue_), defaultValue_(other.defaultValue_),
      table_(other.table_)
{
    InitializeNetworkTables();
}

template<typename T>
Tunable<T>& Tunable<T>::operator=(const Tunable& other)
{
    if (this != &other)
    {
        key_ = other.key_;
        currentValue_ = other.currentValue_;
        defaultValue_ = other.defaultValue_;
        table_ = other.table_;
        InitializeNetworkTables();
    }
    return *this;
}

template<typename T>
Tunable<T>::operator T() const
{
    return Get();
}

template<typename T>
T Tunable<T>::Get() const
{
    // Try to read updated value from NetworkTables
    T networkValue = ReadFromNetworkTables();
    return networkValue;
}

template<typename T>
void Tunable<T>::Set(const T& value)
{
    currentValue_ = value;
    PublishValue();
    
    // Log the value change
    try {
        if constexpr (std::is_arithmetic_v<T>) {
            nfr::logger["Tuning/" + key_] << value;
        } else {
            // For units types, log the numeric value
            nfr::logger["Tuning/" + key_] << static_cast<double>(value.value());
        }
    } catch (...) {
        // Fallback: just log that value was set
        nfr::logger["Tuning/" + key_] << std::string("value_set");
    }
}

template<typename T>
Tunable<T>& Tunable<T>::operator=(const T& value)
{
    Set(value);
    return *this;
}

template<typename T>
void Tunable<T>::Update()
{
    // Force update from NetworkTables
    Get();
}

template<typename T>
void Tunable<T>::InitializeNetworkTables() const
{
    if constexpr (std::is_same_v<T, double>)
    {
        doublePublisher_ = std::make_unique<nt::DoublePublisher>(table_->GetDoubleTopic(key_).Publish());
        doubleSubscriber_ = std::make_unique<nt::DoubleSubscriber>(table_->GetDoubleTopic(key_).Subscribe(defaultValue_));
    }
    else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, long>)
    {
        integerPublisher_ = std::make_unique<nt::IntegerPublisher>(table_->GetIntegerTopic(key_).Publish());
        integerSubscriber_ = std::make_unique<nt::IntegerSubscriber>(table_->GetIntegerTopic(key_).Subscribe(defaultValue_));
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        booleanPublisher_ = std::make_unique<nt::BooleanPublisher>(table_->GetBooleanTopic(key_).Publish());
        booleanSubscriber_ = std::make_unique<nt::BooleanSubscriber>(table_->GetBooleanTopic(key_).Subscribe(defaultValue_));
    }
    else
    {
        // For units and other types, store as double
        // Check if T has a .value() method (units types)
        if constexpr (requires { defaultValue_.value(); })
        {
            doublePublisher_ = std::make_unique<nt::DoublePublisher>(table_->GetDoubleTopic(key_).Publish());
            doubleSubscriber_ = std::make_unique<nt::DoubleSubscriber>(table_->GetDoubleTopic(key_).Subscribe(static_cast<double>(defaultValue_.value())));
        }
        else
        {
            // For other numeric types that can convert to double
            doublePublisher_ = std::make_unique<nt::DoublePublisher>(table_->GetDoubleTopic(key_).Publish());
            doubleSubscriber_ = std::make_unique<nt::DoubleSubscriber>(table_->GetDoubleTopic(key_).Subscribe(static_cast<double>(defaultValue_)));
        }
    }
}

template<typename T>
void Tunable<T>::PublishValue() const
{
    if constexpr (std::is_same_v<T, double>)
    {
        if (doublePublisher_)
        {
            doublePublisher_->Set(currentValue_);
        }
    }
    else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, long>)
    {
        if (integerPublisher_)
        {
            integerPublisher_->Set(currentValue_);
        }
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        if (booleanPublisher_)
        {
            booleanPublisher_->Set(currentValue_);
        }
    }
    else
    {
        // For units and other types, publish as double
        if (doublePublisher_)
        {
            if constexpr (requires { currentValue_.value(); })
            {
                doublePublisher_->Set(static_cast<double>(currentValue_.value()));
            }
            else
            {
                doublePublisher_->Set(static_cast<double>(currentValue_));
            }
        }
    }
}

template<typename T>
T Tunable<T>::ReadFromNetworkTables() const
{
    if constexpr (std::is_same_v<T, double>)
    {
        if (doubleSubscriber_)
        {
            return doubleSubscriber_->Get();
        }
        return currentValue_;
    }
    else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, long>)
    {
        if (integerSubscriber_)
        {
            return static_cast<T>(integerSubscriber_->Get());
        }
        return currentValue_;
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        if (booleanSubscriber_)
        {
            return booleanSubscriber_->Get();
        }
        return currentValue_;
    }
    else
    {
        // For units and other types, read as double and convert
        if (doubleSubscriber_)
        {
            if constexpr (requires { T{static_cast<typename T::value_type>(0.0)}; })
            {
                // Units type constructor
                return T{static_cast<typename T::value_type>(doubleSubscriber_->Get())};
            }
            else
            {
                // Direct conversion
                return static_cast<T>(doubleSubscriber_->Get());
            }
        }
        return currentValue_;
    }
}

} // namespace nfr