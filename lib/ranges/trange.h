#ifndef TRANGE_H
#define TRANGE_H

#include <ranges>
#include <QList>
#include <QCoroGenerator>
#include <functional>

template <typename T> class tRangeBacking {
    public:
        tRangeBacking() : ran(true) {
        }

        tRangeBacking(QCoro::Generator<const T>&& backing) : _backing(std::move(backing)) {
        }

        tRangeBacking(QList<T> list) : _backing(gen_list_impl(list)) {
        }

        typename QCoro::Generator<const T>::iterator begin() {
            if (ran) return end();
            ran = true;
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
        bool ran = false;

        QCoro::Generator<const T> gen_list_impl(QList<T> list) {
            for (auto i = list.cbegin(); i != list.cend(); i++) {
                co_yield* i;
            }
        }
};

template <typename T> class tRange {
    public:
        template <typename R> using MapFunction = std::function<const R(T)>;
        using FilterFunction = std::function<bool(T)>;

        tRange(QList<T> list) : _backing(new tRangeBacking<T>(tRangeBacking(list))) {
        }

        tRange(QCoro::Generator<T>&& gen) : _backing(new tRangeBacking<T>(tRangeBacking(std::move(gen)))) {
        }

        tRange(tRangeBacking<T>&& backing) : _backing(new tRangeBacking<T>(std::move(backing))) {
        }

        tRange(tRange<T>&& other) {
            if (_backing) delete _backing;
            _backing = std::move(other._backing);
            other._backing = new tRangeBacking<T>();
        }

        ~tRange() {
            if (_backing) delete _backing;
        }

        tRange& operator=(tRange<T>&& other) {
            if (_backing) delete _backing;
            _backing = other._backing;
            other._backing = new tRangeBacking<T>();
            return this;
        }

        template <typename R> tRange<R> map(MapFunction<R> mapping) {
            return tRange<R>(map_impl(mapping));
        }

        tRange<T> filter(FilterFunction filtering) {
            return tRange(filter_impl(filtering));
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
        tRangeBacking<T>* _backing = nullptr;

        template <typename R> QCoro::Generator<const R> map_impl(MapFunction<R> mapping) {
            for (auto item : *_backing) {
                co_yield mapping(item);
            }
        }

        QCoro::Generator<const T> filter_impl(FilterFunction filtering) {
            for (auto item : *_backing) {
                if (filtering(item)) co_yield item;
            }
        }
};

#endif // TRANGE_H
