#ifndef TRANGE_H
#define TRANGE_H

#include <QCoroGenerator>
#include <QList>
#include <QUuid>
#include <functional>
#include <ranges>
#include <tlogger.h>

template<typename T> class tRangeBacking {
    public:
        tRangeBacking() {
        }

        tRangeBacking(QCoro::Generator<const T>&& backing) :
            _backing(std::move(backing)) {
        }

        tRangeBacking(QList<T> list) :
            _backing(gen_list_impl(list)) {
        }

        tRangeBacking(tRangeBacking<T>&) = delete;
        tRangeBacking(tRangeBacking<T>&&) = delete;

        typename QCoro::Generator<const T>::iterator begin() {
            if (_ran) {
                tWarn("tRangeBacking") << "tRangeBacking::begin called more than once";
                return end();
            }
            _ran = true;
            return _backing.begin();
        }

        typename QCoro::Generator<const T>::iterator end() {
            return _backing.end();
        }

        operator QCoro::Generator<const T>() {
            return _backing;
        }

    private:
        QCoro::Generator<const T> _backing;
        bool _ran = false;

        QCoro::Generator<const T> gen_list_impl(QList<T> list) {
            for (auto i = list.cbegin(); i != list.cend(); i++) {
                co_yield *i;
            }
        }
};

template<typename T> class tRange {
    public:
        template<typename R> using MapFunction = std::function<const R(T)>;
        template<typename R> using MapFunctionWithIndex = std::function<const R(T, int)>;
        using FilterFunction = std::function<bool(T)>;
        using FilterFunctionWithIndex = std::function<bool(T, int)>;

        tRange(QList<T> list) :
            _backing(new tRangeBacking<T>(list)) {
        }

        tRange(QCoro::Generator<T>&& gen) :
            _backing(new tRangeBacking<T>(std::move(gen))) {
        }

        tRange(tRange<T>&& other) {
            _backing = other._backing;
            other._backing.clear();
        }

        tRange(tRangeBacking<T>* backing) :
            _backing(backing) {
        }

        tRange& operator=(tRange<T>&& other) {
            _backing = other._backing;
            other._backing.clear();
            return this;
        }

        template<typename R> tRange<R> map(MapFunction<R> mapping) {
            return tRange<R>(new tRangeBacking(map_impl(mapping, _backing)));
        }

        template<typename R> tRange<R> map(MapFunctionWithIndex<R> mapping) {
            return tRange<R>(new tRangeBacking(map_impl(mapping, _backing)));
        }

        tRange<T> filter(FilterFunction filtering) {
            return tRange(new tRangeBacking(filter_impl(filtering, _backing)));
        }

        tRange<T> filter(FilterFunctionWithIndex filtering) {
            return tRange(new tRangeBacking(filter_impl(filtering, _backing)));
        }

        bool every(FilterFunction filtering) {
            for (auto item : *_backing) {
                if (!filtering(item)) return false;
            }
            return true;
        }

        bool some(FilterFunction filtering) {
            for (auto item : *_backing) {
                if (filtering(item)) return true;
            }
            return false;
        }

        tRange<T> take(uint num) {
            return tRange(new tRangeBacking(take_impl(num, _backing)));
        }

        tRange<T> skip(uint num) {
            return tRange(new tRangeBacking(skip_impl(num, _backing)));
        }

        typename QCoro::Generator<const T>::iterator begin() {
            return _backing->begin();
        }

        typename QCoro::Generator<const T>::iterator end() {
            return _backing->end();
        }

        operator QCoro::Generator<const T>() {
            return _backing;
        }

    private:
        using Backing = QSharedPointer<tRangeBacking<T>>;
        Backing _backing = nullptr;

        // The original tRange may go out of scope before these coroutines are called.
        // Preserve the backing in order to be able to iterate over it.

        template<typename R> QCoro::Generator<const R> map_impl(MapFunction<R> mapping, Backing backing) {
            return map_impl<R>([mapping](T item, int index) {
                return mapping(item);
            },
                backing);
        }

        template<typename R> QCoro::Generator<const R> map_impl(MapFunctionWithIndex<R> mapping, Backing backing) {
            auto i = 0;
            for (auto item : *backing) {
                co_yield mapping(item, i);
                i++;
            }
        }

        QCoro::Generator<const T> filter_impl(FilterFunction filtering, Backing backing) {
            return filter_impl([filtering, backing](T item, int index) {
                return filtering(item);
            },
                backing);
        }

        QCoro::Generator<const T> filter_impl(FilterFunctionWithIndex filtering, Backing backing) {
            auto i = 0;
            for (auto item : *backing) {
                if (filtering(item, i)) co_yield item;
                i++;
            }
        }

        QCoro::Generator<const T> take_impl(uint num, Backing backing) {
            auto i = 0;
            for (auto item : *backing) {
                co_yield item;
                i++;
                if (i == num) co_return;
            }
        }

        QCoro::Generator<const T> skip_impl(uint num, Backing backing) {
            auto i = 0;
            for (auto item : *backing) {
                i++;
                if (i > num) {
                    co_yield item;
                }
            }
        }
};

#endif // TRANGE_H
