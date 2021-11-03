#pragma once

#include "Core.h"

#include <cstdint>

#include <type_traits>
#include <utility>
#include <vector>

namespace Graphics {
	namespace Detail {
		template <class T>
		struct IsHandleValid {
			static constexpr bool Value = std::is_constructible_v<T, std::nullptr_t> && std::is_assignable_v<T&, std::nullptr_t> && std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> && std::is_move_constructible_v<T> && std::is_move_assignable_v<T> && std::is_destructible_v<T>;
		};

		template <class T>
		static constexpr auto IsHandleValidV = IsHandleValid<T>::Value;

		struct HandleBase {
		public:
			HandleBase()          = default;
			virtual ~HandleBase() = default;

			virtual bool create()  = 0;
			virtual void destroy() = 0;

			virtual bool isValid() const       = 0;
			virtual bool isCreated() const     = 0;
			virtual bool isDestroyable() const = 0;

			void addChild(HandleBase& child);
			void removeChild(HandleBase& child);

			auto& getChildren() const { return m_Children; }

		protected:
			std::size_t m_ChildItr = 0;
			bool m_Recreate        = false;

			std::vector<HandleBase*> m_Children;
			std::vector<HandleBase*> m_DestroyedChildren;
		};

		template <class T>
		struct HandleStorage : public HandleBase {
		public:
			HandleStorage() : m_Handle(nullptr) { }
			HandleStorage(const T& handle) : m_Handle(handle) { }
			HandleStorage(T&& handle) : m_Handle(std::move(handle)) { }

			virtual bool isValid() const override { return m_Handle; }

			auto& getHandle() { return m_Handle; }
			auto& getHandle() const { return m_Handle; }
			auto& operator*() { return m_Handle; }
			auto& operator*() const { return m_Handle; }
			auto* operator->() { return &m_Handle; }
			auto* operator->() const { return &m_Handle; }
			operator T&() { return m_Handle; }
			operator T&() const { return m_Handle; }

		protected:
			T m_Handle;
		};

		template <class T, bool IsDestroyable, bool IsDebuggable, bool IsValid>
		struct Handle;

		template <class T, bool IsDebuggable>
		struct Handle<T, true, IsDebuggable, true> : public HandleStorage<T> {
		public:
			using HandleT                    = T;
			using Base                       = HandleStorage<T>;
			static constexpr bool Debuggable = IsDebuggable;

		public:
			Handle() : Base(), m_Created(false), m_Destroyable(true) { }
			Handle(const T& handle) : Base(handle), m_Created(true), m_Destroyable(false) { }
			Handle(T&& handle) : Base(std::move(handle)), m_Created(true), m_Destroyable(false) { }

			virtual bool create() override;
			virtual void destroy() override;

			virtual bool isCreated() const override { return m_Created; }
			virtual bool isDestroyable() const override { return m_Destroyable; }

		private:
			virtual void createImpl()  = 0;
			virtual bool destroyImpl() = 0;

		private:
			bool m_Created;
			bool m_Destroyable;
		};

		template <class T, bool IsDebuggable>
		struct Handle<T, false, IsDebuggable, true> : public HandleStorage<T> {
		public:
			using HandleT                    = T;
			using Base                       = HandleStorage<T>;
			static constexpr bool Debuggable = IsDebuggable;

		public:
			Handle(const T& handle) : Base(handle) { }
			Handle(T&& handle) : Base(std::move(handle)) { }

			virtual bool create() override;
			virtual void destroy() override;

			virtual bool isCreated() const override { return true; }
			virtual bool isDestroyable() const override { return false; }
		};
	} // namespace Detail

	template <class T, bool IsDestroyable, bool IsDebuggable>
	using Handle = Detail::Handle<T, IsDestroyable, IsDebuggable && Core::s_IsConfigDebug, Detail::IsHandleValidV<T>>;
} // namespace Graphics

namespace Graphics::Detail {
	template <class T, bool IsDebuggable>
	bool Handle<T, true, IsDebuggable, true>::create() {
		bool pCreated = m_Created;
		if (pCreated) {
			Base::m_Recreate = true;
			destroy();
		}

		createImpl();
		m_Created = Base::isValid();
		if (pCreated && m_Created) {
			for (auto& child : Base::m_DestroyedChildren)
				child->create();
			Base::m_DestroyedChildren.clear();
		}
		Base::m_Recreate = false;
		return m_Created;
	}

	template <class T, bool IsDebuggable>
	void Handle<T, true, IsDebuggable, true>::destroy() {
		if (Base::m_Recreate)
			Base::m_DestroyedChildren.clear();

		for (Base::m_ChildItr = 0; Base::m_ChildItr < Base::m_Children.size(); ++Base::m_ChildItr) {
			auto child = Base::m_Children[Base::m_ChildItr];

			if (child->isValid()) {
				child->destroy();

				if (Base::m_Recreate && child->isDestroyable())
					Base::m_DestroyedChildren.push_back(child);
			}
		}

		if (m_Destroyable && isCreated() && destroyImpl())
			Base::m_Handle = nullptr;
		m_Created        = false;
		Base::m_ChildItr = 0;
	}

	template <class T, bool IsDebuggable>
	bool Handle<T, false, IsDebuggable, true>::create() {
		Base::m_Recreate = true;
		destroy();

		for (auto& child : Base::m_DestroyedChildren)
			child->create();
		Base::m_DestroyedChildren.clear();

		Base::m_Recreate = false;
		return true;
	}

	template <class T, bool IsDebuggable>
	void Handle<T, false, IsDebuggable, true>::destroy() {
		Base::m_DestroyedChildren.clear();

		for (Base::m_ChildItr = 0; Base::m_ChildItr < Base::m_Children.size(); ++Base::m_ChildItr) {
			auto child = Base::m_Children[Base::m_ChildItr];

			if (child->isValid()) {
				child->destroy();

				if (child->isDestroyable())
					Base::m_DestroyedChildren.push_back(child);
			}
		}

		Base::m_ChildItr = 0;
	}
} // namespace Graphics::Detail
