#pragma once

#include <d3d11.h>
#include <cstddef>
#include <type_traits>

template<class _T>
class unique_resource
{
public:
	using pointer = _T*;
	using element_type = _T;

	constexpr unique_resource() noexcept : _Ptr(nullptr) {}
	inline explicit unique_resource(pointer ptr) noexcept : _Ptr(ptr) {}
	inline unique_resource(unique_resource&& _Right) noexcept : _Ptr(_Right._Ptr) {
		_Right._Ptr = nullptr;
	}
	unique_resource(const unique_resource&) = delete;
	unique_resource& operator=(const unique_resource&) = delete;

	inline ~unique_resource() noexcept {
		reset();
	}

	unique_resource& operator=(std::nullptr_t) noexcept {
		reset();
		return *this;
	}
	inline unique_resource& operator=(unique_resource&& _Right) noexcept {
		reset();
		_Ptr = _Right._Ptr;
		_Right._Ptr = nullptr;
		return *this;
	}

	inline pointer get() const noexcept {
		return _Ptr;
	}

	inline pointer* ptr_to_ptr() {
		return &_Ptr;
	}

	inline _T& operator*() const noexcept {
		return *_Ptr;
	}

	inline pointer operator->() const noexcept {
		return _Ptr;
	}

	inline explicit operator bool() const noexcept {
		return static_cast<bool>(_Ptr);
	}

	inline void reset() noexcept {
		if (_Ptr) { _Ptr->Release(); }
		_Ptr = nullptr;
	}

private:
	pointer _Ptr;
};