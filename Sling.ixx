export module Sling;

/*
The MIT License (MIT)
Copyright (c) 2019 Adrian-George Bostan <adrg@epistack.com>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

import <functional>;
import <map>;

namespace sling
{
    using SlotKey = std::size_t;

    export template <typename... Args>
    class Signal;

    export template <typename... Args>
    class Slot final
    {
    private:
        struct Connection
        {
            Signal<Args...>* m_signal;
            Slot<Args...>* m_slot;
            SlotKey m_key;
            bool m_managed;

            Connection(Signal<Args...>* signal, Slot<Args...>* slot,
                       const SlotKey key, const bool managed) :
                m_signal(signal), m_slot(slot), m_key(key), m_managed(managed) { }

            // ReSharper disable once CppMemberFunctionMayBeConst
            void releaseSlot()
            {
                this->m_slot->m_connection = nullptr;
                if (this->m_managed)
                {
                    delete this->m_slot;
                }
            }
        };

        std::function<void(Args...)> m_callback;
        Connection* m_connection = nullptr;

        void move(Slot* slot)
        {
            this->m_callback = std::move(slot->m_callback);
            this->m_connection = nullptr;

            if (slot->m_connection != nullptr)
            {
                this->m_connection = slot->m_connection;
                slot->m_connection->releaseSlot();
                this->m_connection->m_slot = this;
                this->m_connection->m_managed = false;
            }
        }

    public:
        Slot() : m_callback() { }

        explicit Slot(const std::function<void(Args...)>& f) : m_callback(f) { }

        explicit Slot(std::function<void(Args...)>&& f) : m_callback(f) { }

        template <class T>
        Slot(T* target, void (T::* method)(Args...))
        {
            setCallback(target, method);
        }

        template <class T>
        Slot(T* target, void (T::* method)(Args...) const)
        {
            setCallback(target, method);
        }

        Slot(const Slot& slot) : m_callback(slot.m_callback) { }

        Slot(Slot&& slot) noexcept
        {
            this->move(&slot);
        }

        ~Slot()
        {
            this->disconnect();
        }

        Slot& operator=(const Slot& slot)
        {
            this->m_callback = slot.m_callback;
            return *this;
        }

        Slot& operator=(Slot&& slot) noexcept
        {
            this->disconnect();
            this->move(&slot);
            return *this;
        }

        void disconnect()
        {
            if (this->m_connection != nullptr)
            {
                this->m_connection->m_signal->disconnect(this);
            }
        }

        void setCallback(const std::function<void(Args...)>& f)
        {
            this->m_callback = f;
        }

        void setCallback(std::function<void(Args...)>&& f)
        {
            this->m_callback = f;
        }

        template <class T>
        void setCallback(T* target, void (T::* method)(Args...))
        {
            this->m_callback = [target, method](Args... args)
            {
                (target->*method)(args...);
            };
        }

        template <class T>
        void setCallback(T* target, void (T::* method)(Args...) const)
        {
            this->m_callback = [target, method](Args... args)
            {
                (target->*method)(args...);
            };
        }

        friend class Signal<Args...>;
    };

    template <typename... Args>
    class Signal final
    {
    private:
        using Connection = typename Slot<Args...>::Connection;

        std::map<std::size_t, Connection> m_connections;
        SlotKey m_sequence;

        SlotKey connect(Slot<Args...>* slot, bool managed)
        {
            if (slot == nullptr)
            {
                return 0;
            }
            if (slot->m_connection != nullptr)
            {
                if (slot->m_connection->m_signal == this)
                {
                    return slot->m_connection->m_key;
                }
                slot->disconnect();
            }

            ++this->m_sequence;
            auto res = this->m_connections.emplace(
                std::make_pair(
                    this->m_sequence,
                    Connection(this, slot, this->m_sequence, managed)
                )
            );

            slot->m_connection = &res.first->second;
            return this->m_sequence;
        }

        void move(Signal* signal)
        {
            this->clear();
            this->m_connections = std::move(signal->m_connections);
            this->m_sequence = signal->m_sequence;

            for (auto& connection : this->m_connections)
            {
                connection.second.m_signal = this;
            }

            signal->m_sequence = 0;
            signal->m_connections.clear();
        }

    public:
        Signal() : m_connections(), m_sequence(0) { }

        Signal(const Signal& signal) = delete;

        Signal(Signal&& signal) noexcept
        {
            this->move(&signal);
        }

        ~Signal()
        {
            this->clear();
        }

        Signal& operator=(const Signal& signal) = delete;

        Signal& operator=(Signal&& signal) noexcept
        {
            this->move(&signal);
            return *this;
        }

        void operator()(Args... args) const
        {
            this->emit(args...);
        }

        SlotKey connect(Slot<Args...>* slot)
        {
            return this->connect(slot, false);
        }

        SlotKey connect(Slot<Args...>& slot)
        {
            return this->connect(&slot, false);
        }

        SlotKey connect(Slot<Args...>&& slot)
        {
            return this->connect(new Slot<Args...>(std::move(slot)), true);
        }

        void disconnect(SlotKey key)
        {
            auto it = this->m_connections.find(key);
            if (it != this->m_connections.end())
            {
                it->second.releaseSlot();
                this->m_connections.erase(it);
            }
        }

        void disconnect(Slot<Args...>* slot)
        {
            if (slot != nullptr && slot->m_connection != nullptr &&
                slot->m_connection->m_signal == this)
            {
                this->disconnect(slot->m_connection->m_key);
            }
        }

        void disconnect(Slot<Args...>& slot)
        {
            this->disconnect(&slot);
        }

        void clear()
        {
            for (auto& conn : this->m_connections)
            {
                conn.second.releaseSlot();
            }
            this->m_connections.clear();
        }

        void emit(Args... args) const
        {
            for (const auto& conn : this->m_connections)
            {
                if (conn.second.m_slot->m_callback)
                {
                    conn.second.m_slot->m_callback(std::forward<Args>(args)...);
                }
            }
        }
    };
}
