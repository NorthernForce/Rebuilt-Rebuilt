#pragma once

#include <functional>
#include <memory>
#include <string_view>

#include "networktables/DoubleTopic.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableInstance.h"
#include "networktables/StructTopic.h"
#include "ntcore_cpp.h"
#include "wpi/struct/Struct.h"
namespace nfr
{
    class TunableManager
    {
    public:
        static TunableManager& GetInstance()
        {
            static TunableManager instance;
            return instance;
        }
        TunableManager(const TunableManager&) = delete;
        TunableManager& operator=(const TunableManager&) = delete;

        std::shared_ptr<nt::NetworkTable> GetTable() const
        {
            return table;
        }

    private:
        std::shared_ptr<nt::NetworkTable> table;
        TunableManager(std::string_view tableName = "tunable")
            : table(nt::NetworkTableInstance::GetDefault().GetTable(tableName))
        {
            if (!table)
            {
                throw std::runtime_error(
                    "Failed to create NetworkTable for tunables.");
            }
        }
        ~TunableManager() = default;
    };
    template <typename T>
        requires wpi::StructSerializable<T>
    class tunable
    {
    public:
        static std::shared_ptr<tunable<T>> CreateTunable(const std::string& key,
                                                         const T& defaultValue)
        {
            auto instance = std::make_shared<tunable<T>>(key, defaultValue);
            nt::NetworkTableInstance::GetDefault().AddListener(
                TunableManager::GetInstance().GetTable()->GetEntry(key),
                nt::EventFlags::kValueRemote,
                [instance](const nt::Event& event)
                {
                    if (event.GetValueEventData()->value.IsValid())
                    {
                        instance->Update();
                    }
                });
            return instance;
        }
        static std::shared_ptr<tunable<T>> CreateTunable(const std::string& key,
                                                         T&& defaultValue)
        {
            auto instance =
                std::make_shared<tunable<T>>(key, std::move(defaultValue));
            nt::NetworkTableInstance::GetDefault().AddListener(
                TunableManager::GetInstance().GetTable()->GetEntry(key),
                nt::EventFlags::kValueRemote,
                [instance](const nt::Event& event)
                {
                    if (event.GetValueEventData()->value.IsValid())
                    {
                        instance->Update();
                    }
                });
            return instance;
        }
        operator T() const
        {
            return value;
        }
        T operator=(const T& newValue)
        {
            if (value != newValue)
            {
                previousValue = value;
                value = newValue;
                publisher.Set(value);
            }
            return value;
        }
        T operator=(T&& newValue)
        {
            if (value != newValue)
            {
                previousValue = value;
                value = std::move(newValue);
                publisher.Set(value);
            }
            return value;
        }
        T Get() const
        {
            return value;
        }
        T GetPrevious() const
        {
            return previousValue;
        }
        bool HasChanged() const
        {
            return value != previousValue;
        }
        void Update()
        {
            auto newValue = subscriber.Get();
            if (newValue != value)
            {
                previousValue = value;
                value = newValue;
                if (updateCallback)
                {
                    updateCallback(value);
                }
            }
        }
        void SetUpdateCallback(std::function<void(const T&)> callback)
        {
            updateCallback = std::move(callback);
        }

    private:
        std::string key;
        T value;
        T previousValue;
        nt::StructSubscriber<T> subscriber;
        nt::StructPublisher<T> publisher;
        std::function<void(const T&)> updateCallback;
        tunable(const std::string& key, const T& defaultValue)
            : key(key), value(defaultValue), previousValue(defaultValue)
        {
            subscriber = TunableManager::GetInstance()
                             .GetTable()
                             ->GetStructTopic<T>(key)
                             .Subscribe(defaultValue);
            publisher = TunableManager::GetInstance()
                            .GetTable()
                            ->GetStructTopic<T>(key)
                            .Publish();
        }
        tunable(const std::string& key, T&& defaultValue)
            : key(key), value(std::move(defaultValue)), previousValue(value)
        {
            subscriber = TunableManager::GetInstance()
                             .GetTable()
                             ->GetStructTopic<T>(key)
                             .Subscribe(value);
            publisher = TunableManager::GetInstance()
                            .GetTable()
                            ->GetStructTopic<T>(key)
                            .Publish();
        }
        tunable(const tunable&) = delete;
        tunable& operator=(const tunable&) = delete;
        tunable(tunable&&) = default;
        tunable& operator=(tunable&&) = default;
    };
    template <>
    class tunable<double>
    {
    public:
        static std::shared_ptr<tunable<double>> CreateTunable(
            const std::string& key, double defaultValue)
        {
            auto instance = std::make_shared<tunable<double>>(key, defaultValue);
            nt::NetworkTableInstance::GetDefault().AddListener(
                TunableManager::GetInstance().GetTable()->GetEntry(key),
                nt::EventFlags::kValueRemote,
                [instance](const nt::Event& event)
                {
                    if (event.GetValueEventData()->value.IsValid())
                    {
                        instance->Update();
                    }
                });
            return instance;
        }
        operator double() const
        {
            return value;
        }
        double operator=(double newValue)
        {
            if (value != newValue)
            {
                previousValue = value;
                value = newValue;
                publisher.Set(value);
            }
            return value;
        }
        double Get() const
        {
            return value;
        }
        double GetPrevious() const
        {
            return previousValue;
        }
        bool HasChanged() const
        {
            return value != previousValue;
        }
        void Update()
        {
            auto newValue = subscriber.Get();
            if (newValue != value)
            {
                previousValue = value;
                value = newValue;
                if (updateCallback)
                {
                    updateCallback(value);
                }
            }
        }
        void SetUpdateCallback(std::function<void(double)> callback)
        {
            updateCallback = std::move(callback);
        }

    private:
        std::string key;
        double value;
        double previousValue;
        nt::DoubleSubscriber subscriber;
        nt::DoublePublisher publisher;
        std::function<void(double)> updateCallback;
        tunable(const std::string& key, double defaultValue)
            : key(key), value(defaultValue), previousValue(defaultValue)
        {
            subscriber = TunableManager::GetInstance()
                             .GetTable()
                             ->GetDoubleTopic(key)
                             .Subscribe(defaultValue);
            publisher = TunableManager::GetInstance()
                            .GetTable()
                            ->GetDoubleTopic(key)
                            .Publish();
        }
        tunable(const tunable&) = delete;
        tunable& operator=(const tunable&) = delete;
        tunable(tunable&&) = default;
        tunable& operator=(tunable&&) = default;
    };
}  // namespace nfr