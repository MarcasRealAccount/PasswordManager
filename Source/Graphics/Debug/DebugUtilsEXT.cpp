#include "DebugUtilsEXT.h"
#include "Graphics/Instance.h"

namespace Graphics::DebugUtilsEXT {
	template <class FunctionPtr>
	static FunctionPtr getInstanceFunction(Instance& instance, const char* name) {
		return reinterpret_cast<FunctionPtr>(vkGetInstanceProcAddr(*instance, name));
	}

	void FunctionPtrs::init(Instance& instance) {
		m_CreateDebugUtilsMessengerEXT  = getInstanceFunction<decltype(m_CreateDebugUtilsMessengerEXT)>(instance, "vkCreateDebugUtilsMessengerEXT");
		m_DestroyDebugUtilsMessengerEXT = getInstanceFunction<decltype(m_DestroyDebugUtilsMessengerEXT)>(instance, "vkDestroyDebugUtilsMessengerEXT");
		m_SetDebugUtilsObjectNameEXT    = getInstanceFunction<decltype(m_SetDebugUtilsObjectNameEXT)>(instance, "vkSetDebugUtilsObjectNameEXT");
	}
} // namespace Graphics::DebugUtilsEXT
