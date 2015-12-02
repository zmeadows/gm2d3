#pragma once

#include <Fl/fl_draw.H>
#include <memory>

Fl_Color LIGHT_RED();
Fl_Color DARK_RED();

Fl_Color LIGHT_GREEN();
Fl_Color DARK_GREEN();

Fl_Color LIGHT_BLUE();
Fl_Color DARK_BLUE();

Fl_Color YELLOW();

/* template jujitsu for simplifying FLTK callbacks */

template<typename T, void (T::*method)()>
inline void cb(Fl_Widget* widget, void* object) {
    (((T*)object)->*method)();
}

template<typename T, void (T::*method)(Fl_Widget*)>
inline void cb(Fl_Widget* widget, void* object) {
        (((T*)object)->*method)(widget);
}

template <typename T>
struct cb_helper {
        T* obj;

        cb_helper(T* obj): obj(obj) {}

        template <void (T::*method)()>
        void connect(Fl_Widget* widget) {
                widget->callback(cb<T, method>, obj);
        }

        template <void (T::*method)(Fl_Widget *)>
        void connect(Fl_Widget* widget) {
                widget->callback(cb<T, method>, obj);
        }
};

template <typename T>
inline cb_helper<T> make_cb_helper(T* o) {
        return cb_helper<T>(o);
}

#define FLTK_CALLBACK(obj, obj_m, wptr) make_cb_helper(obj).connect<obj_m>(dynamic_cast<Fl_Widget*>(wptr.get()))
