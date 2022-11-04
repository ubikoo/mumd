/*
 * event.hpp
 *
 * Copyright (c) 2020 Carlos Braga
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the MIT License.
 *
 * See accompanying LICENSE.md or https://opensource.org/licenses/MIT.
 */

#ifndef ATTO_OPENGL_GRAPHICS_EVENT_H_
#define ATTO_OPENGL_GRAPHICS_EVENT_H_

#include "atto/opengl/base.hpp"

namespace atto {
namespace gl {

/** ---- Event ----------------------------------------------------------------
 * @brief Event is a union of structures representing different events.
 * At any given point, the valid member is the one corresponding to the event
 * specified by the member variable type. Trying to read any other member
 * results in undefined behavior.
 */
struct Event {
    /* Event enumerated type. */
    enum : GLenum {
        /* Window events */
        FramebufferSize         = 1 << 0,
        WindowPos               = 1 << 1,
        WindowSize              = 1 << 2,
        WindowClose             = 1 << 3,
        WindowMaximize          = 1 << 4,
        /* Input events */
        Key                     = 1 << 5,
        CursorEnter             = 1 << 6,
        CursorPos               = 1 << 7,
        MouseButton             = 1 << 8,
        MouseScroll             = 1 << 9,
        /* All events */
        All = FramebufferSize
            | WindowPos
            | WindowSize
            | WindowClose
            | WindowMaximize
            | Key
            | CursorEnter
            | CursorPos
            | MouseButton
            | MouseScroll
    };
    GLenum type;

    /* Type assignment operator. */
    GLenum operator=(const GLenum value) { return (type = value); }

    /* Bitwise type assignment AND, OR and XOR operators. */
    GLenum operator&= (const GLenum &value) { return (type &= value); }
    GLenum operator|= (const GLenum &value) { return (type |= value); }
    GLenum operator^= (const GLenum &value) { return (type ^= value); }

    /* Event data structs and corresponding union. */
    struct FramebufferSizeType {
        int width;
        int height;
    };

    struct WindowPosType {
        int xpos;
        int ypos;
    };

    struct WindowSizeType {
        int width;
        int height;
    };

    struct WindowMaximizeType {
        int iconified;
    };

    struct KeyType {
        int code;
        int scancode;
        int action;
        int mods;
    };

    struct CursorEnterType {
        int entered;
    };

    struct CursorPosType {
        double xpos;
        double ypos;
    };

    struct MouseButtonType {
        int button;
        int action;
        int mods;
    };

    struct MouseScrollType {
        double xoffset;
        double yoffset;
    };

    union {
        FramebufferSizeType framebuffersize;
        WindowPosType windowpos;
        WindowSizeType windowsize;
        WindowMaximizeType windowmaximize;
        KeyType key;
        CursorEnterType cursorenter;
        CursorPosType cursorpos;
        MouseButtonType mousebutton;
        MouseScrollType mousescroll;
    };

    /* Constructor/destructor.  */
    explicit Event(const GLenum value) : type(value) {
        core_assert((type & All), "invalid event type");
    }
    ~Event() = default;
    /* Copy constructor/assignment. */
    Event(const Event &other);
    Event &operator=(const Event &other);
};

/**
 * Event::Event
 * @brief Event copy constructor.
 */
core_inline
Event::Event(const Event &other)
{
    /* Copy event type and event data. */
    type = other.type;

    switch (type) {
    case FramebufferSize:
        framebuffersize = other.framebuffersize;
        break;
    case WindowPos:
        windowpos = other.windowpos;
        break;
    case WindowSize:
        windowsize = other.windowsize;
        break;
    case WindowMaximize:
        windowmaximize = other.windowmaximize;
        break;
    case Key:
        key = other.key;
        break;
    case CursorEnter:
        cursorenter = other.cursorenter;
        break;
    case CursorPos:
        cursorpos = other.cursorpos;
        break;
    case MouseButton:
        mousebutton = other.mousebutton;
        break;
    case MouseScroll:
        mousescroll = other.mousescroll;
        break;
    default:
        break;
    }
}

/**
 * Event::operator=
 * @brief Event copy assignment.
 */
core_inline
Event &Event::operator=(const Event &other)
{
    if (this == &other) {
        return *this;
    }

    /* Copy event type and event data. */
    type = other.type;

    switch (type) {
    case FramebufferSize:
        framebuffersize = other.framebuffersize;
        break;
    case WindowPos:
        windowpos = other.windowpos;
        break;
    case WindowSize:
        windowsize = other.windowsize;
        break;
    case WindowMaximize:
        windowmaximize = other.windowmaximize;
        break;
    case Key:
        key = other.key;
        break;
    case CursorEnter:
        cursorenter = other.cursorenter;
        break;
    case CursorPos:
        cursorpos = other.cursorpos;
        break;
    case MouseButton:
        mousebutton = other.mousebutton;
        break;
    case MouseScroll:
        mousescroll = other.mousescroll;
        break;
    default:
        break;
    }

    return *this;
}

/**
 * @brief Event bitwise unary NOT and binary AND, OR and XOR operators.
 */
core_inline
GLenum operator~(Event lhs) { return ~(lhs.type); }

core_inline
GLenum operator&(Event lhs, const GLenum &rhs) { return (lhs &= rhs); }
core_inline
GLenum operator|(Event lhs, const GLenum &rhs) { return (lhs |= rhs); }
core_inline
GLenum operator^(Event lhs, const GLenum &rhs) { return (lhs ^= rhs); }

core_inline
GLenum operator&(Event lhs, const Event &rhs) { return (lhs &= rhs.type); }
core_inline
GLenum operator|(Event lhs, const Event &rhs) { return (lhs |= rhs.type); }
core_inline
GLenum operator^(Event lhs, const Event &rhs) { return (lhs ^= rhs.type); }

} /* gl */
} /* atto */

#endif /* ATTO_OPENGL_GRAPHICS_EVENT_H_ */
