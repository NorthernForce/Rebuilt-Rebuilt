#pragma once

#include <logging/Logger.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/DoubleTopic.h>
#include <networktables/IntegerTopic.h>
#include <networktables/BooleanTopic.h>

#include <string>
#include <memory>
#include <unordered_map>

namespace nfr
{

/**
 * @brief A tunable value that can be adjusted via NetworkTables while providing
 * transparent access through operator overloading.
 * 
 * @tparam T The underlying type (double, int, bool, or units types)
 */
template<typename T>
class Tunable
{
public:
    /**
     * @brief Construct a new Tunable object
     * 
     * @param key The NetworkTables key for this value
     * @param defaultValue The default value to use
     * @param tableName The NetworkTables table name (defaults to "Tuning")
     */
    Tunable(const std::string& key, const T& defaultValue, const std::string& tableName = "Tuning");

    /**
     * @brief Copy constructor
     */
    Tunable(const Tunable& other);

    /**
     * @brief Assignment operator
     */
    Tunable& operator=(const Tunable& other);

    /**
     * @brief Implicit conversion to the underlying type
     */
    operator T() const;

    /**
     * @brief Get the current value (same as implicit conversion)
     */
    T Get() const;

    /**
     * @brief Set a new value (updates NetworkTables)
     */
    void Set(const T& value);

    /**
     * @brief Assignment operator for setting values
     */
    Tunable& operator=(const T& value);

    /**
     * @brief Get the NetworkTables key
     */
    const std::string& GetKey() const { return key_; }

    /**
     * @brief Update the value from NetworkTables (call periodically)
     */
    void Update();

private:
    std::string key_;
    T currentValue_;
    T defaultValue_;
    std::shared_ptr<nt::NetworkTable> table_;
    
    // Type-specific NetworkTables publishers/subscribers
    mutable std::unique_ptr<nt::DoublePublisher> doublePublisher_;
    mutable std::unique_ptr<nt::DoubleSubscriber> doubleSubscriber_;
    mutable std::unique_ptr<nt::IntegerPublisher> integerPublisher_;
    mutable std::unique_ptr<nt::IntegerSubscriber> integerSubscriber_;
    mutable std::unique_ptr<nt::BooleanPublisher> booleanPublisher_;
    mutable std::unique_ptr<nt::BooleanSubscriber> booleanSubscriber_;

    void InitializeNetworkTables() const;
    void PublishValue() const;
    T ReadFromNetworkTables() const;
};

// Type aliases for common types
using TunableDouble = Tunable<double>;
using TunableInt = Tunable<int>;
using TunableBool = Tunable<bool>;

} // namespace nfr

#include "Tunable.hpp"  // Include template implementation